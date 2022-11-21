
<p align="center">
<img alt="basic-xml" title="Basic XML -- a library to read very basic XML data files."
src="https://snapwebsites.org/sites/snapwebsites.org/files/images/basic-xml.png" width="200" height="200"/>
</p>

# BASIC XML

This library and tools are used to load XML files as configuration files.
This is why it was simplified to the minimum.

## Supported Features

### Processor

The `<?...?>` processor tags are ignored. One can appear at the top (most
likely, the `<?xml ...?>` process tag). It can also appear after the closing
root tag.

### Tags

The usually tags are supported, without namespace.

    <empty/>
    <not-empty>...</not-empty>

The root tag cannot be an empty tag. You may still use a tag without any
data or sub tags as in:

    <!-- this is permitted -->
    <root></root>

### Tag Attributes

The tags can be assigned attributes as in:

    <tag-name attribute-name="attribute-value">...</tag-name>

The number of attributes is not limited, however, the same attribute cannot
be redefined more than once (in most likelihood, in a configuration file, it
would be a duplicate parameter and thus a bug).

### Content

You can include content between an opening and a closing tags. Spaces before
the first non-space character are removed. Spaces between the last non-space
and the closing tag are removed.

You may use the `<![CDATA[...]]>` syntax in order to read verbatim data.

By default, the `node::text()` function trims the text. If you want the raw
text, use the flag: `n->text(false)`.

**WARNING:** All the content is saved in the `node` object directly. If it
is interspersed with sub-tags, the loader loses that information. It will
look like one block of text to the library user. In most cases, configuration
files with tags that expect such data will not include sub-tags.

### Comments

The library supports comments. These are viewed as white spaces and are just
skipped.

    <!-- comments are completely removed from the resulting node tree -->

### Entities

The library is limited to the basic XML entities. No declarations are possible
so you cannot define more.

* `&amp;` -- the `&` character
* `&lt;` -- the `<` character
* `&gt;` -- the `>` character
* `&quot;` -- the `"` character
* `&apos;` -- the `'` character

### Tag & Attribute Names

The characters you can use to name tags and attributes is limited to:

* `a` - `z` -- lower case latin letters
* `A` - `Z` -- upper case latin letters
* `0` - `9` -- digits
* `_` -- the underscore
* `-` -- the dash

A name must start with a letter or the underscore character (`[a-zA-Z_]`).

A name cannot end with a dash (`-`).

### Namespaces

Namespaces are not supported at all.


# KNOWN BUGS

## The `\r` Character

The `\r` character is transformed into a `\n` character in your `text()`.

This is because we transform the `\r\n` sequence in just `\n` and that implies
adding +1 to the current line number. I don't really see why that would ever
be an issue anyway.


# REPORTING BUGS

Submit bug reports and patches on
[github](https://github.com/m2osw/basic-xml/issues).


_This file is part of the [snapcpp project](https://snapwebsites.org/)._

vim: ts=4 sw=4 et
