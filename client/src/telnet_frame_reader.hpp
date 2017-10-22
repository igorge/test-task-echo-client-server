//================================================================================================================================================
// FILE: telnet_frame_reader.h
// (c) GIE 2017-10-22  22:21
//
//================================================================================================================================================
#ifndef H_GUARD_TELNET_FRAME_READER_2017_10_22_22_21
#define H_GUARD_TELNET_FRAME_READER_2017_10_22_22_21
//================================================================================================================================================
#pragma once
//================================================================================================================================================
#include "sockets.hpp"
#include "exceptions.hpp"

#include <vector>
#include <cassert>
//================================================================================================================================================
namespace echosrv {
    struct telnet_frame_reader_t {
        telnet_frame_reader_t(gie::socket& s) : m_s(s) {
            buffer.reserve(1024);
        }

        std::string recv(){

            for(;;){
                if (buffer.empty()){
                    buffer.resize(buffer.capacity());
                    assert(buffer.size()>0);

                    auto const r = m_s.recv(buffer.data(), buffer.size());
                    if (r==0) throw exception::terminate_session("Connection closed by foreign host");
                    buffer.resize(r);
                } else {
                    for(size_t i=0; i<buffer.size(); ++i){
                        if (buffer[i]=='\r') {
                            // just ignore
                        } else if (buffer[i]=='\n') {
                            std::string frame{std::move(m_current_frame)};

                            m_current_frame.clear();
                            buffer.erase( buffer.begin(), buffer.begin()+i+1);

                            return frame;
                        } else {
                            m_current_frame.push_back(buffer[i]);
                        }
                    }
                }
            }

        }

    private:

    private:
        gie::socket& m_s;
        std::vector<char> buffer;
        std::string m_current_frame;

    };

}
//================================================================================================================================================
#endif
//================================================================================================================================================
