// Copyright (c) 2019-2022  Made to Order Software Corp.  All Rights Reserved
//
// https://snapwebsites.org/project/prinbee
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
#pragma once

/** \file
 * \brief Snap! Database exceptions.
 *
 * This files declares a few exceptions that the database uses when a
 * parameter is wrong or something goes wrong (can't open a file, can't
 * create a lock for the context, etc.)
 *
 * The prinbee also makes use of the snaplogger so it will emit
 * a corresponding error to the log before throwing an exception.
 */


// libexcept
//
#include    <libexcept/exception.h>


namespace basic_xml
{



DECLARE_LOGIC_ERROR(logic_error);

DECLARE_OUT_OF_RANGE(out_of_range);

DECLARE_MAIN_EXCEPTION(xml_error);

DECLARE_EXCEPTION(xml_error, file_not_found);
DECLARE_EXCEPTION(xml_error, invalid_entity);
DECLARE_EXCEPTION(xml_error, invalid_number);
DECLARE_EXCEPTION(xml_error, invalid_token);
DECLARE_EXCEPTION(xml_error, invalid_xml);
DECLARE_EXCEPTION(xml_error, node_already_in_tree);
DECLARE_EXCEPTION(xml_error, node_is_root);
DECLARE_EXCEPTION(xml_error, unexpected_eof);
DECLARE_EXCEPTION(xml_error, unexpected_token);



} // namespace basic_xml
// vim: ts=4 sw=4 et
