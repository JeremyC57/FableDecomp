// CContainmentVolume::IsContaining<CIsContainingPartOfSphere>(const CIsContainingPartOfSphere& test) const --
// true when the sphere's centre lies more than Radius inside every containment plane (the signed
// distance exceeds +Radius on each). Note the manifest pairing: this body (`> Radius`, the whole
// sphere inside) carries the "PartOfSphere" label while 0x00A75790 (`> -Radius`, overlap) carries
// "Sphere"; the labels are kept as the manifest names them, the tests are what the bytes say.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A757F0, length 82 bytes (+ int3 padding). No callees.
// PDB: CPlane = { C3DVector Normal; float Distance } (16 bytes), CIsContainingPartOfSphere =
// { C3DVector Point; float Radius }. engine/CContainmentVolume.h (PDB): CPlane*
// ContainmentPlanes +0x04, unsigned long Size +0x08.
// Shape: `Size` is hoisted into ebp, the loop keeps an index in edi and a byte offset in esi
// (0x10 per plane) and re-reads the plane pointer each iteration; the signed distance is
// Z*Nz + Y*Ny + X*Nx - D (each product loads the plane component first, `fld [plane] ; fmul
// [point]`), compared against the radius with `fcomp [radius] ; test ah,41h ; jne` (= "not
// greater"); the early false exit is `xor al,al`, the true result is the constant hoisted into
// bl before the loop. The loop is the flag-controlled `while (contained && i < Size)` form: a
// `for` with `return false` rotates the loop instead.
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

class CIsContainingPartOfSphere { public:
    C3DVector Point;
    float     Radius;
    __forceinline bool operator()(const CPlane& plane) const
    {
        return plane.GetSignedDistance(Point) > Radius;
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

template bool CContainmentVolume_Methods::IsContaining<CIsContainingPartOfSphere>(const CIsContainingPartOfSphere& test) const;