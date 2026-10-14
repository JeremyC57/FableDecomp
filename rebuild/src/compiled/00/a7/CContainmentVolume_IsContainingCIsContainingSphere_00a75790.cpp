// CContainmentVolume::IsContaining<CIsContainingSphere>(const CIsContainingSphere& test) const --
// true when the sphere's centre lies less than Radius behind every containment plane (the signed
// distance exceeds -Radius on each), i.e. the sphere overlaps the volume on every side
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A75790, length 86 bytes (+ int3 padding). No callees.
// PDB: CPlane = { C3DVector Normal; float Distance } (16 bytes), CIsContainingSphere =
// { C3DVector Point; float Radius }. engine/CContainmentVolume.h (PDB): CPlane*
// ContainmentPlanes +0x04, unsigned long Size +0x08. The point sibling @ 0x00A75730 is the same
// loop with a `> 0.0f` test.
// Shape: `Size` is hoisted into ebp, the loop keeps an index in edi and a byte offset in esi
// (0x10 per plane) and re-reads the plane pointer each iteration; the signed distance is
// Z*Nz + Y*Ny + X*Nx - D (each product loads the plane component first, `fld [plane] ; fmul
// [point]`), compared with the negated radius (`fld ; fchs ; fcompp ; test ah,5 ; jp` = the
// ordered less-than of -Radius against the distance); the early false exit is `xor al,al`, the
// true result is the constant hoisted into bl before the loop. The loop is the flag-controlled
// `while (contained && i < Size)` form: a `for` with `return false` rotates the loop instead.
#include "engine/CContainmentVolume.h"

struct C3DVector {
    float X;
    float Y;
    float Z;
};

struct CPlane {
    C3DVector Normal;
    float     Distance;
    __forceinline float GetSignedDistance(const C3DVector& point) const
    {
        return point.Z * Normal.Z + point.Y * Normal.Y + point.X * Normal.X - Distance;
    }
};

struct CIsContainingSphere {
    C3DVector Point;
    float     Radius;
    __forceinline bool operator()(const CPlane& plane) const
    {
        return plane.GetSignedDistance(Point) > -Radius;
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

template bool CContainmentVolume_Methods::IsContaining<CIsContainingSphere>(const CIsContainingSphere& test) const;