#include <cstdio>
#include <cstddef>
// Behaviour model of NTCScriptedControl::CActionDropGenericBox::CActionDropGenericBox @ 0x009036A0.
class CThing;
struct CBase { virtual void v00() {} CBase() {} };
struct CActionDropGenericBox : CBase {
    CThing* m_pBox; bool m_dropped; bool m_started; long m_frame; bool m_finished;
    virtual void v00() {}
    CActionDropGenericBox() : CBase(), m_pBox(0), m_dropped(false), m_started(false), m_frame(-1), m_finished(false) {}
};
int main()
{
    CActionDropGenericBox a;
    bool ok = a.m_pBox == 0 && !a.m_dropped && !a.m_started && a.m_frame == -1 && !a.m_finished;
    if (sizeof(void*) == 4) ok = ok && offsetof(CActionDropGenericBox, m_frame) == 0xc && offsetof(CActionDropGenericBox, m_finished) == 0x10;
    if (!ok) { std::printf("009036a0_TEST FAIL\n"); return 1; }
    std::printf("009036a0_TEST PASS\n");
    return 0;
}