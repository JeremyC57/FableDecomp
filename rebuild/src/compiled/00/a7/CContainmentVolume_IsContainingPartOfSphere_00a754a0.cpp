// CContainmentVolume::IsContainingPartOfSphere(const C3DVector& point, float radius) const -- wrap the sphere in a CIsContainingPartOfSphere functor and run the partial-overlap plane test
// Leaf: IsContainingPartOfSphere
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A754A0, length 49 bytes (+ int3 padding).
// Callee: CContainmentVolume::IsContaining<CIsContainingPartOfSphere>(const CIsContainingPartOfSphere&) const @ 0x00A757F0.
// Catalog label "CTCHeroStats::AddHousePosition" is propagated: the body follows IsContainingSphere (0x00A75460),
// precedes CContainmentVolume::LoadBinary (0x00A754E0) and calls the
// IsContaining<CIsContainingPartOfSphere> template on the unchanged `this`; the Ego_r PDB names the method
// ?IsContainingPartOfSphere@CContainmentVolume@@QBE_NABVC3DVector@@M@Z.
// PDB: CIsContainingPartOfSphere = { C3DVector Point; float Radius } (16 bytes). The functor is built in a
// 16-byte frame (`sub esp,10h`): the point copied as three dwords through edx/eax, the radius moved as
// a dword (a plain float copy needs no FPU), then passed by address; the bool comes back in al
// untouched. Same shape as the landed IsContainingPoint (0x00A75430).
#include "engine/CContainmentVolume.h"

class C3DVector { public:
    float X;
    float Y;
    float Z;
};

class CIsContainingPartOfSphere { public:
    C3DVector Point;
    float     Radius;
    CIsContainingPartOfSphere(const C3DVector& point, float radius) : Point(point), Radius(radius) { }
};

struct CContainmentVolume_Methods : CContainmentVolume {
    bool IsContaining(const CIsContainingPartOfSphere& functor) const;    // IsContaining<CIsContainingPartOfSphere> @ 0x00A757F0
    bool IsContainingPartOfSphere(const C3DVector& point, float radius) const;
};

bool CContainmentVolume_Methods::IsContainingPartOfSphere(const C3DVector& point, float radius) const
{
    return IsContaining(CIsContainingPartOfSphere(point, radius));
}