// Behaviour test for 0x006D8790 CTCHeroExperience::AddSuccessfulStrikeOfType(EDamageAttribute type, float damage, bool increase).
#include <stdio.h>
#include <math.h>
#include <map>
enum EDamageAttribute { DAMAGE_A = 0, DAMAGE_B = 1, DAMAGE_C = 2 };
class CComboMultiplierData { public: float ExperienceMultiplierNumSecondsForAttenuation; std::map<EDamageAttribute, float> ComboMultiplier; };
class CTCHeroExperience { public:
    CComboMultiplierData Data; long ExperienceAttenuationTimer; float ExperienceMultiplierParam; bool MultiStrike;
    int HistoryAdds, ComboUpdates; bool LastIncrease;
    void AddAttackOfTypeToAttackHistory(EDamageAttribute) { ++HistoryAdds; }
    const CComboMultiplierData& GetActiveComboMultiplierData() const { return Data; }
    float GetAttackVariationMultiplier() const { return 2.0f; }
    void UpdateComboMultiplier(bool increase) { ++ComboUpdates; LastIncrease = increase; }
    static float GetConstantFPS() { return 10.0f; }
    void AddSuccessfulStrikeOfType(EDamageAttribute type, float damage, bool increase)
    {
        AddAttackOfTypeToAttackHistory(type);
        const CComboMultiplierData& data = GetActiveComboMultiplierData();
        ExperienceAttenuationTimer = (long)(GetConstantFPS() * data.ExperienceMultiplierNumSecondsForAttenuation);
        if (!MultiStrike) {
            std::map<EDamageAttribute, float>::const_iterator found = data.ComboMultiplier.find(type);
            if (found != data.ComboMultiplier.end()) {
                float strength = (float)fabs(damage);
                ExperienceMultiplierParam += GetAttackVariationMultiplier() * strength * found->second;
                UpdateComboMultiplier(increase);
            }
        }
    }
};
int main()
{
    CTCHeroExperience e; e.Data.ExperienceMultiplierNumSecondsForAttenuation = 3.0f; e.Data.ComboMultiplier[DAMAGE_B] = 0.5f;
    e.ExperienceAttenuationTimer = 0; e.ExperienceMultiplierParam = 1.0f; e.MultiStrike = false; e.HistoryAdds = 0; e.ComboUpdates = 0; e.LastIncrease = false;
    e.AddSuccessfulStrikeOfType(DAMAGE_B, -4.0f, true);
    int ok = e.HistoryAdds == 1 && e.ExperienceAttenuationTimer == 30 && fabs(e.ExperienceMultiplierParam - 5.0f) < 1e-4 && e.ComboUpdates == 1 && e.LastIncrease;
    e.AddSuccessfulStrikeOfType(DAMAGE_C, 4.0f, false);
    ok = ok && e.HistoryAdds == 2 && e.ComboUpdates == 1 && fabs(e.ExperienceMultiplierParam - 5.0f) < 1e-4;
    e.MultiStrike = true;
    e.AddSuccessfulStrikeOfType(DAMAGE_B, 4.0f, false);
    ok = ok && e.HistoryAdds == 3 && e.ComboUpdates == 1;
    printf(ok ? "006d8790_TEST PASS\n" : "006d8790_TEST FAIL\n");
    return ok ? 0 : 1;
}