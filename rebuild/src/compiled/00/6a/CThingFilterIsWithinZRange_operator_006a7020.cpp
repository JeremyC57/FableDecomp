// CThingFilter_IsWithinZRange::operator()(CThing* thing) -- true when the thing has a physics component whose position height is strictly within Range of Z
// Leaf: ??RCThingFilter_IsWithinZRange
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12). Member accesses x/y/z respelled to the PDB X/Y/Z.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006A7020, length 38 bytes (+ int3 padding).
// Callees: none (x87 `fabs` intrinsic).
// Layout: PDB CThingFilter_IsWithinZRange { float Z; float Range; }; retail CThing PhysicsTC +0x60, the component's
// position C3DVector at +0x0C (z at +0x14).
// Shape: `fabs(pos.z - Z) < Range` as `fcomp [Range] ; test ah,5 ; jp` (the "below" test), byte-constant exits.
#include <math.h>

#include "engine/C3DVector.h"

class CTCPhysicsBase { public:
    void*         __vftable;
    unsigned char _pad04[8];
    C3DVector     m_position;                       // +0x0c
};

#include "engine/CThing.h"

class CThingFilter_IsWithinZRange { public:
    float Z;                                        // +0x00
    float Range;                                    // +0x04
    bool operator()(CThing* thing) const;
};

bool CThingFilter_IsWithinZRange::operator()(CThing* thing) const
{
    CTCPhysicsBase* physics = thing->GetPhysicsTC();
    if (physics && fabs(physics->m_position.Z - Z) < Range)
        return true;
    return false;
}