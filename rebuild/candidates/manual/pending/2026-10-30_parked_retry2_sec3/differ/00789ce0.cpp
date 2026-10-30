// _global::GetDamageMultiplierFromThingAtBuildUpTime(const CThing& thing, CDefPointer<const CHeroCombatDef> def, float buildUpTime) -- the ranged-attack damage multiplier after building up a shot: (1 + hero-stat parameter * k) * (tanh-shaped nominal build-up + quadratic super build-up)
// Grade: DIFFER(175v175) -- wave 2 sec3 header-moved: the local CDefPointer/CDefPointeeBase replaced by engine/CDefPointer.h (FABLE_RETAIL_DEF_CLASS_BASE) and the real const LowerBound 0x004365B0 body in the TU; bytes identical to the parked grade (the header Release keeps retail's null-test-free release). Residue unchanged: the inline release's mov/dec/mov is scheduled 1-3 x87 instructions earlier than retail
//   lands one to three x87 instructions earlier than retail (retail stores the count after `fmul [edi+84h]`); the float
//   expression, frame, key slot and exits match.
// Leaf: ?GetDamageMultiplierFromThingAtBuildUpTime@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00789CE0, 175 bytes, `__fastcall` file-local (thing in ecx, the def pointer
//   and the build-up time on the stack, `ret 8`).
// Identity: Ego_r.pdb `?GetDamageMultiplierFromThingAtBuildUpTime@?A0x0524c104@@YIMABVCThing@@V?$CDefPointer@$$CBVCHeroCombatDef@@@@M@Z`
//   (the CTCHeroRangedAttacks TU); callers 0x0078A036 / 0x0078A883 (landed CancelAimingWeapon declares it).
// Callees: CVectorMap LowerBound const 0x004365B0 (inline GetTC 0x68 = CTCHeroExperience after the inline bitset test of
//   +0x2c bit 8), CHeroAbilityStatWeight::GetParameter(const CTCHeroExperience&) const 0x00851BD0, _CItanh 0x00BFECE2.
// Layout: retail CHeroCombatDef = PDB - 0x0c here (the two earlier CHeroAbilityStatWeight maps and the def base are 12-byte
//   STLport containers): RangedDamageMultiplierWeighting +0x78, SpeedWithStatParameter +0x84, SpeedNominalBuildup +0x8c,
//   ScaleNominalBuildup +0x90, SpeedSuperBuildup +0x94, ScaleSuperBuildup +0x98. The def's ~CDefPointer has no null test.
// Tried: `s * s * scale` (VC reassociates to (s*scale)*s), `scale * s * s`, `s * (s * scale)`; `scale * (s * s)` fixes
//   the x87 order, leaving only the refcount slot.
#include <math.h>

#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CDefPointer.h"
class CTCHeroExperience;
enum ETCInterfaceType { TC_INTERFACE_NONE = 0 };
class CTCBase;
namespace std {
template <class _Tp> class allocator { };
template <class _T1, class _T2> struct pair { _T1 first; _T2 second; };
template <class _Tp, class _Alloc = allocator<_Tp> > class vector {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
    const _Tp* end() const { return _M_finish; }
};
}
template <class K, class V> class CKeyPairCompareLess { };
template <class K, class V, class Cmp> class CVectorMap {
public:
    typedef std::pair<K, V> value_type;
    const value_type* LowerBound(const K& key) const;       // 0x004365B0
    std::vector<value_type> Data;
    Cmp  Compare;
    bool Dirty;
};
template <class K, class V, class Cmp>
__declspec(noinline) const std::pair<K, V>* CVectorMap<K, V, Cmp>::LowerBound(const K& key) const   // 0x004365B0: the retail body, out of line (same-TU mod-ref)
{
    long count = Data._M_finish - Data._M_start;
    const value_type* first = Data._M_start;
    while (count > 0) {
        long half = count / 2;
        const value_type* middle = first + half;
        if (middle->first < key) {
            first = middle + 1;
            count = count - half - 1;
        } else
            count = half;
    }
    return first;
}
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                      // +0x20 std::bitset of interface types
    CInterfaceMap Components;                            // +0x44
    __forceinline bool HasTC(long type) const
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
    __forceinline const CTCBase* GetTC(ETCInterfaceType type) const
    {
        const CInterfaceMap* map = &Components;
        const ETCInterfaceType key = type;
        const CInterfaceMap::value_type* it = map->LowerBound(key);
        const CInterfaceMap::value_type* end = map->Data.end();
        return (it == end || type < it->first ? end : it)->second;
    }
};

class CHeroAbilityStatWeight {                           // STLport map<EHeroTrainableStatType, float> (12 bytes)
public:
    void*         _M_header;
    unsigned long _M_node_count;
    unsigned long _M_key_compare;
    float GetParameter(const CTCHeroExperience& experience) const;   // 0x00851BD0
};

class CHeroCombatDef : public CDefPointeeBase {
public:
    unsigned char          _pad1c[0x78 - 0x1c];
    CHeroAbilityStatWeight RangedDamageMultiplierWeighting;               // +0x78
    float                  RangedDamageMultiplierSpeedWithStatParameter;  // +0x84
    float                  RangedDamageMultiplierSpeedWithDistance;       // +0x88
    float                  RangedDamageMultiplierSpeedNominalBuildup;     // +0x8c
    float                  RangedDamageMultiplierScaleNominalBuildup;     // +0x90
    float                  RangedDamageMultiplierSpeedSuperBuildup;       // +0x94
    float                  RangedDamageMultiplierScaleSuperBuildup;       // +0x98
};

namespace {
float __fastcall GetDamageMultiplierFromThingAtBuildUpTime(const CThing& thing, CDefPointer<const CHeroCombatDef> def,
                                                            float buildUpTime)
{
    float statParameter = 1.0f;
    if (thing.HasTC(0x68))
        statParameter = def->RangedDamageMultiplierWeighting.GetParameter(
            *reinterpret_cast<const CTCHeroExperience*>(thing.GetTC((ETCInterfaceType)0x68)));
    float superBuildup = buildUpTime * def->RangedDamageMultiplierSpeedSuperBuildup;
    return (statParameter * def->RangedDamageMultiplierSpeedWithStatParameter + 1.0f)
           * ((float)tanh(buildUpTime * def->RangedDamageMultiplierSpeedNominalBuildup)
                  * def->RangedDamageMultiplierScaleNominalBuildup
              + def->RangedDamageMultiplierScaleSuperBuildup * (superBuildup * superBuildup));
}
}

float (__fastcall* g_keepGetDamageMultiplierFromThingAtBuildUpTime)(const CThing&, CDefPointer<const CHeroCombatDef>, float)
    = GetDamageMultiplierFromThingAtBuildUpTime;
