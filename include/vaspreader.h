/*************************************************************************
 *
 *  This file is part of VeeVee.
 *  https://github.com/imc-codeteam/veevee
 *
 *  Author: Ivo Filot <i.a.w.filot@tue.nl>
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

/*
 * Read in the VASP OUTCAR files. The atoms and corresponding positions and
 * forces are read. All the states (ionic steps) are collected and their
 * energies are corrected for the zero-energy-definition of LAMMPS. That is,
 * all energies are given with respect to all the *atoms* infinitely far
 * apart from each other. This is done by adding the energy of the individual
 * atoms to the total energy of the state. For example, to get the correct
 * H2 energy, twice the energy of a single H atom is added to the H2 energy
 * as calculated by VASP.
 */

#ifndef _VASPREADER_H
#define _VASPREADER_H

#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <sstream>
#include <pcre.h>

#include "lexical_casts.h"
#include "atom.h"
#include "state.h"
#include "atom_constants.h"

/*
 * The program has several operational states, in short called state. These
 * states can have the following flags. The state is configured using bitwise
 * operations.
 */

#define VASP_OUTCAR_READ_STATE_UNDEFINED 0
#define VASP_OUTCAR_READ_STATE_ELEMENTS 1
#define VASP_OUTCAR_READ_STATE_IONS_PER_ELEMENT 2
#define VASP_OUTCAR_READ_STATE_LATTICE_VECTORS 3
#define VASP_OUTCAR_READ_STATE_ATOMS 4
#define VASP_OUTCAR_READ_STATE_OPEN 5
#define VASP_OUTCAR_READ_STATE_FINISHED 6


class VaspReader {
private:
  unsigned int vasp_version;
  unsigned int state;   // read state
  std::vector<unsigned int> nr_atoms_per_elm;
  unsigned int nr_atoms_total;
  std::vector<std::string> elements;
  std::vector<unsigned int> elements_uint;
  unsigned int nr_states;
  std::vector<Atom> atoms;
  std::vector<float> dimensions;
  std::vector<double> energies;

public:
  VaspReader();
  bool read(const char*);
  void clear(); //removes all information from VaspReader

  const unsigned int& get_number_of_states() const;
  const std::vector<std::string>& get_elements() const;

  std::vector<State> states;

private:
  std::vector<std::string> explode(std::string const & s, std::string delim);
  unsigned int get_element_number_from_name(const std::string &name);
};

#endif // _VASPREADER_H
