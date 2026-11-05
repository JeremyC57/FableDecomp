// CTCTurncoatSpell::SetPTarget(CThingCreatureBase* target) -- stores the spell target; when the target is alive and turncoat-able, sets its turncoat duration from the spell def's TurncoatTime entry for the caster's turncoat ability level
// Leaf: SetPTarget
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: re-padded by-value maps +8.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007C6940, 243 bytes (`ret 4`).
// Callees: CBaseIntelligentPointer() 0x00A01B10 (then the CIntelligentPointer vtable 0x0123E7D8); CBaseIntelligentPointer::
//          SetPItem 0x00A01B90 (x2); GetPItem 0x00A01B50 (x4); ~CBaseIntelligentPointer 0x00A01C10;
//          CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound 0x0040F020 (interface 0x9F, CTCTurncoat) and its const
//          instantiation 0x004365B0 (interface 0x6F, CTCInventoryAbilities, through a const CThing*);
//          CTCInventoryAbilities::GetAbilityLevel(EHeroAbility) const 0x005EDE93 (ability 5); CTCTurncoat::
//          SetTurncoatTimeLast(float) 0x007C7810.
// Layout: engine/CTCTurncoatSpell.h carries Ego_r.pdb offsets (PDef +0x44, PTarget +0x48); retail members after the
//          timer sit 4 bytes lower (as the landed StopTurncoatingEffects 0x007C6DA0 / FinalCleanup 0x007C75E0 models):
//          PDef +0x40, PTarget +0x44 -- so a local model. CSpecialAbilitiesTurncoatSpellDef::TurncoatTime (vector<float>)
//          +0x54 (PDB +0x60, 12-byte vectors after a 0x3c base). CThing bitset +0x20 (0x9F = sign bit of word +0x30,
//          0x6F = bit 15 of word +0x2c), interface map +0x44, dead bit +0x91.
// Shape: `PTarget = CIntelligentPointer<CThingCreatureBase>(target)` through the inline operator= (GetPItem of the
//        temporary, SetPItem on the member, the temporary destroyed); IS_THING_ALIVE re-fetches the item; the target's
//        component is fetched with the bare one-ternary GetTC after its presence test (no zero preload), the caster's
//        through the const GetTC; the def is a named local read just before the level call (kept in esi across it).
class CThing;
class CTCBase;

#define FABLE_RETAIL_CTHING
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CThingCreatureBase;

class CBaseObject;
struct CBaseIntelligentPointer {
    virtual void v00();                         // +0x00 vptr
    void*   PData;                              // +0x04
    CBaseIntelligentPointer();                  // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                 // 0x00A01C10
    CBaseObject* GetPItem() const;                   // 0x00A01B50
    void SetPItem(CBaseObject*a0);                // 0x00A01B90
};
template<class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:   // vtable 0x0123E7D8
    virtual void v00();
    CIntelligentPointer(T* item) { SetPItem((CBaseObject*)((CThing*)item)); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other)
    {
        CThing* item = ((CThing*)other.GetPItem());
        SetPItem((CBaseObject*)item);
        return *this;
    }
};

inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (((CThing*)p.GetPItem()) != 0) {
        if (!((CThing*)p.GetPItem())->IsDead())
            return true;
    }
    return false;
}

class CTCTurncoat { public:
    void SetTurncoatTimeLast(float time);       // 0x007C7810
};
class CTCInventoryAbilities { public:
    long GetAbilityLevel(long ability) const;   // 0x005EDE93 (EHeroAbility)
};
struct CFloatVector {                            // STLport vector<float> (12 bytes in retail)
    float* _M_start;
    float* _M_finish;
    float* _M_end_of_storage;
    const float& operator[](unsigned long n) const { return *(_M_start + n); }
};
class CSpecialAbilitiesTurncoatSpellDef { public:
    unsigned char _pad_0x00[0x54];
    CFloatVector  TurncoatTime;                 // +0x54
};

class CTCTurncoatSpell { public:
    void*                     __vftable;        // +0x00
    CThing*                   ParentThing;      // +0x04 (CTCBase)
    unsigned char             _pad_0x08[0x38];
    CSpecialAbilitiesTurncoatSpellDef* PDef;    // +0x40 (CDefPointer<const CSpecialAbilitiesTurncoatSpellDef>)
    CIntelligentPointer<CThingCreatureBase> PTarget;   // +0x44
    void SetPTarget(CThingCreatureBase* target);
};

void CTCTurncoatSpell::SetPTarget(CThingCreatureBase* target)
{
    PTarget = CIntelligentPointer<CThingCreatureBase>(target);
    if (IS_THING_ALIVE(PTarget)) {
        CThing* targetThing = ((CThing*)PTarget.GetPItem());
        if (targetThing->HasTC(TCI_TURNCOAT)) {
            CTCTurncoat* turncoat = (CTCTurncoat*)((CTCTurncoat*)&targetThing->GetTC(TCI_TURNCOAT));
            const CThing* caster = ParentThing;
            if (caster->HasTC(TCI_HERO_ABILITIES)) {
                const CTCInventoryAbilities* abilities = (const CTCInventoryAbilities*)((const CTCInventoryAbilities*)&caster->GetTC(TCI_HERO_ABILITIES));
                CSpecialAbilitiesTurncoatSpellDef* def = PDef;
                turncoat->SetTurncoatTimeLast(def->TurncoatTime[abilities->GetAbilityLevel(5)]);
            }
        }
    }
}