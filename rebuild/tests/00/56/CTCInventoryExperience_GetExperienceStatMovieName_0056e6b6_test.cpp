// Behaviour test for retail 0x0056e6b6: the candidate source below is embedded verbatim (from src/0056e6b6.cpp),
// followed by dependency stubs/spies and a main that asserts on the observable effects.
// ===== candidate source =====
// CTCInventoryExperience::GetExperienceStatMovieName(EHeroTrainableStatType stat) -- selects the stat spending movie path
// Retail: Fable.exe (TLC, VC7.1) @ 0x0056E6B6, 169 bytes. CCharString callees: default ctor 0x0099E4B0, operator=(const char*)
// 0x0099EFE0, copy ctor 0x0099EC30, dtor 0x0099EAE0 (manifest label there is a propagated _Cons_val twin). The default CCharString is repeatedly assigned for each stat id.
// The independent if statements preserve the retail fall-through checks and final copy construction.
#pragma optimize("y", off)
#pragma optimize("s", on)
struct CCharStringData;
struct CCharString {
    CCharStringData* PStringData;
    CCharString();                                      // 0x0099E4B0
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
    CCharString& operator=(const char* text);           // 0x0099EFE0
};
enum EHeroTrainableStatType { EHeroTrainableStatType_Force32 = 0x7fffffff };
struct CTCInventoryExperience {
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
// ===== harness =====
#include <stdio.h>
#include <string.h>
static int g_live, g_assign;
CCharString::CCharString() : PStringData(0) { ++g_live; }
CCharString::CCharString(const CCharString& o) : PStringData(o.PStringData) { ++g_live; }
CCharString::~CCharString() { --g_live; }
CCharString& CCharString::operator=(const char* text) { ++g_assign; PStringData = (CCharStringData*)text; return *this; }
static int check(long stat, const char* expect) {
    CTCInventoryExperience e; g_assign = 0;
    int ok;
    {
        CCharString r = e.GetExperienceStatMovieName((EHeroTrainableStatType)stat);
        const char* got = (const char*)r.PStringData;
        ok = expect ? (got && !strcmp(got, expect) && g_assign == 1) : (!got && g_assign == 0);
    }
    return ok && g_live == 0;
}
int main() {
    int ok = check(0, "Data\\Video\\exp_spend_strength.wmv") && check(1, "Data\\Video\\exp_spend_health.wmv")
          && check(2, "Data\\Video\\exp_spend_tough.wmv") && check(3, "Data\\Video\\exp_spend_speed.wmv")
          && check(4, "Data\\Video\\exp_spend_decapitation.wmv") && check(5, "Data\\Video\\exp_spend_stealth.wmv")
          && check(9, "Data\\Video\\exp_spend_will.wmv") && check(6, 0) && check(-1, 0);
    printf(ok ? "0056e6b6_TEST PASS\n" : "0056e6b6_TEST FAIL\n"); return ok ? 0 : 1;
}