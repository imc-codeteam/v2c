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

#include "state.h"

State::State(
    const double &_energy,
    const std::vector<float> &_dimensions,
    const std::vector<Atom> &_atoms,
    const std::vector<std::string> &_elements,
    const std::vector<unsigned int> &_elements_uint,
    const std::vector<unsigned int> &_nr_atoms,
    const std::string &_filename,
    const unsigned int &_id
  ) {
    this->energy = _energy;

    for(unsigned int i=0; i<3; i++) {
        for(unsigned int j=0; j<3; j++) {
            this->dimensions(i,j) = _dimensions[i * 3 + j];
        }
    }

    this->atoms = _atoms;
    this->elements = _elements;
    this->elements_uint = _elements_uint;
    this->nr_atoms = _nr_atoms;
    this->filename = _filename;
    this->state_id_in_file = _id;
    this->atom_cnt = this->atoms.size();
    this->allocate_coordinate_matrix();
}

State::State(
    const double &_energy,
    const std::vector<float> &_dimensions
  ) {
    this->energy = _energy;

    for(unsigned int i=0; i<3; i++) {
        for(unsigned int j=0; j<3; j++) {
            this->dimensions(i,j) = _dimensions[i * 3 + j];
        }
    }

    this->atom_cnt = this->atoms.size();
    this->allocate_coordinate_matrix();
}

State::State(
    const double &_energy,
    const Matrix3 &_dimensions
  ) {
    this->energy = _energy;
    this->dimensions = _dimensions;
}

const std::string& State::get_filename() const {
    return this->filename;
}

unsigned int State::get_total_nr_atoms() const {
    return this->atom_cnt;
}

unsigned int State::get_nr_elements() const {
    return this->elements.size();
}

unsigned int State::get_atoms_for_element(unsigned int i) const {
    return this->nr_atoms[i];
}

const double& State::get_energy() const {
    return this->energy;
}

std::vector<float> State::get_atom_position(unsigned int i) const {
    std::vector<float> pos;

    pos.push_back(this->atoms[i].get_x());
    pos.push_back(this->atoms[i].get_y());
    pos.push_back(this->atoms[i].get_z());

    return pos;
}

Vector3 State::get_center() {

    if(this->atoms.size() == 0) {
        return this->dimensions.row(0) / 2 +
               this->dimensions.row(1) / 2 +
               this->dimensions.row(2) / 2;
    }

    float x = 0;
    float y = 0;
    float z = 0;

    for(unsigned int i=0; i<this->atoms.size(); i++) {
        x += this->atoms[i].get_x();
        y += this->atoms[i].get_y();
        z += this->atoms[i].get_z();
    }

    return Vector3(x / (float)this->atoms.size(),
                   y / (float)this->atoms.size(),
                   z / (float)this->atoms.size());
}

const std::vector<std::string>& State::get_elements() const {
    return this->elements;
}

void State::allocate_coordinate_matrix() {
    this->coordinates = MatrixX4f(this->atoms.size(), 4);

    for(unsigned int i=0; i<this->atoms.size(); i++) {
        this->coordinates(i,0) = this->atoms[i].get_x();
        this->coordinates(i,1) = this->atoms[i].get_y();
        this->coordinates(i,2) = this->atoms[i].get_z();
        this->coordinates(i,3) = 1.0f;
    }
}

void State::save_to_poscar(const char* filename, const char* name, bool is_vasp5) {
    std::ofstream myfile (filename);

    if (myfile.is_open()) {
        myfile << name << std::endl;

        myfile << 1.00 << std::endl;

        for(unsigned int i=0; i<3; i++) {
            for(unsigned int j=0; j<3; j++) {
                if(j != 2) {
                    myfile << float2str2(this->dimensions(i,j), "%8.7f") << "   ";
                } else {
                    myfile << float2str2(this->dimensions(i,j), "%8.7f");
                }
            }
            myfile << std::endl;
        }

        if(is_vasp5) {

        }

        myfile << output_atoms_line() << std::endl;

        myfile << "Direct" << std::endl;
        myfile << output_atom_coordinates() << std::endl;

        myfile.close();
    }
}

std::string State::output_atoms_line() {
    std::vector<unsigned int> element_numbers;
    std::vector<unsigned int> element_count;

    for(unsigned int i=0; i<this->atom_cnt; i++) {
        bool in_vector = false;
        for(unsigned int j=0; j<element_numbers.size(); j++) {
            if(element_numbers[j] == this->atoms[i].elnr) {
                element_count[j]++;
                in_vector = true;
            }
        }
        if(!in_vector) {
            element_numbers.push_back(this->atoms[i].elnr);
            element_count.push_back(1);
        }
    }

    std::string result;
    for(unsigned int i=0; i<element_numbers.size(); i++) {
        result += int2str(element_count[i]) + std::string("  ");
    }

    return result;
}

std::string State::output_atom_coordinates() {
    MatrixX3f cartesian_coordinates(this->atom_cnt, 3);

    unsigned int counter = 0;
    for(unsigned int i=0; i<this->nr_atoms.size(); i++) {
        for(unsigned int j=0; j<this->atom_cnt; j++) {
            if(this->atoms[j].elnr == this->elements_uint[i]) {
                cartesian_coordinates(counter, 0) = this->atoms[j].get_x();
                cartesian_coordinates(counter, 1) = this->atoms[j].get_y();
                cartesian_coordinates(counter, 2) = this->atoms[j].get_z();
                counter++;
            }
        }
    }

    Matrix3 inverse = this->dimensions.inverse();

    Matrix3Xf direct_coordinates = inverse.transpose() * cartesian_coordinates.transpose();

    std::string result;
    for(unsigned int i=0; i<this->atom_cnt; i++) {
        for(unsigned int j=0; j<3; j++) {
            result += float2str2(direct_coordinates(j,i), "%6.5f") + std::string("  ");
        }
        result += std::string("\n");
    }

    return result;
}
