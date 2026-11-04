// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12). Member accesses x/y/z respelled to the PDB X/Y/Z.
#include "engine/C3DVector.h"
class CTCCreatureNavigation;
struct ISubNav {
    virtual bool v0();
    virtual bool v1();
    virtual bool v2();
    virtual bool v3();
    virtual bool v4();
    virtual bool v5();
    virtual bool Check(CTCCreatureNavigation* nav, C3DVector* dest);
};
class CTCCreatureNavigation { public:
    void* f0;
    ISubNav* sub;
    bool IsNewDestinationGoingToResetNavigation(C3DVector* dest);
};
bool CTCCreatureNavigation::IsNewDestinationGoingToResetNavigation(C3DVector* dest) {
    return sub->Check(this, dest);
}