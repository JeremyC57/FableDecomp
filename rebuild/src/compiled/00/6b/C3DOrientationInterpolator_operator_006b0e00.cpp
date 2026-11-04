// C3DOrientationInterpolator::operator=(const C3DOrientationInterpolator&) -- copy StartOrientation and EndOrientation (the CBase part has nothing to copy)
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006B0E00, oracle length 93 bytes.
// Callees: none.
//
// Retail bytes:  push esi ; mov eax,ecx ; mov ecx,[esp+8] ; lea edx,[ecx+4] ; push edi ; (six dword moves [edx+0..14h] -> [eax+4..18h]) ;
//                add ecx,1Ch ; (six dword moves [ecx+0..14h] -> [eax+1Ch..30h]) ; pop edi ; pop esi ; ret 4
// PDB layout (struct_layouts_egor.tsv, 52 bytes): +4 StartOrientation, +0x1C EndOrientation, both CRightHandedSet
// {C3DVector Up, Forward} = 24 bytes. The copy assignment copies each 24-byte set as six dword moves
// (VC7.1's inline struct copy below the rep-movsd threshold), leaves the vtable alone and returns *this.
#include "engine/C3DVector.h"
class CRightHandedSet { public: C3DVector Up, Forward; };

struct CBase {
    virtual void v00();
};

class C3DOrientationInterpolator : public CBase { public:
    CRightHandedSet StartOrientation;      // +0x4
    CRightHandedSet EndOrientation;        // +0x1C
    C3DOrientationInterpolator& operator=(const C3DOrientationInterpolator& other);
};

C3DOrientationInterpolator& C3DOrientationInterpolator::operator=(const C3DOrientationInterpolator& other)
{
    StartOrientation = other.StartOrientation;
    EndOrientation = other.EndOrientation;
    return *this;
}