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

// basic-xml
//
#include    <basic-xml/node.h>

#include    <basic-xml/exception.h>
#include    <basic-xml/type.h>


// self
//
#include    "catch_main.h"


// libutf8
//
#include    <libutf8/libutf8.h>



CATCH_TEST_CASE("node", "[node][valid]")
{
    CATCH_START_SECTION("node: basic node")
    {
        basic_xml::node n("root");
        CATCH_REQUIRE(n.tag_name() == "root");

        CATCH_REQUIRE(n.text() == "");
        n.append_text("some text");
        CATCH_REQUIRE(n.text() == "some text");
        n.append_text(" more text");
        CATCH_REQUIRE(n.text() == "some text more text");
        n.set_text(" this new text   ");
        CATCH_REQUIRE(n.text() == "this new text");
        CATCH_REQUIRE(n.text(false) == " this new text   ");
        n.set_text(" this new text   \r\n");
        CATCH_REQUIRE(n.text() == "this new text");
        CATCH_REQUIRE(n.text(false) == " this new text   \r\n");

        CATCH_REQUIRE(n.all_attributes().empty());
        CATCH_REQUIRE(n.attribute("unknown").empty());
        n.set_attribute("test", "me");
        CATCH_REQUIRE(n.attribute("test") == "me");
        CATCH_REQUIRE(n.attribute("unknown").empty());
        CATCH_REQUIRE(n.all_attributes().size() == 1);

        CATCH_REQUIRE(n.parent() == nullptr);
        CATCH_REQUIRE(n.first_child() == nullptr);
        CATCH_REQUIRE(n.last_child() == nullptr);
        CATCH_REQUIRE(n.next() == nullptr);
        CATCH_REQUIRE(n.previous() == nullptr);
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("node: node tree")
    {
        basic_xml::node::pointer_t root(std::make_shared<basic_xml::node>("root"));
        CATCH_REQUIRE(root->tag_name() == "root");

        // level 1
        //
        basic_xml::node::pointer_t l1c1(std::make_shared<basic_xml::node>("l1c1"));
        CATCH_REQUIRE(l1c1->tag_name() == "l1c1");

        basic_xml::node::pointer_t l1c2(std::make_shared<basic_xml::node>("l1c2"));
        CATCH_REQUIRE(l1c2->tag_name() == "l1c2");

        basic_xml::node::pointer_t l1c3(std::make_shared<basic_xml::node>("l1c3"));
        CATCH_REQUIRE(l1c3->tag_name() == "l1c3");

        // append l1c1
        //
        root->append_child(l1c1);
        CATCH_REQUIRE(l1c1->parent() == root);
        CATCH_REQUIRE(l1c1->first_child() == nullptr);
        CATCH_REQUIRE(l1c1->last_child() == nullptr);
        CATCH_REQUIRE(l1c1->next() == nullptr);
        CATCH_REQUIRE(l1c1->previous() == nullptr);

        CATCH_REQUIRE(root->parent() == nullptr);
        CATCH_REQUIRE(root->first_child() == l1c1);
        CATCH_REQUIRE(root->last_child() == l1c1);
        CATCH_REQUIRE(root->next() == nullptr);
        CATCH_REQUIRE(root->previous() == nullptr);

        // append l1c2
        //
        root->append_child(l1c2);
        CATCH_REQUIRE(l1c2->parent() == root);
        CATCH_REQUIRE(l1c2->first_child() == nullptr);
        CATCH_REQUIRE(l1c2->last_child() == nullptr);
        CATCH_REQUIRE(l1c2->next() == nullptr);
        CATCH_REQUIRE(l1c2->previous() == l1c1);

        CATCH_REQUIRE(l1c1->next() == l1c2);
        CATCH_REQUIRE(l1c1->previous() == nullptr);

        CATCH_REQUIRE(root->parent() == nullptr);
        CATCH_REQUIRE(root->first_child() == l1c1);
        CATCH_REQUIRE(root->last_child() == l1c2);
        CATCH_REQUIRE(root->next() == nullptr);
        CATCH_REQUIRE(root->previous() == nullptr);

        // append l1c3
        //
        root->append_child(l1c3);
        CATCH_REQUIRE(l1c3->parent() == root);
        CATCH_REQUIRE(l1c3->first_child() == nullptr);
        CATCH_REQUIRE(l1c3->last_child() == nullptr);
        CATCH_REQUIRE(l1c3->next() == nullptr);
        CATCH_REQUIRE(l1c3->previous() == l1c2);

        CATCH_REQUIRE(l1c1->next() == l1c2);
        CATCH_REQUIRE(l1c1->previous() == nullptr);

        CATCH_REQUIRE(l1c2->next() == l1c3);
        CATCH_REQUIRE(l1c2->previous() == l1c1);

        CATCH_REQUIRE(root->parent() == nullptr);
        CATCH_REQUIRE(root->first_child() == l1c1);
        CATCH_REQUIRE(root->last_child() == l1c3);
        CATCH_REQUIRE(root->next() == nullptr);
        CATCH_REQUIRE(root->previous() == nullptr);

        // level 2
        //
        basic_xml::node::pointer_t l2c1(std::make_shared<basic_xml::node>("l2c1"));
        CATCH_REQUIRE(l2c1->tag_name() == "l2c1");

        basic_xml::node::pointer_t l2c2(std::make_shared<basic_xml::node>("l2c2"));
        CATCH_REQUIRE(l2c2->tag_name() == "l2c2");

        basic_xml::node::pointer_t l2c3(std::make_shared<basic_xml::node>("l2c3"));
        CATCH_REQUIRE(l2c3->tag_name() == "l2c3");

        // append l2c1
        //
        l1c2->append_child(l2c1);
        CATCH_REQUIRE(l2c1->parent() == l1c2);
        CATCH_REQUIRE(l2c1->parent()->parent() == root);
        CATCH_REQUIRE(l2c1->first_child() == nullptr);
        CATCH_REQUIRE(l2c1->last_child() == nullptr);
        CATCH_REQUIRE(l2c1->next() == nullptr);
        CATCH_REQUIRE(l2c1->previous() == nullptr);

        CATCH_REQUIRE(root->parent() == nullptr);
        CATCH_REQUIRE(root->first_child() == l1c1);
        CATCH_REQUIRE(root->last_child() == l1c3);
        CATCH_REQUIRE(root->next() == nullptr);
        CATCH_REQUIRE(root->previous() == nullptr);

        // append l2c2
        //
        l1c2->append_child(l2c2);
        CATCH_REQUIRE(l2c2->parent() == l1c2);
        CATCH_REQUIRE(l2c2->parent()->parent() == root);
        CATCH_REQUIRE(l2c2->first_child() == nullptr);
        CATCH_REQUIRE(l2c2->last_child() == nullptr);
        CATCH_REQUIRE(l2c2->next() == nullptr);
        CATCH_REQUIRE(l2c2->previous() == l2c1);

        CATCH_REQUIRE(l2c1->next() == l2c2);
        CATCH_REQUIRE(l2c1->previous() == nullptr);

        CATCH_REQUIRE(root->parent() == nullptr);
        CATCH_REQUIRE(root->first_child() == l1c1);
        CATCH_REQUIRE(root->last_child() == l1c3);
        CATCH_REQUIRE(root->next() == nullptr);
        CATCH_REQUIRE(root->previous() == nullptr);

        // append l2c3
        //
        l1c2->append_child(l2c3);
        CATCH_REQUIRE(l2c3->parent() == l1c2);
        CATCH_REQUIRE(l2c3->parent()->parent() == root);
        CATCH_REQUIRE(l2c3->first_child() == nullptr);
        CATCH_REQUIRE(l2c3->last_child() == nullptr);
        CATCH_REQUIRE(l2c3->next() == nullptr);
        CATCH_REQUIRE(l2c3->previous() == l2c2);

        CATCH_REQUIRE(l2c1->next() == l2c2);
        CATCH_REQUIRE(l2c1->previous() == nullptr);

        CATCH_REQUIRE(l2c2->next() == l2c3);
        CATCH_REQUIRE(l2c2->previous() == l2c1);

        CATCH_REQUIRE(root->parent() == nullptr);
        CATCH_REQUIRE(root->first_child() == l1c1);
        CATCH_REQUIRE(root->last_child() == l1c3);
        CATCH_REQUIRE(root->next() == nullptr);
        CATCH_REQUIRE(root->previous() == nullptr);
    }
    CATCH_END_SECTION()
}



CATCH_TEST_CASE("node_output", "[node][valid]")
{
    CATCH_START_SECTION("node_output: convert string with entities")
    {
        CATCH_REQUIRE(basic_xml::convert_to_entity("nothing to convert", "<>&") == "nothing to convert");

        CATCH_REQUIRE(basic_xml::convert_to_entity("try < and > and & and \" and ' all should change", "<>&\"'") == "try &lt; and &gt; and &amp; and &quot; and &apos; all should change");

        CATCH_REQUIRE(basic_xml::convert_to_entity("try < and > and & but keep \" and '", "<>&") == "try &lt; and &gt; and &amp; but keep \" and '");
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("node_output: node output")
    {
        basic_xml::node n("root");
        CATCH_REQUIRE(n.tag_name() == "root");

        n.append_text("with text");
        n.set_attribute("simple-attribute", "with some value");

        std::stringstream ss;
        ss << n;
        CATCH_REQUIRE(ss.str() == "<root simple-attribute=\"with some value\">with text</root>");
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("node_output: node tree output")
    {
        basic_xml::node::pointer_t root(std::make_shared<basic_xml::node>("root"));
        root->set_attribute("even-root", "can have an \"attribute\"");
        basic_xml::node::pointer_t l1c1(std::make_shared<basic_xml::node>("l1c1"));
        basic_xml::node::pointer_t l1c2(std::make_shared<basic_xml::node>("l1c2"));
        l1c2->append_text("wierd text + sub-tags");
        basic_xml::node::pointer_t l1c3(std::make_shared<basic_xml::node>("l1c3"));
        l1c3->append_text("<\"it's not empty & it works\">");
        root->append_child(l1c1);
        root->append_child(l1c2);
        root->append_child(l1c3);
        basic_xml::node::pointer_t l2c1(std::make_shared<basic_xml::node>("l2c1"));
        l2c1->set_attribute("color", "l'escargot");
        l2c1->append_text("sub-node has data");
        basic_xml::node::pointer_t l2c2(std::make_shared<basic_xml::node>("l2c2"));
        basic_xml::node::pointer_t l2c3(std::make_shared<basic_xml::node>("l2c3"));
        l2c3->set_attribute("i33", "\"it's both this time\"");
        l2c3->append_text("last-node has data too");
        l1c2->append_child(l2c1);
        l1c2->append_child(l2c2);
        l1c2->append_child(l2c3);

        std::stringstream ss;
        ss << *root;
        CATCH_REQUIRE(ss.str() == "<root even-root='can have an \"attribute\"'>"
                    "<l1c1/><l1c2><l2c1 color=\"l'escargot\">sub-node has data</l2c1>"
                    "<l2c2/><l2c3 i33=\"&quot;it's both this time&quot;\">"
                    "last-node has data too</l2c3>wierd text + sub-tags</l1c2>"
                    "<l1c3>&lt;\"it's not empty &amp; it works\"&gt;</l1c3></root>");
    }
    CATCH_END_SECTION()
}



CATCH_TEST_CASE("node_errors", "[node][invalid]")
{
    CATCH_START_SECTION("node_errors: bad tag name -- empty")
    {
        CATCH_REQUIRE_THROWS_MATCHES(
                  basic_xml::node(std::string())
                , basic_xml::invalid_token
                , Catch::Matchers::ExceptionMessage(
                          "xml_error: \"\" is not a valid token for a tag name."));
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("node_errors: bad tag name -- invalid character in name")
    {
        for(int count(0); count < 10; ++count)
        {
            char32_t wc(U'\0');

            // generate a valid tag name
            //
            std::string tag_name;
            int max(rand() % 10 + 10);
            for(int len(0); len < max; ++len)
            {
                if(tag_name.empty())
                {
                    wc = SNAP_CATCH2_NAMESPACE::random_char(SNAP_CATCH2_NAMESPACE::character_t::CHARACTER_ZUNICODE);
                    while(!basic_xml::is_name_start_char(wc))
                    {
                        wc = SNAP_CATCH2_NAMESPACE::random_char(SNAP_CATCH2_NAMESPACE::character_t::CHARACTER_ZUNICODE);
                    }
                }
                else
                {
                    wc = SNAP_CATCH2_NAMESPACE::random_char(SNAP_CATCH2_NAMESPACE::character_t::CHARACTER_ZUNICODE);
                    while(!basic_xml::is_name_char(wc))
                    {
                        wc = SNAP_CATCH2_NAMESPACE::random_char(SNAP_CATCH2_NAMESPACE::character_t::CHARACTER_ZUNICODE);
                    }
                }
                tag_name += wc;
            }

            // invalid if it does not start with an alpha character
            //
            char32_t start(SNAP_CATCH2_NAMESPACE::random_char(SNAP_CATCH2_NAMESPACE::character_t::CHARACTER_ZUNICODE));
            while(basic_xml::is_name_start_char(start))
            {
                start = SNAP_CATCH2_NAMESPACE::random_char(SNAP_CATCH2_NAMESPACE::character_t::CHARACTER_ZUNICODE);
            }
            CATCH_REQUIRE_THROWS_MATCHES(
                      basic_xml::node(start + tag_name)
                    , basic_xml::invalid_token
                    , Catch::Matchers::ExceptionMessage(
                              "xml_error: \""
                            + (start + tag_name)
                            + "\" is not a valid token for a tag name."));

            // invalid if invalid char anywhere within
            //
            for(size_t pos(0); pos <= tag_name.length(); ++pos)
            {
                char32_t mid(SNAP_CATCH2_NAMESPACE::random_char(SNAP_CATCH2_NAMESPACE::character_t::CHARACTER_ZUNICODE));
                while(basic_xml::is_name_char(mid))
                {
                    mid = SNAP_CATCH2_NAMESPACE::random_char(SNAP_CATCH2_NAMESPACE::character_t::CHARACTER_ZUNICODE);
                }
                std::string bad_name(
                          tag_name.substr(0, pos)
                        + mid
                        + tag_name.substr(pos));
                CATCH_REQUIRE_THROWS_MATCHES(
                          basic_xml::node(bad_name)
                        , basic_xml::invalid_token
                        , Catch::Matchers::ExceptionMessage(
                                  "xml_error: \""
                                + tag_name.substr(0, pos)
                                + (mid == U'\0' ? "\\0" : libutf8::to_u8string(mid))
                                + tag_name.substr(pos)
                                + "\" is not a valid token for a tag name."));
            }
        }
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("node_errors: bad attribute name -- invalid character in name")
    {
        basic_xml::node n("tag");

        for(int count(0); count < 10; ++count)
        {
            std::string attribute_name;
            int max(rand() % 10 + 10);
            for(int len(0); len < max; ++len)
            {
                switch(rand() % (attribute_name.empty() ? 3 : 5))
                {
                case 0:
                    attribute_name += static_cast<char>(rand() % 26 + 'a');
                    break;

                case 1:
                    attribute_name += static_cast<char>(rand() % 26 + 'A');
                    break;

                case 2:
                    attribute_name += '_';
                    break;

                case 3:
                    attribute_name += static_cast<char>(rand() % 10 + '0');
                    break;

                case 4:
                    attribute_name += '-';
                    break;

                }
            }

            // invalid if it does not start with an alpha character
            //
            char start(rand() % 255 + 1);
            while(basic_xml::is_name_start_char(start))
            {
                start = rand() % 255 + 1;
            }
            CATCH_REQUIRE_THROWS_MATCHES(
                      n.set_attribute(start + attribute_name, "value")
                    , basic_xml::invalid_token
                    , Catch::Matchers::ExceptionMessage(
                              "xml_error: \""
                            + (start + attribute_name)
                            + "\" is not a valid token for an attribute name."));

            // invalid if invalid char anywhere within
            //
            for(size_t pos(0); pos <= attribute_name.length(); ++pos)
            {
                char mid(rand() % 255 + 1);
                while(basic_xml::is_name_char(mid))
                {
                    mid = rand() % 255 + 1;
                }
                std::string bad_name(
                          attribute_name.substr(0, pos)
                        + mid
                        + attribute_name.substr(pos));
                CATCH_REQUIRE_THROWS_MATCHES(
                          n.set_attribute(bad_name, "value")
                        , basic_xml::invalid_token
                        , Catch::Matchers::ExceptionMessage(
                                  "xml_error: \""
                                + bad_name
                                + "\" is not a valid token for an attribute name."));
            }
        }
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("node_errors: re-append a child fails")
    {
        basic_xml::node::pointer_t root(std::make_shared<basic_xml::node>("top"));

        basic_xml::node::pointer_t l1c1(std::make_shared<basic_xml::node>("l1c1"));
        basic_xml::node::pointer_t l1c2(std::make_shared<basic_xml::node>("l1c2"));
        basic_xml::node::pointer_t l1c3(std::make_shared<basic_xml::node>("l1c3"));

        root->append_child(l1c1);
        root->append_child(l1c2);
        root->append_child(l1c3);

        CATCH_REQUIRE_THROWS_MATCHES(
                  l1c1->append_child(l1c2)
                , basic_xml::node_already_in_tree
                , Catch::Matchers::ExceptionMessage(
                          "xml_error: Somehow you are trying to add a child node of a node that was already added to a tree of nodes."));

        CATCH_REQUIRE_THROWS_MATCHES(
                  l1c2->append_child(l1c1)
                , basic_xml::node_already_in_tree
                , Catch::Matchers::ExceptionMessage(
                          "xml_error: Somehow you are trying to add a child node of a node that was already added to a tree of nodes."));

        CATCH_REQUIRE_THROWS_MATCHES(
                  l1c1->append_child(l1c3)
                , basic_xml::node_already_in_tree
                , Catch::Matchers::ExceptionMessage(
                          "xml_error: Somehow you are trying to add a child node of a node that was already added to a tree of nodes."));

        CATCH_REQUIRE_THROWS_MATCHES(
                  l1c1->append_child(root)
                , basic_xml::node_is_root
                , Catch::Matchers::ExceptionMessage(
                          "xml_error: Trying to append the root node within the sub-tree."));

        basic_xml::node::pointer_t l2c1(std::make_shared<basic_xml::node>("l2c1"));
        basic_xml::node::pointer_t l2c2(std::make_shared<basic_xml::node>("l2c2"));
        basic_xml::node::pointer_t l2c3(std::make_shared<basic_xml::node>("l2c3"));

        l1c3->append_child(l2c1);
        l1c3->append_child(l2c2);
        l1c3->append_child(l2c3);

        CATCH_REQUIRE_THROWS_MATCHES(
                  l2c1->append_child(root)
                , basic_xml::node_is_root
                , Catch::Matchers::ExceptionMessage(
                          "xml_error: Trying to append the root node within the sub-tree."));

        CATCH_REQUIRE_THROWS_MATCHES(
                  l2c2->append_child(root)
                , basic_xml::node_is_root
                , Catch::Matchers::ExceptionMessage(
                          "xml_error: Trying to append the root node within the sub-tree."));

        CATCH_REQUIRE_THROWS_MATCHES(
                  l2c3->append_child(root)
                , basic_xml::node_is_root
                , Catch::Matchers::ExceptionMessage(
                          "xml_error: Trying to append the root node within the sub-tree."));
    }
    CATCH_END_SECTION()
}


// vim: ts=4 sw=4 et
