// Behaviour test for CInputProcessQuickAccessMenu::ProcessInput @ 0x00690FD0 (copy of the model with stub callees).
#include <cstdio>
class CInputEvent { public: int button; long action; };   // 1 pressed, 3 released
class CProcessedInput { public: bool handled; void SetHandled() { handled = true; } };
struct CFrontEndManager { long last; int sends; void SendEvent(long e) { last = e; ++sends; } };
static CFrontEndManager g_fe;
static CFrontEndManager* GetInstance() { return &g_fe; }
class CTCQuickAccessMenu { public:
    bool active, closing;
    bool IsActive() const { return active; }
    bool IsClosing() const { return closing; }
    void RequestClose() { if (active) closing = true; }
};
class CThing { public:
    unsigned long m_tcMask[8]; CTCQuickAccessMenu* menu; unsigned char m_dead : 1;
    unsigned long HasTC(long kind) const { return m_tcMask[kind >> 5] & (1UL << (kind & 31)); }
    CTCQuickAccessMenu* GetTC(long) { return menu; }
};
static CThing* g_hero;
class CInputProcessQuickAccessMenu { public:
    bool IsEventButtonPressed(const CInputEvent& e) { return e.button == 1; }
    bool IsEventButtonReleased(const CInputEvent& e) { return e.button == 3; }
    bool IsEventGameAction(long a, const CInputEvent& e) { return e.action == a; }
    void ProcessInput(const CInputEvent& event, CProcessedInput& processed)
    {
        CThing* hero = g_hero;
        if (!hero || hero->m_dead || !hero->HasTC(0xf6)) return;
        CTCQuickAccessMenu* menu = hero->GetTC(0xf6);
        if (!menu->IsActive() || menu->IsClosing()) return;
        if (IsEventButtonPressed(event)) {
            if (IsEventGameAction(0x25, event)) GetInstance()->SendEvent(0);
            else if (IsEventGameAction(0x26, event)) GetInstance()->SendEvent(1);
            else if (IsEventGameAction(0x27, event)) GetInstance()->SendEvent(2);
            else if (IsEventGameAction(0x28, event)) GetInstance()->SendEvent(3);
            else if (IsEventGameAction(2, event)) { menu->RequestClose(); processed.SetHandled(); return; }
            else return;
        } else if (IsEventButtonReleased(event)) {
            if (IsEventGameAction(0x25, event)) GetInstance()->SendEvent(0);
            else if (IsEventGameAction(0x26, event)) GetInstance()->SendEvent(1);
            else return;
        } else return;
        processed.SetHandled();
    }
};
static CTCQuickAccessMenu g_menu;
static int check(int button, long action, long expect, bool handled)
{
    CInputProcessQuickAccessMenu p; CInputEvent e; CProcessedInput out;
    e.button = button; e.action = action; out.handled = false; g_fe.last = -1; g_fe.sends = 0;
    p.ProcessInput(e, out);
    if (out.handled != handled) return 1;
    if (expect < 0) return g_fe.sends != 0;
    return g_fe.sends != 1 || g_fe.last != expect;
}
int main()
{
    int bad = 0;
    CThing hero; for (int i = 0; i < 8; ++i) hero.m_tcMask[i] = 0;
    hero.m_tcMask[7] = 0x400000; hero.menu = &g_menu; hero.m_dead = 0; g_hero = &hero;
    g_menu.active = true; g_menu.closing = false;
    bad += check(1, 0x25, 0, true); bad += check(1, 0x26, 1, true); bad += check(1, 0x27, 2, true); bad += check(1, 0x28, 3, true);
    bad += check(1, 9, -1, false); bad += check(3, 0x25, 0, true); bad += check(3, 0x26, 1, true); bad += check(3, 0x27, -1, false);
    bad += check(0, 0x25, -1, false);
    bad += check(1, 2, -1, true); if (!g_menu.closing) bad++;
    bad += check(1, 0x25, -1, false);                          // closing: ignored
    g_menu.closing = false; hero.m_dead = 1; bad += check(1, 0x25, -1, false);
    hero.m_dead = 0; hero.m_tcMask[7] = 0; bad += check(1, 0x25, -1, false);
    g_hero = 0; bad += check(1, 0x25, -1, false);
    if (bad) { std::printf("00690fd0_TEST FAIL\n"); return 1; }
    std::printf("00690fd0_TEST PASS\n");
    return 0;
}