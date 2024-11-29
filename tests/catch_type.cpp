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

// self
//
#include    "catch_main.h"


// basic-xml
//
#include    <basic-xml/exception.h>
#include    <basic-xml/type.h>



CATCH_TEST_CASE("types", "[type][valid]")
{
    CATCH_START_SECTION("is_alpha")
    {
        for(int c(0); c < 256; ++c)
        {
            if((c >= 'a' && c <= 'z')
            || (c >= 'A' && c <= 'Z')
            || c == '_')
            {
                CATCH_REQUIRE(basic_xml::is_alpha(c));
            }
            else
            {
                CATCH_REQUIRE_FALSE(basic_xml::is_alpha(c));
            }
        }
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("is_digit")
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

    CATCH_START_SECTION("is_space")
    {
        for(int c(0); c < 256; ++c)
        {
            switch(c)
            {
            case ' ':
            case '\t':
            case '\v':
            case '\f':
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

    CATCH_START_SECTION("is_token -- last cannot be '-'")
    {
        for(int count(0); count < 10; ++count)
        {
            std::string token;
            int max(rand() % 10 + 10);
            bool more(true);
            for(int len(0); len < max || more; ++len)
            {
                more = false;
                switch(rand() % (token.empty() ? 3 : 5))
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
                    token += static_cast<char>(rand() % 10 + '0');
                    break;

                case 4:
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
    CATCH_START_SECTION("is_token -- empty")
    {
        CATCH_REQUIRE_FALSE(basic_xml::is_token(std::string()));
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("is_token -- 1st must be alpha")
    {
        for(int c(1); c < 256; ++c)
        {
            if(basic_xml::is_alpha(c))
            {
                continue;
            }

            std::string token;
            token += static_cast<char>(c);

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

    CATCH_START_SECTION("is_token -- 2nd+ must be alpha or digit")
    {
        for(int c(1); c < 256; ++c)
        {
            if(basic_xml::is_alpha(c)
            || basic_xml::is_digit(c))
            {
                continue;
            }

            std::string token;
            token += static_cast<char>(rand() % 26 + 'a');

            token += static_cast<char>(c);
            CATCH_REQUIRE_FALSE(basic_xml::is_token(token));

            token[0] = static_cast<char>(rand() % 26 + 'A');
            CATCH_REQUIRE_FALSE(basic_xml::is_token(token));

            token[0] = '_';
            CATCH_REQUIRE_FALSE(basic_xml::is_token(token));
        }
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("is_token -- last cannot be '-'")
    {
        for(int count(0); count < 10; ++count)
        {
            std::string token;
            int max(rand() % 10 + 10);
            for(int len(0); len < max; ++len)
            {
                switch(rand() % (token.empty() ? 3 : 5))
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
                    token += static_cast<char>(rand() % 10 + '0');
                    break;

                case 4:
                    token += '-';
                    break;

                }
            }

            token += '-';
            CATCH_REQUIRE_FALSE(basic_xml::is_token(token));

            // make sure the token is actually valid if not ending with '-'
            // (testing the test)
            //
            token += 'q';
            CATCH_REQUIRE(basic_xml::is_token(token));
        }
    }
    CATCH_END_SECTION()
}



// vim: ts=4 sw=4 et
