// CTCInventoryAbilities::OnSpecialAbilityUsed(EHeroAbility, bool) -- cast a special ability: if allowed, use it through the special-abilities component, post the cast message, charge the will cost and train the stat; else play the cast-failed sound
#pragma optimize("y", off)
#pragma optimize("s", on)
enum EHeroAbility { EHeroAbility_Force32 = 0x7fffffff };
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);           // 0x0099EBF0
    ~CCharString();                                            // 0x0099EAE0
};
struct C3DVector { float X, Y, Z; };
struct CTCBase;
struct CPresenceBits { bool test(long bit) const; };           // 0x00410DE0
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);                      // 0x0040F020
    const SEntry* LowerBound(const long* type) const;          // 0x004365B0
};
// same-TU stand-in for the interface-map LowerBound (VC7.1 then knows the entry is not derived from the key)
#pragma auto_inline(off)
__declspec(noinline) CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{
    SEntry* lo = first;
    long count = last - first;
    while (count > 0) {
        long half = count >> 1;
        SEntry* mid = lo + half;
        if (mid->type < *type) {
            lo = mid + 1;
            count -= half + 1;
        } else
            count = half;
    }
    return lo;
}
#pragma auto_inline(on)
#pragma auto_inline(off)
__declspec(noinline) const CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type) const
{
    const SEntry* lo = first;
    long count = last - first;
    while (count > 0) {
        long half = count >> 1;
        const SEntry* mid = lo + half;
        if (mid->type < *type) {
            lo = mid + 1;
            count -= half + 1;
        } else
            count = half;
    }
    return lo;
}
#pragma auto_inline(on)
struct CThing {
    unsigned char  _pad00[0x20];
    CPresenceBits  m_present;                                  // +0x20
    unsigned char  _pad21[0x23];
    CInterfaceMap  m_interfaces;                               // +0x44
    const C3DVector& GetPosition() const;                      // 0x004C73D0
    template<class T> __forceinline bool GetInterface(T** out, long wanted)
    {
        if (m_present.test(wanted)) {
            long type = wanted;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *out = (T*)((it == map->last || wanted < it->type) ? map->last : it)->component;
            return true;
        }
        return false;
    }
    template<class T> __forceinline bool GetInterface(T** out, long wanted) const
    {
        if (m_present.test(wanted)) {
            long type = wanted;
            const CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *out = (T*)((it == map->last || wanted < it->type) ? map->last : it)->component;
            return true;
        }
        return false;
    }
    template<class T> __forceinline bool GetInterfaceFound(T** out, long wanted)
    {
        if (m_present.test(wanted)) {
            long type = wanted;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *out = (T*)((it != map->last && !(wanted < it->type)) ? it : map->last)->component;
            return true;
        }
        return false;
    }
};

struct CDefPointeeBase {
    virtual void v00();
    virtual void Destroy() const;                              // slot 1
    mutable long RefCount;                                     // +0x04
};
struct CHeroAbilityDef : CDefPointeeBase {
    unsigned char _pad08[0x40 - 0x08];
    long  TrainableStat;                                       // +0x40
    unsigned char _pad44[0x70 - 0x44];
    long* WillCosts;                                           // +0x70
    unsigned char _pad74[0x7c - 0x74];
    long  TrainableStatExperience;                             // +0x7c
};
template<class T> struct CDefPointer {
    T* Object;
    __forceinline CDefPointer(const CDefPointer& o) : Object(o.Object) { if (Object) ++Object->RefCount; }
    __forceinline ~CDefPointer() { if (Object && --Object->RefCount == 0) Object->Destroy(); }
    T* operator->() const { return Object; }
};
struct SAbilityEntry { long first; CDefPointer<const CHeroAbilityDef> second; };
struct CAbilityEntryVector {
    SAbilityEntry* _M_start; SAbilityEntry* _M_finish; SAbilityEntry* _M_end_of_storage;
    SAbilityEntry& operator[](unsigned long n) { return _M_start[n]; }
};

