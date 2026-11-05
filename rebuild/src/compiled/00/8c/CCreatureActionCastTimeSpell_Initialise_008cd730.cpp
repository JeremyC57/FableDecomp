// CCreatureAction_CastTimeSpell::Initialise() -- base Initialise; if the owner has no bullet-time component yet, add "CTCBulletTime" and hand it the action's target when that is alive
// Leaf: ?Initialise@CCreatureAction_CastTimeSpell@@UAEXXZ
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008CD730, length 229 bytes (+ int3 padding), plain `ret` (virtual, no arguments).
// Callees: CCombatActionBase::Initialise 0x00857520 (direct), CBaseIntelligentPointer::GetPItem 0x00A01B50, CCharString ctor 0x0099EBF0 /
// dtor 0x0099EAE0 ("CTCBulletTime" @ 0x0123C408), CThing::AddTC(const CCharString&, bool, CTCParamsBase*) 0x004C9D60,
// CBaseIntelligentPointer ctor 0x00A01B10 / SetPItem 0x00A01B90 / dtor 0x00A01C10 (CIntelligentPointer<CThing> vtable 0x01238C6C).
// Shape: presence bit 0x92 (dword +0x30, bit 18) on a fresh owner fetch; the component name is an implicit literal temporary
// (built before `push 0 ; push 1`); the target is a named copy of the action's +0x2c pointer; the component's +0x3c pointer is
// assigned through an inline by-value operator= (the parameter copy lands in the dead string slot).
class CTCParamsBase;
class CBaseObject;

#define FABLE_RETAIL_CCHARSTRING
#include "engine/CThing.h"

class CThing;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"
struct CIntelligentPointerThing : CBaseIntelligentPointer {   // CIntelligentPointer<CThing>, vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointerThing(const CIntelligentPointerThing& other) : CBaseIntelligentPointer() { SetPItem((CBaseObject*)(((CThing*)other.GetPItem()))); }
    CIntelligentPointerThing& operator=(CIntelligentPointerThing other) { SetPItem((CBaseObject*)(((CThing*)other.GetPItem()))); return *this; }
};

class CCombatActionBase { public:
    virtual void v00();
    long _pad04;
    CBaseIntelligentPointer PCreature;          // +0x08 (the owning creature)
    unsigned char _pad10[0x2c - 0x10];
    CIntelligentPointerThing PItemRemoved;      // +0x2c (CCreatureActionBase, Ego_r.pdb name; the spell's target here)
    unsigned char _pad34[0x114 - 0x34];
    virtual void Initialise();                  // 0x00857520
};

class CTCBulletTime { public:                          // the interface-0x92 component
    unsigned char _pad00[0x3c];
    CIntelligentPointerThing PTarget;           // +0x3c
};

class CCreatureAction_CastTimeSpell : public CCombatActionBase { public:
    virtual void Initialise();
};

void CCreatureAction_CastTimeSpell::Initialise()
{
    CCombatActionBase::Initialise();
    if (!((CThing*)PCreature.GetPItem())->HasTC(TCI_BULLET_TIME)) {
        CTCBulletTime* tc = (CTCBulletTime*)((CThing*)PCreature.GetPItem())->AddTC("CTCBulletTime", true, 0);
        CIntelligentPointerThing target = PItemRemoved;
        if (((CThing*)target.GetPItem()) && !((CThing*)target.GetPItem())->IsDead())
            tc->PTarget = target;
    }
}