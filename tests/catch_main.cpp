// Copyright (c) 2021-2024  Made to Order Software Corp.  All Rights Reserved
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

// Tell catch we want it to add the runner code in this file.
#define CATCH_CONFIG_RUNNER

// self
//
#include    "catch_main.h"


// basic-xml
//
#include    <basic-xml/version.h>


// libexcept
//
#include    <libexcept/exception.h>


// last include
//
#include    <snapdev/poison.h>



namespace SNAP_CATCH2_NAMESPACE
{



std::string get_folder_name()
{
    std::string const xml_path(SNAP_CATCH2_NAMESPACE::g_tmp_dir() + "/xml");

    if(snapdev::mkdir_p(xml_path.c_str(), false, 0700) != 0)
    {
        if(errno != EEXIST)
        {
            perror(("could not create directory \"" + xml_path + "\"").c_str());
            return std::string();
        }
    }

    return xml_path;
}



} // namespace 



int main(int argc, char * argv[])
{
    return SNAP_CATCH2_NAMESPACE::snap_catch2_main(
              "basic-xml"
            , BASIC_XML_VERSION_STRING
            , argc
            , argv
            , []() { libexcept::set_collect_stack(libexcept::collect_stack_t::COLLECT_STACK_NO); }
        );
}


// vim: ts=4 sw=4 et
