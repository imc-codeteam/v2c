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

#include "atom.h"

/*
 * Default empty atom constructor
 */
Atom::Atom() {
    this->init(0, 0, 0, 0, 0.0f, 0.0f, 0.0f);
}

/**
 * Atom constructor
 *
 * Default constructor
 *
 */
Atom::Atom(const unsigned int &_an,   // element (i.e. # of protons)
         const float &_x,             // x position in [A]
         const float &_y,             // y position in [A]
         const float &_z) {           // z position in [A]
  this->init(_an, _x, _y, _z, 0.0f, 0.0f, 0.0f);
}

/**
 * Atom constructor
 *
 * Default constructor
 *
 */
Atom::Atom(const unsigned int &_an,   // element (i.e. # of protons)
         const float &_x,             // x position in [A]
         const float &_y,             // y position in [A]
         const float &_z,            // z position in [A]
         const float &_fx,           // x force in [eV/A]
         const float &_fy,           // y force in [eV/A]
         const float &_fz) {         // z force in [eV/A]
  this->init(_an, _x, _y, _z, _fx, _fy, _fz);

}

/**
 * Method
 *
 * Modify the forces on the atom
 *
 */
void Atom::set_force(const float &_fx,           // x force in [eV/A]
                     const float &_fy,           // y force in [eV/A]
                     const float &_fz) {         // z force in [eV/A]
  this->force = Vector3(_fx, _fy, _fz);
}

/**
 * Method
 *
 * return the x position
 *
 */
const float Atom::get_x() const {
  return this->pos(0);
}

/**
 * Method
 *
 * return the y position
 *
 */
const float Atom::get_y() const {
  return this->pos(1);
}

/**
 * Method
 *
 * return the z position
 *
 */
const float Atom::get_z() const {
  return this->pos(2);
}

/**
 * Method
 *
 * Convenience function for the constructors (DRY)
 *
 */
void Atom::init(const unsigned int &_an,    // element (i.e. # of protons)
                           const float &_x,            // x position in [A]
                           const float &_y,            // y position in [A]
                           const float &_z,            // z position in [A]
                           const float &_fx,           // x force in [eV/A]
                           const float &_fy,           // y force in [eV/A]
                           const float &_fz) {         // z force in [eV/A]
  this->elnr = _an;
  this->pos = Vector3(_x, _y, _z);
  this->force = Vector3(_fx, _fy, _fz);
  this->exp = false;
  this->selec_mode = 0;
}
