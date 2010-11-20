/******************************************************************************
*       SOFA, Simulation Open-Framework Architecture, version 1.0 beta 4      *
*                (c) 2006-2009 MGH, INRIA, USTL, UJF, CNRS                    *
*                                                                             *
* This library is free software; you can redistribute it and/or modify it     *
* under the terms of the GNU Lesser General Public License as published by    *
* the Free Software Foundation; either version 2.1 of the License, or (at     *
* your option) any later version.                                             *
*                                                                             *
* This library is distributed in the hope that it will be useful, but WITHOUT *
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or       *
* FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License *
* for more details.                                                           *
*                                                                             *
* You should have received a copy of the GNU Lesser General Public License    *
* along with this library; if not, write to the Free Software Foundation,     *
* Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301 USA.          *
*******************************************************************************
*                               SOFA :: Modules                               *
*                                                                             *
* Authors: The SOFA Team and external contributors (see Authors.txt)          *
*                                                                             *
* Contact information: contact@sofa-framework.org                             *
******************************************************************************/
#ifndef CUDAMATHRIGID_INL
#define CUDAMATHRIGID_INL

#include "CudaMathRigid.h"

#if defined(__cplusplus) && CUDA_VERSION < 2000
namespace sofa
{
namespace gpu
{
namespace cuda
{
#endif

#ifdef SOFA_GPU_CUDA_PRECISE
template<>
__device__ CudaRigidCoord3<float> operator+(CudaRigidCoord3<float> a, CudaRigidDeriv3<float> b)
{
    CudaRigidCoord3<float> x;

    x.pos[0] = a.pos[0] + b.pos.x;
    x.pos[1] = a.pos[1] + b.pos.y;
    x.pos[2] = a.pos[2] + b.pos.z;

    CudaVec4<float> orient = CudaVec4<float>::make(a.rot[0], a.rot[1], a.rot[2], a.rot[3]);
    orient = orient*invnorm(orient);
    CudaVec3<float> vOrient = CudaVec3<float>::make(b.rot.x, b.rot.y, b.rot.z);
    CudaVec4<float> qDot = vectQuatMult(orient, vOrient);
    orient.x += __fmul_rn(qDot.x,0.5f);
    orient.y += __fmul_rn(qDot.y,0.5f);
    orient.z += __fmul_rn(qDot.z,0.5f);
    orient.w += __fmul_rn(qDot.w,0.5f);
    orient = orient*invnorm(orient);

    x.rot[0] = orient.x;
    x.rot[1] = orient.y;
    x.rot[2] = orient.z;
    x.rot[3] = orient.w;

    return x;
}
#endif



#if defined(__cplusplus) && CUDA_VERSION < 2000
} // namespace cuda
} // namespace gpu
} // namespace sofa
#endif
#endif
