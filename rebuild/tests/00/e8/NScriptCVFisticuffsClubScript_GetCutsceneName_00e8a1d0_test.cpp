// Behaviour test for 0x00E8A1D0 CV_FisticuffsClubScript::GetCutsceneName: region 1..4 prefixes, error default.
#include <cstdio>
#include <cstring>
class CCharString { public:
    char text[64];
    CCharString(const char* t, long length = -1) { std::strcpy(text, t); (void)length; }
    CCharString(const CCharString& o) { std::strcpy(text, o.text); }
    ~CCharString() { }
};
static CCharString operator+(const char* left, const CCharString& right)
{
    CCharString r(left); std::strcat(r.text, right.text); return r;
}
static inline CCharString ErrorCutsceneName() { CCharString name("An error", -1); return name; }
struct CV_FisticuffsClubScript {
    long RelevantRegion;
    CCharString GetCutsceneName(CCharString suffix)
    {
        CCharString cutscene = ErrorCutsceneName();
        switch (RelevantRegion)
        {
        case 1: return "CS_PUNCHCLUB_BS_" + suffix;
        case 4: return "CS_PUNCHCLUB_KG_" + suffix;
        case 2: return "CS_PUNCHCLUB_OV_" + suffix;
        case 3: return "CS_PUNCHCLUB_BC_" + suffix;
        }
        return CCharString("An error", -1);
    }
};
int main()
{
    /* retail jump table 0x00E8A298 (regions 1..4) = { 0x00E8A20F "BS" 0x012EB708, 0x00E8A23D "OV" 0x012EB6E0,
       0x00E8A254 "BC" 0x012EB6CC, 0x00E8A226 "KG" 0x012EB6F4 } */
    static const char* expect[6] = { "An error", "CS_PUNCHCLUB_BS_WIN", "CS_PUNCHCLUB_OV_WIN", "CS_PUNCHCLUB_BC_WIN",
                                     "CS_PUNCHCLUB_KG_WIN", "An error" };
    CV_FisticuffsClubScript s;
    for (long r = 0; r < 6; ++r) {
        s.RelevantRegion = r;
        CCharString got = s.GetCutsceneName(CCharString("WIN"));
        if (std::strcmp(got.text, expect[r]) != 0) { std::printf("00e8a1d0_TEST FAIL\n"); return 1; }
    }
    std::printf("00e8a1d0_TEST PASS\n");
    return 0;
}