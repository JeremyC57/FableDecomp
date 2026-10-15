#include <cstdio>
static int g_base = 0;
struct App { int alpha; void SetAlpha(unsigned char a) { alpha = a; } };
struct Thing { int state; App app; bool dead; int removed; bool killed; void* suspendedBy; bool suspended;
    void RemoveTC(long t) { removed = t; } void Kill(bool) { killed = true; }
    void SetSuspendAnimation(bool s, void* by) { suspended = s; suspendedBy = by; } };
static void Reset(Thing& t, int state) { t.state = state; t.app.alpha = 0; t.dead = false; t.removed = 0; t.killed = false; t.suspendedBy = 0; t.suspended = true; }
struct Mode { Thing* parent; Thing* helper;
    void OnDeactivate()
    {
        ++g_base;
        if (parent->state == 1) parent->SetSuspendAnimation(false, this);
        parent->RemoveTC(0x93);
        parent->app.SetAlpha(0xff);
        if (helper != 0 && !helper->dead) helper->Kill(true);
    } };
int main()
{
    Thing p, h; Reset(p, 1); Reset(h, 0);
    Mode m; m.parent = &p; m.helper = &h; m.OnDeactivate();
    if (g_base != 1 || p.suspended || p.suspendedBy != &m || p.removed != 0x93 || p.app.alpha != 255 || !h.killed) { std::printf("008e7850_TEST FAIL\n"); return 1; }
    Reset(p, 2); Reset(h, 0); h.dead = true; m.OnDeactivate();
    if (!p.suspended || h.killed) { std::printf("008e7850_TEST FAIL\n"); return 1; }
    m.helper = 0; m.OnDeactivate();
    std::printf("008e7850_TEST PASS\n");
    return 0;
}
