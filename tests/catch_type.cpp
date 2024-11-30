// Copyright (c) 2019-2024  Made to Order Software Corp.  All Rights Reserved
//
// https://snapwebsites.org/project/basic-xml
// contact@m2osw.com
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

// basic-xml
//
#include    <basic-xml/type.h>

#include    <basic-xml/exception.h>


// self
//
#include    "catch_main.h"


// libutf8
//
#include    <libutf8/libutf8.h>



namespace
{


bool is_xml_alpha_start(char32_t c)
{
    return c == ':'
        || (c >= 'A' && c <= 'Z')
        || c == '_'
        || (c >= 'a' && c <= 'z')
        || (c >= U'\x0000C0' && c <= U'\x0000D6') // TBD: use Unicode character names?
        || (c >= U'\x0000D8' && c <= U'\x0000F6')
        || (c >= U'\x0000F8' && c <= U'\x0002FF')
        || (c >= U'\x000370' && c <= U'\x00037D')
        || (c >= U'\x00037F' && c <= U'\x001FFF')
        || (c >= U'\x00200C' && c <= U'\x00200D')
        || (c >= U'\x002070' && c <= U'\x00218F')
        || (c >= U'\x002C00' && c <= U'\x002FEF')
        || (c >= U'\x003001' && c <= U'\x00D7FF')
        || (c >= U'\x00F900' && c <= U'\x00FDCF')
        || (c >= U'\x00FDF0' && c <= U'\x00FFFD')
        || (c >= U'\x010000' && c <= U'\x0EFFFF');
}


bool is_xml_alpha(char32_t c)
{
    return c == '-'
        || c == '.'
        || (c >= '0' && c <= '9')
        || c == ':'
        || (c >= 'A' && c <= 'Z')
        || c == '_'
        || (c >= 'a' && c <= 'z')
        || c == U'\x0000B7'
        || (c >= U'\x0000C0' && c <= U'\x0000D6') // TBD: use Unicode character names?
        || (c >= U'\x0000D8' && c <= U'\x0000F6')
        || (c >= U'\x0000F8' && c <= U'\x0002FF')
        || (c >= U'\x000300' && c <= U'\x00037D')
        || (c >= U'\x00037F' && c <= U'\x001FFF')
        || (c >= U'\x00200C' && c <= U'\x00200D')
        || (c >= U'\x00203F' && c <= U'\x002040')
        || (c >= U'\x002070' && c <= U'\x00218F')
        || (c >= U'\x002C00' && c <= U'\x002FEF')
        || (c >= U'\x003001' && c <= U'\x00D7FF')
        || (c >= U'\x00F900' && c <= U'\x00FDCF')
        || (c >= U'\x00FDF0' && c <= U'\x00FFFD')
        || (c >= U'\x010000' && c <= U'\x0EFFFF');
}



} // no name namespace



