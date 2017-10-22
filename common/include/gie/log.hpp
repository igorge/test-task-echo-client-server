//================================================================================================================================================
// FILE: log.h
// (c) GIE 2011-07-11  15:21
//
//================================================================================================================================================
#ifndef H_GUARD_LOG_2011_07_11_15_21
#define H_GUARD_LOG_2011_07_11_15_21
//================================================================================================================================================
#pragma once
//================================================================================================================================================

#include <sstream>
#include <cstdio>

#include <assert.h>

//#define GIE_LOGGER_DO_NOT_LOG_THREAD_ID

#ifndef GIE_LOGGER_DO_NOT_LOG_THREAD_ID
	#include <thread>

	#define GIE_IMPL_LOGGER_LOG_TID "["<< ::std::this_thread::get_id() << "] " <<
#else
	#define GIE_IMPL_LOGGER_LOG_TID
#endif

#ifndef GIE_LOGGER_DO_NOT_LOG_TIME

	#ifdef _WIN32
		#define GIE_IMPL_LOGGER_LOG_TIME "["<<  ::time(nullptr) << "] " <<
	#else
        #define GIE_IMPL_LOGGER_LOG_TIME "["<<  std::time(nullptr) << "] " <<
	#endif

#else
	#define GIE_IMPL_LOGGER_LOG_TIME
#endif

//================================================================================================================================================
namespace gie { namespace logger { namespace impl {
	template<class OStream>
	OStream& ostream_wrapper_t(OStream& os){
		return os;
	}
} } }
//================================================================================================================================================
#ifndef GIE_LOG_OPEN_TAG
	#define GIE_LOG_OPEN_TAG ""
#endif
#ifndef GIE_LOG_CLOSE_TAG
	#define GIE_LOG_CLOSE_TAG ""
#endif

#define GIE_LOG_EXT(msg, file, line)  \
        do{ try {               \
                std::ostringstream ostr;        \
                ::gie::logger::impl::ostream_wrapper_t(ostr) << "LOG: " << GIE_IMPL_LOGGER_LOG_TIME GIE_IMPL_LOGGER_LOG_TID  "["<<file<<":"<<line<<"] " << GIE_LOG_OPEN_TAG <<  msg << GIE_LOG_CLOSE_TAG << "\n"; \
                std::fputs( ostr.str().c_str(), stderr );  \
				std::fflush(stderr); \
                } catch(...) { ::gie::debug::exception_while_formatting_logging_message(); } \
        } while(false) \
        /**/

#define GIE_LOG(msg) GIE_LOG_EXT(msg, __FILE__, __LINE__)

namespace gie { namespace debug {

    inline void exception_while_formatting_logging_message(){
        assert(false);
    }
    
} }
//================================================================================================================================================
#endif
//================================================================================================================================================
