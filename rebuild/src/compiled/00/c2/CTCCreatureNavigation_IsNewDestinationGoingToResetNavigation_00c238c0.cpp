// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12). Member accesses x/y/z respelled to the PDB X/Y/Z.
#include "engine/C3DVector.h"

struct Callee {
    virtual bool s0();
    virtual bool s1();
    virtual bool s2();
    virtual bool s3();
    virtual bool s4();
    virtual bool s5();
    virtual bool Query(void* nav, C3DVector* v);
};

class CTCCreatureNavigation { public:
    void* f0;
    Callee* ParentThing;
    bool IsNewDestinationGoingToResetNavigation(C3DVector* dest);
};

bool CTCCreatureNavigation::IsNewDestinationGoingToResetNavigation(C3DVector* dest)
{
    return ParentThing->Query(this, dest);
}