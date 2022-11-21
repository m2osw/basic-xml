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

// self
//
#include    "catch_main.h"


// basic-xml
//
#include    <basic-xml/exception.h>
#include    <basic-xml/xml.h>


// C++
//
#include    <fstream>



CATCH_TEST_CASE("xml", "[xml][valid]")
{
    CATCH_START_SECTION("empty root")
    {
        std::string const xml_path(SNAP_CATCH2_NAMESPACE::get_folder_name());
        std::string const filename(xml_path + "/empty-tag.xml");

        // create file with one empty tag
        {
            std::ofstream f;
            f.open(filename);
            CATCH_REQUIRE(f.is_open());
            f << "<empty></empty>";
        }

        basic_xml::xml x(filename);
        basic_xml::node::pointer_t root(x.root());
        CATCH_REQUIRE(root != nullptr);
        CATCH_REQUIRE(root->tag_name() == "empty");
        CATCH_REQUIRE(root->text().empty());
        CATCH_REQUIRE(root->all_attributes().empty());
        CATCH_REQUIRE(root->parent() == nullptr);
        CATCH_REQUIRE(root->first_child() == nullptr);
        CATCH_REQUIRE(root->last_child() == nullptr);
        CATCH_REQUIRE(root->next() == nullptr);
        CATCH_REQUIRE(root->previous() == nullptr);
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("empty root with preprocessor")
    {
        std::string const filename("empty-tag.xml");

        // create file with one empty tag
        std::stringstream ss;
        ss << "<?xml version=\"1.0\"?><still-empty></still-empty>";

        basic_xml::xml x(filename, ss);
        basic_xml::node::pointer_t root(x.root());
        CATCH_REQUIRE(root != nullptr);
        CATCH_REQUIRE(root->tag_name() == "still-empty");
        CATCH_REQUIRE(root->text().empty());
        CATCH_REQUIRE(root->all_attributes().empty());
        CATCH_REQUIRE(root->parent() == nullptr);
        CATCH_REQUIRE(root->first_child() == nullptr);
        CATCH_REQUIRE(root->last_child() == nullptr);
        CATCH_REQUIRE(root->next() == nullptr);
        CATCH_REQUIRE(root->previous() == nullptr);
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("empty root with comment & preprocessor")
    {
        std::string const xml_path(SNAP_CATCH2_NAMESPACE::get_folder_name());
        std::string const filename(xml_path + "/quite-empty.xml");

        // create file with one empty tag
        {
            std::ofstream f;
            f.open(filename);
            CATCH_REQUIRE(f.is_open());
            f << "<!-- name='rotor' --><?xml version=\"1.0\"?><quite-empty></quite-empty>";
        }

        basic_xml::xml x(filename);
        basic_xml::node::pointer_t root(x.root());
        CATCH_REQUIRE(root != nullptr);
        CATCH_REQUIRE(root->tag_name() == "quite-empty");
        CATCH_REQUIRE(root->text().empty());
        CATCH_REQUIRE(root->all_attributes().empty());
        CATCH_REQUIRE(root->parent() == nullptr);
        CATCH_REQUIRE(root->first_child() == nullptr);
        CATCH_REQUIRE(root->last_child() == nullptr);
        CATCH_REQUIRE(root->next() == nullptr);
        CATCH_REQUIRE(root->previous() == nullptr);
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("empty root with comment & preprocessor & attributes")
    {
        std::string const xml_path(SNAP_CATCH2_NAMESPACE::get_folder_name());
        std::string const filename(xml_path + "/root-attributes.xml");

        // create file with one empty tag
        {
            std::ofstream f;
            f.open(filename);
            CATCH_REQUIRE(f.is_open());
            f << "<!--\n"
                 "name='next level'\n"
                 "-->\n"
                 "\n"
                 "<?xml version=\"1.0\"?>\n"
                 "<root-canal quite=\"quite\" size='123' very=\"true\">"
                 " \t \t \t "
                 "</root-canal>";
        }

        basic_xml::xml x(filename);
        basic_xml::node::pointer_t root(x.root());
        CATCH_REQUIRE(root != nullptr);
        CATCH_REQUIRE(root->tag_name() == "root-canal");
        CATCH_REQUIRE(root->text() == "");
        CATCH_REQUIRE(root->text(true) == "");
        CATCH_REQUIRE(root->text(false) == " \t \t \t ");
        CATCH_REQUIRE(root->all_attributes().size() == 3);
        CATCH_REQUIRE(root->attribute("quite") == "quite");
        CATCH_REQUIRE(root->attribute("size") == "123");
        CATCH_REQUIRE(root->attribute("very") == "true");
        CATCH_REQUIRE(root->parent() == nullptr);
        CATCH_REQUIRE(root->first_child() == nullptr);
        CATCH_REQUIRE(root->last_child() == nullptr);
        CATCH_REQUIRE(root->next() == nullptr);
        CATCH_REQUIRE(root->previous() == nullptr);
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("entities test")
    {
        std::string const xml_path(SNAP_CATCH2_NAMESPACE::get_folder_name());
        std::string const filename(xml_path + "/entities.xml");

        // create file with one empty tag
        {
            std::ofstream f;
            f.open(filename);
            CATCH_REQUIRE(f.is_open());
            f << "<!--\n"
                 "name='entitie&#x73;'\n"
                 "-->\n"
                 "\n"
                 "<?xml version=\"1.0\"?>\n"
                 "<entity-a-gogo quite=\"&#x71;uit&#101;\" size='1&#x32;3'"
                        " very=\"&quot;true&quot;\" special-entry=\"&quot;&lt;it&apos;s special &amp; weird&gt;&quot;\">"
                 "</entity-a-gogo>";
        }

        basic_xml::xml x(filename);
        basic_xml::node::pointer_t root(x.root());
        CATCH_REQUIRE(root != nullptr);
        CATCH_REQUIRE(root->tag_name() == "entity-a-gogo");
        CATCH_REQUIRE(root->all_attributes().size() == 4);
        CATCH_REQUIRE(root->attribute("quite") == "quite");
        CATCH_REQUIRE(root->attribute("size") == "123");
        CATCH_REQUIRE(root->attribute("very") == "\"true\"");
        CATCH_REQUIRE(root->attribute("special-entry") == "\"<it's special & weird>\"");
        CATCH_REQUIRE(root->parent() == nullptr);
        CATCH_REQUIRE(root->first_child() == nullptr);
        CATCH_REQUIRE(root->last_child() == nullptr);
        CATCH_REQUIRE(root->next() == nullptr);
        CATCH_REQUIRE(root->previous() == nullptr);
    }
    CATCH_END_SECTION()
}


CATCH_TEST_CASE("xml", "[xml][invalid]")
{
    CATCH_START_SECTION("file missing")
    {
        std::string const xml_path(SNAP_CATCH2_NAMESPACE::get_folder_name());
        std::string const filename(xml_path + "/file-does-not-exist.xml");

        CATCH_REQUIRE_THROWS_MATCHES(
                  basic_xml::xml(filename)
                , basic_xml::file_not_found
                , Catch::Matchers::ExceptionMessage(
                          "xml_error: could not open XML file \""
                        + filename
                        + "\": "
                        + strerror(ENOENT)
                        + ".", true));
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("file no permission")
    {
        std::string const filename("/root/.bashrc");

        CATCH_REQUIRE_THROWS_MATCHES(
                  basic_xml::xml(filename)
                , basic_xml::file_not_found
                , Catch::Matchers::ExceptionMessage(
                          "xml_error: could not open XML file \""
                        + filename
                        + "\": "
                        + strerror(EACCES)
                        + ".", true));
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("parse empty xml file")
    {
        std::string const xml_path(SNAP_CATCH2_NAMESPACE::get_folder_name());
        std::string const filename(xml_path + "/empty.xml");

        // create empty file
        {
            std::ofstream f;
            f.open(filename);
            CATCH_REQUIRE(f.is_open());
        }

        CATCH_REQUIRE_THROWS_MATCHES(
                  basic_xml::xml(filename)
                , basic_xml::unexpected_token
                , Catch::Matchers::ExceptionMessage(
                          "xml_error: "
                        + filename
                        + ":1: cannot be empty or include anything other than a processor tag and comments before the root tag.", true));
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("empty root tag")
    {
        std::string const filename("empty-tag.xml");

        // create file with one empty tag
        std::stringstream ss;
        ss << "<empty/>";

        CATCH_REQUIRE_THROWS_MATCHES(
                  basic_xml::xml(filename, ss)
                , basic_xml::unexpected_token
                , Catch::Matchers::ExceptionMessage(
                          "xml_error: "
                        + filename
                        + ":1: root tag cannot be an empty tag.", true));
    }
    CATCH_END_SECTION()
}



// vim: ts=4 sw=4 et
