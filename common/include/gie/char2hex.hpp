//================================================================================================================================================
// FILE: char2hex.h
// (c) GIE 2017-10-22  18:18
//
//================================================================================================================================================
#ifndef H_GUARD_CHAR2HEX_2017_10_22_18_18
#define H_GUARD_CHAR2HEX_2017_10_22_18_18
//================================================================================================================================================
#pragma once
//================================================================================================================================================
#include <cassert>
#include <string>
//================================================================================================================================================
namespace gie {

    template <typename NumT>
    char singe_hex2char(NumT const in_num){
        switch( in_num )
        {
            case 0x0: return ('0'); case 0x1: return ('1');
            case 0x2: return ('2'); case 0x3: return ('3');
            case 0x4: return ('4'); case 0x5: return ('5');
            case 0x6: return ('6'); case 0x7: return ('7');
            case 0x8: return ('8'); case 0x9: return ('9');

            case 0xA: return ('A'); case 0xB: return ('B');
            case 0xC: return ('C'); case 0xD: return ('D');
            case 0xE: return ('E'); case 0xF: return ('F');
            default:
                assert(false);
                return 'X';
        }
    }

    inline std::string blob2hex(void const* data, size_t size){
        assert(data);
        assert(size);

        auto as_chars = static_cast<unsigned char const*const>(data);

        std::string tmp;
        tmp.reserve(size*2);

        for(size_t i = 0; i<size; ++i) {
            tmp.push_back( singe_hex2char(as_chars[i] >> 4) );
            tmp.push_back( singe_hex2char(as_chars[i] & 0xF ) );
        }

        return tmp;
    }
}
//================================================================================================================================================
#endif
//================================================================================================================================================
