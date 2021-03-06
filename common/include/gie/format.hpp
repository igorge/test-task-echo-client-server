//================================================================================================================================================
// FILE: format.h
//
//
//================================================================================================================================================
#ifndef H_GUARD_TTOBJ_FORMAT
#define H_GUARD_TTOBJ_FORMAT
//================================================================================================================================================
#pragma once
//================================================================================================================================================
#include <sstream>
//================================================================================================================================================
#define GIE_FORMAT(msg)                 \
    ([&]() -> std::string {				\
            std::ostringstream ostr;    \
            ostr << msg;                \
            return ostr.str();          \
	}())                                \
    /**/

//================================================================================================================================================
#endif
//================================================================================================================================================
