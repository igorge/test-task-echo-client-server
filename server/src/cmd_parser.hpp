//================================================================================================================================================
// FILE: cmd_parser.h
// (c) GIE 2017-10-21  22:20
//
//================================================================================================================================================
#ifndef H_GUARD_CMD_PARSER_2017_10_21_22_20
#define H_GUARD_CMD_PARSER_2017_10_21_22_20
//================================================================================================================================================
#pragma once
//================================================================================================================================================
#include <functional>
#include <vector>
#include <cassert>
//================================================================================================================================================
namespace echosrv {



    struct cmd_parser_def_t {


        struct parser_t {

            explicit parser_t(cmd_parser_def_t const& def)
                : m_def(def)
                , m_possible_token_count(def.m_tokens.size())
            {
                m_parse_status.resize(def.m_tokens.size());
            }

            parser_t(parser_t&&) = default;

            std::pair<bool,bool> feed(std::string::value_type const ch){ //more entries to match, handler called
                if(m_possible_token_count==0) return {false, false};

                auto handler_called = false;

                for (size_t i = 0; i<m_parse_status.size(); ++i){
                    auto& token = m_def.m_tokens[i];
                    auto& status = m_parse_status[i];

                    if (status.current_idx==token.literal.size()) continue;

                    if (token.literal[status.current_idx]==ch) {
                        ++status.current_idx;
                        if(status.current_idx==token.literal.size()) {
                            assert(m_possible_token_count);
                            --m_possible_token_count;
                            token.handler();
                            handler_called = true;
                        }
                    } else {
                        status.current_idx = token.literal.size();
                        assert(m_possible_token_count);
                        --m_possible_token_count;
                    }

                }

                return {m_possible_token_count!=0, handler_called};

            }

            void reset(){
                m_possible_token_count = m_def.m_tokens.size();
                m_parse_status.clear();
                m_parse_status.resize(m_def.m_tokens.size());
            }

        private:

            struct token_status_t {
                size_t current_idx = 0;
            };

            cmd_parser_def_t const& m_def;

            std::vector<token_status_t> m_parse_status;
            size_t m_possible_token_count;
        };


        cmd_parser_def_t& add_token(std::string const& token, std::function<void()> const& handler){
            assert(token.size()>0);

            m_tokens.push_back({token, handler});

            return *this;
        }

        parser_t parser() const {
            return parser_t{*this};
        }

    private:

        struct token_t {
            std::string literal;
            std::function<void()> handler;
        };


        std::vector<token_t> m_tokens;
    };

}
//================================================================================================================================================
#endif
//================================================================================================================================================
