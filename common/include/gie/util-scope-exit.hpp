//================================================================================================================================================
// FILE: util-scope-exit.h
// (c) GIE 2011-02-20  02:05
//
//================================================================================================================================================
#ifndef H_GUARD_UTIL_SCOPE_EXIT_2011_02_20_02_05
#define H_GUARD_UTIL_SCOPE_EXIT_2011_02_20_02_05
//================================================================================================================================================
#pragma once
//================================================================================================================================================
#include "gie/log.hpp"

#include <utility>
//================================================================================================================================================
#define BOOST_PP_CAT_INNER(x,y) x ## y
#define BOOST_PP_CAT(x,y) BOOST_PP_CAT_INNER(x,y)
//================================================================================================================================================
namespace gie {

	template <class FunT>
	struct scope_guard_t
	{

		explicit scope_guard_t(FunT && fun) : m_fun( std::move(fun) ){}

		scope_guard_t(scope_guard_t<FunT> && other) : m_fun( std::move(other.m_fun) ){}
		scope_guard_t(scope_guard_t<FunT> const& other) : m_fun( other.m_fun ){}

		~scope_guard_t(){

			try{
				(void)(*this);
				
				m_fun();

			}catch(...){
                GIE_LOG("Unexpected in dtor");
			}

		}
	private:
		scope_guard_t<FunT>& operator=(scope_guard_t<FunT> const&);
	private:
		FunT m_fun;
	};

	template <class FunT>	
	scope_guard_t<FunT> make_guard(FunT && fun){
		return scope_guard_t<FunT>( std::move(fun) );
	}

#ifdef _WIN32
    #define GIE_UNUSED
#else
    #define GIE_UNUSED  __attribute__ ((unused))
#endif

#define GIE_SCOPE_EXIT GIE_UNUSED auto const& BOOST_PP_CAT(sg__stcrypt__, __COUNTER__) = ::gie::make_guard
	
}
//================================================================================================================================================
#endif
//================================================================================================================================================
