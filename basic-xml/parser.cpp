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
#include    "basic-xml/parser.h"

#include    "basic-xml/exception.h"
#include    "basic-xml/type.h"


// libutf8
//
#include    <libutf8/base.h>
#include    <libutf8/libutf8.h>


// snapdev
//
#include    <snapdev/not_reached.h>
#include    <snapdev/trim_string.h>


// last include
//
#include    <snapdev/poison.h>



namespace basic_xml
{



parser::parser(
          std::string const & filename
        , std::istream & in
        , node::pointer_t & root)
    : f_filename(filename)
    , f_in(in)
{
    load(root);
}


/** \brief Load the XML as node in \p root.
 *
 * This function loads what it considers valid XML.
 *
 * \param[out] root  A reference to the root pointer where the results are saved.
 */
void parser::load(node::pointer_t & root)
{
    token_t tok(get_token(false));

    tok = skip_empty(tok);
    if(tok == token_t::TOK_PROCESSOR) // allow <?xml ... ?>
    {
        tok = get_token(false);
    }
    tok = skip_empty(tok);

    // now we have to have the root tag
    if(tok != token_t::TOK_OPEN_TAG)
    {
        throw unexpected_token(
                  f_filename
                + ':'
                + std::to_string(f_line)
                + ": cannot be empty or include anything other than a processor tag and comments before the root tag.");
    }
    root = std::make_shared<node>(f_value);
    if(read_tag_attributes(root) == token_t::TOK_EMPTY_TAG)
    {
        throw unexpected_token(
                  f_filename
                + ':'
                + std::to_string(f_line)
                + ": root tag cannot be an empty tag.");
    }
    tok = get_token(false);

    node::pointer_t parent(root);
    for(;;)
    {
        switch(tok)
        {
        case token_t::TOK_OPEN_TAG:
            {
                node::pointer_t child(std::make_shared<node>(f_value));
                parent->append_child(child);
                if(read_tag_attributes(child) == token_t::TOK_END_TAG)
                {
                    parent = child;
                }
            }
            break;

        case token_t::TOK_CLOSE_TAG:
            if(parent->tag_name() != f_value)
            {
                throw unexpected_token(
                          f_filename
                        + ':'
                        + std::to_string(f_line)
                        + ": unexpected token \""
                        + f_value
                        + "\" in this closing tag; expected \""
                        + parent->tag_name()
                        + "\" instead.");
            }
            parent = parent->parent();
            if(parent == nullptr)
            {
                for(;;)
                {
                    tok = get_token(false);
                    switch(tok)
                    {
                    case token_t::TOK_EOF:
                        // it worked, we're done
                        //
                        return;

                    case token_t::TOK_TEXT:
                        tok = skip_empty(tok);
                        break;

                    case token_t::TOK_PROCESSOR:
                        // completely ignore those
                        break;

                    default:
                        throw unexpected_token(
                                  f_filename
                                + ':'
                                + std::to_string(f_line)
                                + ": we reached the end of the XML file, but still found a token of type "
                                + std::to_string(static_cast<int>(tok))
                                + " after the closing root tag instead of the end of the file.");

                    }
                }
            }
            break;

        case token_t::TOK_TEXT:
            parent->append_text(f_value);
            break;

        case token_t::TOK_EOF:
            throw unexpected_token(
                    f_filename
                  + ':'
                  + std::to_string(f_line)
                  + ": reached the end of the file without first closing the root tag.");

        // LCOV_EXCL_START
        case token_t::TOK_EMPTY_TAG:
        case token_t::TOK_END_TAG:
        case token_t::TOK_EQUAL:
        case token_t::TOK_IDENTIFIER:
        case token_t::TOK_PROCESSOR:
        case token_t::TOK_STRING:
            throw logic_error("Received an unexpected token in the switch handler.");
        // LCOV_EXCL_STOP

        }
        tok = get_token(false);
    }
}


parser::token_t parser::skip_empty(token_t tok)
{
    while(tok == token_t::TOK_TEXT)
    {
        f_value = snapdev::trim_string(f_value);
        if(!f_value.empty())
        {
            throw unexpected_token(
                      f_filename
                    + ':'
                    + std::to_string(f_line)
                    + ": cannot include text data before or after the root tag.");
        }
        tok = get_token(false);
    }

    return tok;
}


parser::token_t parser::read_tag_attributes(node::pointer_t & tag)
{
    for(;;)
    {
        token_t tok(get_token(true));
        if(tok == token_t::TOK_END_TAG
        || tok == token_t::TOK_EMPTY_TAG)
        {
            return tok;
        }
        if(tok != token_t::TOK_IDENTIFIER)
        {
            throw invalid_xml(
                      f_filename
                    + ':'
                    + std::to_string(f_line)
                    + ": expected the end of the tag (>) or an attribute name.");
        }
        std::string const name(f_value);
        tok = get_token(true);
        if(tok != token_t::TOK_EQUAL)
        {
            throw invalid_xml(
                      f_filename
                    + ':'
                    + std::to_string(f_line)
                    + ": expected the '=' character between the attribute name and value.");
        }
        tok = get_token(true);
        if(tok != token_t::TOK_STRING)
        {
            throw invalid_xml(
                      f_filename
                    + ':'
                    + std::to_string(f_line)
                    + ": expected a quoted value after the '=' sign.");
        }
        if(!tag->attribute(name).empty())
        {
            throw invalid_xml(
                      f_filename
                    + ':'
                    + std::to_string(f_line)
                    + ": attribute \"" + name + "\" defined twice; we do not allow such.");
        }
        tag->set_attribute(name, f_value);
    }
    snapdev::NOT_REACHED();
}


parser::token_t parser::get_token(bool parsing_attributes)
{
    f_value.clear();

    for(;;)
    {
        char32_t c(getc());
        switch(c)
        {
        case static_cast<char32_t>(EOF):
            return token_t::TOK_EOF;

        case ' ':
        case '\t':
        case '\v':
        case '\f':
        case '\n':
            if(parsing_attributes)
            {
                continue;
            }
            break;

        case '<':
            c = getc();
            switch(c)
            {
            case '?':
                // we do not parse the processor entry, we do not care about
                // it at the moment
                //
                for(;;)
                {
                    c = getc();
                    if(c == static_cast<char32_t>(EOF))
                    {
                        throw unexpected_eof(
                              f_filename
                            + ':'
                            + std::to_string(f_line)
                            + ": reached the end of the file while reading a processor (\"<?...?>\") tag.");
                    }
                    while(c == '?')
                    {
                        c = getc();
                        if(c == '>')
                        {
                            return token_t::TOK_PROCESSOR;
                        }
                        f_value += '?';
                    }
                    f_value += libutf8::to_u8string(c);
                }
                snapdev::NOT_REACHED();
                return token_t::TOK_PROCESSOR;

            case '!':
                c = getc();
                if((c >= 'A' && c <= 'Z')
                || (c >= 'a' && c <= 'z'))
                {
                    // of course, this may be anything other than an element but still something we don't support
                    //
                    throw invalid_xml(
                          f_filename
                        + ':'
                        + std::to_string(f_line)
                        + ": found an element definition (such as an \"<!ELEMENT...>\" sequence), which is not supported.");
                }
                if(c == '[')
                {
                    // <![CDATA[ ... or throw
                    //
                    char32_t const * expected = U"CDATA[";
                    for(int j(0); j < 6; ++j)
                    {
                        if(getc() != expected[j])
                        {
                            throw invalid_xml(
                                  f_filename
                                + ':'
                                + std::to_string(f_line)
                                + ": found an unexpected sequence of character in a \"<![CDATA[...\" sequence.");
                        }
                    }
                    for(;;)
                    {
                        c = getc();
                        if(c == static_cast<char32_t>(EOF))
                        {
                            throw unexpected_eof(
                                  f_filename
                                + ':'
                                + std::to_string(f_line)
                                + ": found EOF while parsing a \"<![CDATA[...]]>\" sequence.");
                        }
                        if(c == ']')
                        {
                            c = getc();
                            if(c == ']')
                            {
                                c = getc();
                                while(c == ']')
                                {
                                    f_value += ']';
                                    c = getc();
                                }
                                if(c == '>')
                                {
                                    // this is just like some text
                                    // except we do not convert entities
                                    //
                                    return token_t::TOK_TEXT;
                                }
                                f_value += "]]";
                                f_value += libutf8::to_u8string(c);
                            }
                            else
                            {
                                f_value += ']';
                                f_value += libutf8::to_u8string(c);
                            }
                        }
                        else
                        {
                            f_value += libutf8::to_u8string(c);
                        }
                    }
                }
                if(c == '-')
                {
                    c = getc();
                    if(c == '-')
                    {
                        // this is a comment, we do not record them, they
                        // just get dropped
                        //
                        bool found(false);
                        while(!found)
                        {
                            c = getc();
                            if(c == static_cast<char32_t>(EOF))
                            {
                                throw unexpected_eof(
                                      f_filename
                                    + ':'
                                    + std::to_string(f_line)
                                    + ": found EOF while parsing a comment (\"<!--...-->\") sequence.");
                            }
                            if(c == '-')
                            {
                                c = getc();
                                while(c == '-')
                                {
                                    c = getc();
                                    if(c == '>')
                                    {
                                        found = true;
                                        break;
                                    }
                                }
                            }
                        }
                        continue;
                    }
                }
                throw invalid_token(
                          f_filename
                        + ':'
                        + std::to_string(f_line)
                        + std::string(": character '")
                        + libutf8::to_u8string(c)
                        + "' was not expected after a \"<!\" sequence.");

            case '/':
                c = getc();
                while(is_space(c))
                {
                    c = getc();
                }
                if(!is_name_start_char(c))
                {
                    if(c == static_cast<char32_t>(EOF))
                    {
                        throw unexpected_eof(
                              f_filename
                            + ':'
                            + std::to_string(f_line)
                            + ": expected a tag name after \"</\", not EOF.");
                    }
                    throw invalid_token(
                              f_filename
                            + ':'
                            + std::to_string(f_line)
                            + ": character '"
                            + libutf8::to_u8string(c)
                            + "' is not valid for a tag name.");
                }
                for(;;)
                {
                    f_value += libutf8::to_u8string(c);
                    c = getc();
                    if(!is_name_char(c))
                    {
                        break;
                    }
                }
                while(is_space(c))
                {
                    c = getc();
                }
                if(c != '>')
                {
                    if(c == static_cast<char32_t>(EOF))
                    {
                        throw unexpected_eof(
                              f_filename
                            + ':'
                            + std::to_string(f_line)
                            + ": expected '>', not EOF.");
                    }
                    throw invalid_xml(
                              f_filename
                            + ':'
                            + std::to_string(f_line)
                            + ": found an unexpected '"
                            + static_cast<char>(c)
                            + "' in a closing tag, expected '>' instead.");
                }
                return token_t::TOK_CLOSE_TAG;

            }

            // in this case we need to read the name only, the attributes
            // will be read by the parser instead of the lexer
            //
            while(is_space(c))
            {
                c = getc();
            }
            if(!is_name_start_char(c))
            {
                if(c == static_cast<char32_t>(EOF))
                {
                    throw unexpected_eof(
                          f_filename
                        + ':'
                        + std::to_string(f_line)
                        + ": expected a tag name after '<', not EOF.");
                }
                throw invalid_token(
                          f_filename
                        + ':'
                        + std::to_string(f_line)
                        + ": character '"
                        + libutf8::to_u8string(c)
                        + "' is not valid for a tag name.");
            }
            for(;;)
            {
                f_value += libutf8::to_u8string(c);
                c = getc();
                if(!is_name_char(c))
                {
                    break;
                }
            }
            if(isspace(c))
            {
                do
                {
                    c = getc();
                }
                while(isspace(c));
            }
            else if(c != '>' && c != '/')
            {
                throw invalid_token(
                          f_filename
                        + ':'
                        + std::to_string(f_line)
                        + ": character '"
                        + libutf8::to_u8string(c)
                        + "' is not valid right after a tag name.");
            }
            ungetc(c);
            return token_t::TOK_OPEN_TAG;

        case '>':
            if(parsing_attributes)
            {
                return token_t::TOK_END_TAG;
            }
            break;

        case '/':
            if(parsing_attributes)
            {
                c = getc();
                if(c == '>')
                {
                    return token_t::TOK_EMPTY_TAG;
                }
                ungetc(c);
                c = '/';
            }
            break;

        case '=':
            if(parsing_attributes)
            {
                return token_t::TOK_EQUAL;
            }
            break;

        case '"':
        case '\'':
            if(parsing_attributes)
            {
                auto quote(c);
                for(;;)
                {
                    c = getc();
                    if(c == quote)
                    {
                        unescape_entities();
                        return token_t::TOK_STRING;
                    }
                    if(c == '>')
                    {
                        throw invalid_token(
                              f_filename
                            + ':'
                            + std::to_string(f_line)
                            + ": character '>' not expected inside a tag value; please use \"&gt;\" instead.");
                    }
                    f_value += libutf8::to_u8string(c);
                }
                snapdev::NOT_REACHED();
            }
            break;

        }

        if(parsing_attributes
        && is_name_char(c))
        {
            for(;;)
            {
                f_value += libutf8::to_u8string(c);
                c = getc();
                if(!is_name_char(c))
                {
                    ungetc(c);
                    return token_t::TOK_IDENTIFIER;
                }
            }
            snapdev::NOT_REACHED();
        }

        for(;;)
        {
            f_value += libutf8::to_u8string(c);
            c = getc();
            if(c == '<'
            || c == static_cast<decltype(c)>(EOF))
            {
                ungetc(c);
                unescape_entities();
                return token_t::TOK_TEXT;
            }
        }
    }
}


void parser::unescape_entities()
{
    for(std::string::size_type pos(0);;)
    {
        pos = f_value.find('&', pos);
        if(pos == std::string::npos)
        {
            break;
        }
        std::string::size_type const end(f_value.find(';', pos + 1));
        if(end == std::string::npos)
        {
            // generate an error here?
            //
            break;
        }
        std::string name(f_value.substr(pos + 1, end - pos - 1));
        if(name == "amp")
        {
            f_value.replace(pos, end - pos + 1, 1, '&');
            ++pos;
        }
        else if(name == "quot")
        {
            f_value.replace(pos, end - pos + 1, 1, '"');
            ++pos;
        }
        else if(name == "lt")
        {
            f_value.replace(pos, end - pos + 1, 1, '<');
            ++pos;
        }
        else if(name == "gt")
        {
            f_value.replace(pos, end - pos + 1, 1, '>');
            ++pos;
        }
        else if(name == "apos")
        {
            f_value.replace(pos, end - pos + 1, 1, '\'');
            ++pos;
        }
        else if(name.empty())
        {
            throw invalid_entity(
                      f_filename
                    + ':'
                    + std::to_string(f_line)
                    + ": the name of an entity cannot be empty (\"&;\" is not valid XML).");
        }
        else if(name[0] == '#')
        {
            if(name.length() == 1)
            {
                throw invalid_entity(
                      f_filename
                    + ':'
                    + std::to_string(f_line)
                    + ": a numeric entity must have a number (\"&#;\" is not valid XML).");
            }
            int base(10);
            if(name[1] == 'x'
            || name[1] == 'X')
            {
                name[0] = '0';
                base = 16;
            }
            else
            {
                name[0] = ' ';
            }
            errno = 0;
            char * e(nullptr);
            char32_t const unicode(strtol(name.c_str(), &e, base));
            if(errno != 0
            || e == nullptr
            || *e != '\0')
            {
                throw invalid_number(
                      f_filename
                    + ':'
                    + std::to_string(f_line)
                    + ": the number found in numeric entity, \""
                    + name
                    + "\", is not considered valid.");
            }
            std::string const utf8(libutf8::to_u8string(unicode));
            f_value.replace(pos, end - pos + 1, utf8);
            pos += utf8.length();
        }
        else
        {
            throw invalid_entity(
                      f_filename
                    + ':'
                    + std::to_string(f_line)
                    + ": unsupported entity (\"&"
                    + name
                    + ";\").");
        }
    }
}


char32_t parser::getc()
{
    if(f_ungetc_pos > 0)
    {
        --f_ungetc_pos;
        return f_ungetc[f_ungetc_pos];
    }

    int c(f_in.get());
    if(c == '\r')
    {
        ++f_line;
        c = f_in.get();
        if(c != '\n')
        {
            ungetc(c);
            c = '\n';
        }
    }
    else if(c == '\n')
    {
        ++f_line;
    }

    if(c >= 0x80)
    {
        // define the number of bytes required (assuming valid UTF-8)
        //
        std::size_t const count(c < 0xE0 ? 2UL : (c < 0xF0 ? 3UL : 4UL));
        char input[5];
        input[0] = c;
        std::size_t len(1);
        for(; len < count; ++len)
        {
            c = f_in.get();
            if(c < 0x80 || c >= 0xC0)
            {
                // not valid, at least don't eat the next byte improperly
                //
                ungetc(c);
                break;
            }
            input[len] += c;
        }
        input[len] = '\0';
        char32_t result(U'\0');
        char * s(input);
        if(libutf8::mbstowc(result, s, len) == -1)
        {
            return U'\xFFFD';
        }
        return result;
    }
    else
    {
        return c;
    }
}


void parser::ungetc(char32_t c)
{
    if(c != static_cast<char32_t>(EOF))
    {
        if(f_ungetc_pos >= std::size(f_ungetc))
        {
            // LCOV_EXCL_START
            throw logic_error(
                      f_filename
                    + ':'
                    + std::to_string(f_line)
                    + ": somehow the f_ungetc buffer was overflowed.");
            // LCOV_EXCL_STOP
        }

        f_ungetc[f_ungetc_pos] = c;
        ++f_ungetc_pos;
    }
}



} // namespace basic_xml
// vim: ts=4 sw=4 et
