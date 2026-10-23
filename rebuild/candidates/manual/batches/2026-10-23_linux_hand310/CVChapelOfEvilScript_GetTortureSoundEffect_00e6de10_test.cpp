// Behaviour test for CV_ChapelOfEvilScript::GetTortureSoundEffect (0x00E6DE10).
#include <cstdio>
#include <cstring>
enum ESex { SEX_NONE = 0, SEX_MALE = 1, SEX_FEMALE = 2 };
struct CCharString {
    const char* m_pData;
    CCharString& operator=(const char* text) { m_pData = text; return *this; }
};
struct CV_ChapelOfEvilScript {
    int terminateAfter;                // IsActiveThreadTerminating turns true after this many calls
    int checks;
    CCharString TortureSound;
    long TortureType;
    bool IsActiveThreadTerminating() { return checks++ >= terminateAfter; }
    void GetTortureSoundEffect(ESex sex)
    {
#define ARM(N, NAME) \
        if (TortureType == N) { \
            if (IsActiveThreadTerminating()) return; \
            if (sex == SEX_FEMALE) { if (IsActiveThreadTerminating()) return; TortureSound = "CHAPEL_OF_EVIL_" NAME "_FEMALE"; } \
            else { if (IsActiveThreadTerminating()) return; TortureSound = "CHAPEL_OF_EVIL_" NAME "_MALE"; } \
        }
        ARM(1, "TUG_OF_WAR") else ARM(2, "PESTILENCE") else ARM(3, "INFERNO") else ARM(4, "BONING_MACHINE")
        else ARM(5, "ACID_BATH") else ARM(6, "EVISCERATER")
#undef ARM
    }
};
static bool Check(long type, ESex sex, int terminateAfter, const char* expect)
{
    CV_ChapelOfEvilScript s;
    s.terminateAfter = terminateAfter;
    s.checks = 0;
    s.TortureType = type;
    s.TortureSound.m_pData = "unchanged";
    s.GetTortureSoundEffect(sex);
    return std::strcmp(s.TortureSound.m_pData, expect) == 0;
}
int main()
{
    bool ok = Check(1, SEX_FEMALE, 100, "CHAPEL_OF_EVIL_TUG_OF_WAR_FEMALE")
        && Check(1, SEX_MALE, 100, "CHAPEL_OF_EVIL_TUG_OF_WAR_MALE")
        && Check(4, SEX_NONE, 100, "CHAPEL_OF_EVIL_BONING_MACHINE_MALE")
        && Check(6, SEX_FEMALE, 100, "CHAPEL_OF_EVIL_EVISCERATER_FEMALE")
        && Check(5, SEX_MALE, 100, "CHAPEL_OF_EVIL_ACID_BATH_MALE")
        && Check(7, SEX_MALE, 100, "unchanged")
        && Check(3, SEX_FEMALE, 0, "unchanged")
        && Check(3, SEX_FEMALE, 1, "unchanged")
        && Check(3, SEX_FEMALE, 2, "CHAPEL_OF_EVIL_INFERNO_FEMALE");
    if (!ok) { std::printf("00e6de10_TEST FAIL\n"); return 1; }
    std::printf("00e6de10_TEST PASS\n");
    return 0;
}
