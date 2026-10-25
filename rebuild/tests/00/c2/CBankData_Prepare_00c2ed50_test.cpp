#include <stdio.h>
static int g_calls[3]; static int g_n = 0; static bool g_last = true;
struct CSampleList {
    virtual int v0() { return 0; } virtual int v1() { return 0; } virtual int v2() { return 0; } virtual int v3() { return 0; } virtual int v4() { return 0; }
    virtual bool Prepare() { g_calls[g_n++] = 1; return false; }
};
struct CDriverList {
    virtual int v0() { return 0; } virtual int v1() { return 0; } virtual int v2() { return 0; } virtual int v3() { return 0; } virtual int v4() { return 0; }
    virtual bool Prepare() { g_calls[g_n++] = 2; return false; }
};
struct CEventList {
    virtual int v0() { return 0; } virtual int v1() { return 0; } virtual int v2() { return 0; } virtual int v3() { return 0; } virtual int v4() { return 0; }
    virtual bool Prepare() { g_calls[g_n++] = 3; return g_last; }
};
struct CBankData {
    CSampleList m_samples; char pad[12]; CDriverList m_drivers; char padB[12]; CEventList m_events;
    bool Prepare() { m_samples.Prepare(); m_drivers.Prepare(); return m_events.Prepare(); }
};
int main()
{
    CBankData d;
    bool r1 = d.Prepare();
    g_last = false; g_n = 0;
    bool r2 = d.Prepare();
    if (!r1 || r2 || g_n != 3 || g_calls[0] != 1 || g_calls[1] != 2 || g_calls[2] != 3) { printf("00c2ed50_TEST FAIL\n"); return 1; }
    printf("00c2ed50_TEST PASS\n");
    return 0;
}
