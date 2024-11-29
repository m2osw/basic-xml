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
#include    <basic-xml/node.h>


// C++
//
#include    <istream>



namespace basic_xml
{



class parser
{
public:
                        parser(std::string const & filename, std::istream & in, node::pointer_t & root);

private:
    enum class token_t
    {
        TOK_CLOSE_TAG,
        TOK_EMPTY_TAG,
        TOK_END_TAG,
        TOK_EOF,
        TOK_EQUAL,
        TOK_IDENTIFIER,
        TOK_OPEN_TAG,
        TOK_PROCESSOR,
        TOK_STRING,
        TOK_TEXT
    };

    void                load(node::pointer_t & root);
    token_t             skip_empty(token_t tok);
    token_t             read_tag_attributes(node::pointer_t & tag);
    token_t             get_token(bool parsing_attributes);
    void                unescape_entities();
    int                 getc();
    void                ungetc(int c);

    std::string         f_filename = std::string();
    std::istream &      f_in;
    size_t              f_ungetc_pos = 0;
    int                 f_ungetc[4] = { '\0' };
    int                 f_line = 1;
    std::string         f_value = std::string();
};



} // namespace basic_xml
// vim: ts=4 sw=4 et
