// Copyright (c) 2022-2024  Made to Order Software Corp.  All Rights Reserved
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
#include    <basic-xml/exception.h>
#include    <basic-xml/xml.h>


// snapdev
//
#include    <snapdev/tokenize_string.h>


// C++
//
#include    <iostream>


// C
//
#include    <string.h>


// last include
//
#include    <snapdev/poison.h>







int main(int argc, char * argv[])
{
    std::string filename;
    std::string path;
    bool lint(false);
    bool verbose(false);
    for(int i(1); i < argc; ++i)
    {
        if(argv[i][0] == '-')
        {
            if(strcmp(argv[i], "-h") == 0
            || strcmp(argv[i], "--help") == 0)
            {
                std::cout << "Usage: basic-xml [--lint | --verbose | --help | -h] [<filename>] [xpath]\n";
                return 1;
            }
            else if(strcmp(argv[i], "--lint") == 0)
            {
                lint = true;
            }
            else if(strcmp(argv[i], "--verbose") == 0)
            {
                verbose = true;
            }
            else
            {
                std::cerr << "basic-xml:error: unknown command line option \""
                    << argv[i]
                    << "\".";
                return 1;
            }
        }
        else if(filename.empty())
        {
            filename = argv[i];
        }
        else if(path.empty())
        {
            path = argv[i];
        }
        else
        {
            std::cerr << "basic-xml:error: filename and path already specified.\n";
            return 1;
        }
    }

    if(lint && !path.empty())
    {
        std::cerr << "basic-xml:warning: path ignored when --lint is used.\n";
    }

    basic_xml::xml * xml(nullptr);
    try
    {
        if(filename.empty())
        {
            xml = new basic_xml::xml("stdin", std::cin);
        }
        else
        {
            xml = new basic_xml::xml(filename);
        }
    }
    catch(basic_xml::xml_error const & e)
    {
        std::cerr << "basic-xml:error: an error occurred: " << e.what() << "\n";
        return 1;
    }

    if(!lint)
    {
        if(!path.empty())
        {
            if(path[0] != '/')
            {
                std::cerr << "basic-xml:warning: path should always start with a '/'.\n";
            }

            // TODO: add proper support for '//' (i.e. any tag of that name from that point on)
            //
            std::list<std::string> segments;
            snapdev::tokenize_string(segments, path, "/");
            std::string attribute_name;
            for(auto const & s : segments)
            {
                if(!s.empty()
                && s[0] == '@')
                {
                    attribute_name = s.substr(1);
                }
                else if(!attribute_name.empty())
                {
                    std::cerr << "basic-xml:warning: path can only refer to one attribute (last segment in path).\n";
                    return 1;
                }
            }
            if(!attribute_name.empty())
            {
                segments.pop_back();
            }

            // because the path is expected to start with "/", we get an
            // "invalid" empty string in the very first list item
            //
            if(!segments.empty()
            && segments.front().empty())
            {
                if(path.length() >= 2
                && path[0] == '/'
                && path[1] != '/')
                {
                    segments.pop_front();
                }
            }

            // if the path ends with a '/' will get one more empty entry here
            //
            if(!segments.empty()
            && segments.back().empty())
            {
                segments.pop_back();
            }

            // find the node of interest
            //
            // TODO: move that code to the library, after all, we should be
            //       able to search for a node from the xml object instead
            //
            basic_xml::node::pointer_t n(xml->root());
            basic_xml::node::pointer_t q(n);
            for(auto const & s : segments)
            {
                if(s.empty())
                {
                    std::cerr << "basic-xml:warning: the // in a path is not yet supported.\n";
                    return 1;
                }

                n = q;
                for(;;)
                {
                    if(n->tag_name() == s)
                    {
                        q = n->first_child();
                        break;
                    }
                    n = n->next();
                    if(n == nullptr)
                    {
                        // node not found
                        //
                        if(verbose)
                        {
                            std::cout << "basic-xml:info: node \"" << s << "\" not found.\n";
                        }
                        return 1;
                    }
                }
            }

            if(!attribute_name.empty())
            {
                std::string const attr(n->attribute(attribute_name));
                if(attr.empty())
                {
                    // attribute not found
                    //
                    if(verbose)
                    {
                        std::cout << "basic-xml:info: attribute \"" << attribute_name << "\" not found.\n";
                    }
                    return 1;
                }
                std::cout << attr << "\n";
            }
            else
            {
                std::cout << n->text() << "\n";
            }
        }
        else
        {
            std::cout << *xml->root() << "\n";
        }
    }

    return 0;
}

// vim: ts=4 sw=4 et
