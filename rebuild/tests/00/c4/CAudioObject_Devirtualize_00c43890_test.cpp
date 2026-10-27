// Test for 0x00C43890 CAudioObject::Devirtualize: stores +0xEC; a virtual object loses bit 9, uncounts in states 0/8, calls back.
#include <cstdio>
static int g_uncounts = 0, g_callbacks = 0; static unsigned int g_cbFlags = 0;
struct CObj;
static void Callback(CObj*, unsigned int flags, void*) { ++g_callbacks; g_cbFlags = flags; }
struct CObj { unsigned int m_flags; int m_ec; bool m_hasCallback;
    void Devirtualize(int v) {
        m_ec = v;
        if ((m_flags >> 9) & 1) {
            m_flags &= ~0x200u;
            unsigned int state = m_flags & 0x7f;
            if (state == 0 || state == 8) ++g_uncounts;
            if (m_hasCallback) Callback(this, m_flags & 0xffff, 0);
        } } };
int main()
{
    CObj o; o.m_flags = 0x10208; o.m_ec = 0; o.m_hasCallback = true;
    o.Devirtualize(5);
    bool ok = o.m_ec == 5 && o.m_flags == 0x10008 && g_uncounts == 1 && g_callbacks == 1 && g_cbFlags == 0x0008;
    o.m_flags = 0x203; o.Devirtualize(6);
    ok = ok && o.m_flags == 3 && g_uncounts == 1 && g_callbacks == 2;
    o.Devirtualize(7); ok = ok && o.m_ec == 7 && g_callbacks == 2;
    if (!ok) { std::printf("00c43890_TEST FAIL\n"); return 1; }
    std::printf("00c43890_TEST PASS\n");
    return 0;
}