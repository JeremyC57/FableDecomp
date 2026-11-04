// CThingAICreature::GetTargetPosition() -- the point others aim at: the creature's position raised by the height value of its physics component (+0x34)
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) @ 0x006A9520, 40 bytes. Callees: the thing's position accessor @ 0x006A4D00
// Header: engine/CThingAICreature.h exists but names nothing at these offsets (all `_pad_*`, PDB offsets that
// sit 0x30 above retail at the tail), so the class is modelled locally like the landed 0x00833BA0.
// (returns a const C3DVector&; manifest label "CVertexBufferWin32::DoSizeof" is propagated).
// Shape: wave-24o hidden-pointer C3DVector built field by field in a NAMED local and returned through a memberwise
// user copy constructor: the three values stay on the x87 stack (x and y loaded first, then `height + z`) and
// fold into `fxch st(2)` plus three `fstp` into the hidden result. PhysicsTC is CThing's retail +0x60.
#include "engine/C3DVector.h"

class CTCPhysicsBase { public:
    unsigned char _pad00[0x34];
    float         m_height;                          // +0x34
};

class CThingPhysical { public:  // retail: CThingPhysical derives from CThing; modelled as its base here (empty insertion, layout unchanged) so CThing-typed callers resolve the call
    const C3DVector& PeekPos() const;            // 0x006A4D00
};
class CThing : public CThingPhysical { public:
    void*           __vftable;                       // +0x00
    unsigned char   _pad04[0x60 - 0x04];
    CTCPhysicsBase* PhysicsTC;                       // +0x60 (PDB +0x70)
};

class CThingAICreature : public CThing { public:
    C3DVector GetTargetPosition() const;
};

C3DVector CThingAICreature::GetTargetPosition() const
{
    const C3DVector& position = PeekPos();
    C3DVector target;
    target.X = position.X;
    target.Y = position.Y;
    target.Z = PhysicsTC->m_height + position.Z;
    return target;
}