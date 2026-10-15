#include <cstdio>
// Behaviour of CTCInventoryMap::IsTeleportValid (0x005FAA60): engine flag set AND the CBase slot-3 gate clear.
struct CDisplayEngine { unsigned char m_teleportAllowed; };
static CDisplayEngine g_engine;
struct CTCInventoryBase_Retail { virtual ~CTCInventoryBase_Retail() {} long m_body; };
struct CBase { virtual unsigned char IsBlocked() { return 1; } };
struct CTCInventoryMap : CTCInventoryBase_Retail, CBase {
    unsigned char m_blocked;
    virtual unsigned char IsBlocked() { return m_blocked; }
    CDisplayEngine* GetDisplayEngine() { return &g_engine; }
    bool IsTeleportValid()
    {
        if (GetDisplayEngine()->m_teleportAllowed && !IsBlocked())
            return true;
        return false;
    }
};
int main()
{
    CTCInventoryMap m;
    int bad = 0;
    g_engine.m_teleportAllowed = 1; m.m_blocked = 0; if (!m.IsTeleportValid()) bad = 1;
    g_engine.m_teleportAllowed = 1; m.m_blocked = 1; if (m.IsTeleportValid()) bad = 2;
    g_engine.m_teleportAllowed = 0; m.m_blocked = 0; if (m.IsTeleportValid()) bad = 3;
    if (bad) { std::printf("005faa60_TEST FAIL\n"); return 1; }
    std::printf("005faa60_TEST PASS\n");
    return 0;
}
