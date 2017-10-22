//================================================================================================================================================
// FILE: app.h
// (c) GIE 2017-10-22  19:15
//
//================================================================================================================================================
#ifndef H_GUARD_APP_2017_10_22_19_15
#define H_GUARD_APP_2017_10_22_19_15
//================================================================================================================================================
#pragma once
//================================================================================================================================================
#include "utc_time.hpp"

#include "gie/format.hpp"
#include "gie/log.hpp"

#include <atomic>
#include <mutex>
#include <fstream>
#include <string>
//================================================================================================================================================
namespace echosrv {

    struct app_t {

        app_t()
            : m_log("echosrv.log", std::ios_base::out | std::ios_base::app)
        {
            if (m_log.bad()) {
                throw std::runtime_error("failed to open log file");
            }
        }

        app_t(app_t const&) = delete;

        void inc_session(){
            ++m_sessions_count;
        }

        void dec_session(){
            --m_sessions_count;
        }

        unsigned int num_of_sessions(){
            return m_sessions_count;
        }

        void log(std::string const& msg){
            std::string const formatted_msg = GIE_FORMAT(utc_time() << ", " << msg);


            {
                std::lock_guard<std::mutex> guard(m_log_lock);
                m_log << formatted_msg << std::endl;
            }

            if (m_log.bad()) {
                GIE_LOG("Failed to write into log file: " << formatted_msg );
            }
        }

    private:
        std::atomic_uint m_sessions_count{0};
        std::ofstream m_log;
        std::mutex m_log_lock;
    };

}
//================================================================================================================================================
#endif
//================================================================================================================================================
