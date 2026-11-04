// C3DHelperPoint::C3DHelperPoint(const C3DVector&, long) -- copies the position (memberwise) and stores the bone index at +0xc
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A2E470, length 32 bytes. No callees.
// PDB layout: C3DHelperPoint = {C3DVector Position +0, long BoneIndex +0xc} (16 bytes). The
// position is copied one component at a time through a single register (a memberwise C3DVector
// copy constructor), the index as a dword; ret 8.
#include "engine/C3DVector.h"

class C3DHelperPoint { public:
    C3DVector Position;                    // +0
    long BoneIndex;                        // +0xc
    C3DHelperPoint(const C3DVector& position, long boneIndex);
};

C3DHelperPoint::C3DHelperPoint(const C3DVector& position, long boneIndex)
    : Position(position), BoneIndex(boneIndex)
{
}