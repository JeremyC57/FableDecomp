// CTCDParticleEmitter::OnDie  @ 006e0fa0
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).

class CBaseObject;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"
struct Sub14 {
};

struct Target4 {
    void Notify();  // 0x6e0f10
};

class CTCDParticleEmitter { public:
    int      m0;     // +0x00
    Target4* ParentThing;     // +0x04
    int      m8;     // +0x08
    int      mC;     // +0x0C
    int      ParticleTypeID;    // +0x10
    Sub14    m14;    // +0x14
    void OnDie();
};

void CTCDParticleEmitter::OnDie()
{
    if (((CBaseIntelligentPointer*)&(this->m14))->GetPItem())
        this->ParentThing->Notify();
}