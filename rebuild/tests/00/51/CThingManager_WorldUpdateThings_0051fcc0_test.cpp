// Behaviour test for CThingManager::WorldUpdateThings @ 0x0051FCC0: with updates enabled, every used thing type 1..15 (types 1
// and 5 also behind their own flags) has its update list walked; each visited thing bumps the counter, live activated things
// outside limbo get FrameUpdate and leave the update list when they report no frame update; UpdateDeaths always runs last.
#include <stdio.h>
bool g_UpdateThingsEnabled_01375755 = true;
bool g_UpdateThingType1Enabled_0137575a = true;
bool g_UpdateThingType5Enabled_01375759 = true;
int  g_NoThingsUpdated_013b8a84 = 0;
static int g_frameUpdates = 0, g_removed = 0, g_deaths = 0;
class CThingManager;
class CThing { public:
    virtual bool IsFrameUpdated() const { return KeepUpdating; }
    void FrameUpdate() { g_frameUpdates++; Updated = true; }
    CThing* UpdateListNext;
    bool KeepUpdating, Updated;
    unsigned char Dead : 1;
    unsigned char CreatedByOwnerThingCreation : 1;
    unsigned char IncludeInThingScans : 1;
    unsigned char Activated : 1;
    unsigned char GloballyLoaded : 1;
    unsigned char InLimbo : 1;
    unsigned char KillLocked : 1;
    unsigned char CreatedBySerialisation : 1;
};
class CClassTypeInfo { public: bool Used; };
struct ThingLList { CThing* Head; CThing* Tail; };
class CThingManager { public:
    CClassTypeInfo ClassTypeInfo[16];
    bool           FirstRenderOfFrame;
    ThingLList     UpdateLists[16];
    ThingLList& GetUpdateListFromType(int type) { return UpdateLists[type]; }
    void RemoveFromUpdateList(CThing& thing)                // unlink from whichever list holds it
    {
        g_removed++;
        for (int t = 0; t < 16; ++t) {
            CThing** link = &UpdateLists[t].Head;
            while (*link) { if (*link == &thing) { *link = thing.UpdateListNext; thing.UpdateListNext = 0; return; } link = &(*link)->UpdateListNext; }
        }
    }
    void UpdateDeaths() { g_deaths++; }
    void WorldUpdateThings();
};
void CThingManager::WorldUpdateThings()
{
    g_NoThingsUpdated_013b8a84 = 0;
    FirstRenderOfFrame = true;
    if (g_UpdateThingsEnabled_01375755) {
        for (int type = 1; type < 16; ++type) {
            if (type == 1) {
                if (!g_UpdateThingType1Enabled_0137575a)
                    continue;
            } else if (type == 5) {
                if (!g_UpdateThingType5Enabled_01375759)
                    continue;
            }
            if (!ClassTypeInfo[type].Used)
                continue;
            CThing* thing = GetUpdateListFromType(type).Head;
            while (thing) {
                ++g_NoThingsUpdated_013b8a84;
                CThing* current = thing;
                thing = thing->UpdateListNext;
                if (!current->Dead && current->Activated && !current->InLimbo) {
                    current->FrameUpdate();
                    if (!current->IsFrameUpdated())
                        RemoveFromUpdateList(*current);
                }
            }
        }
    }
    UpdateDeaths();
}
static void init(CThing& t, bool dead, bool activated, bool limbo, bool keep)
{
    t.UpdateListNext = 0; t.KeepUpdating = keep; t.Updated = false;
    t.Dead = dead; t.CreatedByOwnerThingCreation = 0; t.IncludeInThingScans = 0; t.Activated = activated;
    t.GloballyLoaded = 0; t.InLimbo = limbo; t.KillLocked = 0; t.CreatedBySerialisation = 0;
}
static int fail(int line) { printf("0051fcc0_TEST FAIL\nline %d\n", line); return 1; }
int main()
{
    CThingManager m;
    for (int i = 0; i < 16; ++i) { m.ClassTypeInfo[i].Used = true; m.UpdateLists[i].Head = m.UpdateLists[i].Tail = 0; }
    CThing a, b, c, d, e, f, g, z;
    init(a, false, true, false, false);                     // updated, then removed (reports no frame update)
    init(b, true, true, false, true);                       // dead: skipped
    init(c, false, false, false, true);                     // not activated: skipped
    init(d, false, true, true, true);                       // in limbo: skipped
    init(e, false, true, false, true);                      // updated, stays
    a.UpdateListNext = &b; b.UpdateListNext = &c; c.UpdateListNext = &d; d.UpdateListNext = &e;
    m.UpdateLists[3].Head = &a;
    init(f, false, true, false, true); m.UpdateLists[1].Head = &f;    // type 1: gated
    init(g, false, true, false, true); m.UpdateLists[5].Head = &g;    // type 5: gated
    init(z, false, true, false, true); m.UpdateLists[0].Head = &z;    // type 0 is never walked
    m.ClassTypeInfo[7].Used = false; CThing u; init(u, false, true, false, true); m.UpdateLists[7].Head = &u;
    m.FirstRenderOfFrame = false; g_NoThingsUpdated_013b8a84 = 99;
    g_UpdateThingType1Enabled_0137575a = false;
    m.WorldUpdateThings();
    if (!m.FirstRenderOfFrame || g_deaths != 1) return fail(__LINE__);
    if (g_NoThingsUpdated_013b8a84 != 6 || g_frameUpdates != 3 || g_removed != 1) return fail(__LINE__);   // a..e + g
    if (!a.Updated || b.Updated || c.Updated || d.Updated || !e.Updated || f.Updated || !g.Updated || z.Updated || u.Updated)
        return fail(__LINE__);
    if (m.UpdateLists[3].Head != &b) return fail(__LINE__);
    g_UpdateThingType1Enabled_0137575a = true; g_UpdateThingType5Enabled_01375759 = false;
    m.WorldUpdateThings();
    if (g_NoThingsUpdated_013b8a84 != 5 || !f.Updated || g_frameUpdates != 5 || g_deaths != 2) return fail(__LINE__);   // b..e + f
    g_UpdateThingsEnabled_01375755 = false; m.FirstRenderOfFrame = false;
    m.WorldUpdateThings();
    if (g_NoThingsUpdated_013b8a84 != 0 || g_frameUpdates != 5 || !m.FirstRenderOfFrame || g_deaths != 3) return fail(__LINE__);
    printf("0051fcc0_TEST PASS\n");
    return 0;
}