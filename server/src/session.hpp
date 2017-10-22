//================================================================================================================================================
// FILE: session.h
// (c) GIE 2017-10-22  19:37
//
//================================================================================================================================================
#ifndef H_GUARD_SESSION_2017_10_22_19_37
#define H_GUARD_SESSION_2017_10_22_19_37
//================================================================================================================================================
#pragma once
//================================================================================================================================================
#include "app.hpp"
#include "sockets.hpp"

#include "gie/format.hpp"
//================================================================================================================================================
namespace echosrv {

    struct session_t {

        session_t(session_t  const&) = delete;

        session_t(app_t& app, gie::socket&& socket, sockaddr const& m_peer_info)
            : m_app(app)
            , m_socket( std::move(socket))
            , m_peer_info(m_peer_info)
        {
            m_app.inc_session();
        }

        ~session_t(){
            m_app.dec_session();
        }

        app_t& app(){
            return m_app;
        }

        gie::socket& socket(){
            return m_socket;
        }

        sockaddr const& peer_info()const noexcept {
            return m_peer_info;
        }

        void log(std::string const& msg){
            app().log(GIE_FORMAT(gie::to_string(peer_info()) << ", " << msg));
        }

    private:
        app_t& m_app;
        gie::socket m_socket;
        sockaddr m_peer_info;
    };

}
//================================================================================================================================================
#endif
//================================================================================================================================================