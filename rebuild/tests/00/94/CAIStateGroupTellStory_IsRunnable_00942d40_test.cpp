// Behaviour test for CAIStateGroup_TellStory::IsRunnable (0x00942D40): same logic over mocks.
#include <cstdio>
#include <cstring>
struct CMaterialDef { int refs; int destroyed; unsigned long Flags; void Release() { if (--refs == 0) ++destroyed; } };
struct CVillage { bool cookTime; bool IsCookTime() { return cookTime; } };
struct CThing { CThing* workplace; bool hasVillager; CVillage* village; CMaterialDef* material; };
static CVillage* GFGetPVillageInterface(CThing* t) { return t->village; }
static int g_lookups = 0;
static int g_index = 0;
static int GetDefGlobalIndexFromName(const char* name) { ++g_lookups; return std::strcmp(name, "STORY_PLACE_MARKER") == 0 ? 42 : -1; }
static CThing* g_marker = 0;
static bool GetPBestThing(int index, CThing** out) { g_index = index; if (g_marker) { *out = g_marker; return true; } return false; }
static bool IsRunnable(CThing* creature)
{
    CThing* home = creature->workplace;
    if (home && creature->hasVillager) {
        if (GFGetPVillageInterface(creature)) {
            CMaterialDef* material = home->material;
            if (material) {
                ++material->refs;
                bool result = false;
                if ((material->Flags & 4) && home->hasVillager) {
                    if (GFGetPVillageInterface(home) && GFGetPVillageInterface(home)->IsCookTime()) {
                        int index = GetDefGlobalIndexFromName("STORY_PLACE_MARKER");
                        CThing* marker;
                        if (GetPBestThing(index, &marker)) result = true;
                    }
                }
                material->Release();
                return result;
            }
        }
    }
    return false;
}
int main()
{
    CVillage v; v.cookTime = true;
    CMaterialDef m; m.refs = 1; m.destroyed = 0; m.Flags = 4;
    CThing home; home.workplace = 0; home.hasVillager = true; home.village = &v; home.material = &m;
    CThing kid; kid.workplace = &home; kid.hasVillager = true; kid.village = &v; kid.material = 0;
    CThing marker; g_marker = &marker;
    bool ok = IsRunnable(&kid) && g_index == 42 && m.refs == 1 && m.destroyed == 0;
    g_marker = 0; ok = ok && !IsRunnable(&kid) && m.refs == 1;
    g_marker = &marker; v.cookTime = false; int before = g_lookups; ok = ok && !IsRunnable(&kid) && g_lookups == before;
    v.cookTime = true; m.Flags = 0; ok = ok && !IsRunnable(&kid);
    m.Flags = 4; home.hasVillager = false; ok = ok && !IsRunnable(&kid);
    home.hasVillager = true; kid.village = 0; ok = ok && !IsRunnable(&kid);
    kid.village = &v; kid.workplace = 0; ok = ok && !IsRunnable(&kid);
    if (!ok) { std::printf("00942d40_TEST FAIL\n"); return 1; }
    std::printf("00942d40_TEST PASS\n");
    return 0;
}