// NCreatureAI::IsOverridingFactionEnemyOnTarget(const CThingAICreature* creature, const CThing* target) -- whether a village member may treat the target as an enemy despite faction: never for creature group 4 or while scripted control holds it; yes when the target's opinion source overrides faction for this witness or the target keeps a hero deed log and the creature's opinion of the hero overrides the faction enmity
// Leaf: ?IsOverridingFactionEnemyOnTarget@NCreatureAI@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00864B30, 384 bytes incl. int3 padding, /Gr `__fastcall` (creature in
//   ecx, target in edx), plain `ret`.
// Identity: Ego_r/FableWin PDB name set `?IsOverridingFactionEnemyOnTarget@NCreatureAI@@YI_NPBVCThingAICreature@@PBVCThing@@@Z`;
//   callers: the landed NCreatureAI::IsToAttackTarget 0x00864D00 and 0x0076CFBE.
// Callees: CDefClassBase::GetSubDef<CCreatureDef>(CDefPointer<const CCreatureDef>*) const 0x00485815 (a same-TU
//   noinline stand-in, the landed AddWound 0x00757B30 model: VC7.1 then keeps the def in ebx across the later calls and
//   spills the target; external, the def is re-read from its slot), CVectorMap::LowerBound const
//   0x004365B0, CTCScriptedControl::IsActionListEmpty() const 0x007122C0, CThingAICreature::GetCurrentStateGroupType()
//   const 0x00662010 (`[this+0E0h]+0F0h`), CTCOpinionSource::PeekWitnessWillOverrideFactionEnemy(const
//   CThingAICreature*) const 0x008640F0 (landed), CTCCreatureOpinionOfHero::IsFactionEnemyToHeroOverriddenOnOpinion()
//   const 0x00681850 (PDB name set; the catalog's "IsWillingToAttackHeroOnOpinion" belongs to its twin 0x006817F0, which
//   defaults to true where this one defaults to false); the def's virtual slot 1 on the last release.
// Layout: CThing PBaseDef +0x70 (CDefPointer<const CThingBaseDef>, Ego_r +0x80 less the retail 0x10 shift),
//   UsingComponents +0x20, Components +0x44; CCreatureDef CreatureGroup +0xAC (Ego_r +0xBC: four 16-byte Dinkumware
//   containers ahead of it are 12 bytes in retail); CTCScriptedControl Locked +0x18. TC interface types: 0x1F
//   CTCScriptedControl, 0x23 CTCVillageMember, 0x4F CTCCreatureOpinionOfHero, 0x50 CTCOpinionSource, 0x51
//   CTCHeroOpinionDeedLog.
// Shape: the creature-def CDefPointer local (released at every exit through DecRefToZero); HasTC(0x23) guards the
//   rest; the scripted-control test sets a `scripted` flag tested with the opinion conditions (an early `return false`
//   inside it duplicates the release epilogue); the scripted-control and opinion-of-hero components through the
//   direct-return PeekTC; the target's opinion
//   source through an out-pointer lookup (`mov eax,[eax+4] ; mov ecx,eax`); the opinion-of-hero pointer assigned under a
//   second HasTC(0x4F) into an otherwise unset local (absent: the dead target slot is read). Lookups are __forceinline.
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
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:                                      // CVectorMap<ETCInterfaceType, CTCBase*> {first, last}
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;                                          // +0x00
    SEntry* last;                                           // +0x04
    const SEntry* LowerBound(const ETCInterfaceType& type) const;       // 0x004365B0
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CThing {
public:
    unsigned char _pad0[0x20];
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
        ETCInterfaceType key = (ETCInterfaceType)(type);
        const CInterfaceMap* map = &m_interfaces;
        const CInterfaceMap::SEntry* it = map->LowerBound(key);
        const CInterfaceMap::SEntry* end = map->last;
        return (it == end || type < it->first ? end : it)->second;
    }
    template <class T> __forceinline bool PeekTC(long type, const T** out) const    // out-pointer form
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31))) {
            ETCInterfaceType key = (ETCInterfaceType)(type);
            const CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::SEntry* it = map->LowerBound(key);
            if (it == map->last || type < it->first)
                it = map->last;
            *out = (const T*)it->second;
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
    bool PeekWitnessWillOverrideFactionEnemy(const CThingAICreature* witness) const;   // 0x008640F0
};

class CTCCreatureOpinionOfHero {                            // interface type 0x4F
public:
    bool IsFactionEnemyToHeroOverriddenOnOpinion() const;   // 0x00681850 (catalog "IsWillingToAttackHeroOnOpinion" is its twin 0x006817F0)
};

namespace NCreatureAI {

bool __fastcall IsOverridingFactionEnemyOnTarget(const CThingAICreature* creature, const CThing* target)
{
    CDefPointer<const CCreatureDef> creature_def;
    if (creature->PBaseDef->GetSubDef(&creature_def) && creature_def.IsValid() && creature_def->CreatureGroup == 4)
        return false;
    if (creature->HasTC(0x23)) {
        bool scripted = false;
        if (creature->HasTC(0x1f)) {
            const CTCScriptedControl* scripted_control = (const CTCScriptedControl*)creature->PeekTC(0x1f);
            scripted = scripted_control->Locked || !scripted_control->IsActionListEmpty();
        }
        if (!scripted && creature->HasTC(0x4f) && creature->GetCurrentStateGroupType() != 3) {
            const CTCOpinionSource* opinion_source;
            if (target->PeekTC(0x50, &opinion_source) && opinion_source->PeekWitnessWillOverrideFactionEnemy(creature))
                return true;
            const CTCCreatureOpinionOfHero* opinion_of_hero;
            if (creature->HasTC(0x4f))
                opinion_of_hero = (const CTCCreatureOpinionOfHero*)creature->PeekTC(0x4f);
            if (target->HasTC(0x51) && opinion_of_hero->IsFactionEnemyToHeroOverriddenOnOpinion())
                return true;
        }
    }
    return false;
}

}