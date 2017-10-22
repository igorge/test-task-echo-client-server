//================================================================================================================================================
// FILE: sockets.h
//
//
//================================================================================================================================================
#ifndef H_GUARD_SOCKETS_236534763
#define H_GUARD_SOCKETS_236534763
//================================================================================================================================================
#pragma once
//================================================================================================================================================
#include "gie/format.hpp"
#include "sockets_exceptions.hpp"

#include "gie/log.hpp"

#include <array>
#include <errno.h>
#include <cassert>

#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <memory>
#include <chrono>
//================================================================================================================================================

namespace gie {
    using socklen_t = ::socklen_t;
    using socket_t = int;
    using in_port_t = ::in_port_t;


    inline int invalid_socket() noexcept {
        return -1;
    }




    inline int shutdown(socket_t& s) noexcept {
        return ::shutdown(s, SHUT_RDWR);
    }

    inline bool is_socket_error(int err) noexcept {
        return err==-1;
    }

    inline int get_socket_last_error() noexcept {
        return errno;
    }


    inline int close_socket(int sock) noexcept {
        return close(sock);
    }

    inline void sockets_startup(){
        /* empty */
    }

    inline void sockets_cleanup(){
        /* empty */
    }


    inline size_t recv_(socket_t const& s, void *buf, size_t len, int flags){
        assert(buf);

        for(;;){
            auto const ret_size = ::recv(s, buf, len, flags);

            if( is_socket_error(ret_size) ){
                auto const last_error = get_socket_last_error();
                if (last_error == EINTR) throw exception::interrupted_system_call();
                throw exception::socket_error(GIE_FORMAT("revc("<<s<<") failed: " << last_error));
            } else {
                return ret_size;
            }
        }
    }

    inline size_t send_(socket_t const& s, const void *buf, size_t len, int flags){

        auto const ret_size = ::send(s, buf, len, flags);

        if( is_socket_error(ret_size) ){
            auto const last_error = get_socket_last_error();
            throw exception::socket_error(GIE_FORMAT("send("<<s<<") failed: " << last_error));
        } else {
            return ret_size;
        }
    }


    inline void recv_all(socket_t const& s, void* buffer, size_t len, int flags = 0){
        auto const ret_size = recv_(s, buffer, len , flags | MSG_WAITALL);
        if(ret_size!=len) {
            throw exception::socket_error(GIE_FORMAT("revc("<<s<<") failed, got " << ret_size << " bytes, expected "<< len));
        }
    }

    template <size_t N>
    void recv_all(socket_t const& s, std::array<uint8_t ,N>& buffer, int flags = 0){
        recv_all(s, buffer.data(), buffer.size(), flags);
    }


    inline int setsockopt(socket_t const& s, int level, int option_name, const void *option_value, socklen_t option_len){
        return ::setsockopt(s, level, option_name, option_value, option_len);
    }

    inline int setsockopt_SO_RCVTIMEO(socket_t const& s, std::chrono::milliseconds const& to){
        auto const& to_in_secs = std::chrono::duration_cast<std::chrono::seconds>(to);
        timeval tv = {};
        tv.tv_sec = to_in_secs.count();
        tv.tv_usec = (to - to_in_secs).count();

        return setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, static_cast<void* const>(&tv), sizeof(tv));
    }




}
//================================================================================================================================================
#endif
//================================================================================================================================================
