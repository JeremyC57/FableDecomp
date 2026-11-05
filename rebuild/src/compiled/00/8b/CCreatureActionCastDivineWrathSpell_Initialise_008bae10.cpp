// CCreatureAction_CastDivineWrathSpell::Initialise() -- base Initialise, find or add the creature's CTCDivineWrath component, arm it and hand it the action's thing pointer when that thing is alive
// Leaf: ?Initialise@CCreatureAction_CastDivineWrathSpell@@UAEXXZ
// Plan phase 2 (2026-11-04), CVectorMap wave (stand-in map): the local CInterfaceMap stand-in (and its same-TU LowerBound body) replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<..> > derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D; the same-TU noinline LowerBound is the header's); entries read as pair first/second, the map end through end(). Spellings unified: 0040f020: ?LowerBound@CInterfaceMap@@QAEPAUSEntry@1@PBJ@Z -> ?LowerBound@?$CVectorMap@W4ETCInterfaceType@@PAVCTCBase@@V?$CKeyPairCompareLess@W4ETCInterfaceType@@PAVCTCBase@@@@@@QAEPAU?$pair@W4ETCInterfaceType@@PAVCTCBase@@@std@@ABW4ETCInterfaceType@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x008BAE10, 271 bytes (272 with padding), virtual Initialise. Byte twin of
//   CCreatureAction_CastUnholyPowerSpell::Initialise 0x008BAF20 (type 0x86 / "CTCUnholyPower").
// Callees: CCombatActionBase::Initialise 0x00857520 (direct base call, first), CBaseIntelligentPointer::GetPItem 0x00A01B50,
//   CVectorMap::LowerBound 0x0040F020 (inlined GetInterface, type 0x85 = bit 5 of the word at +0x30),
//   CCharString(const char*, long = -1) 0x0099EBF0 ("CTCDivineWrath" 0x0123C280), CThing::AddTC(const CCharString&, bool,
//   CTCParamsBase*) 0x004C9D60, ~CCharString 0x0099EAE0, CBaseIntelligentPointer ctor 0x00A01B10 / SetPItem 0x00A01B90 /
//   dtor 0x00A01C10 (CIntelligentPointer<CThing> vtable 0x01238C6C).
// Layout: PCreature +0x08 and the base action's CIntelligentPointer<CThing> at +0x2c (PItemRemoved in the PDB layout of
//   CCreatureActionBase; retail's early base offsets agree with the PDB); the component's armed byte +0x18 and its
//   CIntelligentPointer<CThing> at +0x10.
// Shape: the find-or-add is the landed out-parameter GetInterface with the AddTC fallback in the else block (its
//   name string a block-scoped local); the thing pointer is copied into a named CIntelligentPointer (inline copy
//   ctor: base ctor, vtable store, SetPItem(other.GetPItem())), tested with IS_THING_ALIVE, and passed BY VALUE to an
//   inline setter on the component (the copy is the second frame object, sharing the string's slot). The component
//   name is an IMPLICIT CCharString conversion temporary: a named block local takes its own frame word instead of
//   sharing the inlined lookup key's.
class CTCBase;
class CTCParamsBase;
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"

#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CBaseObject;
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                                  // +4
    CBaseIntelligentPointer();                        // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                       // 0x00A01C10
    CBaseObject* GetPItem() const;                         // 0x00A01B50
    void SetPItem(CBaseObject*a0);                      // 0x00A01B90
};
struct CIntelligentPointer : CBaseIntelligentPointer {   // CIntelligentPointer<CThing>, vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointer() : CBaseIntelligentPointer() { }
    CIntelligentPointer(const CIntelligentPointer& other) : CBaseIntelligentPointer() { SetPItem((CBaseObject*)(((CThing*)other.GetPItem()))); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other) { SetPItem((CBaseObject*)(((CThing*)other.GetPItem()))); return *this; }
};
inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (((CThing*)p.GetPItem()) != 0) {
        if (!((CThing*)p.GetPItem())->IsDead())
            return true;
    }
    return false;
}

class CTCDivineWrath { public:
    unsigned char _pad00[0x10];
    CIntelligentPointer PTarget;                      // +0x10
    unsigned char m_bArmed;                           // +0x18
    void SetTarget(CIntelligentPointer target) { PTarget = target; }
};

class CCombatActionBase { public:                            // retail sizeof 0x114
    virtual void v00();
    unsigned char _pad04[4];
    CBaseIntelligentPointer PCreature;                // +0x08
    unsigned char _pad10[0x2c - 0x10];
    CIntelligentPointer PItemRemoved;                 // +0x2c
    unsigned char _pad34[0x114 - 0x34];
    virtual void Initialise();                                // 0x00857520
};
class CCreatureAction_CastDivineWrathSpell : public CCombatActionBase { public:
    void Initialise();
};

void CCreatureAction_CastDivineWrathSpell::Initialise()
{
    CCombatActionBase::Initialise();
    CTCDivineWrath* wrath;
    if (!((CThing*)PCreature.GetPItem())->GetTC(TCI_DIVINE_WRATH, &wrath))
        wrath = (CTCDivineWrath*)((CThing*)PCreature.GetPItem())->AddTC("CTCDivineWrath", true, 0);
    wrath->m_bArmed = 1;
    CIntelligentPointer thing(PItemRemoved);
    if (IS_THING_ALIVE(thing))
        wrath->SetTarget(thing);
}