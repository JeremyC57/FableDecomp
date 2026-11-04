#include "engine/C3DVector.h"
// Leaf: IsNewDestinationGoingToResetNavigation
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12). Member accesses x/y/z respelled to the PDB X/Y/Z.

struct Sub {
    virtual bool m00();
    virtual bool m04();
    virtual bool m08();
    virtual bool m0c();
    virtual bool m10();
    virtual bool m14();
    virtual bool Check(void* origThis, C3DVector* pos);   // slot 6 -> +0x18
};

class CTCCreatureNavigation { public:
    void* f0;
    Sub*  ParentThing;
};

bool __fastcall IsNewDestinationGoingToResetNavigation(CTCCreatureNavigation* self, int /*edx*/, C3DVector* pos)
{
    return self->ParentThing->Check(self, pos);
}