// Byte-exact reconstruction of CTCCreatureNavigation::IsPosNavLineOfSightTo @ 0x00c1ea60
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12). Member accesses x/y/z respelled to the PDB X/Y/Z.

#include "engine/C3DVector.h"

// arg1 type: a real object with a __fastcall member (0xc26260) taking a C3DVector*
struct Thing {
    void* Compute(C3DVector* pos);   // __fastcall member, returns pointer
};

class CTCCreatureNavigation { public:
    // virtual slot 2 (vtable + 8): bool(void*)
    virtual void v0();
    virtual void v1();
    virtual bool CanReach(void* r);

    bool IsPosNavLineOfSightTo(C3DVector* pos, Thing* arg1);
};

bool CTCCreatureNavigation::IsPosNavLineOfSightTo(C3DVector* pos, Thing* arg1)
{
    return this->CanReach(arg1->Compute(pos));
}