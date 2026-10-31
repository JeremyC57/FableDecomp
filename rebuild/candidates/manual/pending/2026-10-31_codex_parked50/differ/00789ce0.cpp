// 00789ce0 GetDamageMultiplierFromThingAtBuildUpTime
// Codex parked-50 best attempt: DIFFER(175v175); residue: 175/175 bytes; corrected typed model preserves ESI over a larger region than retail; conditional save/restore and lookup scheduling differ.
// Flags:
// Leaf: ?GetDamageMultiplierFromThingAtBuildUpTime@
#include <math.h>

#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CDefPointer.h"
#include "engine/CParentDefClassBase.h"
#define CHeroCombatDef CHeroCombatDefFields
#include "engine/_quarantine/CHeroCombatDef.h"
#undef CHeroCombatDef
#define CClass CClassFields
#include "engine/CClass.h"
#undef CClass
#define CThing CThingFields
#define CTCGraphicAppearance CTCGraphicAppearanceFields
#include "engine/_quarantine/CThing.h"
#undef CThing
#undef CTCGraphicAppearance
#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CBaseObject.h"
class CTCHeroExperience;
enum ETCInterfaceType {};
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
private:
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

#pragma pack(push,1)
class CClass:public CBaseObject {unsigned char unreadClass[sizeof(CClassFields)-sizeof(CBaseObject)];};
// Layout: actual class hierarchy and bitset32/components68; shared native extent98.
class CThing:public CClass {
 unsigned char unreadThingPrefix[0x20-sizeof(CClass)];
 unsigned long UsingComponents[9]; // opaque bitset274 storage, no template methods used
 CInterfaceMap Components;
 unsigned char unreadThingTail[sizeof(CThingFields)-0x54];
public:
 __forceinline bool HasTC(ETCInterfaceType type)const {
  return (UsingComponents[(unsigned)type>>5]&(1UL<<((unsigned)type&31)))!=0;
 }
 __forceinline const CTCBase& GetTC(ETCInterfaceType type)const {
  const ETCInterfaceType key=type;
  const CInterfaceMap::value_type*it=Components.LowerBound(key);
  const CInterfaceMap::value_type*end=Components.Data.end();
  return *(it==end || type<it->first ? end : it)->second;
 }
};
#pragma pack(pop)
class CHeroAbilityStatWeight {                           // STLport map<EHeroTrainableStatType, float> (12 bytes)
public:
    unsigned char StatWeights[12]; // unread original map<EHeroTrainableStatType,float>
    float GetParameter(const CTCHeroExperience& experience) const;   // 0x00851BD0
};

#pragma pack(push,1)
class CHeroCombatDef : public CParentDefClassBase {
public:
    unsigned char          unreadCombatPrefix[0x78 - sizeof(CParentDefClassBase)];
    CHeroAbilityStatWeight RangedDamageMultiplierWeighting;               // +0x78
    float                  RangedDamageMultiplierSpeedWithStatParameter;  // +0x84
    float                  RangedDamageMultiplierSpeedWithDistance;       // +0x88
    float                  RangedDamageMultiplierSpeedNominalBuildup;     // +0x8c
    float                  RangedDamageMultiplierScaleNominalBuildup;     // +0x90
    float                  RangedDamageMultiplierSpeedSuperBuildup;       // +0x94
    float                  RangedDamageMultiplierScaleSuperBuildup;       // +0x98
};

#pragma pack(pop)
namespace {
float __fastcall GetDamageMultiplierFromThingAtBuildUpTime(const CThing& thing, CDefPointer<const CHeroCombatDef> def,
                                                            float buildUpTime)
{
    float statParameter = 1.0f;
    if (thing.HasTC((ETCInterfaceType)0x68))
        statParameter = def->RangedDamageMultiplierWeighting.GetParameter(
            reinterpret_cast<const CTCHeroExperience&>(thing.GetTC((ETCInterfaceType)0x68)));
    float superBuildup = buildUpTime * def->RangedDamageMultiplierSpeedSuperBuildup;
    buildUpTime = (statParameter * def->RangedDamageMultiplierSpeedWithStatParameter + 1.0f)
           * ((float)tanh(buildUpTime * def->RangedDamageMultiplierSpeedNominalBuildup)
                  * def->RangedDamageMultiplierScaleNominalBuildup
              + def->RangedDamageMultiplierScaleSuperBuildup * (superBuildup * superBuildup));
    return buildUpTime;
}
}

float (__fastcall* parked50_keep_00789ce0)(const CThing&, CDefPointer<const CHeroCombatDef>, float)
    = GetDamageMultiplierFromThingAtBuildUpTime;
