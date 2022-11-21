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
 * \brief XML Nodes.
 *
 * Each tag found in the input is transformed in a node. A node includes
 * the tag name, attributes, and the text found inside the node. There is
 * no separate node created for text data so if your node includes
 * multiple text strings between tags, there is no way to know the correct
 * order. We do not offer that capability.
 */

// self
//
#include    "basic-xml/node.h"

#include    "basic-xml/exception.h"
#include    "basic-xml/type.h"


// snapdev
//
#include    <snapdev/not_reached.h>
#include    <snapdev/trim_string.h>


// last include
//
#include    <snapdev/poison.h>



namespace basic_xml
{



node::node(std::string const & name)
    : f_name(name)
{
    if(!is_token(name))
    {
        throw invalid_token("\"" + name + "\" is not a valid token for a tag name.");
    }
}


std::string const & node::tag_name() const
{
    return f_name;
}


std::string node::text(bool trim) const
{
    if(trim)
    {
        return snapdev::trim_string(f_text);
    }
    return f_text;
}


void node::set_text(std::string const & text)
{
    f_text = text;
}


void node::append_text(std::string const & text)
{
    f_text += text;
}


node::attribute_map_t node::all_attributes() const
{
    return f_attributes;
}


std::string node::attribute(std::string const & name) const
{
    auto const it(f_attributes.find(name));
    if(it == f_attributes.end())
    {
        return std::string();
    }
    return it->second;
}


void node::set_attribute(std::string const & name, std::string const & value)
{
    if(!is_token(name))
    {
        throw invalid_token("\"" + name + "\" is not a valid token for an attribute name.");
    }
    f_attributes[name] = value;
}


void node::append_child(pointer_t n)
{
    if(n->f_next != nullptr
    || n->f_previous.lock() != nullptr
    || n->f_parent.lock() != nullptr)
    {
        throw node_already_in_tree("Somehow you are trying to add a child node of a node that was already added to a tree of nodes.");
    }
    if(n == root())
    {
        throw node_is_root("Trying to append the root node within the sub-tree.");
    }

    auto l(last_child());
    if(l == nullptr)
    {
        f_child = n;
    }
    else
    {
        l->f_next = n;
        n->f_previous = l;
    }

    n->f_parent = shared_from_this();
}


node::pointer_t node::root() const
{
    node::pointer_t result(const_cast<node *>(this)->shared_from_this());
    for(;;)
    {
        node::pointer_t p(result->parent());
        if(p == nullptr)
        {
            return result;
        }
        result = p;
    }
    snapdev::NOT_REACHED();
}


node::pointer_t node::parent() const
{
    auto result(f_parent.lock());
    return result;
}


node::pointer_t node::first_child() const
{
    return f_child;
}


node::pointer_t node::last_child() const
{
    if(f_child == nullptr)
    {
        return node::pointer_t();
    }

    pointer_t l(f_child);
    while(l->f_next != nullptr)
    {
        l = l->f_next;
    }

    return l;
}


node::pointer_t node::next() const
{
    return f_next;
}


node::pointer_t node::previous() const
{
    return f_previous.lock();
}


std::string convert_to_entity(std::string const & raw, std::string const & which)
{
    std::string result;
    result.reserve(raw.length() + 10);
    for(auto const & c : raw)
    {
        if(which.find(c) == std::string::npos)
        {
            result += c;
        }
        else
        {
            switch(c)
            {
            case '&':
                result += "&amp;";
                break;

            case '<':
                result += "&lt;";
                break;

            case '>':
                result += "&gt;";
                break;

            case '"':
                result += "&quot;";
                break;

            case '\'':
                result += "&apos;";
                break;

            }
        }
    }
    return result;
}

std::ostream & operator << (std::ostream & out, node const & n)
{
    out << '<';
    out << n.tag_name();
    for(auto const & a : n.all_attributes())
    {
        // use attr='...' if the string includes one or more `"` and no
        // apostrophe otherwise use attr="..." and convert any `"` with &quot;
        // that way we never need &apos;
        //
        char const quote(a.second.find('"') != std::string::npos
                        && a.second.find('\'') == std::string::npos ? '\'' : '"');
        out << ' '
            << a.first
            << '='
            << quote
            << (quote == '"'
                    ? convert_to_entity(a.second, "&<>\"")
                    : convert_to_entity(a.second, "&<>"))
            << quote;
    }
    auto child(n.first_child());
    bool empty(child == nullptr && n.text().empty() && n.parent() != nullptr);
    if(empty)
    {
        out << '/';
    }
    out << '>';
    if(child != nullptr)
    {
        while(child != nullptr)
        {
            out << *child;       // recursive call
            child = child->next();
        }
    }
    if(!n.text().empty())
    {
        // in this case we can safely keep the " as is instead of &quot;
        //
        out << convert_to_entity(n.text(), "&<>");
    }
    if(!empty)
    {
        out << "</"
            << n.tag_name()
            << '>';
    }

    return out;
}



} // namespace prinbee
// vim: ts=4 sw=4 et
