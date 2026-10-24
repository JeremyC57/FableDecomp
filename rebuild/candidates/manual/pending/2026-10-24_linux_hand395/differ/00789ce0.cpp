// _global::GetDamageMultiplierFromThingAtBuildUpTime(const CThing& thing, CDefPointer<const CHeroCombatDef> def, float buildUpTime) -- the ranged-attack damage multiplier after building up a shot: (1 + hero-stat parameter * k) * (tanh-shaped nominal build-up + quadratic super build-up)
// Grade: DIFFER(175v175) -- scheduling only: the inlined ~CDefPointer refcount `mov eax,[edi+4] / dec / mov [edi+4],eax`
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

class CTCBase;
class CTCHeroExperience;

struct CInterfaceMap {                                   // CVectorMap<ETCInterfaceType, CTCBase*> {first, last}
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) const;          // 0x004365B0
};

class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                      // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                          // +0x44
    __forceinline bool HasTC(long type) const
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
    __forceinline CTCBase* GetTC(long type) const
    {
        long key = type;
        const CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&key);
        CInterfaceMap::SEntry* end = map->last;
        return (it == end || type < it->type ? end : it)->component;
    }
};

class CDefPointeeBase {
public:
    virtual void v00();
    virtual void Destroy() const;                        // slot 1
    mutable long RefCount;                               // +0x04
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
    unsigned char          _pad08[0x78 - 0x08];
    CHeroAbilityStatWeight RangedDamageMultiplierWeighting;               // +0x78
    float                  RangedDamageMultiplierSpeedWithStatParameter;  // +0x84
    float                  RangedDamageMultiplierSpeedWithDistance;       // +0x88
    float                  RangedDamageMultiplierSpeedNominalBuildup;     // +0x8c
    float                  RangedDamageMultiplierScaleNominalBuildup;     // +0x90
    float                  RangedDamageMultiplierSpeedSuperBuildup;       // +0x94
    float                  RangedDamageMultiplierScaleSuperBuildup;       // +0x98
};

template <class T> class CDefPointer {
public:
    T* Object;
    CDefPointer(const CDefPointer& other) : Object(other.Object) { if (Object) ++Object->RefCount; }
    ~CDefPointer() { if (--Object->RefCount == 0) Object->Destroy(); }
    T* operator->() const { return Object; }
};

namespace {
float __fastcall GetDamageMultiplierFromThingAtBuildUpTime(const CThing& thing, CDefPointer<const CHeroCombatDef> def,
                                                            float buildUpTime)
{
    float statParameter = 1.0f;
    if (thing.HasTC(0x68))
        statParameter = def->RangedDamageMultiplierWeighting.GetParameter(
            *reinterpret_cast<const CTCHeroExperience*>(thing.GetTC(0x68)));
    float superBuildup = buildUpTime * def->RangedDamageMultiplierSpeedSuperBuildup;
    return (statParameter * def->RangedDamageMultiplierSpeedWithStatParameter + 1.0f)
           * ((float)tanh(buildUpTime * def->RangedDamageMultiplierSpeedNominalBuildup)
                  * def->RangedDamageMultiplierScaleNominalBuildup
              + def->RangedDamageMultiplierScaleSuperBuildup * (superBuildup * superBuildup));
}
}

float (__fastcall* g_keepGetDamageMultiplierFromThingAtBuildUpTime)(const CThing&, CDefPointer<const CHeroCombatDef>, float)
    = GetDamageMultiplierFromThingAtBuildUpTime;
