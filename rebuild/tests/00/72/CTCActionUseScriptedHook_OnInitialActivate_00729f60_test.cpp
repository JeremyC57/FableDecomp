// Behaviour test for CTCActionUseScriptedHook::OnInitialActivate (0x00729F60), g++ model of the retail logic.
#include <cstdio>
#include <set>
#include <string>
struct Thing { std::set<std::string> tcs; bool targetable; unsigned targetingFlags; };
struct Hook {
    Thing* parent; unsigned long long entranceUID, cameraUID; int baseCalls, entranceResolves; Thing* camera;
};
static Thing g_cameraThing;
static Thing* GetPThingWithUID(unsigned long long uid) { return uid == 77 ? &g_cameraThing : 0; }
static void OnInitialActivate(Hook& h)
{
    ++h.baseCalls;
    if (!h.parent->tcs.count("CTCTargeted")) {
        h.parent->tcs.insert("CTCTargeted");
        h.parent->targetable = true;
        h.parent->targetingFlags |= 1;
    }
    if (!h.parent->tcs.count("CTCEntityEvents"))
        h.parent->tcs.insert("CTCEntityEvents");
    if (h.entranceUID != 0) ++h.entranceResolves;
    if (h.cameraUID != 0) h.camera = GetPThingWithUID(h.cameraUID);
}
int main()
{
    Thing t; t.targetable = false; t.targetingFlags = 0;
    Hook h = { &t, 5, 77, 0, 0, 0 };
    OnInitialActivate(h);
    int ok = h.baseCalls == 1 && t.tcs.size() == 2 && t.targetable && t.targetingFlags == 1 && h.entranceResolves == 1 && h.camera == &g_cameraThing;
    Thing u; u.targetable = false; u.targetingFlags = 0; u.tcs.insert("CTCTargeted");
    Hook k = { &u, 0, 0, 0, 0, 0 };
    OnInitialActivate(k);
    ok &= !u.targetable && u.targetingFlags == 0 && u.tcs.count("CTCEntityEvents") && k.entranceResolves == 0 && k.camera == 0;
    std::printf(ok ? "00729f60_TEST PASS\n" : "00729f60_TEST FAIL\n");
    return ok ? 0 : 1;
}