CATCH_TEST_CASE("types", "[type][valid]")
{
    CATCH_START_SECTION("types: is_name_start_char()")
    {
        for(char32_t c(0); c < 0x110000; ++c)
        {
            if(is_xml_alpha_start(c))
            {
                CATCH_REQUIRE(basic_xml::is_name_start_char(c));
            }
            else
            {
                CATCH_REQUIRE_FALSE(basic_xml::is_name_start_char(c));
            }
        }
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("types: is_name_char()")
    {
        for(char32_t c(0); c < 0x110000; ++c)
        {
            if(is_xml_alpha(c))
            {
                CATCH_REQUIRE(basic_xml::is_name_char(c));
            }
            else
            {
                CATCH_REQUIRE_FALSE(basic_xml::is_name_char(c));
            }
        }
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("types: is_digit()")
    {
        for(int c(0); c < 256; ++c)
        {
            if((c >= '0' && c <= '9')
            || c == '-')
            {
                CATCH_REQUIRE(basic_xml::is_digit(c));
            }
            else
            {
                CATCH_REQUIRE_FALSE(basic_xml::is_digit(c));
            }
        }
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("types: is_space()")
    {
        for(int c(0); c < 256; ++c)
        {
            switch(c)
            {
            case ' ':
            case '\t':
            case '\n':
            case '\r':
                CATCH_REQUIRE(basic_xml::is_space(c));
                break;

            default:
                CATCH_REQUIRE_FALSE(basic_xml::is_space(c));
                break;

            }
        }
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("types: is_token()")
    {
        for(int count(0); count < 10; ++count)
        {
            std::string token;
            int max(rand() % 10 + 10);
            bool more(true);
            for(int len(0); len < max || more; ++len)
            {
                more = false;
                switch(rand() % (token.empty() ? 4 : 6))
                {
                case 0:
                    token += static_cast<char>(rand() % 26 + 'a');
                    break;

                case 1:
                    token += static_cast<char>(rand() % 26 + 'A');
                    break;

                case 2:
                    token += '_';
                    break;

                case 3:
                    token += ':';   // this is a strange one, it should only be used for namespaces, but it is allowed as per the reference
                    break;

                case 4:
                    token += static_cast<char>(rand() % 10 + '0');
                    break;

                case 5:
                    token += '-';
                    more = true; // do not end with '-'
                    break;

                }
            }

            CATCH_REQUIRE(basic_xml::is_token(token));
        }
    }
    CATCH_END_SECTION()
}



CATCH_TEST_CASE("invalid_tokens", "[type][invalid]")
{
    CATCH_START_SECTION("invalid_tokens: is_token -- empty")
    {
        CATCH_REQUIRE_FALSE(basic_xml::is_token(std::string()));
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("invalid_tokens: is_token -- 1st must be alpha")
    {
        for(char32_t c(1); c < 0x110000; ++c)
        {
            if(c >= 0xD800 && c <= 0xDFFF)
            {
                // those are not characters, so don't bother (the libutf8::to_u8string() will fail
                // here and would fail there but more or less we can't pass those in UTF-8)
                //
                continue;
            }
            if(is_xml_alpha(c))
            {
                continue;
            }

            std::string token;
            token += libutf8::to_u8string(c);

            CATCH_REQUIRE_FALSE(basic_xml::is_token(token));

            token += static_cast<char>(rand() % 26 + 'a');
            CATCH_REQUIRE_FALSE(basic_xml::is_token(token));

            token[1] = static_cast<char>(rand() % 26 + 'A');
            CATCH_REQUIRE_FALSE(basic_xml::is_token(token));

            token[1] = '_';
            CATCH_REQUIRE_FALSE(basic_xml::is_token(token));

            token[1] = static_cast<char>(rand() % 10 + '0');
            CATCH_REQUIRE_FALSE(basic_xml::is_token(token));

            token[1] = '-';
            token += 'g';
            CATCH_REQUIRE_FALSE(basic_xml::is_token(token));
        }
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("invalid_tokens: is_token -- 2nd+ must be alpha or digit")
    {
        for(char32_t c(1); c < 0x110000; ++c)
        {
            if(c >= 0xD800 && c <= 0xDFFF)
            {
                // those are not characters, so don't bother (the libutf8::to_u8string() will fail
                // here and would fail there but more or less we can't pass those in UTF-8)
                //
                continue;
            }
            if(is_xml_alpha(c)
            || basic_xml::is_digit(c))
            {
                continue;
            }

            std::string token;
            token += static_cast<char>(rand() % 26 + 'a');

            token += libutf8::to_u8string(c);
            CATCH_REQUIRE_FALSE(basic_xml::is_token(token));

            token[0] = static_cast<char>(rand() % 26 + 'A');
            CATCH_REQUIRE_FALSE(basic_xml::is_token(token));

            token[0] = '_';
            CATCH_REQUIRE_FALSE(basic_xml::is_token(token));
        }
    }
    CATCH_END_SECTION()
}



// vim: ts=4 sw=4 et
