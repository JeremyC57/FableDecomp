#include "engine/C3DVector.h"
// Leaf: IsNewDestinationGoingToResetNavigation
// Plan phase 4 (2026-11-05), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
class CTCCreatureNavigation;

struct Sub {
    // virtual with slot7 (+0x1c) taking (CTCCreatureNavigation*, C3DVector*)
    virtual void s0();
    virtual void s1();
    virtual void s2();
    virtual void s3();
    virtual void s4();
    virtual void s5();
    virtual void s6();
    virtual bool s7(CTCCreatureNavigation* thisNav, C3DVector* dest);
};

class CTCCreatureNavigation { public:
    void* pad0;
    Sub* sub;   // +4
};

bool __fastcall IsNewDestinationGoingToResetNavigation(CTCCreatureNavigation* self, int /*edx*/, C3DVector* dest)
{
    return self->sub->s7(self, dest);
}