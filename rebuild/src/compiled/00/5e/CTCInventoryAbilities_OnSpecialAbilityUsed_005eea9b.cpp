// CTCInventoryAbilities::OnSpecialAbilityUsed(EHeroAbility, bool) -- cast a special ability: if allowed, use it through the special-abilities component, post the cast message, charge the will cost and train the stat; else play the cast-failed sound
// Plan phase 4 (2026-11-05), C3DVector wave (rows with a second value class, from their landed source): the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Plan phase 3 (2026-11-04), CVectorMap wave (stand-in map): the local stand-in map class (and its same-TU LowerBound body) replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<..> > derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D; the same-TU noinline LowerBound is the header's); entries read as pair first/second, the map end through end(). Also: the empty local std::bitset stand-in is the STLport bitset<274> (36 bytes) of stlport/fable_containers.h, test() declared only under FABLE_RETAIL_STLP_BITSET_TEST_DECL (retail calls 0x00410DE0); bitset pad _pad21[0x23] removed. Spellings unified: 0040f020: ?LowerBound@CInterfaceMap@@QAEPAUSEntry@1@PBJ@Z -> ?LowerBound@?$CVectorMap@W4ETCInterfaceType@@PAVCTCBase@@V?$CKeyPairCompareLess@W4ETCInterfaceType@@PAVCTCBase@@@@@@QAEPAU?$pair@W4ETCInterfaceType@@PAVCTCBase@@@std@@ABW4ETCInterfaceType@@@Z; 004365b0: ?LowerBound@CInterfaceMap@@QBEPBUSEntry@1@PBJ@Z -> ?LowerBound@?$CVectorMap@W4ETCInterfaceType@@PAVCTCBase@@V?$CKeyPairCompareLess@W4ETCInterfaceType@@PAVCTCBase@@@@@@QBEPBU?$pair@W4ETCInterfaceType@@PAVCTCBase@@@std@@ABW4ETCInterfaceType@@@Z
// Naming wave 18: the interface-presence stand-in CPresenceBits is std::bitset<274> (STLport 4.6); its test is ?test@?$bitset@$0BBC@@std@@QBE_NI@Z (name-only).
// Naming wave 18: the global stand-in g_FrameRate (0x01375550) is the protected static long CMainGameComponent::ConstantFPS, read through the
//   inline static accessor GetConstantFPS() as retail code outside the class does (name-only: the bytes are unchanged).
#pragma optimize("y", off)
#pragma optimize("s", on)
enum EHeroAbility { EHeroAbility_Force32 = 0x7fffffff };
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);           // 0x0099EBF0
    ~CCharString();                                            // 0x0099EAE0
};
#include "engine/C3DVector.h"
class CTCBase;
#define FABLE_RETAIL_STLP_BITSET_TEST_DECL
#include "engine/CVectorMap.h"
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing { public:
    unsigned char  _pad00[0x20];
    std::bitset<274>  m_present;                                  // +0x20
    CInterfaceMap  m_interfaces;                               // +0x44
    const C3DVector& GetPos() const;                      // 0x004C73D0
    template<class T> __forceinline bool GetInterface(T** out, long wanted)
    {
        if (m_present.test(wanted)) {
            long type = wanted;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::value_type* it = map->LowerBound((const ETCInterfaceType&)type);
            *out = (T*)((it == map->end() || wanted < it->first) ? map->end() : it)->second;
            return true;
        }
        return false;
    }
    template<class T> __forceinline bool GetInterface(T** out, long wanted) const
    {
        if (m_present.test(wanted)) {
            long type = wanted;
            const CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::value_type* it = map->LowerBound((const ETCInterfaceType&)type);
            *out = (T*)((it == map->end() || wanted < it->first) ? map->end() : it)->second;
            return true;
        }
        return false;
    }
    template<class T> __forceinline bool GetInterfaceFound(T** out, long wanted)
    {
        if (m_present.test(wanted)) {
            long type = wanted;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::value_type* it = map->LowerBound((const ETCInterfaceType&)type);
            *out = (T*)((it != map->end() && !(wanted < it->first)) ? it : map->end())->second;
            return true;
        }
        return false;
    }
};

