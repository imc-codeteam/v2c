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

#ifndef _MATHFUNC_H
#define _MATHFUNC_H

#include <eigen3/Eigen/Dense>

typedef Eigen::Matrix<float, 3, 1> Vector3;
typedef Eigen::Matrix<float, 3, 3> Matrix3;

typedef Eigen::Matrix<float, 4, 1> Vector4;
typedef Eigen::Matrix<float, 4, 4> Matrix4;

typedef Eigen::Matrix<float, Eigen::Dynamic, 3> MatrixX3f;
typedef Eigen::Matrix<float, Eigen::Dynamic, 4> MatrixX4f;
typedef Eigen::Matrix<float, 3, Eigen::Dynamic> Matrix3Xf;
typedef Eigen::Matrix<float, 4, Eigen::Dynamic> Matrix4Xf;

typedef Eigen::AngleAxis<float> AngleAxis;

#endif // _MATHFUNC_H
