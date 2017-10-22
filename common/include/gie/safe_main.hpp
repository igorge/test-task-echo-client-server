//================================================================================================================================================
// FILE: safe_main.h
// (c) GIE 2011-07-11  15:18
//
//================================================================================================================================================
#ifndef H_GUARD_SAFE_MAIN_2011_07_11_15_18
#define H_GUARD_SAFE_MAIN_2011_07_11_15_18
//================================================================================================================================================
#pragma once
//================================================================================================================================================
#include "gie/log.hpp"

#include <typeinfo>
#include <exception>
#include <iostream>
//================================================================================================================================================
namespace gie {

    namespace impl {

        inline void e_filter_log_exception(){
            try {
                throw;
            } catch( std::exception const & e ) {
                GIE_LOG( "\n======= uncaught exception =======\n" << typeid(e).name() << "\nwhat(): " << e.what() );
                throw ;
            } catch( ... ) {
                GIE_LOG( "\n======= unknown uncaught exception =======" );
                throw ;
            }
        }

        inline
        int safe_main_exception_filter(){
            try{ e_filter_log_exception(); } catch(...){ }
            return EXIT_FAILURE;
        }
    }

    template <class MainFun>
    typename std::enable_if< std::is_same<void, typename std::result_of<MainFun()>::type >::value, int>::type
    main(MainFun const& fun){
        try {
            fun();
        } catch(...){
            return impl::safe_main_exception_filter();
        }
        
        return EXIT_SUCCESS;
    }

    template <class MainFun>
    typename std::enable_if< std::is_integral<typename std::result_of<MainFun()>::type >::value, int>::type
    main(MainFun const& fun){
        try {
            return fun();
        } catch(...){
            return impl::safe_main_exception_filter();
        }
    }

}
//================================================================================================================================================
#endif
//================================================================================================================================================
