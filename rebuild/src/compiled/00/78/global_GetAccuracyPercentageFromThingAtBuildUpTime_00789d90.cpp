// _global::GetAccuracyPercentageFromThingAtBuildUpTime(const CThing& thing, CDefPointer<const CHeroCombatDef> def, float buildUpTime) -- the ranged shot's accuracy percentage: the def's stats range at the hero's trainable-stat fraction times its build-up range at the build-up time, capped at 100
// Leaf: ?GetAccuracyPercentageFromThingAtBuildUpTime@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00789D90, 202 bytes (+ int3 padding to 208), `__fastcall` file-local (thing
//   in ecx, the def pointer and the build-up time on the stack, `ret 8`).
// Identity: Ego_r.pdb `?GetAccuracyPercentageFromThingAtBuildUpTime@?A0x0524c104@@YIMABVCThing@@V?$CDefPointer@$$CBVCHeroCombatDef@@@@M@Z`
//   (the CTCHeroRangedAttacks TU); caller 0x0078A051 (landed CancelAimingWeapon 0x00789FF0 declares it).
// Callees: CVectorMap LowerBound const 0x004365B0 (inline GetTC 0x68 = CTCHeroExperience after the inline bitset test of
//   +0x2c bit 8), CTCHeroExperience::GetTrainableStatLevel 0x006D6520 / GetTrainableStatMaxLevel 0x006D6770 (stat 4).
// Constants: 0x0122DED8 = 1.0f, 0x01230A08 = 100.0f (the cap, compared as an immediate operand; the 100.0f the min
//   returns by reference is stored into the dead build-up-time slot).
// Layout: retail CHeroCombatDef = PDB - 4 here (the def base's 12-byte container): RangedAccuracyPercentFractionWithBuildup
//   +0x44, RangedAccuracyPercentWithStats +0x4c; CFloatRange {HighValue +0, LowValue +4}. ~CDefPointer has no null test.
// Shape: `statFraction = 1.0f` stays on the x87 stack (popped on the interface path); the level is spilled as a float
//   across the second call (`fild ; fstp dword`) and divided with `fdivr`; each range is `Low + (High - Low) * t` with Low
//   re-read; the product is a named float whose address `std::min(accuracy, 100.0f)` takes (`lea eax` select, `mov edx,[eax]`)
//   and the result is parked in the dead def slot while the by-value CDefPointer releases its count (`dec ; jne ;
//   call [vtbl+4]`); the function pointer below only keeps the file-local function emitted.

class CTCBase;
enum EHeroTrainableStatType { };

class CTCHeroExperience {
public:
    long GetTrainableStatLevel(EHeroTrainableStatType stat) const;     // 0x006D6520
    long GetTrainableStatMaxLevel(EHeroTrainableStatType stat) const;  // 0x006D6770
};

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

class CFloatRange { public:
    float HighValue;                                     // +0x00
    float LowValue;                                      // +0x04
};

class CHeroCombatDef : public CDefPointeeBase {
public:
    unsigned char _pad08[0x3c - 0x08];
    CFloatRange   RangedAccuracySecondsForMaximumWithSkill;   // +0x3c
    CFloatRange   RangedAccuracyPercentFractionWithBuildup;   // +0x44
    CFloatRange   RangedAccuracyPercentWithStats;             // +0x4c
};

template <class T> class CDefPointer {
public:
    T* Object;
    CDefPointer(const CDefPointer& other) : Object(other.Object) { if (Object) ++Object->RefCount; }
    ~CDefPointer() { if (--Object->RefCount == 0) Object->Destroy(); }
    T* operator->() const { return Object; }
};

namespace std {
template <class _Tp> inline const _Tp& (min)(const _Tp& __a, const _Tp& __b) { return __b < __a ? __b : __a; }
}

namespace {
float __fastcall GetAccuracyPercentageFromThingAtBuildUpTime(const CThing& thing, CDefPointer<const CHeroCombatDef> def,
                                                              float buildUpTime)
{
    float statFraction = 1.0f;
    if (thing.HasTC(0x68)) {
        const CTCHeroExperience* experience = reinterpret_cast<const CTCHeroExperience*>(thing.GetTC(0x68));
        statFraction = (float)experience->GetTrainableStatLevel((EHeroTrainableStatType)4)
                       / (float)experience->GetTrainableStatMaxLevel((EHeroTrainableStatType)4);
    }
    float accuracy = (def->RangedAccuracyPercentWithStats.LowValue
                      + (def->RangedAccuracyPercentWithStats.HighValue - def->RangedAccuracyPercentWithStats.LowValue)
                            * statFraction)
                     * (def->RangedAccuracyPercentFractionWithBuildup.LowValue
                        + (def->RangedAccuracyPercentFractionWithBuildup.HighValue
                           - def->RangedAccuracyPercentFractionWithBuildup.LowValue) * buildUpTime);
    return std::min(accuracy, 100.0f);
}
}

float (__fastcall* g_keepGetAccuracyPercentageFromThingAtBuildUpTime)(const CThing&, CDefPointer<const CHeroCombatDef>, float)
    = GetAccuracyPercentageFromThingAtBuildUpTime;