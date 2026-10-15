#include <cstdio>
#include <cstring>
// Behaviour test for CTCTeleporter::vfunc_1 (0x007DFF40).
struct CPersistContext {
    int calls; const char* names[4]; bool* values[4]; bool defs[4];
    void Transfer(const char* n, bool& v, const bool& d) { names[calls] = n; values[calls] = &v; defs[calls] = d; ++calls; }
};
struct CTCTeleporter {
    unsigned char _pad[0x10];
    bool m_teleportActive;
    bool m_teleportActivated;
    void vfunc_1(CPersistContext* context, bool persist);
};
void CTCTeleporter::vfunc_1(CPersistContext* context, bool persist)
{
    if (persist) {
        context->Transfer("TeleportActive", m_teleportActive, false);
        context->Transfer("TeleportActivated", m_teleportActivated, false);
    }
}
int main()
{
    CTCTeleporter t;
    CPersistContext c; c.calls = 0; c.defs[0] = c.defs[1] = true;
    t.vfunc_1(&c, false);
    if (c.calls != 0) { std::printf("007dff40_TEST FAIL\n"); return 1; }
    t.vfunc_1(&c, true);
    if (c.calls != 2 || std::strcmp(c.names[0], "TeleportActive") != 0 || c.values[0] != &t.m_teleportActive || c.defs[0]
        || std::strcmp(c.names[1], "TeleportActivated") != 0 || c.values[1] != &t.m_teleportActivated || c.defs[1])
    { std::printf("007dff40_TEST FAIL\n"); return 1; }
    std::printf("007dff40_TEST PASS\n");
    return 0;
}