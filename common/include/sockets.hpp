//================================================================================================================================================
// FILE: sock.h
// (c) GIE 2017-10-19  21:46
//
//================================================================================================================================================
#ifndef H_GUARD_SOCK_2017_10_19_21_46
#define H_GUARD_SOCK_2017_10_19_21_46
//================================================================================================================================================
#pragma once
//================================================================================================================================================
#include "gie/char2hex.hpp"

#include "platform/sockets.hpp"
//================================================================================================================================================
namespace gie {

    inline bool is_socket(socket_t const& s){
        return s!=invalid_socket();
    }

    inline std::shared_ptr<addrinfo> getaddrinfo(char const* node, char const* service_name = nullptr, addrinfo const* hint = nullptr){
        assert(node);

        addrinfo* info = nullptr;

        auto const ret = ::getaddrinfo(node, service_name, hint, &info);

        if( ret !=0 ){
            throw exception::socket_error(GIE_FORMAT("getaddrinfo() for '"<< node <<"' had failed, err code: "<< ret));
        }

        return std::shared_ptr<addrinfo>(  info, [](addrinfo * info){
            if(info) freeaddrinfo(info);
        });

    }


    namespace impl {
        inline std::string format46(int family, void const* addr, in_port_t const& port){

            std::array<char,INET6_ADDRSTRLEN> buf;

            auto formatted = inet_ntop(family, addr, buf.data(), buf.size());
            if (formatted == nullptr) {
                auto const err = errno;
                throw std::runtime_error(GIE_FORMAT("inet_ntop() has failed, err: " << err));
            }
            return GIE_FORMAT(formatted<<":"<<port);
        }
    }

    inline std::string to_string(sockaddr const& addr){


        switch (addr.sa_family) {
            case AF_INET: {
                auto const &addr4 = reinterpret_cast<sockaddr_in const &>(addr);
                return impl::format46(addr.sa_family, &addr4.sin_addr, addr4.sin_port);

            }

            case AF_INET6: {
                auto const &addr6 = reinterpret_cast<sockaddr_in6 const &>(addr);
                return impl::format46(addr.sa_family, &addr6.sin6_addr, addr6.sin6_port);
            }

            default: {
                return  GIE_FORMAT("<sa_family: "<<addr.sa_family<<", addr: "<<blob2hex(&addr.sa_data[0], sizeof(addr.sa_data))<<">");
            }
        }

        assert(false);
    }


    struct socket {

        socket() : m_socket(invalid_socket()) { }
        explicit socket(socket_t&& sock) : m_socket(sock) { }

        socket(int socket_domain, int socket_type, int socket_protocol) : m_socket(::socket(socket_domain , socket_type, socket_protocol)) {

            if ( !is_socket(m_socket) ) {
                auto const last_err = get_socket_last_error();
                throw exception::socket_error( GIE_FORMAT("socket() failed: " << last_err) );
            }
        }

        socket(socket const&) = delete;

        socket(socket && other) noexcept : m_socket(other.m_socket) {
            other.m_socket = invalid_socket();
        };

        ~socket(){
            if (is_socket(m_socket)){
                if (is_socket_error(close_socket(m_socket))) {
                    auto const last_err = get_socket_last_error();
                    GIE_LOG( GIE_FORMAT("socket close() failed: " << last_err) );
                };
            }
        }


        socket& bind(const struct sockaddr *address, socklen_t address_len){

            if ( ::bind(m_socket, address, address_len) !=0 ) {
                auto const last_err = get_socket_last_error();
                throw exception::socket_error( GIE_FORMAT("bind() failed: " << last_err) );
            }

            return *this;
        }

        socket& listen(int backlog){
            if ( ::listen(m_socket , backlog) != 0 ) {
                auto const last_err = get_socket_last_error();
                throw exception::socket_error( GIE_FORMAT("listen() failed: " << last_err) );
            }

            return *this;
        }

        socket accept(sockaddr * const peer_addr, socklen_t * const peer_add_size){

            auto s_tmp = ::accept(m_socket, peer_addr, peer_add_size);

            if ( !is_socket(s_tmp) ) {
                auto const last_err = get_socket_last_error();
                throw exception::socket_error( GIE_FORMAT("accept() failed: " << last_err) );
            }

            return socket( std::move(s_tmp) );
        }

        socket& connect(struct sockaddr * address, socklen_t address_len){

            if( ::connect(m_socket, address, address_len) !=0 ){
                auto const errcode = get_socket_last_error();
                throw exception::socket_error(GIE_FORMAT("connect() has failed, error code: " << errcode));
            }

            return *this;
        }

        socket& setsockopt(int level, int option_name, const void *option_value, socklen_t option_len){
            if (is_socket_error(::gie::setsockopt(m_socket, level, option_name, option_value, option_len))) {
                auto const last_err = get_socket_last_error();
                throw exception::socket_error( GIE_FORMAT("setsockopt(level="<<level<<", option_name="<<option_name<<") has failed, err: "<<last_err) );
            }

            return *this;
        }

        socket& setsockopt_SO_RCVTIMEO(std::chrono::milliseconds const& to){
            if (is_socket_error(gie::setsockopt_SO_RCVTIMEO(m_socket, to))) {
                auto const last_err = get_socket_last_error();
                throw exception::socket_error( GIE_FORMAT("setsockopt_SO_RCVTIMEO("  << to.count() <<"ms) has failed, err: "<<last_err) );
            };

            return *this;
        }

        void send(void const* buffer,  size_t len, int flags = 0){
            auto const ret_size = send_(m_socket, buffer, len, flags);
            if(ret_size!=len) {
                throw exception::socket_error(GIE_FORMAT("send("<<m_socket<<") failed, sent " << ret_size << " bytes, expected "<<len));
            }
        }

        inline size_t recv(void *buf, size_t len, int flags = 0){
            return recv_(m_socket, buf, len, flags);
        }


    private:
        socket_t m_socket;
    };


}
//================================================================================================================================================
#endif
//================================================================================================================================================
