// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12). Member accesses x/y/z respelled to the PDB X/Y/Z.
#include "engine/C3DVector.h"
class CTCCreatureNavigation;
struct Helper {
    virtual void s0(); virtual void s1(); virtual void s2();
    virtual void s3(); virtual void s4(); virtual void s5();
    virtual bool s6(CTCCreatureNavigation* nav, C3DVector* v);
};
class CTCCreatureNavigation { public:
    void* field0;
    Helper* ParentThing;
    bool IsNewDestinationGoingToResetNavigation(C3DVector* dest);
};
bool CTCCreatureNavigation::IsNewDestinationGoingToResetNavigation(C3DVector* dest)
{
    return ParentThing->s6(this, dest);
}