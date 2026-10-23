// CTCInventoryAbilities::IsAbleToUseAbility(EHeroAbility ability, bool checkCost) -- whether a known ability can be used now: with checkCost, the hero's will energy must cover the spell's cost (fireball, unholy power and divine wrath read it from their special-ability defs)
// Grade: DIFFER(555v549) -- the hero-stats lookup key gets its own frame word (ours [ebp-4], retail shares the dead checkCost slot [ebp+0xC] with the out pointer and the def-name temporaries, ours put those in the ability slot); the tail-merged def release forms lea eax,[ecx+4] ; dec [eax] where retail has dec [ecx+4]
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005EE49A, 549 bytes, `ret 8`; frame-pointer size TU
// (`optimize("y", off)` + `("s", on)`, as the landed FrameUpdate 0x005EE418).
// Callees: std::bitset::test 0x00410DE0, CVectorMap::LowerBound const 0x004365B0, CGameDefinitionManager::Get 0x0044C6B0
// (__fastcall), CDefinitionManager::GetGlobalIndexFromClassIndex(const CCharString&, long) const 0x009AD390,
// CDefinitionManager::GetDef<...> 0x005F8A57 / 0x005F8AB9 / 0x005F8B1B (fireball / unholy power / divine wrath),
// CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0, interface 0x91's cost getter 0x0079A610
// (manifest label CPixelShader::CountReferences is propagated).
// Layout: retail CTCInventoryAbilities ability entries {Level, PDef} at *(+0x160) (landed GetAbilityLevel 0x005EDE93);
// the hero stats interface (type 4) keeps the will energy at +0x58; the ability def's per-level cost vector at +0x70.
#pragma optimize("y", off)
#pragma optimize("s", on)

struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);  // 0x0099EBF0
    ~CCharString();                                   // 0x0099EAE0
};

struct CRefCounted {
    long m_count;                                     // +0x04 in the def
    __forceinline void DecRef() { --m_count; }
    __forceinline long GetRefCount() const { return m_count; }
};
struct CDefVirtual {
    virtual void v00();
    virtual void Destroy();                           // slot 1
};
struct CDefBase : CDefVirtual, CRefCounted {
    __forceinline void ReleaseTemporary() { DecRef(); if (GetRefCount() == 0) Destroy(); }
};
struct CSpecialAbilitiesFireballDef : CDefBase {
    unsigned char _pad_0x08[0x3C];
    float*        ManaCost;                           // +0x44 (vector<float> first)
};
struct CSpecialAbilitiesPowerDef : CDefBase {         // unholy power / divine wrath (same layout)
    unsigned char _pad_0x08[0xB0];
    float*        CostMultiplier;                     // +0xB8 (vector<float> first)
    unsigned char _pad_0xbc[0x20];
    float*        BaseCost;                           // +0xDC (vector<float> first)
};
struct CSpecialAbilitiesUnholyPowerDef : CSpecialAbilitiesPowerDef { };
struct CSpecialAbilitiesDivineWrathDef : CSpecialAbilitiesPowerDef { };

template<class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) { }
    __forceinline ~CDefPointer()
    {
        Object->ReleaseTemporary();
    }
    T* operator->() const { return Object; }
};

struct CDefinitionManager {
    long GetGlobalIndexFromClassIndex(const CCharString& className, long classIndex) const;   // 0x009AD390
    bool GetDef(long index, CDefPointer<CSpecialAbilitiesFireballDef>* out) const;       // 0x005F8A57
    bool GetDef(long index, CDefPointer<CSpecialAbilitiesUnholyPowerDef>* out) const;    // 0x005F8AB9
    bool GetDef(long index, CDefPointer<CSpecialAbilitiesDivineWrathDef>* out) const;    // 0x005F8B1B
};
struct CGameDefinitionManager : CDefinitionManager {
    static CGameDefinitionManager& __fastcall Get();  // 0x0044C6B0
};

