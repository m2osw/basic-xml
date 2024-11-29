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
    bool lint(false);
    for(int i(1); i < argc; ++i)
    {
        if(argv[i][0] == '-')
        {
            if(strcmp(argv[i], "-h") == 0
            || strcmp(argv[i], "--help") == 0)
            {
                std::cout << "Usage: basic-xml [--lint | --help | -h] [<filename>]\n";
                return 1;
            }
            else if(strcmp(argv[i], "--lint") == 0)
            {
                lint = true;
            }
            else
            {
                std::cerr << "basic-xml:error: unknownn command line option \""
                    << argv[i]
                    << "\".";
                return 1;
            }
        }
        else if(!filename.empty())
        {
            std::cerr << "basic-xml:error: filename already specified.\n";
            return 1;
        }
        else
        {
            filename = argv[i];
        }
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
        std::cout << *xml->root() << "\n";
    }

    return 0;
}

// vim: ts=4 sw=4 et
