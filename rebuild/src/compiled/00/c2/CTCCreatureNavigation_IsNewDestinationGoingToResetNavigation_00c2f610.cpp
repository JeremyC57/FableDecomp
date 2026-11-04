// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12). Member accesses x/y/z respelled to the PDB X/Y/Z.
#include "engine/C3DVector.h"
class CTCCreatureNavigation;

struct Helper {
    virtual void v0();
    virtual void v1();
    virtual void v2();
    virtual void v3();
    virtual void v4();
    virtual void v5();
    virtual bool check(CTCCreatureNavigation* nav, C3DVector* v);
};

class CTCCreatureNavigation { public:
    void* field_0;
    Helper* helper;
    bool IsNewDestinationGoingToResetNavigation(C3DVector* v);
};

bool CTCCreatureNavigation::IsNewDestinationGoingToResetNavigation(C3DVector* v) {
    return this->helper->check(this, v);
}