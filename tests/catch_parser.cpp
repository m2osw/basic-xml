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
#include    <basic-xml/parser.h>


// C++
//
#include    <sstream>



CATCH_TEST_CASE("parser", "[parser][valid]")
{
    CATCH_START_SECTION("empty root")
    {
        std::string const filename("empty-tag.xml");

        std::stringstream ss;
        ss << "<empty></empty>";

        basic_xml::node::pointer_t root;
        basic_xml::parser p(filename, ss, root);
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

    CATCH_START_SECTION("empty root with processor")
    {
        std::string const filename("empty-tag.xml");

        std::stringstream ss;
        ss << "<?xml version=\"1.0\"?><still-empty></still-empty>";

        basic_xml::node::pointer_t root;
        basic_xml::parser p(filename, ss, root);
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

    CATCH_START_SECTION("empty root with comment & processor")
    {
        std::string const filename("quite-empty.xml");

        std::stringstream ss;
        ss << "<!-- name='rotor' --><?xml version=\"1.0\"?><quite-empty></quite-empty>";

        basic_xml::node::pointer_t root;
        basic_xml::parser p(filename, ss, root);
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

    CATCH_START_SECTION("empty root with processor & comment")
    {
        std::string const filename("swapped-empty.xml");

        std::stringstream ss;
        ss << "<?xml version=\"1.0\"?><!-- name='rotor' --><swapped-empty></swapped-empty>";

        basic_xml::node::pointer_t root;
        basic_xml::parser p(filename, ss, root);
        CATCH_REQUIRE(root != nullptr);
        CATCH_REQUIRE(root->tag_name() == "swapped-empty");
        CATCH_REQUIRE(root->text().empty());
        CATCH_REQUIRE(root->all_attributes().empty());
        CATCH_REQUIRE(root->parent() == nullptr);
        CATCH_REQUIRE(root->first_child() == nullptr);
        CATCH_REQUIRE(root->last_child() == nullptr);
        CATCH_REQUIRE(root->next() == nullptr);
        CATCH_REQUIRE(root->previous() == nullptr);
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("empty root with comment & processor & attributes")
    {
        std::string const filename("root-attributes.xml");

        std::stringstream ss;
        ss << "<!--\n"
              "name='next level'\n"
              "-->\n"
              "\n"
              "<?xml version=\"1.0\"?>\n"
              "<root-canal quite=\"quite\" size='123' very=\"true\">"
              " \t \t \t "
              "</root-canal>";

        basic_xml::node::pointer_t root;
        basic_xml::parser p(filename, ss, root);
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
        std::string const filename("entities.xml");

        std::stringstream ss;
        ss << "<!--\n"
              "name='entitie&#x73;'\n"
              "-->\n"
              "\n"
              "<?xml version=\"1.0\"?>\n"
              "<entity-a-gogo quite=\"&#x71;uit&#101;\" size='1&#x32;3'"
                  " very=\"&quot;true&quot;\" special-entry=\"&quot;&lt;it&apos;s special &amp; weird&gt;&quot;\">"
              "</entity-a-gogo><?php echo 'why am I ignoring these here?'; ?????>";

        basic_xml::node::pointer_t root;
        basic_xml::parser p(filename, ss, root);
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

    CATCH_START_SECTION("parse tree")
    {
        std::string const filename("tree.xml");

        std::stringstream ss;
        ss << "<!--\n"
              "name='tree'\n"
              "-->\n"
              "\n"
              "<?xml version=\"1.0\"?>\n"
              "<tree \t \f >"
                  "<       branch level='1'>"
                      "<leaf level = '2'>"
                        "with \f funny \t text \v and \r white \n spaces"
                      "</  leaf  >"
                  "</branch  >"
              "</  tree>\n \t \t \n";

        basic_xml::node::pointer_t root;
        basic_xml::parser p(filename, ss, root);
        CATCH_REQUIRE(root != nullptr);
        CATCH_REQUIRE(root->tag_name() == "tree");
        CATCH_REQUIRE(root->all_attributes().empty());

        basic_xml::node::pointer_t branch(root->first_child());
        CATCH_REQUIRE(branch->parent() == root);
        CATCH_REQUIRE(branch->root() == root);
        CATCH_REQUIRE(branch->all_attributes().size() == 1);
        CATCH_REQUIRE(branch->attribute("level") == "1");

        basic_xml::node::pointer_t leaf(branch->first_child());
        CATCH_REQUIRE(leaf->parent() == branch);
        CATCH_REQUIRE(leaf->root() == root);
        CATCH_REQUIRE(leaf->all_attributes().size() == 1);
        CATCH_REQUIRE(leaf->attribute("level") == "2");
        CATCH_REQUIRE(leaf->text() == "with \f funny \t text \v and \n white \n spaces");
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("parse tree (CDATA)")
    {
        std::string const filename("tree.xml");

        std::stringstream ss;
        ss << "<tree>"
                  "<branch level='1'>"
                      "<leaf level = '2'>"
                        "with \f funny \t text \v and \r white \n spaces and a > for fun"
                      "</  leaf  >"
                      "<leaf level = '2'>"
                        "<![CDATA[-> with \f funny \t text \v and \r white \n spaces <-]]>"
                      "</  leaf  >"
                  "</branch  >"
                  "<branch level=\"1\">"
                      "<leaf level=\"2\">"
                          "This one too has <![CDATA[some <need< for CDATA >and> such]]]]]>"
                      "</leaf>"
                      "<leaf level='2' ushes='warning'>=== text starts with equal ===</leaf>"
                      "<leaf level='2' feature='on'>>>> here it is</leaf>"
                      "<leaf level='2' message='perfect div'>/// next one ///</leaf>"
                      "<leaf level='2' hire=\"me\"><![CDATA[we have a case with ]] within the data]]></leaf>"
                      "<leaf level='2' width=\"250px\"><![CDATA[we have a case with one ] within the data too]]></leaf>"
                      "<leaf level='2' height=\"350px\">\"string data\"</leaf>"
                      "<leaf level='2' depth=\"35px\">'string data'</leaf>"
                      "<leaf level='2' z=\"-9\">bad & but we keep it quiet</leaf>"
                  "</ branch >"
                  "<branch level=\"1\" color='orange' />"
                  "<branch level=\"1\" color='purple'>"
                      "here is an equal = which I think works"
                  "</   branch   >"
              "</  tree>\n \t \t \n";

        basic_xml::node::pointer_t root;
        basic_xml::parser p(filename, ss, root);
        CATCH_REQUIRE(root != nullptr);
        CATCH_REQUIRE(root->tag_name() == "tree");
        CATCH_REQUIRE(root->all_attributes().empty());

        basic_xml::node::pointer_t branch1(root->first_child());
        CATCH_REQUIRE(branch1->parent() == root);
        CATCH_REQUIRE(branch1->root() == root);
        CATCH_REQUIRE(branch1->all_attributes().size() == 1);
        CATCH_REQUIRE(branch1->attribute("level") == "1");

        basic_xml::node::pointer_t leaf1_1(branch1->first_child());
        CATCH_REQUIRE(leaf1_1->parent() == branch1);
        CATCH_REQUIRE(leaf1_1->root() == root);
        CATCH_REQUIRE(leaf1_1->all_attributes().size() == 1);
        CATCH_REQUIRE(leaf1_1->attribute("level") == "2");
        CATCH_REQUIRE(leaf1_1->text() == "with \f funny \t text \v and \n white \n spaces and a > for fun");

        basic_xml::node::pointer_t leaf1_2(leaf1_1->next());
        CATCH_REQUIRE(leaf1_2->parent() == branch1);
        CATCH_REQUIRE(leaf1_2->root() == root);
        CATCH_REQUIRE(leaf1_2->all_attributes().size() == 1);
        CATCH_REQUIRE(leaf1_2->attribute("level") == "2");
        CATCH_REQUIRE(leaf1_2->text() == "-> with \f funny \t text \v and \n white \n spaces <-");

        basic_xml::node::pointer_t branch2(branch1->next());
        CATCH_REQUIRE(branch2->parent() == root);
        CATCH_REQUIRE(branch2->root() == root);
        CATCH_REQUIRE(branch2->all_attributes().size() == 1);
        CATCH_REQUIRE(branch2->attribute("level") == "1");

        basic_xml::node::pointer_t leaf2_1(branch2->first_child());
        CATCH_REQUIRE(leaf2_1->parent() == branch2);
        CATCH_REQUIRE(leaf2_1->root() == root);
        CATCH_REQUIRE(leaf2_1->all_attributes().size() == 1);
        CATCH_REQUIRE(leaf2_1->attribute("level") == "2");
        CATCH_REQUIRE(leaf2_1->text() == "This one too has some <need< for CDATA >and> such]]]");

        basic_xml::node::pointer_t leaf2_2(leaf2_1->next());
        CATCH_REQUIRE(leaf2_2->parent() == branch2);
        CATCH_REQUIRE(leaf2_2->root() == root);
        CATCH_REQUIRE(leaf2_2->all_attributes().size() == 2);
        CATCH_REQUIRE(leaf2_2->attribute("level") == "2");
        CATCH_REQUIRE(leaf2_2->attribute("ushes") == "warning");
        CATCH_REQUIRE(leaf2_2->text() == "=== text starts with equal ===");

        basic_xml::node::pointer_t leaf2_3(leaf2_2->next());
        CATCH_REQUIRE(leaf2_3->parent() == branch2);
        CATCH_REQUIRE(leaf2_3->root() == root);
        CATCH_REQUIRE(leaf2_3->all_attributes().size() == 2);
        CATCH_REQUIRE(leaf2_3->attribute("level") == "2");
        CATCH_REQUIRE(leaf2_3->attribute("feature") == "on");
        CATCH_REQUIRE(leaf2_3->text() == ">>> here it is");

        basic_xml::node::pointer_t leaf2_4(leaf2_3->next());
        CATCH_REQUIRE(leaf2_4->parent() == branch2);
        CATCH_REQUIRE(leaf2_4->root() == root);
        CATCH_REQUIRE(leaf2_4->all_attributes().size() == 2);
        CATCH_REQUIRE(leaf2_4->attribute("level") == "2");
        CATCH_REQUIRE(leaf2_4->attribute("message") == "perfect div");
        CATCH_REQUIRE(leaf2_4->text() == "/// next one ///");

        basic_xml::node::pointer_t leaf2_5(leaf2_4->next());
        CATCH_REQUIRE(leaf2_5->parent() == branch2);
        CATCH_REQUIRE(leaf2_5->root() == root);
        CATCH_REQUIRE(leaf2_5->all_attributes().size() == 2);
        CATCH_REQUIRE(leaf2_5->attribute("level") == "2");
        CATCH_REQUIRE(leaf2_5->attribute("hire") == "me");
        CATCH_REQUIRE(leaf2_5->text() == "we have a case with ]] within the data");

        basic_xml::node::pointer_t leaf2_6(leaf2_5->next());
        CATCH_REQUIRE(leaf2_6->parent() == branch2);
        CATCH_REQUIRE(leaf2_6->root() == root);
        CATCH_REQUIRE(leaf2_6->all_attributes().size() == 2);
        CATCH_REQUIRE(leaf2_6->attribute("level") == "2");
        CATCH_REQUIRE(leaf2_6->attribute("width") == "250px");
        CATCH_REQUIRE(leaf2_6->text() == "we have a case with one ] within the data too");

        basic_xml::node::pointer_t leaf2_7(leaf2_6->next());
        CATCH_REQUIRE(leaf2_7->parent() == branch2);
        CATCH_REQUIRE(leaf2_7->root() == root);
        CATCH_REQUIRE(leaf2_7->all_attributes().size() == 2);
        CATCH_REQUIRE(leaf2_7->attribute("level") == "2");
        CATCH_REQUIRE(leaf2_7->attribute("height") == "350px");
        CATCH_REQUIRE(leaf2_7->text() == "\"string data\"");

        basic_xml::node::pointer_t leaf2_8(leaf2_7->next());
        CATCH_REQUIRE(leaf2_8->parent() == branch2);
        CATCH_REQUIRE(leaf2_8->root() == root);
        CATCH_REQUIRE(leaf2_8->all_attributes().size() == 2);
        CATCH_REQUIRE(leaf2_8->attribute("level") == "2");
        CATCH_REQUIRE(leaf2_8->attribute("depth") == "35px");
        CATCH_REQUIRE(leaf2_8->text() == "'string data'");

        basic_xml::node::pointer_t leaf2_9(leaf2_8->next());
        CATCH_REQUIRE(leaf2_9->parent() == branch2);
        CATCH_REQUIRE(leaf2_9->root() == root);
        CATCH_REQUIRE(leaf2_9->all_attributes().size() == 2);
        CATCH_REQUIRE(leaf2_9->attribute("level") == "2");
        CATCH_REQUIRE(leaf2_9->attribute("z") == "-9");
        CATCH_REQUIRE(leaf2_9->text() == "bad & but we keep it quiet");

        basic_xml::node::pointer_t branch3(branch2->next());
        CATCH_REQUIRE(branch3->parent() == root);
        CATCH_REQUIRE(branch3->root() == root);
        CATCH_REQUIRE(branch3->all_attributes().size() == 2);
        CATCH_REQUIRE(branch3->attribute("level") == "1");
        CATCH_REQUIRE(branch3->attribute("color") == "orange");
        CATCH_REQUIRE(branch3->text().empty());

        basic_xml::node::pointer_t branch4(branch3->next());
        CATCH_REQUIRE(branch4->parent() == root);
        CATCH_REQUIRE(branch4->root() == root);
        CATCH_REQUIRE(branch4->all_attributes().size() == 2);
        CATCH_REQUIRE(branch4->attribute("level") == "1");
        CATCH_REQUIRE(branch4->attribute("color") == "purple");
        CATCH_REQUIRE(branch4->text() == "here is an equal = which I think works");
    }
    CATCH_END_SECTION()
}


CATCH_TEST_CASE("parser_errors", "[parser][invalid]")
{
    CATCH_START_SECTION("parser does not like a completely empty input file")
    {
        std::stringstream ss;
        std::string const filename("empty.xml");

        basic_xml::node::pointer_t root;
        CATCH_REQUIRE_THROWS_MATCHES(
                  basic_xml::parser(filename, ss, root)
                , basic_xml::unexpected_token
                , Catch::Matchers::ExceptionMessage(
                          "xml_error: "
                        + filename
                        + ":1: cannot be empty or include anything other than a processor tag and comments before the root tag.", true));
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("file with only a processor tag")
    {
        std::stringstream ss;
        std::string const filename("processor.xml");
        ss << "<?xml version=\"1.0\"?>\n";

        basic_xml::node::pointer_t root;
        CATCH_REQUIRE_THROWS_MATCHES(
                  basic_xml::parser(filename, ss, root)
                , basic_xml::unexpected_token
                , Catch::Matchers::ExceptionMessage(
                          "xml_error: "
                        + filename
                        + ":2: cannot be empty or include anything other than a processor tag and comments before the root tag.", true));
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("file with only a comment tag")
    {
        std::stringstream ss;
        std::string const filename("comment.xml");
        ss << "<!-- only a comment... -->\n";

        basic_xml::node::pointer_t root;
        CATCH_REQUIRE_THROWS_MATCHES(
                  basic_xml::parser(filename, ss, root)
                , basic_xml::unexpected_token
                , Catch::Matchers::ExceptionMessage(
                          "xml_error: "
                        + filename
                        + ":2: cannot be empty or include anything other than a processor tag and comments before the root tag.", true));
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("file with only comment and preprocessor tags")
    {
        std::stringstream ss;
        std::string const filename("comment-and-processor.xml");
        ss << "<!-- comment at the start... -->\n"
              "<?xml version=\"1.0\"?>\n"
              "<!-- comment further down... -->\n";

        basic_xml::node::pointer_t root;
        CATCH_REQUIRE_THROWS_MATCHES(
                  basic_xml::parser(filename, ss, root)
                , basic_xml::unexpected_token
                , Catch::Matchers::ExceptionMessage(
                          "xml_error: "
                        + filename
                        + ":4: cannot be empty or include anything other than a processor tag and comments before the root tag.", true));
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("file with two processor tags and a root")
    {
        std::stringstream ss;
        std::string const filename("processor-and-root.xml");
        ss << "<?xml version=\"1.0\"?>\n"
              "<?php\necho 'invalid code';\n?>\n"  // <-- err on this one
              "<root>too late</root>\n";

        basic_xml::node::pointer_t root;
        CATCH_REQUIRE_THROWS_MATCHES(
                  basic_xml::parser(filename, ss, root)
                , basic_xml::unexpected_token
                , Catch::Matchers::ExceptionMessage(
                          "xml_error: "
                        + filename
                        + ":4: cannot be empty or include anything other than a processor tag and comments before the root tag.", true));
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("empty root tag")
    {
        std::stringstream ss;
        std::string const filename("empty-root.xml");
        ss << "<!-- root can't be empty -->\n"
              "<root/>\n";

        basic_xml::node::pointer_t root;
        CATCH_REQUIRE_THROWS_MATCHES(
                  basic_xml::parser(filename, ss, root)
                , basic_xml::unexpected_token
                , Catch::Matchers::ExceptionMessage(
                          "xml_error: "
                        + filename
                        + ":2: root tag cannot be an empty tag.", true));
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("wrong closing tag")
    {
        std::stringstream ss;
        std::string const filename("empty-root.xml");
        ss << "<!-- tag mismatch -->\n"
              "<root><this>incorrect</that></root>\n";

        basic_xml::node::pointer_t root;
        CATCH_REQUIRE_THROWS_MATCHES(
                  basic_xml::parser(filename, ss, root)
                , basic_xml::unexpected_token
                , Catch::Matchers::ExceptionMessage(
                          "xml_error: "
                        + filename
                        + ":2: unexpected token \"that\" in this closing tag; expected \"this\" instead."));
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("text after closing root tag")
    {
        std::stringstream ss;
        std::string const filename("extra-text.xml");
        ss << "<!-- text at the end -->\n"
              "<root><sub-tag>all good here</sub-tag></root>but no there\n";

        basic_xml::node::pointer_t root;
        CATCH_REQUIRE_THROWS_MATCHES(
                  basic_xml::parser(filename, ss, root)
                , basic_xml::unexpected_token
                , Catch::Matchers::ExceptionMessage(
                          "xml_error: "
                        + filename
                        + ":3: cannot include text data before or after the root tag."));
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("tag after closing root tag")
    {
        std::stringstream ss;
        std::string const filename("extra-text.xml");
        ss << "<!-- text at the end -->\n"
              "<root><sub-tag>all good here</sub-tag></root><more>invalid</mode>\n";

        basic_xml::node::pointer_t root;
        CATCH_REQUIRE_THROWS_MATCHES(
                  basic_xml::parser(filename, ss, root)
                , basic_xml::unexpected_token
                , Catch::Matchers::ExceptionMessage(
                          "xml_error: "
                        + filename
                        + ":2: we reached the end of the XML file, but still"
                          " found a token of type 6 after the closing root tag"
                          " instead of the end of the file."));
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("not closing root tag")
    {
        std::stringstream ss;
        std::string const filename("root-still-open.xml");
        ss << "<!-- text at the end -->\n"
              "<root><sub-tag>all good here</sub-tag> <!-- invalid end -->\n";

        basic_xml::node::pointer_t root;
        CATCH_REQUIRE_THROWS_MATCHES(
                  basic_xml::parser(filename, ss, root)
                , basic_xml::unexpected_token
                , Catch::Matchers::ExceptionMessage(
                          "xml_error: "
                        + filename
                        + ":3: reached the end of the file without first"
                          " closing the root tag."));
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("attribute expects identifier")
    {
        std::stringstream ss;
        std::string const filename("attribute-identifier.xml");
        ss << "<root><sub-tag attr=\"good\" =\"bad\">all good here</sub-tag></root>\n";

        basic_xml::node::pointer_t root;
        CATCH_REQUIRE_THROWS_MATCHES(
                  basic_xml::parser(filename, ss, root)
                , basic_xml::invalid_xml
                , Catch::Matchers::ExceptionMessage(
                          "xml_error: "
                        + filename
                        + ":1: expected the end of the tag (>) or an attribute name."));
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("attribute missing equal")
    {
        std::stringstream ss;
        std::string const filename("attribute-identifier.xml");
        ss << "<root><sub-tag attr \"bad\">all good here</sub-tag></root>\n";

        basic_xml::node::pointer_t root;
        CATCH_REQUIRE_THROWS_MATCHES(
                  basic_xml::parser(filename, ss, root)
                , basic_xml::invalid_xml
                , Catch::Matchers::ExceptionMessage(
                          "xml_error: "
                        + filename
                        + ":1: expected the '=' character between the attribute name and value."));
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("attribute value must be string")
    {
        std::stringstream ss;
        std::string const filename("attribute-identifier.xml");
        ss << "<root><sub-tag attr=bad>quotes are missing...</sub-tag></root>\n";

        basic_xml::node::pointer_t root;
        CATCH_REQUIRE_THROWS_MATCHES(
                  basic_xml::parser(filename, ss, root)
                , basic_xml::invalid_xml
                , Catch::Matchers::ExceptionMessage(
                          "xml_error: "
                        + filename
                        + ":1: expected a quoted value after the '=' sign."));
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("attribute defined twice")
    {
        std::stringstream ss;
        std::string const filename("attribute-duplicated.xml");
        ss << "<root><sub-tag attr=\"one\" attr=\"two\">attribute defined twice...</sub-tag></root>\n";

        basic_xml::node::pointer_t root;
        CATCH_REQUIRE_THROWS_MATCHES(
                  basic_xml::parser(filename, ss, root)
                , basic_xml::invalid_xml
                , Catch::Matchers::ExceptionMessage(
                          "xml_error: "
                        + filename
                        + ":1: attribute \"attr\" defined twice; we do not allow such."));
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("processor not ended")
    {
        std::stringstream ss;
        std::string const filename("attribute-duplicated.xml");
        ss << "<root><sub-tag><?php echo 'this is legal in PHP, but not for us...';\n";

        basic_xml::node::pointer_t root;
        CATCH_REQUIRE_THROWS_MATCHES(
                  basic_xml::parser(filename, ss, root)
                , basic_xml::unexpected_eof
                , Catch::Matchers::ExceptionMessage(
                          "xml_error: "
                        + filename
                        + ":2: reached the end of the file while reading a processor (\"<?...?>\") tag."));
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("element definitions not supported")
    {
        std::stringstream ss;
        std::string const filename("element.xml");
        ss << "<root><sub-tag><!ELEMENT abc></sub-tag></root>\n";

        basic_xml::node::pointer_t root;
        CATCH_REQUIRE_THROWS_MATCHES(
                  basic_xml::parser(filename, ss, root)
                , basic_xml::invalid_xml
                , Catch::Matchers::ExceptionMessage(
                          "xml_error: "
                        + filename
                        + ":1: found an element definition (such as an \"<!ELEMENT...>\" sequence), which is not supported."));
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("bad CDATA introducer")
    {
        std::stringstream ss;
        std::string const filename("element.xml");
        ss << "<root><sub-tag><![BAD[abc]]></sub-tag></root>\n";

        basic_xml::node::pointer_t root;
        CATCH_REQUIRE_THROWS_MATCHES(
                  basic_xml::parser(filename, ss, root)
                , basic_xml::invalid_xml
                , Catch::Matchers::ExceptionMessage(
                          "xml_error: "
                        + filename
                        + ":1: found an unexpected sequence of character in a \"<![CDATA[...\" sequence."));
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("EOF before closing CDATA tag")
    {
        std::stringstream ss;
        std::string const filename("element.xml");
        ss << "<root><sub-tag><![CDATA[abc]]\n";

        basic_xml::node::pointer_t root;
        CATCH_REQUIRE_THROWS_MATCHES(
                  basic_xml::parser(filename, ss, root)
                , basic_xml::unexpected_eof
                , Catch::Matchers::ExceptionMessage(
                          "xml_error: "
                        + filename
                        + ":2: found EOF while parsing a \"<![CDATA[...]]>\" sequence."));
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("EOF before closing comment tag")
    {
        std::stringstream ss;
        std::string const filename("element.xml");
        ss << "<root><sub-tag><!-- file\nends\nwith\ncomment\nopen\n...\n";

        basic_xml::node::pointer_t root;
        CATCH_REQUIRE_THROWS_MATCHES(
                  basic_xml::parser(filename, ss, root)
                , basic_xml::unexpected_eof
                , Catch::Matchers::ExceptionMessage(
                          "xml_error: "
                        + filename
                        + ":7: found EOF while parsing a comment (\"<!--...-->\") sequence."));
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("unknown <!... sequence")
    {
        std::stringstream ss;
        std::string const filename("element.xml");
        ss << "<root><sub-tag><!+ unknown +></sub-tag></root>\n";

        basic_xml::node::pointer_t root;
        CATCH_REQUIRE_THROWS_MATCHES(
                  basic_xml::parser(filename, ss, root)
                , basic_xml::invalid_token
                , Catch::Matchers::ExceptionMessage(
                          "xml_error: "
                        + filename
                        + ":1: character '+' was not expected after a \"<!\" sequence."));
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("closing tag missing name")
    {
        std::stringstream ss;
        std::string const filename("element.xml");
        ss << "<root><sub-tag>bad closing tag</\n";

        basic_xml::node::pointer_t root;
        CATCH_REQUIRE_THROWS_MATCHES(
                  basic_xml::parser(filename, ss, root)
                , basic_xml::unexpected_eof
                , Catch::Matchers::ExceptionMessage(
                          "xml_error: "
                        + filename
                        + ":2: expected a tag name after \"</\", not EOF."));
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("closing tag not starting with alpha character")
    {
        std::stringstream ss;
        std::string const filename("element.xml");
        ss << "<root><sub-tag>bad closing tag</-bad-name->\n";

        basic_xml::node::pointer_t root;
        CATCH_REQUIRE_THROWS_MATCHES(
                  basic_xml::parser(filename, ss, root)
                , basic_xml::invalid_token
                , Catch::Matchers::ExceptionMessage(
                          "xml_error: "
                        + filename
                        + ":1: character '-' is not valid for a tag name."));
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("closing tag missing >")
    {
        std::stringstream ss;
        std::string const filename("element.xml");
        ss << "<root><sub-tag>bad closing tag</good-name\n";

        basic_xml::node::pointer_t root;
        CATCH_REQUIRE_THROWS_MATCHES(
                  basic_xml::parser(filename, ss, root)
                , basic_xml::unexpected_eof
                , Catch::Matchers::ExceptionMessage(
                          "xml_error: "
                        + filename
                        + ":2: expected '>', not EOF."));
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("closing tag name followed by ';'")
    {
        std::stringstream ss;
        std::string const filename("element.xml");
        ss << "<root><sub-tag>bad closing tag</good-name;>\n";

        basic_xml::node::pointer_t root;
        CATCH_REQUIRE_THROWS_MATCHES(
                  basic_xml::parser(filename, ss, root)
                , basic_xml::invalid_xml
                , Catch::Matchers::ExceptionMessage(
                          "xml_error: "
                        + filename
                        + ":1: found an unexpected ';' in a closing tag, expected '>' instead."));
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("opening tag missing >")
    {
        std::stringstream ss;
        std::string const filename("element.xml");
        ss << "<root><\n";

        basic_xml::node::pointer_t root;
        CATCH_REQUIRE_THROWS_MATCHES(
                  basic_xml::parser(filename, ss, root)
                , basic_xml::unexpected_eof
                , Catch::Matchers::ExceptionMessage(
                          "xml_error: "
                        + filename
                        + ":2: expected a tag name after '<', not EOF."));
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("opening tag bad name")
    {
        std::stringstream ss;
        std::string const filename("element.xml");
        ss << "<root><{sub-tag>bad opening tag\n";

        basic_xml::node::pointer_t root;
        CATCH_REQUIRE_THROWS_MATCHES(
                  basic_xml::parser(filename, ss, root)
                , basic_xml::invalid_token
                , Catch::Matchers::ExceptionMessage(
                          "xml_error: "
                        + filename
                        + ":1: character '{' is not valid for a tag name."));
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("opening tag name followed by ';'")
    {
        std::stringstream ss;
        std::string const filename("element.xml");
        ss << "<root><sub-tag}>bad closing tag\n";

        basic_xml::node::pointer_t root;
        CATCH_REQUIRE_THROWS_MATCHES(
                  basic_xml::parser(filename, ss, root)
                , basic_xml::invalid_token
                , Catch::Matchers::ExceptionMessage(
                          "xml_error: "
                        + filename
                        + ":1: character '}' is not valid right after a tag name."));
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("attribute with '<'")
    {
        std::stringstream ss;
        std::string const filename("element.xml");
        ss << "<root><sub-tag name='perfect <tag>'>bad closing tag</sub-tag></root>\n";

        basic_xml::node::pointer_t root;
        CATCH_REQUIRE_THROWS_MATCHES(
                  basic_xml::parser(filename, ss, root)
                , basic_xml::invalid_token
                , Catch::Matchers::ExceptionMessage(
                          "xml_error: "
                        + filename
                        + ":1: character '>' not expected inside a tag value; please use \"&gt;\" instead."));
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("empty entity (&;)")
    {
        std::stringstream ss;
        std::string const filename("element.xml");
        ss << "<root><sub-tag name='empty entity: &;'>bad closing tag</sub-tag></root>\n";

        basic_xml::node::pointer_t root;
        CATCH_REQUIRE_THROWS_MATCHES(
                  basic_xml::parser(filename, ss, root)
                , basic_xml::invalid_entity
                , Catch::Matchers::ExceptionMessage(
                          "xml_error: "
                        + filename
                        + ":1: the name of an entity cannot be empty (\"&;\" is not valid XML)."));
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("empty numeric entity (&#;)")
    {
        std::stringstream ss;
        std::string const filename("element.xml");
        ss << "<root><sub-tag name='empty numeric entity: &#;'>bad closing tag</sub-tag></root>\n";

        basic_xml::node::pointer_t root;
        CATCH_REQUIRE_THROWS_MATCHES(
                  basic_xml::parser(filename, ss, root)
                , basic_xml::invalid_entity
                , Catch::Matchers::ExceptionMessage(
                          "xml_error: "
                        + filename
                        + ":1: a numeric entity must have a number (\"&#;\" is not valid XML)."));
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("invalid number of numeric entity (not decimal)")
    {
        std::stringstream ss;
        std::string const filename("element.xml");
        ss << "<root><sub-tag name='empty numeric entity: &#12ab34;'>bad closing tag</sub-tag></root>\n";

        basic_xml::node::pointer_t root;
        CATCH_REQUIRE_THROWS_MATCHES(
                  basic_xml::parser(filename, ss, root)
                , basic_xml::invalid_number
                , Catch::Matchers::ExceptionMessage(
                          "xml_error: "
                        + filename
                        + ":1: the number found in numeric entity, \" 12ab34\", is not considered valid."));
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("invalid number of numeric entity (not hexadecimal)")
    {
        std::stringstream ss;
        std::string const filename("element.xml");
        ss << "<root><sub-tag name='empty numeric entity: &#x10abz4;'>bad closing tag</sub-tag></root>\n";

        basic_xml::node::pointer_t root;
        CATCH_REQUIRE_THROWS_MATCHES(
                  basic_xml::parser(filename, ss, root)
                , basic_xml::invalid_number
                , Catch::Matchers::ExceptionMessage(
                          "xml_error: "
                        + filename
                        + ":1: the number found in numeric entity, \"0x10abz4\", is not considered valid."));
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("\"unknown\" entity")
    {
        std::stringstream ss;
        std::string const filename("element.xml");
        ss << "<root><sub-tag name='empty numeric entity: &unknown;'>bad closing tag</sub-tag></root>\n";

        basic_xml::node::pointer_t root;
        CATCH_REQUIRE_THROWS_MATCHES(
                  basic_xml::parser(filename, ss, root)
                , basic_xml::invalid_entity
                , Catch::Matchers::ExceptionMessage(
                          "xml_error: "
                        + filename
                        + ":1: unsupported entity (\"&unknown;\")."));
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("separate attributes with a slash")
    {
        std::stringstream ss;
        std::string const filename("element.xml");
        ss << "<root><sub-tag name='one' / ignore='two'>bad closing tag</sub-tag></root>\n";

        basic_xml::node::pointer_t root;
        CATCH_REQUIRE_THROWS_MATCHES(
                  basic_xml::parser(filename, ss, root)
                , basic_xml::invalid_xml
                , Catch::Matchers::ExceptionMessage(
                          "xml_error: "
                        + filename
                        + ":1: expected the end of the tag (>) or an attribute name."));
    }
    CATCH_END_SECTION()
}



// vim: ts=4 sw=4 et
