#include <cstdio>
// Behaviour model of CTCPhysicsBase::FrameUpdate (0x006AFD60)
static int g_refresh, g_finished, g_moved; static long g_flags;
struct CTCGraphicAppearance { void SetActivePersistenceFlags(long f) { g_flags = f; } };
struct CThing { CTCGraphicAppearance* PGraphicAppearance; void RefreshGraphic(bool) { ++g_refresh; } };
struct CTCPhysicsBase {
    CThing* ParentThing; bool enabled; bool Moving, OldMoving;
    bool IsPhysicsEnabled() { return enabled; } void OnMoved() { ++g_moved; } void OnMoveFinished() { ++g_finished; }
    void FrameUpdate() {
        if (IsPhysicsEnabled()) {
            if (Moving || OldMoving) { ParentThing->RefreshGraphic(true); OnMoveFinished(); }
            if (Moving) OnMoved();
        } else if (Moving || OldMoving) { ParentThing->RefreshGraphic(true); OnMoveFinished(); Moving = false; }
        CTCGraphicAppearance* g = ParentThing->PGraphicAppearance;
        if (g) { if (!(Moving || OldMoving)) g->SetActivePersistenceFlags(0); else g->SetActivePersistenceFlags(2); }
    } };
int main()
{
    CTCGraphicAppearance ga; CThing t = { &ga };
    CTCPhysicsBase p = { &t, true, true, false };
    g_flags = -1; p.FrameUpdate();
    if (g_refresh != 1 || g_finished != 1 || g_moved != 1 || g_flags != 2 || !p.Moving) { std::printf("006afd60_TEST FAIL\n"); return 1; }
    p.enabled = false; p.FrameUpdate();
    if (g_refresh != 2 || p.Moving || g_flags != 0) { std::printf("006afd60_TEST FAIL\n"); return 1; }
    std::printf("006afd60_TEST PASS\n");
    return 0;
}
