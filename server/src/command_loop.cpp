//================================================================================================================================================
// FILE: command_loop.cpp
// (c) GIE 2017-10-19  22:57
//
//================================================================================================================================================
//#include "command_loop.hpp"
//================================================================================================================================================
#include "utc_time.hpp"
#include "session.hpp"
#include "app.hpp"
#include "cmd_parser.hpp"
#include "exceptions.hpp"

#include "sockets.hpp"

#include "gie/safe_main.hpp"
#include "gie/util-scope-exit.hpp"

#include <chrono>
//================================================================================================================================================
namespace {

    auto const eol = "\r\n";
}

namespace echosrv {

    void command_loop(std::shared_ptr<session_t> session){

        gie::main([&](){

            GIE_SCOPE_EXIT([&]{
                session->log("Peer disconnected.");
                GIE_LOG("Command loop terminated for peer " << gie::to_string(session->peer_info()) );
            });

            session->log("Peer connected.");

            auto& s = session->socket();

            auto const max_wait_time = std::chrono::minutes{1};

            std::chrono::milliseconds time_left = max_wait_time;

            auto const reset_time_left = [&](){
                time_left = max_wait_time;
            };

            auto const update_time_left = [&](std::chrono::milliseconds const& elapsed_time) -> bool {
                if(elapsed_time>=time_left){
                    return false;
                } else {
                    time_left -= elapsed_time;
                    return true;
                }

            };

            auto const handle_time = [&]{
                auto const& t = utc_time() + eol;
                s.send(t.data(), t.size());
            };

            auto const handle_session = [&]{
                auto const num_of_sessions = GIE_FORMAT(session->app().num_of_sessions() << eol);
                s.send(num_of_sessions.data(), num_of_sessions.size());
            };

            cmd_parser_def_t parser_def{};
            parser_def.add_token("TIME\r\n", [&](){
                session->log("TIME");

                handle_time();

            }).add_token("SESSION\r\n", [&](){
                session->log("SESSION");

                handle_session();

            }).add_token("END\r\n", [&](){
                session->log("END");

                throw exception::terminate_session("Session closed");
            });

            auto parser = parser_def.parser();


            for(;;){

                auto const& start = std::chrono::steady_clock::now();


                try {
                    std::array<char, 1024> buf;

//                    GIE_LOG(" setting to: " << time_left.count());

                    s.setsockopt_SO_RCVTIMEO(time_left);
                    auto r = s.recv(buf.data(), buf.size());

                    if (r==0) throw exception::terminate_session("Connection closed by foreign host");

                    for(size_t i=0; i<r; ++i){
                        auto const& r = parser.feed(buf[i]);
                        if (r.second){ //handler called
                            reset_time_left();
                            parser.reset();

                        } else if (!r.first){
                            throw std::runtime_error("Invalid command");
                        }
                    }

                } catch(gie::exception::interrupted_system_call const&) {
                    GIE_LOG("interrupted_system_call");
                }

                if (!update_time_left(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start))) {
                    throw exception::terminate_session();
                }

            }


        });

    }


}
//================================================================================================================================================
