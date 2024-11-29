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

// basic-xml
//
#include    <basic-xml/version.h>


// self
//
#include    "catch_main.h"


// last include
//
#include    <snapdev/poison.h>




CATCH_TEST_CASE("version", "[version][valid]")
{
    CATCH_START_SECTION("verify runtime vs compile time version numbers")
    {
        CATCH_REQUIRE(basic_xml::get_major_version()   == BASIC_XML_VERSION_MAJOR);
        CATCH_REQUIRE(basic_xml::get_release_version() == BASIC_XML_VERSION_MINOR);
        CATCH_REQUIRE(basic_xml::get_patch_version()   == BASIC_XML_VERSION_PATCH);
        CATCH_REQUIRE(strcmp(basic_xml::get_version_string(), BASIC_XML_VERSION_STRING) == 0);
    }
    CATCH_END_SECTION()
}


// vim: ts=4 sw=4 et