struct CTCBase;
struct CPresenceBits {
    bool test(long bit) const;                        // 0x00410DE0 (std::bitset::test)
};
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;                                    // +0x00
    SEntry* last;                                     // +0x04
    const SEntry* LowerBound(const long* type) const; // 0x004365B0
};
struct CThing {
    unsigned char _pad_0x00[0x20];
    CPresenceBits m_present;                          // +0x20
    unsigned char _pad_0x21[0x23];
    CInterfaceMap m_interfaces;                       // +0x44
    template<class T> __forceinline bool GetInterface(T** out, long wanted) const
    {
        if (m_present.test(wanted)) {
            *out = GetTC<T>(wanted);
            return true;
        }
        return false;
    }
    template<class T> __forceinline T* GetTC(long wanted) const
    {
        long type = wanted;
        const CInterfaceMap* map = &m_interfaces;
        const CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (T*)(it == map->last || wanted < it->type ? map->last : it)->component;
    }
};
struct CTCHeroStats {                                 // interface type 4
    unsigned char _pad_0x00[0x58];
    long          WillEnergy;                         // +0x58
};
struct CTCSpellCost {                                 // interface type 0x91
    long GetCost();                                   // 0x0079A610
};
struct CAbilityDef {
    unsigned char _pad_0x00[0x70];
    long*         CostPerLevel;                       // +0x70 (vector<long> first)
};
struct SAbilityEntry {
    long         Level;                               // +0x00
    CAbilityDef* PDef;                                // +0x04
};

struct CTCInventoryAbilities {
    void*          __vftable;                         // +0x00
    const CThing*  ParentThing;                       // +0x04
    unsigned char  _pad_0x08[0x158];
    SAbilityEntry* Abilities;                         // +0x160 (vector first)
    bool IsAbleToUseAbility(long ability, bool checkCost);
};

bool CTCInventoryAbilities::IsAbleToUseAbility(long ability, bool checkCost)
{
    if (ability == 0 || Abilities[ability].Level < 0)
        return false;
    if (checkCost) {
        CTCHeroStats* stats;
        ParentThing->GetInterface(&stats, 4);
        if (ability == 0x10) {
            long index = CGameDefinitionManager::Get().GetGlobalIndexFromClassIndex("SPECIAL_ABILITIES_FIREBALL_SPELL_DEF", 1);
            CDefPointer<CSpecialAbilitiesFireballDef> def;
            CGameDefinitionManager::Get().GetDef(index, &def);
            bool result = stats->WillEnergy >= def->ManaCost[0];
            return result;
        }
        if (ability == 0x13) {
            long index = CGameDefinitionManager::Get().GetGlobalIndexFromClassIndex("SPECIAL_ABILITIES_UNHOLY_POWER_DEF", 1);
            CDefPointer<CSpecialAbilitiesUnholyPowerDef> def;
            CGameDefinitionManager::Get().GetDef(index, &def);
            long level = Abilities[0x13].Level;
            bool result = stats->WillEnergy >= def->BaseCost[level] * def->CostMultiplier[level] + 20.0f;
            return result;
        }
        if (ability == 0x12) {
            long index = CGameDefinitionManager::Get().GetGlobalIndexFromClassIndex("SPECIAL_ABILITIES_DIVINE_WRATH_DEF", 1);
            CDefPointer<CSpecialAbilitiesDivineWrathDef> def;
            CGameDefinitionManager::Get().GetDef(index, &def);
            long level = Abilities[0x12].Level;
            bool result = stats->WillEnergy >= def->BaseCost[level] * def->CostMultiplier[level] + 20.0f;
            return result;
        }
        long cost = Abilities[ability].PDef->CostPerLevel[Abilities[ability].Level];
        if (ability == 0xA) {
            CTCSpellCost* spell;
            if (ParentThing->GetInterface(&spell, 0x91))
                cost = spell->GetCost();
        }
        return stats->WillEnergy >= cost;
    }
    return true;
}
