#include "engine/C3DVector.h"
// Leaf: IsNewDestinationGoingToResetNavigation
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12). Member accesses x/y/z respelled to the PDB X/Y/Z.
class CTCCreatureNavigation;

struct Sub {
    virtual void v0();
    virtual void v1();
    virtual void v2();
    virtual void v3();
    virtual void v4();
    virtual void v5();
    virtual bool v6(CTCCreatureNavigation* owner, C3DVector* dest);  // slot +0x18
};

class CTCCreatureNavigation { public:
    void* pad0;
    Sub* ParentThing;
};

bool __fastcall IsNewDestinationGoingToResetNavigation(CTCCreatureNavigation* self, int /*edx*/, C3DVector* dest)
{
    return self->ParentThing->v6(self, dest);
}