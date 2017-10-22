

#include "session.hpp"
#include "app.hpp"
#include "command_loop.hpp"
#include "sockets.hpp"

#include "gie/format.hpp"

#include "gie/util-scope-exit.hpp"
#include "gie/safe_main.hpp"


#include <memory>

char const*const msg_args = "Usage: ";


int main(int argc, char *argv[]) {
    return gie::main([&](){

        (void)argc;
        (void)argv;

        gie::sockets_startup();
        GIE_SCOPE_EXIT([]{ gie::sockets_cleanup(); });

        echosrv::app_t app{};

        gie::socket sock{AF_INET , SOCK_STREAM , IPPROTO_TCP};

        sockaddr_in addr = {};

        addr.sin_family = AF_INET;
        addr.sin_addr.s_addr = INADDR_ANY;
        addr.sin_port = htons( 8888 );

        sock.bind(reinterpret_cast<sockaddr const*>(&addr), sizeof(addr)).listen(256);

        GIE_LOG("Waiting for clients ...");

        while(true){

            sockaddr  peer_addr = {};
            socklen_t peer_add_size=sizeof(peer_addr);

            auto peersocket = sock.accept( &peer_addr, &peer_add_size);

            GIE_LOG("Got new client connection from " << gie::to_string(peer_addr));

            auto session = std::make_shared<echosrv::session_t>(app, std::move(peersocket), std::move(peer_addr));

            std::thread t{echosrv::command_loop, std::move(session)};
            t.detach();
        }

    });
}
