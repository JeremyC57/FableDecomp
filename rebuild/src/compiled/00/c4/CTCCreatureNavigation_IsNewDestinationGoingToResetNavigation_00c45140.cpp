#include "engine/C3DVector.h"
// Leaf: IsNewDestinationGoingToResetNavigation
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12). Member accesses x/y/z respelled to the PDB X/Y/Z.
class CTCCreatureNavigation;

struct Helper {
    virtual void v0();
    virtual void v1();
    virtual void v2();
    virtual void v3();
    virtual void v4();
    virtual void v5();
    virtual void v6();
    virtual bool slot7(CTCCreatureNavigation* nav, C3DVector* v); // vtable [+0x1c]
};

class CTCCreatureNavigation { public:
    void* f0;
    Helper* ParentThing;
};

bool __fastcall IsNewDestinationGoingToResetNavigation(CTCCreatureNavigation* self, int /*edx*/, C3DVector* v) {
    return self->ParentThing->slot7(self, v);
}