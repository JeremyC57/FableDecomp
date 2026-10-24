// Behaviour test for CTCDNone::Create (0x00835A20): map 0 when flagged, driver bit required, activation required.
#include <cstdio>
struct C3DVector { float X, Y, Z; };
class CTCDNone;
struct CWorldMap { long GetMapNumberAt(const C3DVector& pos) const { return (long)pos.X + 10; } };
struct CThingManager { CWorldMap* WorldMap; };
static CWorldMap g_map; static CThingManager g_mgr = { &g_map }; static CThingManager* g_pThingManager = &g_mgr;
static long g_lastMap = -1, g_lastDef = -1, g_lastPlayer = -1; static bool g_lastPersistent = false;
static bool g_activate = true, g_fail = false;
class CThing {
public:
    unsigned int UsingComponents[9];
    void* PTCDriver;
    static CThing g_thing;
    static CThing* Create(long defIndex, long player, long map, bool levelPersistent)
    { g_lastDef = defIndex; g_lastPlayer = player; g_lastMap = map; g_lastPersistent = levelPersistent; return g_fail ? 0 : &g_thing; }
    bool DoActivationSequence() { return g_activate; }
    bool GetTCDriver(CTCDNone** out)
    {
        if (UsingComponents[3] & 1) { *out = (CTCDNone*)PTCDriver; return true; }
        return false;
    }
};
CThing CThing::g_thing;
static CThing* CreateNone(long defIndex, const C3DVector& pos, bool noMap)
{
    long map = 0;
    if (!noMap)
        map = g_pThingManager->WorldMap->GetMapNumberAt(pos);
    CThing* thing = CThing::Create(defIndex, 4, map, true);
    CThing* result = 0;
    CTCDNone* driver;
    if (thing && thing->GetTCDriver(&driver) && thing->DoActivationSequence())
        result = thing;
    return result;
}
int main()
{
    C3DVector p = { 5, 0, 0 };
    CThing::g_thing.UsingComponents[3] = 1;
    if (CreateNone(3, p, false) != &CThing::g_thing || g_lastMap != 15 || g_lastDef != 3 || g_lastPlayer != 4 || !g_lastPersistent) { std::printf("00835a20_TEST FAIL\n"); return 1; }
    if (CreateNone(3, p, true) != &CThing::g_thing || g_lastMap != 0) { std::printf("00835a20_TEST FAIL\n"); return 1; }
    CThing::g_thing.UsingComponents[3] = 2;
    if (CreateNone(3, p, true) != 0) { std::printf("00835a20_TEST FAIL\n"); return 1; }
    CThing::g_thing.UsingComponents[3] = 1; g_activate = false;
    if (CreateNone(3, p, true) != 0) { std::printf("00835a20_TEST FAIL\n"); return 1; }
    g_activate = true; g_fail = true;
    if (CreateNone(3, p, true) != 0) { std::printf("00835a20_TEST FAIL\n"); return 1; }
    std::printf("00835a20_TEST PASS\n");
    return 0;
}