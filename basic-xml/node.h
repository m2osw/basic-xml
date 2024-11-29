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
 * \brief Node for the basic XML tree.
 *
 * The following declares the basic XML node object.
 */

// C++
//
#include    <deque>
#include    <map>
#include    <memory>
#include    <ostream>
#include    <vector>



namespace basic_xml
{



class node
    : public std::enable_shared_from_this<node>
{
public:
    typedef std::shared_ptr<node>   pointer_t;
    typedef std::map<std::string, pointer_t>
                                    map_t;
    typedef std::weak_ptr<node>     weak_pointer_t;
    typedef std::map<std::string, std::string>
                                    attribute_map_t;
    typedef std::vector<pointer_t>  vector_t;
    typedef std::deque<pointer_t>   deque_t;

                                    node(std::string const & name);

    std::string const &             tag_name() const;
    std::string                     text(bool trim = true) const;
    void                            set_text(std::string const & text);
    void                            append_text(std::string const & text);
    attribute_map_t                 all_attributes() const;
    std::string                     attribute(std::string const & name) const;
    void                            set_attribute(std::string const & name, std::string const & value);
    void                            append_child(pointer_t n);

    pointer_t                       root() const;
    pointer_t                       parent() const;
    pointer_t                       first_child() const;
    pointer_t                       last_child() const;
    pointer_t                       next() const;
    pointer_t                       previous() const;

private:
    std::string const               f_name;
    std::string                     f_text = std::string();
    attribute_map_t                 f_attributes = attribute_map_t();

    pointer_t                       f_next = pointer_t();
    weak_pointer_t                  f_previous = weak_pointer_t();

    pointer_t                       f_child = pointer_t();
    weak_pointer_t                  f_parent = weak_pointer_t();
};


extern std::string convert_to_entity(std::string const & raw, std::string const & which);
extern std::ostream & operator << (std::ostream & out, node const & n);



} // namespace basic_xml
// vim: ts=4 sw=4 et
