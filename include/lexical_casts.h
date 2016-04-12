/*************************************************************************
 *
 *  This file is part of VeeVee.
 *
 *  Author: Ivo Filot <ivo@ivofilot.nl>
 *
 *  VeeVee is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  VeeVee is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with VeeVee.  If not, see <http://www.gnu.org/licenses/>.
 *
 ************************************************************************/

#ifndef _LEXICAL_CAST_H
#define _LEXICAL_CAST_H

#include <string>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>

std::string int2str(const int &i);
std::string float2str(const float &i);
std::string float2str2(const float &i, const char* str);
std::string double2str(const double &i);

float str2float(const std::string &_str);
int hex2int(const std::string &_str);

#endif
