#include <cstdio>

// Model of CTCHeroExperience::GetShotAccuracyPercentage @ 0x006D6EA0: (long)((High - Low) * t + Low) with t the
// stat weighting's parameter, through a counted copy of the combat def (count restored afterwards).
static int g_destroyed = 0;
struct CDefPointeeBase { long m_count; virtual void Destroy() { ++g_destroyed; } virtual ~CDefPointeeBase() { } };
template<class T> struct CDefPointer {
    T* Object;
    CDefPointer(const CDefPointer& o) : Object(o.Object) { if (Object) ++Object->m_count; }
    explicit CDefPointer(T* p) : Object(p) { }
    ~CDefPointer() { if (Object) if (--Object->m_count == 0) Object->Destroy(); }
    T* operator->() const { return Object; }
    bool IsValid() const { return Object != 0; }
};
struct CTCHeroExperience;
struct CHeroAbilityStatWeight { float Value; float GetParameter(const CTCHeroExperience&) const { return Value; } };
struct CFloatRange { float HighValue; float LowValue; };
struct CHeroCombatDef : CDefPointeeBase { CFloatRange RangedAccuracyPercentWithStats; CHeroAbilityStatWeight RangedAccuracyWeighting; };
struct CTCHeroExperience {
    CDefPointer<CHeroCombatDef> PHeroCombatDef;
    CTCHeroExperience(CHeroCombatDef* d) : PHeroCombatDef(d) { }
    long GetShotAccuracyPercentage() const
    {
        CDefPointer<CHeroCombatDef> combat = PHeroCombatDef;
        float t = combat->RangedAccuracyWeighting.GetParameter(*this);
        return (long)((combat->RangedAccuracyPercentWithStats.HighValue - combat->RangedAccuracyPercentWithStats.LowValue) * t
                      + combat->RangedAccuracyPercentWithStats.LowValue);
    }
};

int main()
{
    CHeroCombatDef* def = new CHeroCombatDef; def->m_count = 2;
    def->RangedAccuracyPercentWithStats.HighValue = 90.0f; def->RangedAccuracyPercentWithStats.LowValue = 40.0f;
    def->RangedAccuracyWeighting.Value = 0.5f;
    {
        CTCHeroExperience e(def);
        long v = e.GetShotAccuracyPercentage();
        if (v != 65 || def->m_count != 2 || g_destroyed != 0) { std::printf("006d6ea0_TEST FAIL\n"); return 1; }
        def->RangedAccuracyWeighting.Value = 0.99f;
        if (e.GetShotAccuracyPercentage() != 89) { std::printf("006d6ea0_TEST FAIL\n"); return 1; }   // truncation
        e.PHeroCombatDef.Object = 0;
    }
    delete def;
    std::printf("006d6ea0_TEST PASS\n");
    return 0;
}