// CContainmentVolume::IsContainingSphere(const C3DVector& point, float radius) const -- wrap the sphere in a CIsContainingSphere functor and run the plane test
// Leaf: IsContainingSphere
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A75460, length 49 bytes (+ int3 padding).
// Callee: CContainmentVolume::IsContaining<CIsContainingSphere>(const CIsContainingSphere&) const @ 0x00A75790.
// Catalog label "CTCHeroStats::AddHousePosition" is propagated: the body sits between
// CContainmentVolume::IsContainingPoint (0x00A75430) and LoadBinary (0x00A754E0) and calls the
// IsContaining<CIsContainingSphere> template on the unchanged `this`; the Ego_r PDB names the method
// ?IsContainingSphere@CContainmentVolume@@QBE_NABVC3DVector@@M@Z.
// PDB: CIsContainingSphere = { C3DVector Point; float Radius } (16 bytes). The functor is built in a
// 16-byte frame (`sub esp,10h`): the point copied as three dwords through edx/eax, the radius moved as
// a dword (a plain float copy needs no FPU), then passed by address; the bool comes back in al
// untouched. Same shape as the landed IsContainingPoint (0x00A75430).
#include "engine/CContainmentVolume.h"

struct C3DVector {
    float X;
    float Y;
    float Z;
};

struct CIsContainingSphere {
    C3DVector Point;
    float     Radius;
    CIsContainingSphere(const C3DVector& point, float radius) : Point(point), Radius(radius) { }
};

struct CContainmentVolume_Methods : CContainmentVolume {
    bool IsContaining(const CIsContainingSphere& functor) const;    // IsContaining<CIsContainingSphere> @ 0x00A75790
    bool IsContainingSphere(const C3DVector& point, float radius) const;
};

bool CContainmentVolume_Methods::IsContainingSphere(const C3DVector& point, float radius) const
{
    return IsContaining(CIsContainingSphere(point, radius));
}