struct CTCSpecialAbilities {                                   // interface 0xA7
    bool ToggleUsingAbility(EHeroAbility ability);             // 0x0072AC90
    bool UseSpecialAbility(EHeroAbility ability);              // 0x0072CF40
};
struct CTCHeroStats {                                          // interface 4
    unsigned char _pad00[0x130];
    long* AbilityUseCounts;                                    // +0x130
    void ModifyStaminaBy(long amount);                         // 0x0057B1DB
};
struct CTCHeroState { unsigned char _pad00[0xd]; bool CanUseAbilities; };   // interface 0x29
struct CTCSpellCost { long GetCost(); };                       // interface 0x91, 0x0079A610
struct CTCHeroExperience { void AddTrainableStatExperience(long stat, long amount); };   // interface 0x68, 0x006D8300
struct CSoundInfo { unsigned long PlaySound(const CCharString& name, long a, long b, long c, long d); };   // interface 0x10, 0x0066BD00
struct CMessageEvent;
struct CMessageEventManager {
    CMessageEvent* AddMessage(long type, unsigned long flags, const C3DVector* position, CThing* thing1, CThing* thing2, float a, float b, long priority);   // 0x00687540
};
struct CWorld { unsigned char _pad00[0x60]; CMessageEventManager* PMessageEventManager; };   // +0x60
extern long g_FrameRate;                                       // 0x01375550

// Layout: retail CTCInventoryAbilities (Ego_r.pdb - 0x18): CurrentSpecialAbilityUsedFromQuickAccess +0x142, Abilities +0x160.
struct CTCInventoryAbilities {
    void*   __vftable;
    CThing* ParentThing;                                       // +0x04
    unsigned char _pad08[0x142 - 0x08];
    bool    CurrentSpecialAbilityUsedFromQuickAccess;          // +0x142
    unsigned char _pad143[0x160 - 0x143];
    CAbilityEntryVector Abilities;                             // +0x160
    CWorld* GetWorld();                                        // 0x00686A70 (CTCBase)
    bool IsAbleToUseAbility(EHeroAbility ability, bool checkCost) const;   // 0x005EE49A
    void OnSpecialAbilityUsed(EHeroAbility ability, bool fromQuickAccess);
};
void CTCInventoryAbilities::OnSpecialAbilityUsed(EHeroAbility ability, bool fromQuickAccess)
{
    CTCSpecialAbilities* special;
    if (!ParentThing->GetInterface(&special, 0xa7))
        return;
    if (special->ToggleUsingAbility(ability))
        return;
    if (IsAbleToUseAbility(ability, true)) {
        CTCHeroStats* stats = 0;
        ParentThing->GetInterface(&stats, 4);
        CTCHeroState* state;
        if (!static_cast<const CThing*>(ParentThing)->GetInterface(&state, 0x29))
            return;
        if (!state->CanUseAbilities)
            return;
        CAbilityEntryVector* abilitiesP = &Abilities;
        CAbilityEntryVector& abilities = *abilitiesP;
        CDefPointer<const CHeroAbilityDef> def = abilities[ability].second;
        long cost = -def->WillCosts[abilities[ability].first];
        if (ability == 10) {
            const CThing* parent = ParentThing;
            CTCSpellCost* spellCost;
            if (parent->GetInterface(&spellCost, 0x91))
                cost = -spellCost->GetCost();
        }
        if (special->UseSpecialAbility(ability)) {
            CurrentSpecialAbilityUsedFromQuickAccess = fromQuickAccess;
            CThing* parent = ParentThing;
            long frames = g_FrameRate;
            CMessageEventManager* messages = GetWorld()->PMessageEventManager;
            messages->AddMessage(0x44, frames, &parent->GetPosition(), 0, 0, 0.0f, 0.0f, ability);
            stats->ModifyStaminaBy(cost);
            ++stats->AbilityUseCounts[ability];
            if (ability != 0xb && ability != 5 && ability != 0x12 && ability != 0x13) {
                CTCHeroExperience* experience = 0;
                ParentThing->GetInterface(&experience, 0x68);
                experience->AddTrainableStatExperience(def->TrainableStat, def->TrainableStatExperience);
            }
        }
    } else {
        CSoundInfo* sound;
        if (ParentThing->GetInterfaceFound(&sound, 0x10))
            sound->PlaySound("HERO_ABILITY_CAST_FAILED", 1, 0, 0, -1);
    }
}