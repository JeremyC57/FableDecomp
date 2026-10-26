#include <cstdio>
#include <cstring>
class CCharString { public: char m_text[16]; CCharString(const char* t, long) { std::strcpy(m_text, t); } };
struct CTCMeleeTrap {
    long m_trapType; bool PlayingContinuous; bool StrikeAnimation; int type3; char lastAnim[16]; int anims; bool lastLoop;
    void StartType3Trap() { ++type3; }
    void InitialiseAnimation(const CCharString& n, bool loop) { std::strcpy(lastAnim, n.m_text); ++anims; lastLoop = loop; }
    void InitTrap()
    {
        PlayingContinuous = false; StrikeAnimation = false;
        if (m_trapType == 3) StartType3Trap();
        else { CCharString animationName("DEFAULT", -1); InitialiseAnimation(animationName, false); }
    }
};
int main()
{
    CTCMeleeTrap t; t.m_trapType = 3; t.PlayingContinuous = true; t.StrikeAnimation = true; t.type3 = 0; t.anims = 0; t.lastLoop = true; t.lastAnim[0] = 0;
    t.InitTrap();
    if (t.PlayingContinuous || t.StrikeAnimation || t.type3 != 1 || t.anims != 0) { std::printf("007883c3_TEST FAIL\n"); return 1; }
    t.m_trapType = 1; t.InitTrap();
    if (t.type3 != 1 || t.anims != 1 || std::strcmp(t.lastAnim, "DEFAULT") != 0 || t.lastLoop) { std::printf("007883c3_TEST FAIL\n"); return 1; }
    std::printf("007883c3_TEST PASS\n");
    return 0;
}