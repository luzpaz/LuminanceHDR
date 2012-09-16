/*
* This file is a part of Luminance HDR package.
* ----------------------------------------------------------------------
* Copyright (C) 2011-2012 Davide Anastasia
*
*  This library is free software; you can redistribute it and/or
*  modify it under the terms of the GNU Lesser General Public
*  License as published by the Free Software Foundation; either
*  version 2.1 of the License, or (at your option) any later version.
*
*  This library is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
*  Lesser General Public License for more details.
*
*  You should have received a copy of the GNU Lesser General Public
*  License along with this library; if not, write to the Free Software
*  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
* ----------------------------------------------------------------------
*/

#ifndef VEX_VEX_H
#define VEX_VEX_H

//! \brief This file contains a series of extensions for vector operations
//! exploiting TBB to improve parallelism
//! It will slowly replace the old vex header
//! \note VEX stays for Vector EXtensions
//! \author Davide Anastasia <davideanastasia@users.sourceforge.net>

#include <functional>
#include <numeric>

namespace vex
{
namespace numeric
{

//! \brief Extension of std::plus to compute A + s*B
template <typename T>
struct vadds : std::binary_function <T, T, T>
{
    vadds(const T& s)
        : s_(s) {}
    T
    operator()(const T& t1, const T& t2) const
    { return (t1 + (s_*t2)); }
private:
    T s_;
};

//! \brief Extension of std::minus to compute A - s*B
template <typename T>
struct vsubs : std::binary_function <T, T ,T>
{
    vsubs(const T& s)
        : s_(s) {}
    T
    operator()(const T& t1, const T& t2) const
    { return (t1 - (s_*t2)); }
private:
    T s_;
};

} // vex::numeric

//! \brief multiplies element-wise \c A and \c B and stores into \c C
//! C[i] = A[i] * B[i]
//! \param[in] A first input vector
//! \param[in] B second input vector
//! \param[out] C output vector
//! \param[in] N size of the vectors
//! \note caller is in charge of properly allocate all the vectors
template <typename _Type>
void vmul(const _Type* A, const _Type* B, _Type* C, size_t size);

//! \brief multiplies element-wise \c A and \c B and stores into \c C
//! C[i] = A[i] / B[i]
template <typename _Type>
void vdiv(const _Type* A, const _Type* B, _Type* C, size_t size);


//! \brief multiplies element-wise \c A and \c B and stores into \c C
//! C[i] = A[i] + B[i]
template <typename _Type>
void vadd(const _Type* A, const _Type* B, _Type* C, size_t size);

//! \brief multiplies element-wise \c A and \c B and stores into \c C
//! C[i] = A[i] + (s * B[i])
template <typename _Type>
void vadds(const _Type* A, const _Type& s, const _Type* B, _Type* C, size_t size);

//! \brief multiplies element-wise \c A and \c B and stores into \c C
//! C[i] = A[i] - B[i]
template <typename _Type>
void vsub(const _Type* A, const _Type* B, _Type* C, size_t size);

//! \brief multiplies element-wise \c A and \c B and stores into \c C
//! C[i] = A[i] - (s * B[i])
template <typename _Type>
void vsubs(const _Type* A, const _Type& s, const _Type* B, _Type* C, size_t size);


} // vex

#include "vex.hxx"

#endif // VEX_VEX_H
