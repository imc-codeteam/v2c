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

#include "lexical_casts.h"

std::string int2str(const int &i) {
  std::stringstream ss;
  ss << i;
  return std::string(ss.str());
}

std::string float2str(const float &i) {
  std::stringstream ss;
  ss << i;
  return std::string(ss.str());
}

std::string float2str2(const float &i, const char* str) {
    char buffer [100];
	#ifdef WIN32
		_snprintf_s(buffer, 100, str, i);
	#else
		snprintf(buffer, 100, str, i);
	#endif
    return std::string(buffer);
}

std::string double2str(const double &i) {
  std::stringstream ss;
  ss << i;
  return std::string(ss.str());
}

float str2float(const std::string &_str) {
    float result = (float)atof(_str.c_str());
    return result;
}

int hex2int(const std::string &_str) {
    unsigned int x;
    std::stringstream ss;
    ss << std::hex << _str;
    ss >> x;
    return x;
}
