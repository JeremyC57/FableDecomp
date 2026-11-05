// CPhysicsMeshObject::IsRigidBodyBoundingPointsValid(vector<C3DVector>& points) const -- the bounding points of a rigid body are valid when they are centred on the body: their sum has (near) zero length
// Plan phase 4 (2026-11-05), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12). Also: local GetLengthSquared respelled GetSquaredMagnitude (the header / FableWin.pdb method, same computation).
// Naming wave 18: the shared const float 1e-4f at 0x0129BA3C is ?DELTA_NOTIONAL_ZERO@@3MB (raw Ego_r public + typed data record; was EPSILON).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A11610, 317 bytes, `ret 4` (`this` unused). PDB:
//   ?IsRigidBodyBoundingPointsValid@CPhysicsMeshObject@@QBE_NAAV?$vector@VC3DVector@@V?$allocator@VC3DVector@@@std@@@std@@@Z.
// Constants: 0x0122DEDC = 0.0f; 0x0129BA3C is the 1e-4f epsilon VARIABLE (two memory operands).
// Shape: a summing loop over size() (12-byte elements, reciprocal-multiply divide) that VC7.1 unrolls by four; the index is
//   unsigned against a signed count (`jl` unroll guard, `jae` remainder loop); the length test is a file-static bool
//   helper (`return lenSq > eps * eps;` gives `mov eax,1 / xor eax,eax`) negated in the return (`test al,al ; sete cl`).
extern const float DELTA_NOTIONAL_ZERO;           // 0x0129BA3C (1e-4f)

#include "engine/C3DVector.h"

template <class T> struct vector {
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    long size() const { return _M_finish - _M_start; }
    T& operator[](unsigned long n) { return *(_M_start + n); }
};

static bool IsLongerThanEpsilon(const C3DVector& v)
{
    return v.GetSquaredMagnitude() > DELTA_NOTIONAL_ZERO * DELTA_NOTIONAL_ZERO;
}

class CPhysicsMeshObject {
public:
    bool IsRigidBodyBoundingPointsValid(vector<C3DVector>& points) const;
};

bool CPhysicsMeshObject::IsRigidBodyBoundingPointsValid(vector<C3DVector>& points) const
{
    C3DVector sum(0.0f, 0.0f, 0.0f);
    long count = points.size();
    for (unsigned long i = 0; i < count; ++i)
        sum += points[i];
    return !IsLongerThanEpsilon(sum);
}