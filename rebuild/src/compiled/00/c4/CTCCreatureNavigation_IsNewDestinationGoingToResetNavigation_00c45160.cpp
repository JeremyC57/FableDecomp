// Plan phase 4 (2026-11-05), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
#include "engine/C3DVector.h"
class CTCCreatureNavigation;

struct Helper {
    virtual void m0();
    virtual void m1();
    virtual void m2();
    virtual void m3();
    virtual void m4();
    virtual void m5();
    virtual bool m6(CTCCreatureNavigation* nav, C3DVector* pos); // slot 6 = +0x18
};

class CTCCreatureNavigation { public:
    void* f0;
    Helper* ParentThing;
    bool IsNewDestinationGoingToResetNavigation(C3DVector* pos);
};

bool CTCCreatureNavigation::IsNewDestinationGoingToResetNavigation(C3DVector* pos)
{
    return ParentThing->m6(this, pos);
}