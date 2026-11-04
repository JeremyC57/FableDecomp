// Behaviour test for 0x005EBFA0 CTCTavernGame::CameraUseCameraPoint(const CCharString&): a model. The world things with the script
// name are collected (scan-visible ones only); with none nothing happens; the first must exist, be alive and carry the camera-point
// component (interface 0x3F); for a scripted point (type 6) the camera manager gets a tavern-game module going to the point's start
// position (no origin / axes) looking along its look direction. Only the first match is considered.
#include <cstdio>
#include <vector>
#include <string>

struct Vec { float X, Y, Z; };
struct Thing { std::string name; bool scan, dead, camera; int type; Vec start, look; };
struct Module { Vec goTo, lookDirection; };
static int g_modes;
static Module g_last;

static void CameraUseCameraPoint(const std::vector<Thing>& world, const std::string& name)
{
    std::vector<const Thing*> things;
    for (size_t i = 0; i < world.size(); ++i)
        if (world[i].scan && world[i].name == name)
            things.push_back(&world[i]);
    if (things.empty())
        return;
    const Thing* thing = things[0];
    if (thing && !thing->dead && thing->camera) {
        if (thing->type != 6)
            return;   // retail reads an uninitialised point pointer here; the model only exercises scripted points
        Module m = { thing->start, thing->look };
        g_last = m;
        ++g_modes;
    }
}

int main()
{
    int ok = 1;
    std::vector<Thing> w;
    Thing a = { "CAM", true, false, true, 6, { 1, 2, 3 }, { 0, 1, 0 } };
    Thing b = { "DEAD", true, true, true, 6, { 4, 5, 6 }, { 1, 0, 0 } };
    Thing c = { "PLAIN", true, false, false, 6, { 7, 8, 9 }, { 0, 0, 1 } };
    Thing d = { "HIDDEN", false, false, true, 6, { 1, 1, 1 }, { 1, 1, 1 } };
    Thing e = { "CAM", true, false, true, 6, { 9, 9, 9 }, { 9, 9, 9 } };
    w.push_back(a); w.push_back(b); w.push_back(c); w.push_back(d); w.push_back(e);
    CameraUseCameraPoint(w, "NONE");
    CameraUseCameraPoint(w, "DEAD");
    CameraUseCameraPoint(w, "PLAIN");
    CameraUseCameraPoint(w, "HIDDEN");
    ok &= g_modes == 0;
    CameraUseCameraPoint(w, "CAM");
    ok &= g_modes == 1 && g_last.goTo.X == 1 && g_last.goTo.Z == 3 && g_last.lookDirection.Y == 1;
    std::printf(ok ? "005EBFA0_TEST PASS\n" : "005EBFA0_TEST FAIL\n");
    return ok ? 0 : 1;
}