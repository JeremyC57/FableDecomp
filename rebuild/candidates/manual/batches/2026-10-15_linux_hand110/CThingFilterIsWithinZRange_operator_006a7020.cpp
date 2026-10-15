// CThingFilter_IsWithinZRange::operator()(CThing* thing) -- true when the thing has a physics component whose position height is strictly within Range of Z
// Leaf: ??RCThingFilter_IsWithinZRange
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006A7020, length 38 bytes (+ int3 padding).
// Callees: none (x87 `fabs` intrinsic).
// Layout: PDB CThingFilter_IsWithinZRange { float Z; float Range; }; retail CThing PhysicsTC +0x60, the component's
// position C3DVector at +0x0C (z at +0x14).
// Shape: `fabs(pos.z - Z) < Range` as `fcomp [Range] ; test ah,5 ; jp` (the "below" test), byte-constant exits.
#include <math.h>

struct C3DVector {
    float x, y, z;
};

struct CTCPhysicsBase {
    void*         __vftable;
    unsigned char _pad04[8];
    C3DVector     m_position;                       // +0x0c
};

struct CThing {
    unsigned char   _pad00[0x60];
    CTCPhysicsBase* PhysicsTC;                      // +0x60
};

struct CThingFilter_IsWithinZRange {
    float Z;                                        // +0x00
    float Range;                                    // +0x04
    bool operator()(CThing* thing) const;
};

bool CThingFilter_IsWithinZRange::operator()(CThing* thing) const
{
    CTCPhysicsBase* physics = thing->PhysicsTC;
    if (physics && fabs(physics->m_position.z - Z) < Range)
        return true;
    return false;
}
