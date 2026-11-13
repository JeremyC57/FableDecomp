#include <cstdio>
#include <list>
// Behaviour model of CTCGraphicAppearance::SetPrimitive's control logic: reasons, resend bit, retry list, reason clear.
struct DrawInfo { unsigned long ReasonsForDraw; bool HiddenMaterialsUsed; void* PTextureMorphs; void* PTextureDecals; };
struct Component {
    int failuresLeft; bool sentEveryFrame; int draws;
    bool IsAppearanceSentEveryRenderFrame() { return sentEveryFrame; }
    bool OnAppearanceDraw(const DrawInfo&) { ++draws; if (failuresLeft > 0) { --failuresLeft; return false; } return true; }
};
struct Appearance {
    std::list<Component*> Components;
    unsigned char UpdatePrimitiveReasons;
    bool ResendPrimsEveryRenderFrame;
    unsigned long GetUpdatePrimitiveReasons() const { if (!UpdatePrimitiveReasons && ResendPrimsEveryRenderFrame) return 4; return UpdatePrimitiveReasons; }
    unsigned long lastReasons;
    void SetPrimitive(int hiddenCount)
    {
        if (Components.empty()) return;
        DrawInfo info;
        info.PTextureMorphs = 0; info.PTextureDecals = 0;
        info.HiddenMaterialsUsed = false;
        info.ReasonsForDraw = GetUpdatePrimitiveReasons();
        lastReasons = info.ReasonsForDraw;
        if (hiddenCount != 0) info.HiddenMaterialsUsed = true;
        bool all_drawn = true;
        std::list<Component*> retry;
        for (std::list<Component*>::iterator it = Components.begin(); it != Components.end(); ++it) {
            Component* c = *it;
            bool sent = c->IsAppearanceSentEveryRenderFrame();
            ResendPrimsEveryRenderFrame = ResendPrimsEveryRenderFrame || sent;
            if (!c->OnAppearanceDraw(info)) retry.push_back(c);
        }
        for (std::list<Component*>::iterator it2 = retry.begin(); it2 != retry.end(); ++it2)
            if (!(*it2)->OnAppearanceDraw(info)) all_drawn = false;
        if (all_drawn) UpdatePrimitiveReasons = 0;
    }
};
int main()
{
    Component a = {0, false, 0}, b = {1, true, 0}, c = {2, false, 0};
    Appearance app; app.UpdatePrimitiveReasons = 2; app.ResendPrimsEveryRenderFrame = false; app.lastReasons = 99;
    app.SetPrimitive(0);                        // empty: nothing happens
    if (app.lastReasons != 99 || app.UpdatePrimitiveReasons != 2) { std::printf("004bd6e0_TEST FAIL\n"); return 1; }
    app.Components.push_back(&a); app.Components.push_back(&b);
    app.SetPrimitive(1);                        // b fails once, retry succeeds -> reasons cleared, resend set by b
    if (app.lastReasons != 2 || app.UpdatePrimitiveReasons != 0 || !app.ResendPrimsEveryRenderFrame || a.draws != 1 || b.draws != 2) {
        std::printf("004bd6e0_TEST FAIL\n"); return 1;
    }
    app.Components.push_back(&c); app.UpdatePrimitiveReasons = 0;
    app.SetPrimitive(0);                        // no reason but resend -> 4; c fails twice -> all_drawn false
    if (app.lastReasons != 4 || c.draws != 2 || app.UpdatePrimitiveReasons != 0) { std::printf("004bd6e0_TEST FAIL\n"); return 1; }
    Component d = {2, false, 0};                // retried component fails again: non-zero reasons must be KEPT
    app.Components.push_back(&d); app.UpdatePrimitiveReasons = 5;
    app.SetPrimitive(0);                        // c now draws first time; d fails, retry fails -> reasons stay 5
    if (app.lastReasons != 5 || app.UpdatePrimitiveReasons != 5 || c.draws != 3 || d.draws != 2) { std::printf("004bd6e0_TEST FAIL\n"); return 1; }
    app.Components.remove(&d);
    app.UpdatePrimitiveReasons = 3;
    app.SetPrimitive(0);                        // every component draws -> clear
    if (app.lastReasons != 3 || app.UpdatePrimitiveReasons != 0 || c.draws != 4) { std::printf("004bd6e0_TEST FAIL\n"); return 1; }
    std::printf("004bd6e0_TEST PASS\n");
    return 0;
}