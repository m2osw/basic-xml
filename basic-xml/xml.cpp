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
#include    "basic-xml/xml.h"

#include    "basic-xml/exception.h"
#include    "basic-xml/parser.h"

// C++
//
#include    <fstream>


// C
//
#include    <string.h>


// last include
//
#include    <snapdev/poison.h>



/** \mainpage
 *
 * The Basic XML library and tools are used to handle very basic XML files.
 * At the moment, the library can load a file composed of XML tags with
 * attributes and text. It does not support anything more. This is primarily
 * useful for simple tree based files defining settings.
 *
 * You can use this library to read simple XML files. Just define an XML
 * object and get the root to go through the tree of nodes.
 *
 * \code
 *     basic_xml::xml x(filename);
 *     basic_xml::node::pointer_t root(x.root());
 *     ...
 * \endcode
 *
 * The nodes have the following basic functions:
 *
 * * node::tag_name() -- returns the name of the tag, it has to be a valid token
 * * node::text() -- returns the text found between the start and end tags
 * * node::attribute() -- retrieve the named attribute
 * * node::first_child() -- get the first child of that node
 * * node::next() -- get the next node from this node
 *
 * Other functions are available but the ones above should generally be
 * sufficient to go through all the nodes as required by your application.
 *
 * The project includes the `basic-xml` tool which can be used to reformat
 * an XML file in one long line, extract a value, or lint a file.
 *
 * The library allows you to specify a C++ stream as input. This allows you
 * to read from stdin:
 *
 * \code
 *     basic_xml::xml x("stdin", std::cin);
 *     ...
 * \endcode
 *
 * The library can be used to safely create an XML file (i.e. making sure
 * that closing tags will always be present, that tag names are valid
 * tokens, that attributes are defined as expected, etc.) For that purpose,
 * create a root node and then print the result once done.
 *
 * \code
 *     basic_xml::node::pointer_t root(std::make_shared<basic_xml::node>("simple"));
 *
 *     basic_xml::node::pointer_t node_one(std::make_shared<basic_xml::node>("one"));
 *     one.set_text("some data here");
 *     root.append_child(node_one);
 *
 *     std::cout << *root << "\n";
 * \endcode
 *
 * Note that the library does not allow for deleting tags. In other words,
 * you need to create the correct file from the start.
 *
 * \warning
 * The library reads the entire file in memory. It is not expected to be
 * used to stream XML data.
 */


namespace basic_xml
{



xml::xml(std::string const & filename)
{
    std::ifstream in(filename);
    if(!in.is_open())
    {
        int const e(errno);
        throw file_not_found("could not open XML file \""
                           + filename
                           + "\": " + strerror(e) + ".");
    }

    parser p(filename, in, f_root);
}


xml::xml(std::string const & filename, std::istream & in)
{
    parser p(filename, in, f_root);
}


node::pointer_t xml::root()
{
    return f_root;
}



} // namespace prinbee
// vim: ts=4 sw=4 et
