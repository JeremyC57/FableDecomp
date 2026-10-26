// NCreatureAI::IsWillingToAttackTarget(const CThingAICreature* creature, const CThing* target) -- whether the creature is willing to attack the target: always for creature group 4, non-village-members, creatures under scripted control, summoned creatures, those without an opinion of the hero or in state group 3; a village member of a village hostile through crime needs a hero-enemy opinion first; a target opinion source that says witnesses will not attack, or a hero deed log the opinion is unwilling to attack on, says no
// Leaf: ?IsWillingToAttackTarget@NCreatureAI@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008648A0, 656 bytes incl. int3 padding, /Gr `__fastcall` (creature in
//   ecx, target in edx), plain `ret`.
// Identity: Ego_r/FableWin PDB name set `?IsWillingToAttackTarget@NCreatureAI@@YI_NPBVCThingAICreature@@PBVCThing@@@Z`
//   (the NCreatureAI group 0x00864250..0x00864D00 of this chunk).
// Callees: CDefClassBase::GetSubDef<CCreatureDef>(CDefPointer<const CCreatureDef>*) const 0x00485815 (same-TU noinline
//   stand-in, as the sibling 0x00864B30: the def stays in ebp, the target spills), CVectorMap::LowerBound const
//   0x004365B0, CTCVillageMember::GetPVillage() const 0x007EE540 (called twice), CThingAICreature::
//   GetCurrentStateGroupType() const 0x00662010, CTCCreatureOpinionOfHero::IsHeroOpinionEnemy() const 0x006827A0 and
//   IsWillingToAttackHeroOnOpinion() const 0x006817F0, CTCScriptedControl::IsActionListEmpty() const 0x007122C0,
//   CTCOpinionSource::PeekWitnessWillNotAttack(const CThingAICreature*) const 0x008640C0; the def's virtual slot 1.
// Layout: CThing ThingType +0x10 (2 = creature), UsingComponents +0x20, Components +0x44, PBaseDef +0x70; CCreatureDef
//   CreatureGroup +0xAC; CTCVillage IsEnemyBecauseOfCrime +0x7C; CTCScriptedControl Locked +0x18;
//   CTCSummonableCreature HasBeenSummoned +0x10. TC interface types: 0x1F CTCScriptedControl, 0x23 CTCVillageMember,
//   0x4F CTCCreatureOpinionOfHero, 0x50 CTCOpinionSource, 0x51 CTCHeroOpinionDeedLog, 0xA0 CTCSummonableCreature.
// Shape: the sibling IsOverridingFactionEnemyOnTarget's skeleton with the answers inverted: the village block reads the
//   village member through the direct-return PeekTC before the target-type test and calls GetPVillage twice; its state
//   test is `if (state != 3) { if (!(HasTC(0x4F) && IsHeroOpinionEnemy())) return false; } else return true;` (the
//   `== 3` early return lays the return-true epilogue inline); the scripted test sets a flag; the summonable and target
//   opinion-source components through out-pointer lookups; the opinion-of-hero pointer is assigned under a second
//   HasTC(0x4F) into an otherwise unset local. Lookups are __forceinline.
// Layout (supersedes any local-model note above): engine/CTCVillage.h now has the retail STLport 4.6 offsets, so the data
//   members come from the shared header; the class below keeps only the declarations this body needs.
#define CTCVillage CTCVillageDonor
#include "engine/CTCVillage.h"
#undef CTCVillage
class CThingAICreature;

static __forceinline bool DecRefToZero(long& count) { return --count == 0; }

class CCreatureDef {
public:
    virtual void v00();
    virtual void Destroy() const;                           // slot 1 (the reference count is mutable)
    mutable long  RefCount;                                 // +0x04
    unsigned char _pad08[0xAC - 0x08];
    unsigned long CreatureGroup;                            // +0xAC
};

template <class T> class CDefPointer {
public:
    T* Object;
    CDefPointer() : Object(0) { }
    ~CDefPointer() { if (Object) { if (DecRefToZero(Object->RefCount)) Object->Destroy(); } }
    T* operator->() const { return Object; }
    bool IsValid() const { return Object != 0; }
    CDefPointer& operator=(const CDefPointer& other)
    {
        if (Object != other.Object) {
            if (Object) {
                if (--Object->RefCount == 0)
                    Object->Destroy();
                Object = 0;
            }
            Object = other.Object;
            if (Object)
                ++Object->RefCount;
        }
        return *this;
    }
};

