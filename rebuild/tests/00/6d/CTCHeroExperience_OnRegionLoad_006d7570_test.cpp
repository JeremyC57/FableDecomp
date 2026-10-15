#include <cstdio>

// Model of CTCHeroExperience::OnRegionLoad @ 0x006D7570: only with a combat def -- param *= region-change multiplier,
// timer = (long)(fps * seconds-for-attenuation of the active combo data).
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
static long g_FramesPerSecond = 30;
struct CHeroCombatDef : CDefPointeeBase { };
struct CComboMultiplierData { float ExperienceMultiplierNumSecondsForAttenuation; };
struct CHeroExperienceDef { float ExperienceMultiplierRegionChangeMultiplier; CComboMultiplierData Active; };
struct CTCHeroExperience {
    float ExperienceMultiplierParam; long ExperienceAttenuationTimer; CHeroExperienceDef* PDef; CDefPointer<CHeroCombatDef> PHeroCombatDef;
    CTCHeroExperience(CHeroCombatDef* d) : PHeroCombatDef(d) { }
    CDefPointer<CHeroCombatDef> GetPHeroCombatDef() const { return PHeroCombatDef; }
    const CComboMultiplierData& GetActiveComboMultiplierData() const { return PDef->Active; }
    void OnRegionLoad()
    {
        if (GetPHeroCombatDef().IsValid()) {
            ExperienceMultiplierParam *= PDef->ExperienceMultiplierRegionChangeMultiplier;
            ExperienceAttenuationTimer = (long)(g_FramesPerSecond * GetActiveComboMultiplierData().ExperienceMultiplierNumSecondsForAttenuation);
        }
    }
};

int main()
{
    CHeroExperienceDef xdef; xdef.ExperienceMultiplierRegionChangeMultiplier = 0.5f; xdef.Active.ExperienceMultiplierNumSecondsForAttenuation = 2.5f;
    CHeroCombatDef* combat = new CHeroCombatDef; combat->m_count = 1;
    {
        CTCHeroExperience e(combat); e.PDef = &xdef; e.ExperienceMultiplierParam = 8.0f; e.ExperienceAttenuationTimer = 0;
        e.OnRegionLoad();
        if (e.ExperienceMultiplierParam != 4.0f || e.ExperienceAttenuationTimer != 75 || combat->m_count != 1 || g_destroyed != 0)
            { std::printf("006d7570_TEST FAIL\n"); return 1; }
        e.PHeroCombatDef.Object = 0;
        e.OnRegionLoad();                             // no combat def: nothing changes
        if (e.ExperienceMultiplierParam != 4.0f || e.ExperienceAttenuationTimer != 75) { std::printf("006d7570_TEST FAIL\n"); return 1; }
    }
    delete combat;
    std::printf("006d7570_TEST PASS\n");
    return 0;
}