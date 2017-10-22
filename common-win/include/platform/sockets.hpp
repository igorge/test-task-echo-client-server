//================================================================================================================================================
// FILE: sockets.h
//
//
//================================================================================================================================================
#ifndef H_GUARD_SOCKETS_09834673789
#define H_GUARD_SOCKETS_09834673789
//================================================================================================================================================
#pragma once
//================================================================================================================================================
#include "gie/format.hpp"
#include "sockets_exceptions.hpp"

#include "gie/log.hpp"

#include <array>
#include <errno.h>
#include <cassert>

#include <winsock2.h>
#include <ws2tcpip.h>

#include <memory>
#include <chrono>
//================================================================================================================================================

namespace gie {
    using socklen_t = int;
    using socket_t = SOCKET;
    using in_port_t = int;


    inline SOCKET invalid_socket() noexcept {
        return INVALID_SOCKET;
    }



    inline int shutdown(socket_t& s) noexcept {
        return ::shutdown(s, SD_BOTH);
    }

    inline bool is_socket_error(int err) noexcept {
        return err==SOCKET_ERROR;
    }

    inline int get_socket_last_error() noexcept {
        return WSAGetLastError();
    }


    inline int close_socket(int sock) noexcept {
        return closesocket(sock);
    }

    inline void sockets_startup(){
        WSADATA wsaData = {0};
        auto const ret = WSAStartup(MAKEWORD(2, 2), &wsaData);
        if (ret != 0) {
            throw exception::socket_initialization_error(GIE_FORMAT("WSAStartup() failed: "<<ret));
        }
    }

    inline void sockets_cleanup(){
        auto const ret = WSACleanup();
        assert(ret==0);
    }


    inline size_t recv_(socket_t const& s, void *buf, size_t len, int flags){
        assert(buf);

        auto const ret_size = ::recv(s, static_cast<char*>(buf), len, flags);

        if( is_socket_error(ret_size) ){
            auto const last_error = get_socket_last_error();
            throw exception::socket_error(GIE_FORMAT("revc("<<s<<") failed: " << last_error));
        } else {
            return ret_size;
        }
    }

    inline size_t send_(socket_t const& s, const void *buf, size_t len, int flags){

        auto const ret_size = ::send(s, static_cast<char const*>(buf), len, flags);

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
        return ::setsockopt(s, level, option_name, static_cast<char const*>(option_value), option_len);
    }


    inline int setsockopt_SO_RCVTIMEO(socket_t const& s, std::chrono::milliseconds const& to){
        DWORD tv = static_cast<DWORD>(to.count());

        return setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, static_cast<void* const>(&tv), sizeof(tv));
    }


}
//================================================================================================================================================
#endif
//================================================================================================================================================
