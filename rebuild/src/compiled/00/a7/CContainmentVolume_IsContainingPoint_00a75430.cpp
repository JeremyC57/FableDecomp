// CContainmentVolume::IsContainingPoint(const C3DVector& point) const -- wrap the point in a
// CIsContainingPoint functor and run the plane test
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A75430, length 41 bytes (+ int3 padding).
// Callee: CContainmentVolume::IsContaining<CIsContainingPoint>(const CIsContainingPoint&) @ 0x00A75730.
// PDB: CIsContainingPoint = { C3DVector Point } (12 bytes); the functor is built in a 12-byte
// frame (`sub esp,0Ch`, three dword copies through edx/eax) and passed by address; the bool result
// comes back in al untouched. engine/CContainmentVolume.h (PDB) supplies the layout (unused here
// beyond `this`).
#include "engine/CContainmentVolume.h"

struct C3DVector {
    float X;
    float Y;
    float Z;
};

struct CIsContainingPoint {
    C3DVector Point;
    CIsContainingPoint(const C3DVector& point) : Point(point) { }
};

struct CContainmentVolume_Methods : CContainmentVolume {
    bool IsContaining(const CIsContainingPoint& functor) const;    // IsContaining<CIsContainingPoint> @ 0x00A75730
    bool IsContainingPoint(const C3DVector& point) const;
};

bool CContainmentVolume_Methods::IsContainingPoint(const C3DVector& point) const
{
    return IsContaining(CIsContainingPoint(point));
}