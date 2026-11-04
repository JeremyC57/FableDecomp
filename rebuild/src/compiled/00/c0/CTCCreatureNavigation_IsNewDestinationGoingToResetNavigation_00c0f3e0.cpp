#include "engine/C3DVector.h"
// Leaf: IsNewDestinationGoingToResetNavigation
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12). Member accesses x/y/z respelled to the PDB X/Y/Z.

struct Inner {
    // vtable auto at offset 0
    virtual bool method0();      // +0x00
    virtual bool method1();      // +0x04
    virtual bool method2();      // +0x08
    virtual bool method3();      // +0x0c
    virtual bool method4();      // +0x10
    virtual bool method5();      // +0x14
    virtual bool method6();      // +0x18
    virtual bool Check(void* nav, C3DVector* dest);  // +0x1c
};

class CTCCreatureNavigation { public:
    void*  field_0;
    Inner* ParentThing;
};

bool __fastcall IsNewDestinationGoingToResetNavigation(
        CTCCreatureNavigation* self, int /*edx*/, C3DVector* dest)
{
    return self->ParentThing->Check(self, dest);
}