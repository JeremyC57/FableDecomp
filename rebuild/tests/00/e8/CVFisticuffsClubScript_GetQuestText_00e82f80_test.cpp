// Behaviour test for CV_FisticuffsClubScript::GetQuestText (0x00E82F80): region -> TEXT_QST_<n>_<suffix>.
#include <cstdio>
#include <cstring>
class CCharString {
public:
    char m_text[64];
    CCharString(const char* text) { std::strcpy(m_text, text); }
    CCharString(const CCharString& other) { std::strcpy(m_text, other.m_text); }
};
CCharString operator+(const CCharString& left, const char* right)
{
    CCharString r(left);
    std::strcat(r.m_text, right);
    return r;
}
CCharString operator+(const CCharString& left, const CCharString& right)
{
    CCharString r(left);
    std::strcat(r.m_text, right.m_text);
    return r;
}
struct CV_FisticuffsClubScript {
    long RelevantRegion;
    CCharString GetQuestText(CCharString suffix)
    {
        CCharString text = (const CCharString&)"Should not happen";
        switch (RelevantRegion) {
        case 1: return CCharString("TEXT_QST_") + "008" + CCharString("_") + suffix;
        case 2: return CCharString("TEXT_QST_") + "079" + CCharString("_") + suffix;
        case 3: return CCharString("TEXT_QST_") + "009" + CCharString("_") + suffix;
        case 4: return CCharString("TEXT_QST_") + "040" + CCharString("_") + suffix;
        default: return CCharString("TEXT_QST_") + "040" + CCharString("_") + suffix;
        }
    }
};
static bool Check(long region, const char* suffix, const char* expect)
{
    CV_FisticuffsClubScript s;
    s.RelevantRegion = region;
    CCharString r = s.GetQuestText(CCharString(suffix));
    return std::strcmp(r.m_text, expect) == 0;
}
int main()
{
    bool ok = Check(1, "INTRO", "TEXT_QST_008_INTRO") && Check(2, "WIN", "TEXT_QST_079_WIN")
        && Check(3, "LOSE", "TEXT_QST_009_LOSE") && Check(4, "X", "TEXT_QST_040_X")
        && Check(0, "D", "TEXT_QST_040_D") && Check(9, "", "TEXT_QST_040_");
    if (!ok) { std::printf("00e82f80_TEST FAIL\n"); return 1; }
    std::printf("00e82f80_TEST PASS\n");
    return 0;
}