class CDefPointeeBase { public:
    virtual void v00();
    virtual void Destroy() const;                              // slot 1
    mutable long RefCount;                                     // +0x04
};
class CHeroAbilityDef : public CDefPointeeBase { public:
    unsigned char _pad08[0x40 - 0x08];
    long  TrainableStat;                                       // +0x40
    unsigned char _pad44[0x70 - 0x44];
    long* WillCosts;                                           // +0x70
    unsigned char _pad74[0x7c - 0x74];
    long  TrainableStatExperience;                             // +0x7c
};
template<class T> class CDefPointer { public:
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

class CTCSpecialAbilities { public:                                   // interface 0xA7
    bool ToggleUsingAbility(EHeroAbility ability);             // 0x0072AC90
    bool UseSpecialAbility(EHeroAbility ability);              // 0x0072CF40
};
class CTCHeroStats { public:                                          // interface 4
    unsigned char _pad00[0x130];
    long* AbilityUseCounts;                                    // +0x130
    void ModifyStaminaBy(long amount);                         // 0x0057B1DB
};
struct CTCHeroState { unsigned char _pad00[0xd]; bool CanUseAbilities; };   // interface 0x29
struct CTCSpellCost { long GetCost(); };                       // interface 0x91, 0x0079A610
class CTCHeroExperience { public: void AddTrainableStatExperience(long stat, long amount); };   // interface 0x68, 0x006D8300
class CTCSoundPlayer { public: public:
    unsigned long PlayAttachedCriteria(const CCharString&, bool, bool, bool, long);   // 0x0066BD00
};   // interface 0x10, 0x0066BD00
class CMessageEvent;
class CMessageEventManager { public:
    CMessageEvent* AddMessage(long type, unsigned long flags, const C3DVector* position, CThing* thing1, CThing* thing2, float a, float b, long priority);   // 0x00687540
};
class CWorld { public: unsigned char _pad00[0x60]; CMessageEventManager* PMessageEventManager; };   // +0x60
class CMainGameComponent {
protected:
    static long ConstantFPS;                             // 0x01375550 ?ConstantFPS@CMainGameComponent@@1JA (protected static)
public:
    static long __fastcall GetConstantFPS() { return ConstantFPS; }   // Ego_r / FableWin ?GetConstantFPS@CMainGameComponent@@SIJXZ (inline)
};

// Layout: retail CTCInventoryAbilities (Ego_r.pdb - 0x18): CurrentSpecialAbilityUsedFromQuickAccess +0x142, Abilities +0x160.
class CTCBase
{
protected:
    CWorld& GetWorld() const;   // 0x00686A70
};
namespace NInventory {
class CTCInventoryBase : public CTCBase
{
public:
};
}

class CTCInventoryAbilities : public NInventory::CTCInventoryBase { public:
    void*   __vftable;
    CThing* ParentThing;                                       // +0x04
    unsigned char _pad08[0x142 - 0x08];
    bool    CurrentSpecialAbilityUsedFromQuickAccess;          // +0x142
    unsigned char _pad143[0x160 - 0x143];
    CAbilityEntryVector Abilities;                             // +0x160
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
            long frames = CMainGameComponent::GetConstantFPS();
            CMessageEventManager* messages = GetWorld().PMessageEventManager;
            messages->AddMessage(0x44, frames, &parent->GetPos(), 0, 0, 0.0f, 0.0f, ability);
            stats->ModifyStaminaBy(cost);
            ++stats->AbilityUseCounts[ability];
            if (ability != 0xb && ability != 5 && ability != 0x12 && ability != 0x13) {
                CTCHeroExperience* experience = 0;
                ParentThing->GetInterface(&experience, 0x68);
                experience->AddTrainableStatExperience(def->TrainableStat, def->TrainableStatExperience);
            }
        }
    } else {
        CTCSoundPlayer* sound;
        if (ParentThing->GetInterfaceFound(&sound, 0x10))
            sound->PlayAttachedCriteria("HERO_ABILITY_CAST_FAILED", 1, 0, 0, -1);
    }
}