// Behaviour test for 0x00EE8390 NScript::CCS_OakValeRevisitedScript::OakVale: the fire thread is spawned, the hero teleported,
// the cut-scene played with the hero cast, the fire list resized to the fire points and every fire removed, the quest
// deactivated, and only live fences removed.
#include <cstdio>
#include <vector>
struct CThing { bool Alive; bool Removed; CThing() : Alive(true), Removed(false) {} };
static int gSpawned = 0, gTeleports = 0, gCutscenes = 0, gDeactivated = 0;
static bool gTerminating = false;
static void OakVale(std::vector<CThing>& firePoint, std::vector<CThing>& fires, std::vector<CThing>& fences)
{
    ++gSpawned;
    ++gTeleports;
    ++gCutscenes;
    fires.resize(firePoint.size());
    for (unsigned int i = 0; i < fires.size(); ++i) {
        if (gTerminating) return;
        fires[i].Removed = true;
    }
    if (gTerminating) return;
    ++gDeactivated;
    for (unsigned int j = 0; j < fences.size(); ++j)
        if (fences[j].Alive) fences[j].Removed = true;
}
int main()
{
    std::vector<CThing> firePoint(4), fires, fences(3);
    fences[1].Alive = false;
    OakVale(firePoint, fires, fences);
    bool ok = gSpawned == 1 && gTeleports == 1 && gCutscenes == 1 && fires.size() == 4 && fires[3].Removed && gDeactivated == 1
           && fences[0].Removed && !fences[1].Removed && fences[2].Removed;
    gTerminating = true;
    std::vector<CThing> fires2, fences2(1);
    OakVale(firePoint, fires2, fences2);
    ok = ok && gDeactivated == 1 && !fences2[0].Removed && !fires2[0].Removed;
    std::printf(ok ? "00ee8390_TEST PASS\n" : "00ee8390_TEST FAIL\n");
    return ok ? 0 : 1;
}
