//================================================================================================================================================
// FILE: sockets_exceptions.h
// (c) GIE 2017-10-19  22:11
//
//================================================================================================================================================
#ifndef H_GUARD_SOCKETS_EXCEPTIONS_2017_10_19_22_11
#define H_GUARD_SOCKETS_EXCEPTIONS_2017_10_19_22_11
//================================================================================================================================================
#pragma once
//================================================================================================================================================
namespace gie {

    namespace exception {

        struct interrupted_system_call : std::runtime_error {
            interrupted_system_call() : std::runtime_error("interrupted system call"){}
        };

        struct socket_error : virtual std::runtime_error {
            socket_error(char const*const what) : std::runtime_error(what){}
            socket_error(std::string const& what) : std::runtime_error(what){}
        };

        struct socket_initialization_error : virtual socket_error {
            socket_initialization_error(char const*const what) : std::runtime_error(what),  socket_error(what) {}
            socket_initialization_error(std::string const& what) : std::runtime_error(what), socket_error(what){}
        };


    }

}
//================================================================================================================================================
#endif
//================================================================================================================================================
