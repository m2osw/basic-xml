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


/** \file
 * \brief Database file implementation.
 *
 * Each table uses one or more files. Each file is handled by a dbfile
 * object and a corresponding set of blocks.
 */

// self
//
#include    "basic-xml/type.h"


// libutf8
//
#include    <libutf8/iterator.h>


// last include
//
#include    <snapdev/poison.h>



namespace basic_xml
{


namespace
{



// inclusive range of characters
//
struct char_range_t
{
    char32_t        f_first = U'\0';
    char32_t        f_last = U'\0';

    bool operator < (char_range_t const & rhs) const
    {
        return f_first < rhs.f_first;
    }
};


constexpr char_range_t const g_name_start_char[] =
{
    { 0x00003A, 0x00003A },     // :
    { 0x000041, 0x00005A },     // A-Z
    { 0x00005F, 0x00005F },     // _
    { 0x000061, 0x00006A },     // a-z
    { 0x0000C0, 0x0000D6 },
    { 0x0000D8, 0x0000F6 },
    { 0x0000F8, 0x0002FF },
    { 0x000370, 0x00037D },
    { 0x00037F, 0x001FFF },
    { 0x00200C, 0x00200D },
    { 0x002070, 0x00218F },
    { 0x002C00, 0x002FEF },
    { 0x003001, 0x00D7FF },
    { 0x00F900, 0x00FDCF },
    { 0x00FDF0, 0x00FFFD },
    { 0x010000, 0x0EFFFF },     // forbid all FFFE & FFFF?
};

constexpr char_range_t const g_name_char[] =
{
    { 0x00002D, 0x00002E },     // --.
    { 0x000030, 0x000039 },     // 0-9
    { 0x0000B7, 0x0000B7 },     // bullet point
    { 0x000300, 0x00036F },
    { 0x00203F, 0x002040 },
};



} // no name namespace



bool is_alpha(char c)
{
    return (c >= 'a' && c <= 'z')
        || (c >= 'A' && c <= 'Z')
        || c == '_';
}


bool is_digit(char c)
{
    return (c >= '0' && c <= '9')
        || c == '-';
}


bool is_space(char c)
{
    return c == ' '
        || c == '\t'
        || c == '\v'
        || c == '\f'
        || c == '\n'
        || c == '\r';
}


/** \brief Check whether a token is considered valid.
 *
 * This function checks whether a name represents an ASCII token. This
 * is used to make it simpler, to accept tokens that are simpler to use
 * in languages (i.e. like a name field in a structure).
 *
 * The is_token() function, on the other hand, allows many more characters
 * which means that a token can more easily not be compatible with a
 * standard language such as JavaScript.
 *
 * \param[in] token  The token to be checked.
 *
 * \return true if \p token is considered valid.
 */
bool is_ascii_token(std::string const & token)
{
    if(token.empty())
    {
        return false;
    }

    if(!is_alpha(token[0]))
    {
        return false;
    }

    std::string::size_type const max(token.length());
    for(std::string::size_type idx(1); idx < max; ++idx)
    {
        char const c(token[idx]);
        if(!is_alpha(c)
        && !is_digit(c))
        {
            return false;
        }
    }
    if(token[max - 1] == '-')
    {
        return false;
    }

    return true;
}


/** \brief Verify that \p token is a valid string.
 *
 * A token is considered valid only if it includes characters that are
 * considered valid by the XML reference.
 *
 * The following is the current XML 1.0 reference:
 *
 * \code
 * [4]  NameStartChar   ::= ":" | [A-Z] | "_" | [a-z]
 *                          | [#xC0-#xD6] | [#xD8-#xF6] | [#xF8-#x2FF]
 *                          | [#x370-#x37D] | [#x37F-#x1FFF] | [#x200C-#x200D]
 *                          | [#x2070-#x218F] | [#x2C00-#x2FEF]
 *                          | [#x3001-#xD7FF] | [#xF900-#xFDCF]
 *                          | [#xFDF0-#xFFFD] | [#x10000-#xEFFFF]
 * [4a] NameChar	    ::= NameStartChar | "-" | "." | [0-9] | #xB7
 *                          | [#x0300-#x036F] | [#x203F-#x2040]
 * [5]  Name            ::= NameStartChar (NameChar)*
 * \endcode
 *
 * \param[in] token  The token to be checked.
 *
 * \return true if the string represents a valid token name.
 */
bool is_token(std::string const & token)
{
    if(token.empty())
    {
        return false;
    }

    // here it != token.end() is always true since the token is not empty
    //
    libutf8::utf8_iterator it(token);

    char_range_t c = { *it, U'\0' };
    auto range(std::lower_bound(
          std::begin(g_name_start_char)
        , std::end(g_name_start_char)
        , c));
    if(range == std::end(g_name_start_char)
    || c.f_last > range->f_last)
    {
        return false;
    }

    for(++it; it != token.end(); ++it)
    {
        c = { *it, U'\0' };
        range = std::lower_bound(
              std::begin(g_name_char)
            , std::end(g_name_char)
            , c);
        if(range == std::end(g_name_char)
        || c.f_last > range->f_last)
        {
            return false;
        }
    }

    return true;
}



} // namespace basic_xml
// vim: ts=4 sw=4 et
