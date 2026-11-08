// CCreatureAction_CastWillDrainLifeSpell::PerformAction() -- when the target is alive and the caster has its spell-casting component (interface 8), add a CTCDrainLife component, point the caster's spell at the target, give an AI-state caster the action's cast parameters, and hand a live target thing to the drain
// Leaf: ?PerformAction@CCreatureAction_CastWillDrainLifeSpell@@UAEXXZ
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Plan phase 2 (2026-11-04), CVectorMap wave (stand-in map): the local CInterfaceMap stand-in (and its same-TU LowerBound body) replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<..> > derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D; the same-TU noinline LowerBound is the header's); entries read as pair first/second, the map end through end(). Spellings unified: 0040f020: ?LowerBound@CInterfaceMap@@QAEPAUSEntry@1@PBJ@Z -> ?LowerBound@?$CVectorMap@W4ETCInterfaceType@@PAVCTCBase@@V?$CKeyPairCompareLess@W4ETCInterfaceType@@PAVCTCBase@@@@@@QAEPAU?$pair@W4ETCInterfaceType@@PAVCTCBase@@@std@@ABW4ETCInterfaceType@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x008BA7F0, 375 bytes (384 with padding).
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50, ctor 0x00A01B10, SetPItem 0x00A01B90, dtor 0x00A01C10
//          (CIntelligentPointer<CThing> vtable 0x01238C6C); CVectorMap::LowerBound 0x0040F020 (inlined GetInterface,
//          type 8 = bit 8 of the bitset dword +0x20); CThing::AddTC(const CCharString&, bool, CTCParamsBase*)
//          0x004C9D60 with "CTCDrainLife" 0x0123D18C (CCharString ctor 0x0099EBF0 / dtor 0x0099EAE0); the spell
//          component's virtual slot 34 (+0x88); CTCDrainLife::SetAICast(long, long) 0x007C3040.
//
// Shape: the landed Cast*Spell family (0x008B9970): the target's live test reads PTarget twice; `this` is advanced
// to PCreature in place and re-read from its frame copy (every register is taken); the component name is an
// implicit literal temporary; the drain rides in ebp; the target thing is a named copy assigned through the
// inline by-value operator= when alive.
// Layout: local model -- retail CCombatActionBase is 0x114 bytes (PCreature +8, PItemRemoved +0x2c, PTarget +0xa8);
// the action's cast parameters at +0x114 / +0x118 (descriptive names); the drain's target pointer +0x14.
// Frame: the same-TU CVectorMap::LowerBound stand-in (auto_inline(off), above the caller) is what lets the lookup key
// share its frame word with the name temporary and the by-value operator= parameter, as retail (LTCG) does.
#include "fable_compiler.h"
class CTCParamsBase;
class CBaseObject;
class CTCBase;

#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
class CThing;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"
struct CIntelligentPointerThing : CBaseIntelligentPointer {   // CIntelligentPointer<CThing>, vtable 0x01238C6C
    virtual void v00();
    FABLE_FORCEINLINE CIntelligentPointerThing(const CIntelligentPointerThing& other) : CBaseIntelligentPointer() { SetPItem((CBaseObject*)(((CThing*)other.GetPItem()))); }
    FABLE_FORCEINLINE CIntelligentPointerThing& operator=(CIntelligentPointerThing other) { SetPItem((CBaseObject*)(((CThing*)other.GetPItem()))); return *this; }
};

#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CCombatActionBase { public:
    virtual void v00();
    long _pad04;
    CBaseIntelligentPointer PCreature;          // +0x08 (the owning creature)
    unsigned char _pad10[0x2c - 0x10];
    CIntelligentPointerThing PItemRemoved;      // +0x2c (CCreatureActionBase, Ego_r.pdb name; the spell's target thing)
    unsigned char _pad34[0xa8 - 0x34];
    CBaseIntelligentPointer PTarget;            // +0xa8
    unsigned char _padb0[0x114 - 0xb0];
    virtual void Initialise();                  // 0x00857520
};

struct CTCSpellCasting {                        // interface type 8
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33();
    virtual void SetSpellTarget(CThing* target);    // slot 34 (+0x88)
};
class CTCDrainLife { public:
    unsigned char _pad00[0x14];
    CIntelligentPointerThing PTarget;           // +0x14
    void SetAICast(long level, long power);     // 0x007C3040
};

class CCreatureAction_CastWillDrainLifeSpell : public CCombatActionBase { public:
    long SpellLevel;                            // +0x114
    long SpellPower;                            // +0x118
    virtual void PerformAction();
};

void CCreatureAction_CastWillDrainLifeSpell::PerformAction()
{
    if (((CThing*)PTarget.GetPItem()) && !((CThing*)PTarget.GetPItem())->IsDead()) {
        CTCSpellCasting* casting;
        if (((CThing*)PCreature.GetPItem())->GetTC(TCI_TARGETING, &casting)) {
            CTCDrainLife* drain = (CTCDrainLife*)((CThing*)PCreature.GetPItem())->AddTC("CTCDrainLife", true, 0);
            casting->SetSpellTarget(((CThing*)PTarget.GetPItem()));
            if (((CThing*)PCreature.GetPItem())->GetType() == 1)
                drain->SetAICast(SpellLevel, SpellPower);
            CIntelligentPointerThing target = PItemRemoved;
            if (((CThing*)target.GetPItem()) && !((CThing*)target.GetPItem())->IsDead())
                drain->PTarget = target;
        }
    }
}