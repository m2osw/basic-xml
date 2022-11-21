// Copyright (c) 2019-2022  Made to Order Software Corp.  All Rights Reserved
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


// last include
//
#include    <snapdev/poison.h>



namespace basic_xml
{



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


bool is_token(std::string const s)
{
    if(s.empty())
    {
        return false;
    }

    if(!is_alpha(s[0]))
    {
        return false;
    }

    std::string::size_type const max(s.length());
    for(std::string::size_type idx(1); idx < max; ++idx)
    {
        char const c(s[idx]);
        if(!is_alpha(c)
        && !is_digit(c))
        {
            return false;
        }
    }
    if(s[max - 1] == '-')
    {
        return false;
    }

    return true;
}



} // namespace basic_xml
// vim: ts=4 sw=4 et
