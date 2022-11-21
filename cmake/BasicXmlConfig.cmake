# - Find BasicXml
#
# BASIC_XML_FOUND        - System has BasicXml
# BASIC_XML_INCLUDE_DIRS - The BasicXml include directories
# BASIC_XML_LIBRARIES    - The libraries needed to use BasicXml
# BASIC_XML_DEFINITIONS  - Compiler switches required for using BasicXml
#
# License:
#
# Copyright (c) 2011-2022  Made to Order Software Corp.  All Rights Reserved
#
# https://snapwebsites.org/project/basic-xml
# contact@m2osw.com
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <https://www.gnu.org/licenses/>.

find_path(
    BASIC_XML_INCLUDE_DIR
        basic-xml/xml.h

    PATHS
        ENV BASIC_XML_INCLUDE_DIR
)

find_library(
    BASIC_XML_LIBRARY
        basic-xml

    PATHS
        ${BASIC_XML_LIBRARY_DIR}
        ENV BASIC_XML_LIBRARY
)

mark_as_advanced(
    BASIC_XML_INCLUDE_DIR
    BASIC_XML_LIBRARY
)

set(BASIC_XML_INCLUDE_DIRS ${BASIC_XML_INCLUDE_DIR})
set(BASIC_XML_LIBRARIES    ${BASIC_XML_LIBRARY})

include(FindPackageHandleStandardArgs)

find_package_handle_standard_args(
    BasicXml
    REQUIRED_VARS
        BASIC_XML_INCLUDE_DIR
        BASIC_XML_LIBRARY
)

# vim: ts=4 sw=4 et
