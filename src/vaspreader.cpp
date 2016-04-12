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

#include "vaspreader.h"

/*
 * Default constructor
 *
 * Set the program state to 0. (no particular state)
 */
VaspReader::VaspReader() {
  this->state = 0x00000000;
}

/*
 * Read method
 *
 * Read in a VASP file referred to by <filename>. The state of the program
 * is changed according to which line is being read. First, the number
 * of atoms and the elements are read. Then the dimension of the unit cell
 * and finally the number of atoms and the energy of each of the states. All
 * information is collected and the VaspReader class can be accessed by the
 * Dataset class for data handling.
 */
bool VaspReader::read(const char* filename) {
  std::ifstream infile(filename);


  this->state |= (1 << VASP_OUTCAR_READ_STATE_ELEMENTS);
  this->state |= (1 << VASP_OUTCAR_READ_STATE_IONS_PER_ELEMENT);
  this->state |= (1 << VASP_OUTCAR_READ_STATE_OPEN);

  this->nr_atoms_total = 0;
  this->nr_states = 0;

  /*
   * For each type of regex pattern recognition, a new set of pcre variables
   * are constructed.
   */
  pcre *regex_compiled_vasp_version;
  pcre_extra *pcre_extra_vasp_version;
  pcre *regex_compiled_element;
  pcre_extra *pcre_extra_element;
  pcre *regex_compiled_ions_per_element;
  pcre_extra *pcre_extra_ions_per_element;
  pcre *regex_compiled_lattice_vectors;
  pcre_extra *pcre_extra_lattice_vectors;
  pcre *regex_compiled_atoms;
  pcre_extra *pcre_extra_atoms;
  pcre *regex_compiled_grab_numbers;
  pcre_extra *pcre_extra_grab_numbers;
  pcre *regex_compiled_grab_energy;
  pcre_extra *pcre_extra_grab_energy;

  const char *pcre_error_string;
  const char *pcre_substring_match_string;
  int pcre_error_offset = 0;
  int pcre_exec_ret = 0;
  int pcre_substring_vec[30];
  int pos = 2;

  /*
   * Define all the regex patterns
   */
  const char *string_regex_vasp_version = "^\\s*vasp.([0-9]).[0-9]+.[0-9]+.*$";
  const char *string_regex_element = "^\\s*(VRHFIN\\s+=)([A-Za-z]+)\\s*:.*$";
  const char *string_regex_ions_per_element = "^\\s*(ions per type =\\s+)([0-9 ]+)$";
  const char *string_regex_lattice_vectors = "^\\s*direct lattice vectors.*$";
  const char *string_regex_atoms = "^\\s*POSITION.*$";
  const char *string_regex_grab_numbers = "^\\s+([0-9.-]+)\\s+([0-9.-]+)\\s+([0-9.-]+)\\s+([0-9.-]+)\\s+([0-9.-]+)\\s+([0-9.-]+).*$";
  const char *string_regex_grab_energy = "^\\s+energy  without entropy=\\s+([0-9.-]+)\\s+energy\\(sigma->0\\) =\\s+([0-9.-]+).*$";

 /*
  * Compile all the regex patterns
  */
  regex_compiled_vasp_version = pcre_compile(string_regex_vasp_version, 0, &pcre_error_string, &pcre_error_offset, NULL);
  pcre_extra_vasp_version = pcre_study(regex_compiled_vasp_version, 0, &pcre_error_string);
  regex_compiled_element = pcre_compile(string_regex_element, 0, &pcre_error_string, &pcre_error_offset, NULL);
  pcre_extra_element = pcre_study(regex_compiled_element, 0, &pcre_error_string);
  regex_compiled_ions_per_element = pcre_compile(string_regex_ions_per_element, 0, &pcre_error_string, &pcre_error_offset, NULL);
  pcre_extra_ions_per_element = pcre_study(regex_compiled_ions_per_element, 0, &pcre_error_string);
  regex_compiled_lattice_vectors = pcre_compile(string_regex_lattice_vectors, 0, &pcre_error_string, &pcre_error_offset, NULL);
  pcre_extra_lattice_vectors = pcre_study(regex_compiled_lattice_vectors, 0, &pcre_error_string);
  regex_compiled_atoms = pcre_compile(string_regex_atoms, 0, &pcre_error_string, &pcre_error_offset, NULL);
  pcre_extra_atoms = pcre_study(regex_compiled_atoms, 0, &pcre_error_string);
  regex_compiled_grab_numbers = pcre_compile(string_regex_grab_numbers, 0, &pcre_error_string, &pcre_error_offset, NULL);
  pcre_extra_grab_numbers = pcre_study(regex_compiled_grab_numbers, 0, &pcre_error_string);
  regex_compiled_grab_energy = pcre_compile(string_regex_grab_energy, 0, &pcre_error_string, &pcre_error_offset, NULL);
  pcre_extra_grab_energy = pcre_study(regex_compiled_grab_energy, 0, &pcre_error_string);

  std::string line;
  while (std::getline(infile, line)) { // loop over all the lines in the file

    /*
     * Collect the vasp version (4 or 5)
     */
    if(this->state & (1 << VASP_OUTCAR_READ_STATE_ELEMENTS) ) {
      // get the elements and put these in an array
      pos = 1;
      pcre_exec_ret = pcre_exec(regex_compiled_vasp_version, pcre_extra_vasp_version, line.c_str(), line.length(),
                           0, 0, pcre_substring_vec, 30);
      if(pcre_exec_ret > 0) {
        pcre_get_substring(line.c_str(), pcre_substring_vec, pcre_exec_ret, pos, &(pcre_substring_match_string));
        this->vasp_version = atoi(pcre_substring_match_string);

      }
    }

    /*
     * Collect the elements
     */
    if(this->state & (1 << VASP_OUTCAR_READ_STATE_ELEMENTS) ) {
      // get the elements and put these in an array
      pos = 2;
      pcre_exec_ret = pcre_exec(regex_compiled_element, pcre_extra_element, line.c_str(), line.length(),
                           0, 0, pcre_substring_vec, 30);
      if(pcre_exec_ret > 0) {
        pcre_get_substring(line.c_str(), pcre_substring_vec, pcre_exec_ret, pos, &(pcre_substring_match_string));
        this->elements.push_back(pcre_substring_match_string);
      }
    }

    /*
     * Collect the number of ions of each element type
     */
    if(this->state & (1 << VASP_OUTCAR_READ_STATE_IONS_PER_ELEMENT) ) {
      // get the number of ions per element (type)
      pos = 2;
      pcre_exec_ret = pcre_exec(regex_compiled_ions_per_element, pcre_extra_ions_per_element, line.c_str(), line.length(),
                           0, 0, pcre_substring_vec, 30);
      if(pcre_exec_ret > 0) {
        pcre_get_substring(line.c_str(), pcre_substring_vec, pcre_exec_ret, pos, &(pcre_substring_match_string));
        std::vector<std::string> elements_vector = this->explode(pcre_substring_match_string, " ");
        for(unsigned int i=0; i<elements_vector.size(); i++) {
          if(elements_vector[i].empty() == false) {
            nr_atoms_per_elm.push_back(atoi(elements_vector[i].c_str() ) );
            nr_atoms_total += atoi(elements_vector[i].c_str() );
          }
        }

        // allocate element_uint vector
        for(unsigned int i=0; i<this->elements.size(); i++) {
          this->elements_uint.push_back(this->get_element_number_from_name(this->elements[i]));
        }

        // remove ions state and elements state
        this->state &= ~(1 << VASP_OUTCAR_READ_STATE_ELEMENTS);
        this->state &= ~(1 << VASP_OUTCAR_READ_STATE_IONS_PER_ELEMENT);
        this->state |= (1 << VASP_OUTCAR_READ_STATE_LATTICE_VECTORS);
      }
    }

    /*
     * Collect the dimensions of the unit cell. Note is an IBRION=3 calculation is
     * being run, this is not gathered by this class. It is assumed that each state
     * has the same unit cell. (that means, IBRION != 3 calculations)
     */
    if(this->state & (1 << VASP_OUTCAR_READ_STATE_LATTICE_VECTORS) ) {
      // get the dimensionality of the unit cell
      pos = 0;
      pcre_exec_ret = pcre_exec(regex_compiled_lattice_vectors, pcre_extra_lattice_vectors, line.c_str(), line.length(),
                           0, 0, pcre_substring_vec, 30);
      if(pcre_exec_ret > 0) {
        pcre_get_substring(line.c_str(), pcre_substring_vec, pcre_exec_ret, pos, &(pcre_substring_match_string));
        // grab three lines
        for(int i=0; i<3; i++) {
          std::getline(infile, line);
          pcre_exec_ret = pcre_exec(regex_compiled_grab_numbers, pcre_extra_grab_numbers, line.c_str(), line.length(),
                           0, 0, pcre_substring_vec, 30);
          if(pcre_exec_ret > 0) {
            pos = 1;
            pcre_get_substring(line.c_str(), pcre_substring_vec, pcre_exec_ret, pos, &(pcre_substring_match_string));
            this->dimensions.push_back(atof(pcre_substring_match_string));
            pos = 2;
            pcre_get_substring(line.c_str(), pcre_substring_vec, pcre_exec_ret, pos, &(pcre_substring_match_string));
            this->dimensions.push_back(atof(pcre_substring_match_string));
            pos = 3;
            pcre_get_substring(line.c_str(), pcre_substring_vec, pcre_exec_ret, pos, &(pcre_substring_match_string));
            this->dimensions.push_back(atof(pcre_substring_match_string));
          }
        }
        this->state &= ~(1 << VASP_OUTCAR_READ_STATE_LATTICE_VECTORS);
        this->state |= (1 << VASP_OUTCAR_READ_STATE_ATOMS);
      }
    }

    /*
     * Collect the energy of the state
     */
    if(this->state & (1 << VASP_OUTCAR_READ_STATE_ATOMS) ) {
      pos = 2;
      pcre_exec_ret = pcre_exec(regex_compiled_grab_energy, pcre_extra_grab_energy, line.c_str(), line.length(),
                           0, 0, pcre_substring_vec, 30);
      if(pcre_exec_ret > 0) {
        pcre_get_substring(line.c_str(), pcre_substring_vec, pcre_exec_ret, pos, &(pcre_substring_match_string));
        //std::cout << atof(pcre_substring_match_string) << std::endl;
        this->energies.push_back(atof(pcre_substring_match_string));

        if(this->vasp_version == 5) {
          this->states.push_back(State(this->energies[this->nr_states - 1], this->dimensions, this->atoms, this->elements, this->elements_uint, this->nr_atoms_per_elm, filename, this->nr_states));
          this->atoms.clear();
        }
      }
    }

    /*
     * Collect the atomic positions and forces for this state
     */
    if(this->state & (1 << VASP_OUTCAR_READ_STATE_ATOMS) ) {
      // get the atoms per state
      pos = 0;
      pcre_exec_ret = pcre_exec(regex_compiled_atoms, pcre_extra_atoms, line.c_str(), line.length(),
                           0, 0, pcre_substring_vec, 30);
      if(pcre_exec_ret > 0) {
        this->nr_states++;
        pcre_get_substring(line.c_str(), pcre_substring_vec, pcre_exec_ret, pos, &(pcre_substring_match_string));
        //printf("'%s'\n", pcre_substring_match_string);
        std::getline(infile, line); // discard this line
        for(unsigned i=0; i<this->nr_atoms_per_elm.size(); i++) {
          for(unsigned int j=0; j<this->nr_atoms_per_elm[i]; j++) {
            std::getline(infile, line);
            pcre_exec_ret = pcre_exec(regex_compiled_grab_numbers, pcre_extra_grab_numbers, line.c_str(), line.length(),
                             0, 0, pcre_substring_vec, 30);
            if(pcre_exec_ret > 0) {
              //std::cout << "Atom #" << (i+1) << std::endl;
              float x, y, z, fx, fy, fz;
              pos = 1;
              pcre_get_substring(line.c_str(), pcre_substring_vec, pcre_exec_ret, pos, &(pcre_substring_match_string));
              x = atof(pcre_substring_match_string);
              pos = 2;
              pcre_get_substring(line.c_str(), pcre_substring_vec, pcre_exec_ret, pos, &(pcre_substring_match_string));
              y = atof(pcre_substring_match_string);
              pos = 3;
              pcre_get_substring(line.c_str(), pcre_substring_vec, pcre_exec_ret, pos, &(pcre_substring_match_string));
              z = atof(pcre_substring_match_string);
              pos = 4;
              pcre_get_substring(line.c_str(), pcre_substring_vec, pcre_exec_ret, pos, &(pcre_substring_match_string));
              fx = atof(pcre_substring_match_string);
              pos = 5;
              pcre_get_substring(line.c_str(), pcre_substring_vec, pcre_exec_ret, pos, &(pcre_substring_match_string));
              fy = atof(pcre_substring_match_string);
              pos = 6;
              pcre_get_substring(line.c_str(), pcre_substring_vec, pcre_exec_ret, pos, &(pcre_substring_match_string));
              fz = atof(pcre_substring_match_string);
              this->atoms.push_back(Atom(
                  this->get_element_number_from_name(this->elements[i]),
                  x, y, z, fx, fy, fz
                ));
            }
          }
        }

        if(this->vasp_version == 4) {
          this->states.push_back(State(this->energies[this->nr_states - 1], this->dimensions, this->atoms, this->elements, this->elements_uint, this->nr_atoms_per_elm, filename, this->nr_states));
          this->atoms.clear();
        }
      }
    }
  }
  this->state = (1 << VASP_OUTCAR_READ_STATE_FINISHED);

  // output all information
  std::string elements_string;
  for(unsigned int i=0; i < this->elements.size(); i++) {
    elements_string.append(elements[i]);
    elements_string.append("(");
    elements_string.append(int2str(nr_atoms_per_elm[i]) );
    elements_string.append(")");
    elements_string.append(" ");
  }

  return true;
}

