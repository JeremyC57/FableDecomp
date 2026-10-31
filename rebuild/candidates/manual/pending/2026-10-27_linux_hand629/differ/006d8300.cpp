// CTCHeroExperience::AddTrainableStatExperience(EHeroExperienceStatCategory, long) -- scales the experience by the combo multiplier and the weapon modifier, books it on the stat category (clamped at 0) and forwards it to the hero stats
// Grade: DIFFER(173v173) -- one SIB order only: retail `lea eax,[ecx+edx]` (scaled index + begin), ours `lea eax,[edx+ecx]`
// Tried (sweep): `*(n + begin())`, a `long&` local for the += (177). Chunk 629: a vector reference local, a signed index,
//   `begin()[n]`, `v[c] = v[c] + g` / `g + v[c]`, an inline ModifyTrainableStatExperience helper, char-offset address forms
//   (`*(T*)(n*sizeof(T) + (char*)begin())`), the explicit `if (<0) = 0` clamp (168) -- the base/index order never flips.
// Leaf: ?AddTrainableStatExperience@CTCHeroExperience@@QAEXW4EHeroExperienceStatCategory@@J@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006D8300, 173 bytes, `ret 8`.
// Identity: Ego_r.pdb / FableWin.pdb ?AddTrainableStatExperience@CTCHeroExperience@@QAEXW4EHeroExperienceStatCategory@@J@Z
//   (the parameter is the stat CATEGORY, the index of StatExperiencePoints, as in the landed SpendExperience 0x006D6F60).
// Callees: __ftol2 0x00BFEA70 (x2), NHeroExperience::GetWeaponModifiedExperienceMultiplierForThing(const CThing&)
//   0x006D76C0 (__fastcall, x87 result; landed), CVectorMap<ETCInterfaceType,CTCBase*,...>::LowerBound 0x0040F020,
//   CTCHeroStats::AddHeroStatExperience(EHeroExperienceStatCategory, long) 0x0057A8DA. Pool: 1.0 (double) 0x01237F00.
// Shape: the combo multiplier is `(long)(ExperienceMultiplierParam / def->PerUnit + 1.0)` (a double 1.0 pool constant)
//   floored at 1; the gain `(long)(weaponMultiplier * (multiplier * amount))` uses `fimul` on the integer product spilled
//   into the dead `amount` slot. The stat entry is reached through STLport's `*(begin() + n)` (`lea ecx,[ebx*4]` shared,
//   begin re-read for the `long&` ClampMin). The hero-stats component (interface 4) is the inlined raw bitset probe
//   (`test byte ptr [thing+20h],10h`) then an inline lookup whose key lands in the same dead slot.
// Layout: engine/CTCHeroExperience.h (StatExperiencePoints +0x18, ExperienceMultiplierParam +0x30, AbleToGainExperience
//   +0x3C, PDef_Object +0x54) through the `#define` pattern of the landed 0x006D63A0; CTCBase::ParentThing +0x04 sits in
//   the header's flattened base bytes, so the derived class reads it through a typed accessor.
#define CTCHeroExperience CTCHeroExperienceLayout
#include "engine/CTCHeroExperience.h"
#undef CTCHeroExperience

class CTCBase;

static inline void ClampMin(long& value, long minimum)
{
    if (value < minimum)
        value = minimum;
}

enum ETCInterfaceType { };
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type);       // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CThing { public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                  // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                      // +0x44
    bool HasTC(long wanted)
    {
        if (m_presentBits[wanted >> 5] & (1UL << (wanted & 31)))
            return true;
        return false;
    }
    template<class T> __forceinline T* GetTC(long wanted)
    {
        ETCInterfaceType type = (ETCInterfaceType)wanted;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(type);
        if (it == map->last || wanted < it->first)
            it = map->last;
        return (T*)it->second;
    }
};

namespace NHeroExperience {
float __fastcall GetWeaponModifiedExperienceMultiplierForThing(const CThing& thing);   // 0x006D76C0
}

enum EHeroExperienceStatCategory { HERO_EXPERIENCE_STAT_CATEGORY_NULL };

class CTCHeroStats { public:                         // interface type 4
    void AddHeroStatExperience(EHeroExperienceStatCategory category, long amount);   // 0x0057A8DA
};

class CHeroExperienceDef { public:                   // retail offsets, Ego_r names
    unsigned char _pad00[0x64];
    float ExperienceMultiplierParamPerUnitComboMultiplier;   // +0x64
};

template <class T> struct CRetailVector {            // STLport 4.6 vector<T>: {_M_start, _M_finish, _M_end_of_storage}
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T* begin() { return _M_start; }
    T& operator[](unsigned long n) { return *(begin() + n); }
};

class CTCHeroExperience : public CTCHeroExperienceLayout {
public:
    CThing* GetParentThing() const { return *(CThing* const*)(_base_0x04); }   // CTCBase::ParentThing +0x04
    CRetailVector<long>& GetStatExperiencePoints() { return *(CRetailVector<long>*)StatExperiencePoints; }
    void AddTrainableStatExperience(EHeroExperienceStatCategory category, long amount);
};

void CTCHeroExperience::AddTrainableStatExperience(EHeroExperienceStatCategory category, long amount)
{
    if (AbleToGainExperience) {
        long multiplier = (long)(ExperienceMultiplierParam
            / ((const CHeroExperienceDef*)PDef_Object)->ExperienceMultiplierParamPerUnitComboMultiplier + 1.0);
        if (multiplier < 1)
            multiplier = 1;
        long gained = (long)(NHeroExperience::GetWeaponModifiedExperienceMultiplierForThing(*GetParentThing())
            * (multiplier * amount));
        GetStatExperiencePoints()[category] += gained;
        ClampMin(GetStatExperiencePoints()[category], 0);
        CThing* thing = GetParentThing();
        if (thing->HasTC(4))
            thing->GetTC<CTCHeroStats>(4)->AddHeroStatExperience(category, gained);
    }
}
