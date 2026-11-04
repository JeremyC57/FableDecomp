// CThingFilter_In3DRange::operator()(CThing* thing) -- true when the thing has a physics component and its position lies strictly within Range of Pos (full 3D distance)
// Leaf: ??RCThingFilter_In3DRange
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12). Member accesses x/y/z respelled to the PDB X/Y/Z.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006A7780, length 86 bytes.
// Callees: none (x87 only).
// Label correction: the manifest calls this row CThingFilter_In2DRange::operator() (propagated); the body sums all three
// squared deltas, so it is the In3DRange filter (same PDB layout).
// Layout: PDB CThingFilter_In3DRange { C3DVector Pos; float Range; } (16 bytes). Retail CThing holds PhysicsTC at +0x60;
// the component's position is the C3DVector at +0x0C.
// Shape: an absent component returns the byte constant (`xor al,al`); the float compare materialises an int
// (`mov eax,1` / `xor eax,eax`); Range is read into a local first (retail loads it before the deltas).
#include "engine/C3DVector.h"

class CTCPhysicsBase { public:
    void*     __vftable;
    unsigned char _pad04[8];
    C3DVector m_position;                           // +0x0c
};

class CThing { public:
    unsigned char   _pad00[0x60];
    CTCPhysicsBase* PhysicsTC;                      // +0x60
};

class CThingFilter_In3DRange { public:
    C3DVector Pos;                                  // +0x00
    float     Range;                                // +0x0c
    bool operator()(CThing* thing) const;
};

bool CThingFilter_In3DRange::operator()(CThing* thing) const
{
    CTCPhysicsBase* physics = thing->PhysicsTC;
    if (physics) {
        float range = Range;
        float dx = Pos.X - physics->m_position.X;
        float dy = Pos.Y - physics->m_position.Y;
        float dz = Pos.Z - physics->m_position.Z;
        return dx * dx + dy * dy + dz * dz < range * range;
    }
    return false;
}