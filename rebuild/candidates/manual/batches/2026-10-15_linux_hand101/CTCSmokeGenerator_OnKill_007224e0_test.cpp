// Behaviour test for CTCSmokeGenerator::OnKill (0x007224E0): every live smoke thing is killed once; null and already
// killed things are skipped; nothing happens before the positions exist.
#include <cstdio>

static int g_killCalls = 0;

struct CThing {
    unsigned char m_flags;
    bool          m_lastImmediate;
    void Kill(bool immediately) { ++g_killCalls; m_lastImmediate = immediately; m_flags |= 1; }
};

struct CBaseIntelligentPointer {
    CThing* m_pItem;
    CThing* GetPItem() const { return m_pItem; }
};

struct CGenerationPos {
    float                   Position[3];
    CBaseIntelligentPointer PThing;
};

struct CTCSmokeGenerator {
    float           GenerationDistance;
    CGenerationPos* m_pFirst;
    CGenerationPos* m_pLast;
    CGenerationPos* m_pEnd;
    bool            PositionsCreated;
    void OnKill();
};

void CTCSmokeGenerator::OnKill()
{
    if (PositionsCreated) {
        for (CGenerationPos* pos = m_pFirst; pos != m_pLast; ++pos) {
            if (pos->PThing.GetPItem() && !(pos->PThing.GetPItem()->m_flags & 1))
                pos->PThing.GetPItem()->Kill(true);
        }
    }
}

int main()
{
    CThing a = { 0, false }, b = { 1, false }, c = { 0, false };
    CGenerationPos pos[4];
    pos[0].PThing.m_pItem = &a; pos[1].PThing.m_pItem = 0; pos[2].PThing.m_pItem = &b; pos[3].PThing.m_pItem = &c;
    CTCSmokeGenerator g;
    g.m_pFirst = pos; g.m_pLast = pos + 4; g.m_pEnd = pos + 4; g.PositionsCreated = false;
    g.OnKill();
    if (g_killCalls != 0) { std::printf("007224e0_TEST FAIL\n"); return 1; }
    g.PositionsCreated = true;
    g.OnKill();
    if (g_killCalls != 2 || !(a.m_flags & 1) || !(c.m_flags & 1) || !a.m_lastImmediate || b.m_lastImmediate) { std::printf("007224e0_TEST FAIL\n"); return 1; }
    g.OnKill();
    if (g_killCalls != 2) { std::printf("007224e0_TEST FAIL\n"); return 1; }
    std::printf("007224e0_TEST PASS\n");
    return 0;
}
