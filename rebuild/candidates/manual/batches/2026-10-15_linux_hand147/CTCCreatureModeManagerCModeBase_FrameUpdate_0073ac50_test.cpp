#include <cstdio>
struct Mgr { long removed; int calls; void RemoveMode(long t) { removed = t; ++calls; } };
struct Thing { bool has; Mgr mgr; Mgr* GetTC() { return has ? &mgr : 0; } };
struct Mode {
    virtual long GetModeType() { return 7; }
    Thing* parent; long frames;
    void FrameUpdate()
    {
        if (frames > 0) --frames;
        if (frames == 0) { Mgr* m = 0; if (parent->has) m = parent->GetTC(); m->RemoveMode(GetModeType()); }
    }
};
int main()
{
    Thing t; t.has = true; t.mgr.removed = 0; t.mgr.calls = 0;
    Mode m; m.parent = &t; m.frames = 2;
    m.FrameUpdate(); if (m.frames != 1 || t.mgr.calls != 0) { std::printf("0073ac50_TEST FAIL\n"); return 1; }
    m.FrameUpdate(); if (m.frames != 0 || t.mgr.calls != 1 || t.mgr.removed != 7) { std::printf("0073ac50_TEST FAIL\n"); return 1; }
    Mode n; n.parent = &t; n.frames = -1; n.FrameUpdate();
    if (n.frames != -1 || t.mgr.calls != 1) { std::printf("0073ac50_TEST FAIL\n"); return 1; }
    std::printf("0073ac50_TEST PASS\n");
    return 0;
}
