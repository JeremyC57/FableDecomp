// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12). Member accesses x/y/z respelled to the PDB X/Y/Z.
#include "engine/C3DVector.h"

struct INavTarget {
    virtual bool slot0();
    virtual bool slot1();
    virtual bool slot2();
    virtual bool slot3();
    virtual bool slot4();
    virtual bool slot5();
    virtual bool Query(void* self, C3DVector* v); // slot6 -> [edx+0x18]
};

class CTCCreatureNavigation { public:
    void* field_0;
    INavTarget* ParentThing;
    bool IsNewDestinationGoingToResetNavigation(C3DVector* dest);
};

bool CTCCreatureNavigation::IsNewDestinationGoingToResetNavigation(C3DVector* dest)
{
    return ParentThing->Query(this, dest);
}