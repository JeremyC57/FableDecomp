// Behaviour test for CGameScriptInterface::SetDeadCreaturesAndExperienceOrbsAndDropBagsAsHidden 0x00894EC0 (model copy).
#include <cstdio>
#include <cstring>
struct Thing { int kind; int tc; const char* def; bool dead; int drawable; void SetAsDrawable(bool d) { drawable = d ? 1 : 0; } bool IsDead() const { return dead; } };
struct World {
    Thing* things; int count;
    // the four searches append to the output list
    void DeadCreatures(Thing** out, int& n) { for (int i = 0; i < count; ++i) if (things[i].kind == 1) out[n++] = &things[i]; }
    void HasTC(int tc, Thing** out, int& n) { for (int i = 0; i < count; ++i) if (things[i].tc == tc) out[n++] = &things[i]; }
    void HasDefName(const char* d, Thing** out, int& n) { for (int i = 0; i < count; ++i) if (things[i].def && std::strcmp(things[i].def, d) == 0) out[n++] = &things[i]; }
};
void SetHidden(World* w, bool hidden)
{
    Thing* list[32]; int n = 0;
    w->DeadCreatures(list, n);
    w->HasTC(0x69, list, n);
    w->HasDefName("OBJECT_CHEST_REWARD_ON_DEATH", list, n);
    w->HasDefName("OBJECT_GOLDBAG_SMALL_WITH_COINS", list, n);
    w->HasTC(0xcc, list, n);
    for (int i = 0; i < n; ++i) {
        Thing* thing = list[i];
        if (thing && !thing->IsDead())
            thing->SetAsDrawable(!hidden);
    }
}
int main()
{
    Thing t[6] = {
        { 1, 0, 0, false, -1 }, { 0, 0x69, 0, false, -1 }, { 0, 0, "OBJECT_CHEST_REWARD_ON_DEATH", false, -1 },
        { 0, 0, "OBJECT_GOLDBAG_SMALL_WITH_COINS", true, -1 }, { 0, 0xcc, 0, false, -1 }, { 0, 7, "OTHER", false, -1 } };
    World w; w.things = t; w.count = 6;
    SetHidden(&w, true);
    bool ok = t[0].drawable == 0 && t[1].drawable == 0 && t[2].drawable == 0 && t[3].drawable == -1 && t[4].drawable == 0 && t[5].drawable == -1;
    SetHidden(&w, false);
    if (t[0].drawable != 1 || t[4].drawable != 1 || t[3].drawable != -1) ok = false;
    if (!ok) { std::printf("00894ec0_TEST FAIL\n"); return 1; }
    std::printf("00894ec0_TEST PASS\n");
    return 0;
}