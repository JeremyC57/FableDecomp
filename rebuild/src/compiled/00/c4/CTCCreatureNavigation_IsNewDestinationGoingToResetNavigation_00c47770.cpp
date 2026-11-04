// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12). Member accesses x/y/z respelled to the PDB X/Y/Z.
#include "engine/C3DVector.h"
class CTCCreatureNavigation;
struct IHelper {
    virtual int v0();
    virtual int v1();
    virtual int v2();
    virtual int v3();
    virtual int v4();
    virtual int v5();
    virtual bool v6(CTCCreatureNavigation* nav, C3DVector* dest);
};
class CTCCreatureNavigation { public:
    void* f0;
    IHelper* ParentThing;
    bool IsNewDestinationGoingToResetNavigation(C3DVector* dest);
};
bool CTCCreatureNavigation::IsNewDestinationGoingToResetNavigation(C3DVector* dest)
{
    return this->ParentThing->v6(this, dest);
}