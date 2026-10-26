// Grade: RELOCATION_MATCH (Fable landing 2026-10-19 after the Codex review hold: test exercises a hand-written stand-in, not the candidate)
// CTCInventoryExperience::GetExperienceStatMovieName(EHeroTrainableStatType stat) -- selects the stat spending movie path
// Retail: Fable.exe (TLC, VC7.1) @ 0x0056E6B6, 169 bytes. CCharString callees: default ctor 0x0099E4B0, operator=(const char*)
// 0x0099EFE0, copy ctor 0x0099EC30, dtor 0x0099EAE0 (manifest label there is a propagated _Cons_val twin). The default CCharString is repeatedly assigned for each stat id.
// The independent if statements preserve the retail fall-through checks and final copy construction.
#pragma optimize("y", off)
#pragma optimize("s", on)
class CCharStringData;
class CCharString { public:
    CCharStringData* PStringData;
    CCharString();                                      // 0x0099E4B0
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
    CCharString& operator=(const char* text);           // 0x0099EFE0
};
enum EHeroTrainableStatType { EHeroTrainableStatType_Force32 = 0x7fffffff };
class CTCInventoryExperience { public:
    CCharString GetExperienceStatMovieName(EHeroTrainableStatType stat);
};
CCharString CTCInventoryExperience::GetExperienceStatMovieName(EHeroTrainableStatType stat)
{
    CCharString result;
    if (stat == 1) result = "Data\\Video\\exp_spend_health.wmv";
    if (stat == 0) result = "Data\\Video\\exp_spend_strength.wmv";
    if (stat == 2) result = "Data\\Video\\exp_spend_tough.wmv";
    if (stat == 3) result = "Data\\Video\\exp_spend_speed.wmv";
    if (stat == 5) result = "Data\\Video\\exp_spend_stealth.wmv";
    if (stat == 4) result = "Data\\Video\\exp_spend_decapitation.wmv";
    if (stat == 9) result = "Data\\Video\\exp_spend_will.wmv";
    return result;
}