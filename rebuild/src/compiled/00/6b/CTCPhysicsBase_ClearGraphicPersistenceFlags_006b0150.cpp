// CTCPhysicsBase::ClearGraphicPersistenceFlags() -- if the parent thing has a graphic appearance, reset its active persistence flags (argument 0 = the def default)
// Retail: Fable.exe (TLC, VC7.1) @ 0x006B0150, 18 bytes, CTCPhysicsBase vtable slot 45 (catalogued as vfunc_45).
// Callees: NGraphicAppearance::CTCGraphicAppearance::SetActivePersistenceFlags @ 0x004BFFD0 (0 = take the value from the def).
// Layout: engine/CTCPhysicsBase.h (+0x04 the parent thing); CThing +0x64 = the graphic-appearance TC pointer.
// Shape: the pointer read into ecx, null-tested, then the call with the zero pushed; void, no tail call (a `push`).
// Leaf: ClearGraphicPersistenceFlags
#include "engine/CTCPhysicsBase.h"

struct CTCGraphicAppearance {
    void SetActivePersistenceFlags(unsigned long flags);   // 0x004BFFD0
};

class CThing { public:
    unsigned char         _pad00[0x64];
    CTCGraphicAppearance* PGraphicAppearance;              // +0x64
};

struct CTCPhysicsBase_Methods : CTCPhysicsBase {
    CThing* GetThing() { return reinterpret_cast<CThing*>(sub); }
    void ClearGraphicPersistenceFlags();
};

void CTCPhysicsBase_Methods::ClearGraphicPersistenceFlags()
{
    CTCGraphicAppearance* appearance = GetThing()->PGraphicAppearance;
    if (appearance)
        appearance->SetActivePersistenceFlags(0);
}