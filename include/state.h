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

#ifndef _STATE_H
#define _STATE_H

#include <vector>
#include <string>
#include <fstream>
#include <iostream>

#include "lexical_casts.h"
#include "atom.h"
#include "mathfunc.h"

class State {
private:
  double energy;                  // energy of the system [eV]
  std::vector<std::string> elements;
  std::vector<unsigned int> elements_uint;
  std::vector<unsigned int> nr_atoms;
  std::string filename;
  unsigned int state_id_in_file;

public:
  State(
    const double &_energy,
    const std::vector<float> &_dimensions,
    const std::vector<Atom> &_atoms,
    const std::vector<std::string> &_elements,
    const std::vector<unsigned int> &_elements_uint,
    const std::vector<unsigned int> &_nr_atoms,
    const std::string &_filename,
    const unsigned int &_id
  );

  State(
    const double &_energy,
    const std::vector<float> &_dimensions
  );

  State(
    const double &_energy,
    const Matrix3 &_dimensions
  );

  unsigned int atom_cnt;
  unsigned int bond_cnt;

  std::vector<Atom> atoms;        // atoms in the unit cell
  Matrix3 dimensions;             // dimension of the unit cell [A]
  MatrixX4f coordinates;           // 3xN matrix holding the atom positions

  Vector3 get_center();
  const std::string& get_filename() const;
  unsigned int get_total_nr_atoms() const;
  unsigned int get_atoms_for_element(unsigned int i) const;
  unsigned int get_nr_elements() const;
  const double& get_energy() const;
  std::vector<float> get_atom_position(unsigned int i) const;
  const std::vector<std::string>& get_elements() const;
  void allocate_coordinate_matrix();

  std::string output_atoms_line();
  std::string output_atom_coordinates();
  void save_to_poscar(const char* filename, const char* name, bool is_vasp5);

private:

};

#endif //_STATE_H
