
#include "telnet_frame_reader.hpp"
#include "sockets.hpp"
#include "exceptions.hpp"

#include "gie/util-scope-exit.hpp"
#include "gie/safe_main.hpp"

#include <thread>
#include <chrono>
#include <random>
#include <type_traits>

char const*const msg_args =
        "Usage: echo-client.exe ip_or_dns_name (linear | (random number_of_requests))";


int main(int argc, char *argv[]) {
    return gie::main([&](){

        if( argc<3 ) throw echosrv::exception::invalid_arguments(GIE_FORMAT("Expected at least 2 arguments.\n" << msg_args));

        gie::sockets_startup();
        GIE_SCOPE_EXIT([]{ gie::sockets_cleanup(); });


        addrinfo hint = {};
        auto const addr = gie::getaddrinfo(argv[1], "8888", &hint);

        gie::socket sock{AF_INET, SOCK_STREAM, IPPROTO_TCP};

        sock.connect(addr->ai_addr, addr->ai_addrlen);

        auto const& mode = std::string(argv[2]);


        echosrv::telnet_frame_reader_t frame_reader{sock};

        auto sleep = []{
            std::this_thread::sleep_for(std::chrono::seconds{10});
        };

        auto const issue_request = [&](std::string request, bool expect_result = true){
            GIE_LOG("Sending " << request);

            std::string req{std::move(request)+"\r\n"};
            sock.send(req.data(), req.size());
            if (expect_result){
                auto const& response = frame_reader.recv();
                GIE_LOG("GOT: " <<  response );
            }
        };

        auto const issue_time = [&]{
            issue_request("TIME");
        };

        auto const issue_session = [&]{
            issue_request("SESSION");
        };

        auto const issue_end = [&]{
            issue_request("END", false);
        };


        if (mode=="linear") {

            issue_session();
            sleep();

            issue_time();
            sleep();

            issue_end();

        } else if (mode=="random") {

            if( argc!=4 ) throw echosrv::exception::invalid_arguments(GIE_FORMAT("Expected number_of_requests to be specified.\n" << msg_args));

            auto const& num_of_runs_str = std::string(argv[3]);
            auto const numf_of_runs = std::stoul(num_of_runs_str);

            if(numf_of_runs==0) echosrv::exception::invalid_arguments(GIE_FORMAT("Expected number_of_requests to be greater than 0.\n" << msg_args));

            std::random_device rd{};
            std::mt19937 mt_rand{rd()};
            std::uniform_int_distribution<int> dist(0, 1);


            for ( std::remove_cv<decltype(numf_of_runs)>::type i = 0; i<numf_of_runs; ++i){

                switch (dist(mt_rand)){
                    case 0:
                        issue_session();
                        break;

                    case 1:
                        issue_time();
                        break;

                }

                sleep();
            }

            issue_end();

        } else {
            throw echosrv::exception::invalid_arguments(GIE_FORMAT("Unknown mode specified.\n" << msg_args));
        }


    });
}
