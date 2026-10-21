// Behaviour test for CInputProcessProjectileTargetingAnalogueZoom::ProcessInput @ 0x0068F320 (model copy, stub callees).
#include <cstdio>
struct C2DVector { float x, y; };
struct CInputEvent { int pressed; long action; bool analogue; C2DVector v; };
struct CProcessedInput { bool handled; void SetHandled() { handled = true; } };
struct CTCProjectileZoom {
    float zoom;
    void AdjustZoom(float d) { zoom += d; if (zoom < 0.0f) zoom = 0.0f; else if (zoom > 1.0f) zoom = 1.0f; }
};
struct CThing {
    unsigned long m_tcMask[9]; CTCProjectileZoom* z; unsigned char m_dead : 1;
    unsigned long HasTC(long kind) const { return m_tcMask[kind >> 5] & (1UL << (kind & 31)); }
    CTCProjectileZoom* GetTC(long kind) { CTCProjectileZoom* tc = 0; if (HasTC(kind)) tc = z; return tc; }
};
static CThing* g_hero;
struct CInputProcessProjectileTargetingAnalogueZoom {
    bool IsEventButtonPressed(const CInputEvent& e) { return e.pressed != 0; }
    bool IsEventGameAction(long a, const CInputEvent& e) { return e.action == a; }
    bool GetEventGameActionVector(long a, const CInputEvent& e, C2DVector* v) { if (!e.analogue || e.action != a) return false; *v = e.v; return true; }
    void ProcessInput(const CInputEvent& event, CProcessedInput& processed)
    {
        CThing* hero = g_hero;
        if (!hero || hero->m_dead) return;
        C2DVector zoom;
        if (GetEventGameActionVector(0x54, event, &zoom)) hero->GetTC(0x81)->AdjustZoom(zoom.y * (1.0f / 255.0f));
        else if (IsEventGameAction(0x6e, event)) {
            if (!IsEventButtonPressed(event)) return;
            hero->GetTC(0x81)->AdjustZoom(196.0f / 255.0f);
        } else if (IsEventGameAction(0x6f, event) && IsEventButtonPressed(event)) hero->GetTC(0x81)->AdjustZoom(-196.0f / 255.0f);
        else return;
        processed.SetHandled();
    }
};
static bool run(int pressed, long action, bool analogue, float y)
{
    CInputProcessProjectileTargetingAnalogueZoom p; CInputEvent e; CProcessedInput out;
    e.pressed = pressed; e.action = action; e.analogue = analogue; e.v.x = 0; e.v.y = y; out.handled = false;
    p.ProcessInput(e, out); return out.handled;
}
static bool near(float a, float b) { return a - b < 0.0001f && b - a < 0.0001f; }
int main()
{
    int bad = 0;
    CTCProjectileZoom z; z.zoom = 0.0f;
    CThing hero; for (int i = 0; i < 9; ++i) hero.m_tcMask[i] = 0; hero.m_tcMask[4] = 2; hero.z = &z; hero.m_dead = 0; g_hero = &hero;
    if (!run(0, 0x54, true, 51.0f) || !near(z.zoom, 0.2f)) bad++;
    if (!run(1, 0x6e, false, 0) || !near(z.zoom, 0.2f + 196.0f / 255.0f)) bad++;
    if (run(0, 0x6e, false, 0) || !near(z.zoom, 0.2f + 196.0f / 255.0f)) bad++;
    if (!run(1, 0x6f, false, 0) || !near(z.zoom, 0.2f)) bad++;
    if (run(0, 0x6f, false, 0)) bad++;
    if (run(1, 0x70, false, 0)) bad++;
    hero.m_dead = 1; if (run(1, 0x6e, false, 0)) bad++;
    g_hero = 0; if (run(1, 0x6e, false, 0)) bad++;
    if (bad) { std::printf("0068f320_TEST FAIL\n"); return 1; }
    std::printf("0068f320_TEST PASS\n");
    return 0;
}
