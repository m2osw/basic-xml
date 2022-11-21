# Copyright (c) 2019-2022  Made to Order Software Corp.  All Rights Reserved
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

cmake_minimum_required(VERSION 3.10.2)

# The Snap! C++ Versatile Database Support
project(basic_xml_project)

find_package(Boost            REQUIRED)
find_package(LibExcept        REQUIRED)
find_package(LibUtf8          REQUIRED)
find_package(SnapCMakeModules REQUIRED)
find_package(SnapDev          REQUIRED)

SnapGetVersion(BASIC_XML ${CMAKE_CURRENT_SOURCE_DIR})

include_directories(
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
)


add_subdirectory(cmake)
#add_subdirectory(conf)
add_subdirectory(basic-xml)
add_subdirectory(tests)
add_subdirectory(doc)


# vim: ts=4 sw=4 et
