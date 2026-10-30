// C3DOrientationInterpolator::C3DOrientationInterpolator(const CRightHandedSet& start, const CRightHandedSet& end) -- CBase(), vtable, copy both orientations
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005508D0, length 98 bytes.
// Leaf: ??0C3DOrientationInterpolator
// Callees: CBaseClass::CBaseClass @ 0x0099A2D0. Vtable 0x01239824 = C3DOrientationInterpolator (rtti-vtables.tsv).
// Layout (PDB, 52 bytes): +4 StartOrientation, +0x1c EndOrientation (CRightHandedSet = two C3DVectors, 24 bytes).
// Shape: the sibling C3DPositionInterpolator(start, end) (landed 0x006AF570) with 24-byte members: every dword
// moves through ecx/edx in turn -- the engine's memberwise C3DVector copy constructor under CRightHandedSet's
// implicit copy constructor.
class C3DVector { public:
    float X, Y, Z;
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }   // the engine's memberwise copy constructor
};
class CRightHandedSet { public:                   // 24 bytes (PDB)
    C3DVector Up;                          // +0x00
    C3DVector Forward;                     // +0x0c
};

#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0 (slot 0), implicit copy

class C3DOrientationInterpolator : public CBaseClass { public:
    CRightHandedSet StartOrientation;      // +0x04
    CRightHandedSet EndOrientation;        // +0x1c
    C3DOrientationInterpolator(const CRightHandedSet& start, const CRightHandedSet& end);
};

C3DOrientationInterpolator::C3DOrientationInterpolator(const CRightHandedSet& start, const CRightHandedSet& end)
    : CBaseClass(), StartOrientation(start), EndOrientation(end)
{
}