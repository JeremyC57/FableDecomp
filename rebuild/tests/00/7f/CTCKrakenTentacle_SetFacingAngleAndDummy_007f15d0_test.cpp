#include <cstdio>
#include <cstring>
struct CCharString {
    char m_text[32];
    CCharString(const char* t) { std::strcpy(m_text, t); }
    CCharString& operator=(const CCharString& o) { std::strcpy(m_text, o.m_text); return *this; }
};
struct CTCKrakenTentacle {
    float FacingAngle; CCharString DummyName; int returns; bool lastImmediate;
    CTCKrakenTentacle() : FacingAngle(0), DummyName(""), returns(0), lastImmediate(false) { }
    void ReturnToDesiredPos(bool immediately) { ++returns; lastImmediate = immediately; }
    void SetFacingAngleAndDummy(float a, const CCharString& d) { FacingAngle = a; DummyName = d; ReturnToDesiredPos(true); }
};
int main()
{
    CTCKrakenTentacle t; CCharString name("DUMMY_TENTACLE_3");
    t.SetFacingAngleAndDummy(1.25f, name);
    if (t.FacingAngle != 1.25f || std::strcmp(t.DummyName.m_text, "DUMMY_TENTACLE_3") != 0 || t.returns != 1 || !t.lastImmediate) { std::printf("007f15d0_TEST FAIL\n"); return 1; }
    std::printf("007f15d0_TEST PASS\n");
    return 0;
}