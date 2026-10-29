// CTCSummonSpell::CTCSummonSpell(CThing& parent) -- the summon-spell component: look up its SPECIAL_ABILITIES_SUMMON_SPELL_DEF, resolve the base summon creature's global def once (class static), start both fade-in timers from the def's fade-in time and take the caster's summon ability level
// Leaf: ??0CTCSummonSpell
// Retail: Fable.exe (TLC, VC7.1) @ 0x0079A6E0, 305 bytes, `ret 4`; vtable 0x0126C2B4.
// Callees: CTCBase::CTCBase(CThing*) @ 0x00686800, CBaseIntelligentPointer ctor @ 0x00A01B10 (x2) / SetPItem @ 0x00A01B90,
//   CCharString(const char*, long) @ 0x0099EBF0 ("SPECIAL_ABILITIES_SUMMON_SPELL_DEF" @ 0x01235C3C), CGameDefinitionManager::Get()
//   @ 0x0044C6B0 (`__fastcall`, three times), CDefinitionManager::GetGlobalIndexFromClassIndex(const CCharString&, long) const
//   @ 0x009AD390, ~CCharString @ 0x0099EAE0 (x2), CDefinitionManager::GetDef<CSpecialAbilitiesSummonSpellDef> @ 0x0079BD10,
//   CDefStringTable::GetString(long) @ 0x009D49B0 (on g_DefStringTable @ 0x013CA828), CDefinitionManager::
//   GetDefGlobalIndexFromName(const CCharString&) @ 0x009AD410, __ftol2 @ 0x00BFEA70, the const interface LowerBound
//   @ 0x004365B0 (interface 0x6F), CTCInventoryAbilities::GetAbilityLevel(EHeroAbility) @ 0x005EDE93 (ability 10).
//   Globals: CTCSummonSpell::SummonCreatureGlobalDefNum @ 0x0138306C (class static), g_framesPerSecond @ 0x01375550.
// Layout: Ego_r.pdb CTCSummonSpell (pack(1): Minion at the odd +0x11, CIntelligentPointer<CThingAICreature> vtable
//   0x0123E7E0): PDef +0x0C, JustReleased +0x10, FadeInTimer +0x1C, CurFadeInTimer +0x20, SpellLevel +0x24, CurTimer +0x28,
//   PItemRemoved (CIntelligentPointer<CThing>, vtable 0x01238C6C) +0x2C, CutSceneCancelled +0x34; CSpecialAbilitiesSummonSpellDef
//   BaseSummonCreatureName +0x48, FadeInTime +0x4C (PDB -8: the def's 12-byte vector).
// Shape: the landed CTCForcePushPower ctor (0x007A54A0): init-list members after the vtable store sharing the ebx zero; the
//   class-name string in its own block; the creature-name string a block-scoped by-value result; `FadeInTimer =
//   CurFadeInTimer = ...` stores CurFadeInTimer first.
class CThing;
class CBaseObject;
class CTCBase;
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length);         // 0x0099EBF0
    CCharString(const CCharString& other);
    ~CCharString();                                     // 0x0099EAE0
};
struct CDefStringTable {
public:
    CCharString GetString(long) const;   // 0x009D49B0
};
extern CDefStringTable g_DefStringTable;                // 0x013CA828
class CSpecialAbilitiesSummonSpellDef {
public:
    virtual void v00();
    long RefCount;                                      // +0x04
    unsigned char _pad08[0x48 - 0x08];
    long  BaseSummonCreatureName;                       // +0x48 CDefString
    float FadeInTime;                                   // +0x4c
};
template<class T> class CDefPointer {
public:
    T* Object;
    CDefPointer() : Object(0) {}
    T* operator->() const { return Object; }
};
class CDefinitionManager {
public:
    long GetGlobalIndexFromClassIndex(const CCharString& className, long classIndex) const;   // 0x009AD390
    template<class T> bool GetDef(long index, CDefPointer<const T>* out) const;   // 0x0079BD10 for the summon-spell def
public:
    long GetDefGlobalIndexFromName(const CCharString&) const;   // 0x009AD410
};
class CGameDefinitionManager : public CDefinitionManager {
public:
    static CGameDefinitionManager& __fastcall Get();   // 0x0044C6B0
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                                    // +0x04
    CBaseIntelligentPointer();                          // 0x00A01B10
    void SetPItem(CBaseObject* item);                   // 0x00A01B90
};
template<class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:
    virtual void v00();
    CIntelligentPointer() : CBaseIntelligentPointer() {}
    CIntelligentPointer(T* item) : CBaseIntelligentPointer() { SetPItem((CBaseObject*)item); }
};
class CThingAICreature;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    const SEntry* LowerBound(const ETCInterfaceType& type) const;         // 0x004365B0
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CTCInventoryAbilities {
public:
    long GetAbilityLevel(long ability);                 // 0x005EDE93 (EHeroAbility)
};
class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                     // +0x20
    CInterfaceMap m_interfaces;                         // +0x44
    __forceinline bool HasTC(long type) const
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
    __forceinline CTCBase* GetTC(long type) const
    {
        ETCInterfaceType key = (ETCInterfaceType)(type);
        const CInterfaceMap* map = &m_interfaces;
        const CInterfaceMap::SEntry* it = map->LowerBound(key);
        CInterfaceMap::SEntry* end = map->last;
        return (it == end || type < it->first ? end : it)->second;
    }
};
extern long g_framesPerSecond;                          // 0x01375550
class CTCBase {
public:
    CTCBase(CThing* parent);                            // 0x00686800
    virtual void v00();
    CThing* ParentThing;                                // +0x04
    unsigned char _pad08[0x4];                          // ToBeRemoved, IsInGlobalUpdateTCs
};
#pragma pack(push, 1)
class CTCSummonSpell : public CTCBase {                 // vtable 0x0126C2B4
public:
    CDefPointer<const CSpecialAbilitiesSummonSpellDef> PDef;   // +0x0c
    bool JustReleased;                                  // +0x10
    CIntelligentPointer<CThingAICreature> Minion;       // +0x11
    unsigned char _pad19[3];
    long FadeInTimer;                                   // +0x1c
    long CurFadeInTimer;                                // +0x20
    long SpellLevel;                                    // +0x24
    long CurTimer;                                      // +0x28
    CIntelligentPointer<CThing> PItemRemoved;           // +0x2c
    bool CutSceneCancelled;                             // +0x34
    static long SummonCreatureGlobalDefNum;             // 0x0138306C
    CTCSummonSpell(CThing& parent);
    virtual void v00();
};
#pragma pack(pop)

CTCSummonSpell::CTCSummonSpell(CThing& parent)
    : CTCBase(&parent), JustReleased(true), FadeInTimer(0), CurFadeInTimer(0), SpellLevel(0), CurTimer(0),
      PItemRemoved(0), CutSceneCancelled(false)
{
    long index;
    {
        CCharString name("SPECIAL_ABILITIES_SUMMON_SPELL_DEF", -1);
        index = CGameDefinitionManager::Get().GetGlobalIndexFromClassIndex(name, 1);
    }
    CGameDefinitionManager::Get().GetDef(index, &PDef);
    if (SummonCreatureGlobalDefNum == -1) {
        CCharString creatureName = g_DefStringTable.GetString(PDef->BaseSummonCreatureName);
        SummonCreatureGlobalDefNum = CGameDefinitionManager::Get().GetDefGlobalIndexFromName(creatureName);
    }
    FadeInTimer = CurFadeInTimer = (long)(g_framesPerSecond * PDef->FadeInTime);
    if (ParentThing->HasTC(0x6f))
        SpellLevel = ((CTCInventoryAbilities*)ParentThing->GetTC(0x6f))->GetAbilityLevel(10);
}