#include <cstdio>
#ifndef _MSC_VER
#define __fastcall
#endif

// Model: accuracy = stats range at level/max times build-up range at the build-up time, capped at 100; the by-value
// def pointer releases its reference on exit.
static int g_destroyed = 0;

class CTCHeroExperience {
public:
    long Level, MaxLevel;
    long GetTrainableStatLevel(int stat) const { return stat == 4 ? Level : -1; }
    long GetTrainableStatMaxLevel(int stat) const { return stat == 4 ? MaxLevel : -1; }
};

class CThing {
public:
    CTCHeroExperience* Experience;                      // null: no CTCHeroExperience interface
    bool HasTC(long type) const { return type == 0x68 && Experience != 0; }
    const CTCHeroExperience* GetTC(long) const { return Experience; }
};

class CFloatRange { public: float HighValue, LowValue; };

class CHeroCombatDef {
public:
    mutable long RefCount;
    CFloatRange  RangedAccuracyPercentFractionWithBuildup;
    CFloatRange  RangedAccuracyPercentWithStats;
    void Destroy() const { ++g_destroyed; }
};

template <class T> class CDefPointer {
public:
    T* Object;
    explicit CDefPointer(T* p) : Object(p) { ++Object->RefCount; }
    CDefPointer(const CDefPointer& other) : Object(other.Object) { if (Object) ++Object->RefCount; }
    ~CDefPointer() { if (--Object->RefCount == 0) Object->Destroy(); }
    T* operator->() const { return Object; }
};

template <class _Tp> inline const _Tp& Min(const _Tp& __a, const _Tp& __b) { return __b < __a ? __b : __a; }

static float __fastcall GetAccuracyPercentageFromThingAtBuildUpTime(const CThing& thing, CDefPointer<const CHeroCombatDef> def,
                                                                     float buildUpTime)
{
    float statFraction = 1.0f;
    if (thing.HasTC(0x68)) {
        const CTCHeroExperience* experience = thing.GetTC(0x68);
        statFraction = (float)experience->GetTrainableStatLevel(4) / (float)experience->GetTrainableStatMaxLevel(4);
    }
    float accuracy = (def->RangedAccuracyPercentWithStats.LowValue
                      + (def->RangedAccuracyPercentWithStats.HighValue - def->RangedAccuracyPercentWithStats.LowValue)
                            * statFraction)
                     * (def->RangedAccuracyPercentFractionWithBuildup.LowValue
                        + (def->RangedAccuracyPercentFractionWithBuildup.HighValue
                           - def->RangedAccuracyPercentFractionWithBuildup.LowValue) * buildUpTime);
    return Min(accuracy, 100.0f);
}

int main()
{
    CHeroCombatDef d;
    d.RefCount = 0;
    d.RangedAccuracyPercentWithStats.HighValue = 100.0f;
    d.RangedAccuracyPercentWithStats.LowValue = 50.0f;
    d.RangedAccuracyPercentFractionWithBuildup.HighValue = 1.0f;
    d.RangedAccuracyPercentFractionWithBuildup.LowValue = 0.5f;
    bool ok = true;
    {
        CDefPointer<const CHeroCombatDef> def(&d);
        CTCHeroExperience exp;
        exp.Level = 1;
        exp.MaxLevel = 2;
        CThing hero, other;
        hero.Experience = &exp;
        other.Experience = 0;
        ok = ok && GetAccuracyPercentageFromThingAtBuildUpTime(hero, def, 0.0f) == 75.0f * 0.5f;   // half the stats range
        ok = ok && GetAccuracyPercentageFromThingAtBuildUpTime(hero, def, 1.0f) == 75.0f;
        ok = ok && GetAccuracyPercentageFromThingAtBuildUpTime(other, def, 1.0f) == 100.0f;        // no stats: full range
        ok = ok && GetAccuracyPercentageFromThingAtBuildUpTime(other, def, 3.0f) == 100.0f;        // capped (200 -> 100)
        ok = ok && d.RefCount == 1 && g_destroyed == 0;                                            // copies released
    }
    ok = ok && d.RefCount == 0 && g_destroyed == 1;
    if (!ok) {
        std::printf("00789d90_TEST FAIL\n");
        return 1;
    }
    std::printf("00789d90_TEST PASS\n");
    return 0;
}