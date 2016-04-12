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

#ifndef _ATOM_H
#define _ATOM_H

#include <string>
#include <iostream>

#include "mathfunc.h"

class Atom {
public:
  unsigned int elnr;
  Vector3 pos;
  Vector3 force;
  bool exp;
  int selec_mode;

  // default constructor
  Atom();
  Atom(const unsigned int &_an,   // element (i.e. # of protons)
       const float &_x,           // x position in [A]
       const float &_y,           // y position in [A]
       const float &_z);          // z position in [A]

  Atom(const unsigned int &_an,    // element (i.e. # of protons)
       const float &_x,            // x position in [A]
       const float &_y,            // y position in [A]
       const float &_z,            // z position in [A]
       const float &_fx,           // x force in [eV/A]
       const float &_fy,           // y force in [eV/A]
       const float &_fz);          // z force in [eV/A]

  void set_force(const float &_fx,           // x force in [eV/A]
                 const float &_fy,           // y force in [eV/A]
                 const float &_fz);          // z force in [eV/A]

  const float get_x() const;
  const float get_y() const;
  const float get_z() const;

private:
  void init(const unsigned int &_an,    // element (i.e. # of protons)
            const float &_x,            // x position in [A]
             const float &_y,            // y position in [A]
             const float &_z,            // z position in [A]
             const float &_fx,           // x force in [eV/A]
             const float &_fy,           // y force in [eV/A]
             const float &_fz);          // z force in [eV/A]

};

#endif //_ATOM_H
