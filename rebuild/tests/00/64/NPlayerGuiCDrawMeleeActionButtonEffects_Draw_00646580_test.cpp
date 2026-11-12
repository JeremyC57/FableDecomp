// Behaviour test for NPlayerGui::CDrawMeleeActionButtonEffects::Draw (0x00646580): the same control flow through a copy of the
// model -- the special-buttons reset arm, the two timers, the once-only emitter creation at the anchored effect position, the
// flourish sprite at anchored position + graphic offset while the emitter lives, and the final ClearEffects on a zero effect.
#include <cstdio>
class C2DVector { public:
    float X, Y;
    C2DVector() { }
    C2DVector(float x, float y) : X(x), Y(y) { }
    C2DVector(const C2DVector& p) : X(p.X), Y(p.Y) { }
    C2DVector operator+(const C2DVector& vector) const { return C2DVector(X + vector.X, Y + vector.Y); }
};
static long g_width = 1024, g_height = 768;
static C2DVector GFAnchorToBottomRight(const C2DVector& pos)
{
    return C2DVector((float)g_width + pos.X - 640.0f, (float)g_height + pos.Y - 480.0f);
}
class CPlayerGuiDef { public:
    C2DVector AbilityButtonFlourishEffectPos;
    long      AbilityButtonFlourishGraphic;
    C2DVector AbilityButtonFlourishGraphicOffset;
};
static CPlayerGuiDef g_def;
class CPlayerGui { public: bool ShowSpecialAbilityButtons; float Alpha; };
struct CEmitter { long Effect; C2DVector Position; long Layer; bool Start; };
static CEmitter g_emitterStore;
static int g_emittersCreated, g_spritesDrawn, g_clears;
static C2DVector g_spritePos;
static long g_spriteGraphic, g_spriteLayer, g_spriteFlags;
static unsigned char g_spriteAlpha;
static CEmitter* Create2DEmitter(long effect, const C2DVector& position, long layer, bool start)
{
    ++g_emittersCreated;
    g_emitterStore.Effect = effect; g_emitterStore.Position = position; g_emitterStore.Layer = layer; g_emitterStore.Start = start;
    return &g_emitterStore;
}
struct CDrawMeleeActionButtonEffects {
    CPlayerGui* PlayerGui;
    bool        Drawing;
    CEmitter*   Effects[1];
    long        FlashTimer, ComboLoadingIconTimer, FlourishCounter, FlourishEffect;
    unsigned char ScaleAlpha(unsigned char alpha) const { return (unsigned char)(long)((float)alpha * PlayerGui->Alpha); }
    void ClearEffects() { ++g_clears; Effects[0] = 0; }
    void Draw()
    {
        if (!PlayerGui->ShowSpecialAbilityButtons) {
            if ((FlashTimer >= 0 || ComboLoadingIconTimer >= 0) && FlourishEffect > 0) {
                Drawing = true;
                const CPlayerGuiDef& def = g_def;
                if (FlashTimer >= 0 && Effects[0] == 0) {
                    C2DVector position = GFAnchorToBottomRight(def.AbilityButtonFlourishEffectPos);
                    Effects[0] = Create2DEmitter(FlourishEffect, position, 0x44, true);
                }
                if (Effects[0] != 0) {
                    C2DVector anchored = GFAnchorToBottomRight(def.AbilityButtonFlourishEffectPos);
                    C2DVector position = def.AbilityButtonFlourishGraphicOffset + anchored;
                    ++g_spritesDrawn;
                    g_spritePos = C2DVector(position); g_spriteLayer = 0x43; g_spriteGraphic = def.AbilityButtonFlourishGraphic;
                    g_spriteAlpha = ScaleAlpha(0xff); g_spriteFlags = 0xc0;
                }
            }
        } else {
            FlourishEffect = 0;
        }
        if (FlourishEffect == 0)
            ClearEffects();
    }
};
static void Reset(CDrawMeleeActionButtonEffects& d, CPlayerGui& gui, long flash, long combo, long effect, CEmitter* emitter, bool show)
{
    gui.ShowSpecialAbilityButtons = show; gui.Alpha = 0.5f;
    d.PlayerGui = &gui; d.Drawing = false; d.Effects[0] = emitter; d.FlashTimer = flash; d.ComboLoadingIconTimer = combo;
    d.FlourishCounter = 0; d.FlourishEffect = effect;
    g_emittersCreated = g_spritesDrawn = g_clears = 0;
    g_spritePos = C2DVector(-1.0f, -1.0f); g_spriteGraphic = g_spriteLayer = g_spriteFlags = 0; g_spriteAlpha = 0;
}
int main()
{
    bool ok = true;
    CDrawMeleeActionButtonEffects d; CPlayerGui gui; CEmitter existing;
    g_def.AbilityButtonFlourishEffectPos = C2DVector(600.0f, 440.0f);
    g_def.AbilityButtonFlourishGraphic = 77;
    g_def.AbilityButtonFlourishGraphicOffset = C2DVector(-16.0f, 8.0f);

    // 1. special ability buttons showing: the effect is dropped and the emitters cleared, nothing drawn
    Reset(d, gui, 5, 5, 9, &existing, true); d.Draw();
    if (d.FlourishEffect != 0 || g_clears != 1 || g_spritesDrawn != 0 || g_emittersCreated != 0 || d.Drawing) ok = false;

    // 2. both timers negative: nothing happens, the effect stays, no clear
    Reset(d, gui, -1, -1, 9, 0, false); d.Draw();
    if (d.FlourishEffect != 9 || g_clears != 0 || g_spritesDrawn != 0 || g_emittersCreated != 0 || d.Drawing) ok = false;

    // 3. flash running, effect set, no emitter yet: created once at the anchored position, then the sprite at anchored + offset
    Reset(d, gui, 0, -1, 9, 0, false); d.Draw();
    if (!d.Drawing || g_emittersCreated != 1 || d.Effects[0] != &g_emitterStore) ok = false;
    if (g_emitterStore.Effect != 9 || g_emitterStore.Layer != 0x44 || !g_emitterStore.Start) ok = false;
    if (g_emitterStore.Position.X != 984.0f || g_emitterStore.Position.Y != 728.0f) ok = false;
    if (g_spritesDrawn != 1 || g_spritePos.X != 968.0f || g_spritePos.Y != 736.0f) ok = false;
    if (g_spriteGraphic != 77 || g_spriteLayer != 0x43 || g_spriteFlags != 0xc0 || g_spriteAlpha != 127 || g_clears != 0) ok = false;

    // 4. the emitter already lives: not created again, sprite still drawn
    d.Draw();
    if (g_emittersCreated != 1 || g_spritesDrawn != 2 || g_clears != 0) ok = false;

    // 5. only the combo loading icon timer runs and there is no emitter: no creation, no sprite
    Reset(d, gui, -1, 3, 9, 0, false); d.Draw();
    if (!d.Drawing || g_emittersCreated != 0 || g_spritesDrawn != 0 || g_clears != 0 || d.Effects[0] != 0) ok = false;

    // 6. only the combo timer runs but an emitter lives: sprite drawn, no creation
    Reset(d, gui, -1, 3, 9, &existing, false); d.Draw();
    if (g_emittersCreated != 0 || g_spritesDrawn != 1 || d.Effects[0] != &existing || g_clears != 0) ok = false;

    // 7. no flourish effect: nothing drawn, emitters cleared
    Reset(d, gui, 4, 4, 0, &existing, false); d.Draw();
    if (d.Drawing || g_spritesDrawn != 0 || g_emittersCreated != 0 || g_clears != 1 || d.Effects[0] != 0) ok = false;

    // 8. a different render target moves the anchor
    g_width = 1280; g_height = 720;
    Reset(d, gui, 1, -1, 2, 0, false); d.Draw();
    if (g_emitterStore.Position.X != 1240.0f || g_emitterStore.Position.Y != 680.0f) ok = false;
    if (g_spritePos.X != 1224.0f || g_spritePos.Y != 688.0f) ok = false;

    if (!ok) { std::printf("00646580_TEST FAIL\n"); return 1; }
    std::printf("00646580_TEST PASS\n");
    return 0;
}