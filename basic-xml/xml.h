// Copyright (c) 2019-2024  Made to Order Software Corp.  All Rights Reserved
//
// https://snapwebsites.org/project/prinbee
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
#pragma once


/** \file
 * \brief Very simple XML parser.
 *
 * The definitions of the Snap! Database tables is done in XML files.
 * These classes are used to load and parse those files to use them
 * as the schema definition of the tables.
 */

// self
//
#include    <basic-xml/node.h>



namespace basic_xml
{



class xml
{
public:
    typedef std::shared_ptr<xml>    pointer_t;
    typedef std::map<std::string, pointer_t>
                                    map_t;

                                    xml(std::string const & filename);
                                    xml(std::string const & filename, std::istream & in);

    node::pointer_t                 root();

private:
    node::pointer_t                 f_root = node::pointer_t();
};



} // namespace prinbee
// vim: ts=4 sw=4 et