class CCharString {
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CDefinitionManager {
public:
    CDefPointer<const CCreatureDef> GetPDefFromGlobalIndex(long index);   // 0x009ADA10 (hidden result pointer)
};

class CDefClassBase {
public:
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0c(); virtual void v10();
    virtual void v14(); virtual void v18(); virtual void v1c(); virtual void v20(); virtual void v24();
    virtual void v28(); virtual void v2c(); virtual void v30(); virtual void v34();
    virtual const long* GetSubDefIndex(const CCharString& name) const;   // slot 14 (+0x38)
    unsigned char _pad04[0x1c - 4];
    CDefinitionManager* m_pManager;                         // +0x1c
    template <class T> bool GetSubDef(CDefPointer<const T>* out) const;
};

// Same-TU stand-in for GetSubDef<CCreatureDef> 0x00485815 (the landed AddWound / PickUpGoods model).
#pragma auto_inline(off)
template <> bool CDefClassBase::GetSubDef<CCreatureDef>(CDefPointer<const CCreatureDef>* out) const
{
    const long* index;
    {
        CCharString name("CCreatureDef");
        index = GetSubDefIndex(name);
    }
    if (index) {
        CDefPointer<const CCreatureDef> result = m_pManager->GetPDefFromGlobalIndex(*index);
        *out = result;
        return true;
    }
    return false;
}
#pragma auto_inline(on)

class CThingBaseDef : public CDefClassBase { };

class CTCBase;
struct CInterfaceMap {                                      // CVectorMap<ETCInterfaceType, CTCBase*> {first, last}
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;                                          // +0x00
    SEntry* last;                                           // +0x04
    const SEntry* LowerBound(const long* type) const;       // 0x004365B0
};

class CThing {
public:
    unsigned char _pad0[0x10];
    unsigned char ThingType;                                // +0x10 (2 = creature)
    unsigned char _pad11[0x20 - 0x11];
    unsigned long m_presentBits[9];                         // +0x20 UsingComponents bitset
    CInterfaceMap m_interfaces;                             // +0x44 Components
    unsigned char _pad4c[0x70 - 0x4c];
    CDefPointer<const CThingBaseDef> PBaseDef;              // +0x70

    __forceinline bool HasTC(long type) const
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
    __forceinline const CTCBase* PeekTC(long type) const
    {
        long key = type;
        const CInterfaceMap* map = &m_interfaces;
        const CInterfaceMap::SEntry* it = map->LowerBound(&key);
        const CInterfaceMap::SEntry* end = map->last;
        return (it == end || type < it->type ? end : it)->component;
    }
    template <class T> __forceinline bool PeekTC(long type, const T** out) const    // out-pointer form
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31))) {
            long key = type;
            const CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::SEntry* it = map->LowerBound(&key);
            if (it == map->last || type < it->type)
                it = map->last;
            *out = (const T*)it->component;
            return true;
        }
        return false;
    }
};

enum EAIStateGroupType { AI_STATE_GROUP_TYPE_3 = 3 };

class CThingCreatureBase : public CThing { };
class CThingAICreature : public CThingCreatureBase {
public:
    EAIStateGroupType GetCurrentStateGroupType() const;     // 0x00662010
};

class CTCScriptedControl {                                  // interface type 0x1F
public:
    unsigned char _pad0[0x18];
    bool Locked;                                            // +0x18
    bool IsActionListEmpty() const;                         // 0x007122C0
};

class CTCOpinionSource {                                    // interface type 0x50
public:
    bool PeekWitnessWillNotAttack(const CThingAICreature* witness) const;   // 0x008640C0
};

class CTCVillage : public CTCVillageDonor {
public:
};

class CTCVillageMember {                                    // interface type 0x23
public:
    CTCVillage* GetPVillage() const;                        // 0x007EE540
};

class CTCSummonableCreature {                               // interface type 0xA0
public:
    unsigned char _pad0[0x10];
    bool HasBeenSummoned;                                   // +0x10
};

class CTCCreatureOpinionOfHero {                            // interface type 0x4F
public:
    bool IsHeroOpinionEnemy() const;                        // 0x006827A0
    bool IsWillingToAttackHeroOnOpinion() const;            // 0x006817F0
};

namespace NCreatureAI {

bool __fastcall IsWillingToAttackTarget(const CThingAICreature* creature, const CThing* target)
{
    CDefPointer<const CCreatureDef> creature_def;
    if (creature->PBaseDef->GetSubDef(&creature_def) && creature_def.IsValid() && creature_def->CreatureGroup == 4)
        return true;
    if (creature->HasTC(0x23)) {
        const CTCVillageMember* village_member = (const CTCVillageMember*)creature->PeekTC(0x23);
        if (target->ThingType == 2 && village_member->GetPVillage() && village_member->GetPVillage()->IsEnemyBecauseOfCrime) {
            if (creature->GetCurrentStateGroupType() != 3) {
                if (!creature->HasTC(0x4f) ||
                    !((const CTCCreatureOpinionOfHero*)creature->PeekTC(0x4f))->IsHeroOpinionEnemy())
                    return false;
            } else
                return true;
        }
        bool scripted = false;
        if (creature->HasTC(0x1f)) {
            const CTCScriptedControl* scripted_control = (const CTCScriptedControl*)creature->PeekTC(0x1f);
            scripted = scripted_control->Locked || !scripted_control->IsActionListEmpty();
        }
        const CTCSummonableCreature* summonable;
        if (!scripted && !(creature->PeekTC(0xa0, &summonable) && summonable->HasBeenSummoned) &&
            creature->HasTC(0x4f) && creature->GetCurrentStateGroupType() != 3) {
            const CTCOpinionSource* opinion_source;
            if (target->PeekTC(0x50, &opinion_source) && opinion_source->PeekWitnessWillNotAttack(creature))
                return false;
            const CTCCreatureOpinionOfHero* opinion_of_hero;
            if (creature->HasTC(0x4f))
                opinion_of_hero = (const CTCCreatureOpinionOfHero*)creature->PeekTC(0x4f);
            if (target->HasTC(0x51) && !opinion_of_hero->IsWillingToAttackHeroOnOpinion())
                return false;
        }
    }
    return true;
}

}