/*
 * Clear the VASPReader class by setting default value to all class variables
 */
void VaspReader::clear() {
  this->state = 0x00000000;
  this->elements.clear();
  this->nr_states = 0;
  this->atoms.clear();
  this->nr_atoms_per_elm.clear();
  this->nr_atoms_total = 0;
  this->dimensions.clear();
  this->states.clear();
  this->energies.clear();
}

/*
 * Returns the number of states currently being held in the class data
 */
const unsigned int& VaspReader::get_number_of_states() const{
  return this->nr_states;
}

/*
 * Returns the number of elements
 */
const std::vector<std::string>& VaspReader::get_elements() const {
  return this->elements;
}

/*
 * Returns a vector of strings given a parent string and a delimiter character. This
 * function is inspired on the PHP function "explode"
 */
std::vector<std::string> VaspReader::explode(std::string const & s, std::string delim) {
    std::vector<std::string> result;
    std::istringstream iss(s);

    for (std::string token; std::getline(iss, token, delim.at(0) ); ) {
        result.push_back(token);
    }

    return result;
}

/*
 * Returns the atom number given a string. This function is made so that the
 * atom number can be used to indicate a position in an array instead of using
 * key-pair arrays.
 */
unsigned int VaspReader::get_element_number_from_name(const std::string &name) {
  if(name.compare("H") == 0) {
    return ATOM_H;
  }
  if(name.compare("C") == 0) {
    return ATOM_C;
  }
  if(name.compare("N") == 0) {
    return ATOM_N;
  }
  if(name.compare("O") == 0) {
    return ATOM_O;
  }
  if(name.compare("Rh") == 0) {
    return ATOM_RH;
  }
  if(name.compare("Fe") == 0) {
    return ATOM_FE;
  }
  if(name.compare("Pd") == 0) {
    return 46;
  }
  return 0;
}
