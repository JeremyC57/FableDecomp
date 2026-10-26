// CContainmentVolume::IsContaining<CIsContainingPoint>(const CIsContainingPoint& test) const --
// true when the point lies on the positive side of every containment plane
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A75730, length 86 bytes (+ int3 padding). No callees.
// Float pool: 0.0f @ 0x0122DEDC. PDB: CPlane = { C3DVector Normal; float Distance } (16 bytes),
// CIsContainingPoint = { C3DVector Point }. engine/CContainmentVolume.h (PDB): CPlane*
// ContainmentPlanes +0x04, unsigned long Size +0x08.
// Shape: `Size` is hoisted into ebp, the loop keeps an index in edi and a byte offset in esi
// (0x10 per plane) and re-reads the plane pointer each iteration; the signed distance is
// Z*Nz + Y*Ny + X*Nx - D (each product loads the plane component first, `fld [plane] ; fmul
// [point]`), compared against 0.0f with `test ah,41h ; jne` (= "not greater"); the early false
// exit is `xor al,al`, the true result is the constant hoisted into bl before the loop.
#include "engine/CContainmentVolume.h"

class C3DVector { public:
    float X;
    float Y;
    float Z;
};

class CPlane { public:
    C3DVector Normal;
    float     Distance;
    __forceinline float GetSignedDistance(const C3DVector& point) const
    {
        return point.Z * Normal.Z + point.Y * Normal.Y + point.X * Normal.X - Distance;
    }
};

class CIsContainingPoint { public:
    C3DVector Point;
    __forceinline bool operator()(const CPlane& plane) const
    {
        return plane.GetSignedDistance(Point) > 0.0f;
    }
};

struct CContainmentVolume_Methods : CContainmentVolume {
    template<class TTest> bool IsContaining(const TTest& test) const;
};

template<class TTest>
bool CContainmentVolume_Methods::IsContaining(const TTest& test) const
{
    bool contained = true;
    unsigned long i = 0;
    while (contained && i < Size) {
        if (!test(reinterpret_cast<const CPlane*>(ContainmentPlanes)[i]))
            contained = false;
        else
            ++i;
    }
    return contained;
}

template bool CContainmentVolume_Methods::IsContaining<CIsContainingPoint>(const CIsContainingPoint& test) const;