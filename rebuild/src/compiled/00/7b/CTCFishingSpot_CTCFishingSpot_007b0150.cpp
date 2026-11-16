// CTCFishingSpot::CTCFishingSpot(CThing&) -- CTCBase(thing), the two particle pointers and the fish particle pointer constructed empty, enabled
// Leaf: ??0CTCFishingSpot@@QAE@AAVCThing@@@Z
// Ported 2026-11-16 onto the shared engine/CTCBase.h: the local base model CTCBase (which bound ??0CTCBase@@QAE@PAVCThing@@@Z to 0x00686800) is gone;
//   the base call is ??0CTCBase@@QAE@AAVCThing@@@Z and this constructor takes CThing& as Ego_r.pdb and FableWin.pdb spell it (??0CTCFishingSpot@@QAE@AAVCThing@@@Z).
//
// Retail 0x007B0150, 106 bytes (`ret 4`). Callees: CTCBase::CTCBase(CThing&) @ 0x00686800,
// CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x00A01B10 and ::SetPItem @ 0x00A01B90 (three of
// each; the CIntelligentPointer<CThing> vtable 0x01238C6C between them). Vtable 0x0126D51C =
// CTCFishingSpot.
// Layout = Ego_r.pdb CTCFishingSpot (0x28, no retail shift): PParticle1 +0xc, PParticle2 +0x14,
// PParticleFish +0x1c, Enabled +0x24.
// Shape: each pointer is built from a null item (`CIntelligentPointer<CThing>(0)`: base ctor, vtable
// store, SetPItem(0)), addressed once in edi; Enabled is the last initialiser.
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"                     // CTCBase(CThing&) 0x00686800 ??0CTCBase@@QAE@AAVCThing@@@Z
class CBaseObject;
class CThing;

struct CBaseIntelligentPointer {                            // 8 bytes: vtable + item pointer
    virtual void v00();
    CBaseObject* m_pItem;                                   // +4
    CBaseIntelligentPointer();                              // 0x00A01B10
    void SetPItem(CBaseObject* item);                       // 0x00A01B90
};
template<class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:
    virtual void v00();
    CIntelligentPointer(T* item) : CBaseIntelligentPointer() { SetPItem((CBaseObject*)item); }
};

class CTCFishingSpot : public CTCBase { public:                           // vtable 0x0126D51C
    CIntelligentPointer<CThing> PParticle1;                 // +0x0c
    CIntelligentPointer<CThing> PParticle2;                 // +0x14
    CIntelligentPointer<CThing> PParticleFish;              // +0x1c
    bool                        Enabled;                    // +0x24
    CTCFishingSpot(CThing& thing);
};

CTCFishingSpot::CTCFishingSpot(CThing& thing)
    : CTCBase(thing)
    , PParticle1(0)
    , PParticle2(0)
    , PParticleFish(0)
    , Enabled(true)
{
}