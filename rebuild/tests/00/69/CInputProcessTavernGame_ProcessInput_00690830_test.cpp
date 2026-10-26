// Behaviour test for CInputProcessTavernGame::ProcessInput @ 0x00690830 (copy of the model with stub callees).
#include <cstdio>
class C2DVector { public: float X, Y; };
class CInputEvent { public: int button; long action; C2DVector MouseMovementXY; };   // 1 pressed, 2 held
class CProcessedInput { public: int dummy; };
struct CFrontEndManager { long last; int sends; C2DVector MouseMovement; void SendEvent(long e) { last = e; ++sends; } };
static CFrontEndManager g_fe;
static CFrontEndManager* GetInstance() { return &g_fe; }
class CInputProcessTavernGame;
class CTCTavernGame { public: int calls; void ProcessInput(const CInputEvent&, CProcessedInput&, CInputProcessTavernGame*) { ++calls; } };
class CTCGamePlayer { public: CTCTavernGame* game; CTCTavernGame* GetGame() { return this ? game : 0; } };
class CThing { public:
    unsigned long m_tcMask[8]; CTCGamePlayer* player;
    unsigned long HasTC(long kind) const { return m_tcMask[kind >> 5] & (1UL << (kind & 31)); }
    CTCGamePlayer* GetTC(long) { return player; }
};
static CThing g_hero;
class CInputProcessTavernGame { public:
    bool IsEventButtonPressed(const CInputEvent& e) { return e.button == 1; }
    bool IsEventButtonHeld(const CInputEvent& e) { return e.button == 2; }
    bool IsEventGameAction(long a, const CInputEvent& e) { return e.action == a; }
    void ProcessInput(const CInputEvent& event, CProcessedInput& processed)
    {
        CThing* hero = &g_hero;
        CTCGamePlayer* player = 0;
        if (hero->HasTC(0xd0)) player = hero->GetTC(0xd0);
        if (player->GetGame()) player->GetGame()->ProcessInput(event, processed, this);
        if (IsEventButtonPressed(event)) {
            if (IsEventGameAction(0x19, event)) GetInstance()->SendEvent(0x1a);
            else if (IsEventGameAction(0x4c, event)) GetInstance()->SendEvent(0x1b);
        } else if (IsEventButtonHeld(event)) {
            if (IsEventGameAction(0x19, event)) GetInstance()->SendEvent(0x1c);
            else if (IsEventGameAction(0x4c, event)) GetInstance()->SendEvent(0x1d);
        } else if (IsEventGameAction(0x47, event)) {
            C2DVector movement = event.MouseMovementXY;
            GetInstance()->MouseMovement = movement;
            GetInstance()->SendEvent(0x19);
        }
    }
};
static int check(int button, long action, long expect)
{
    CInputProcessTavernGame p; CInputEvent e; CProcessedInput out;
    e.button = button; e.action = action; e.MouseMovementXY.X = 3.0f; e.MouseMovementXY.Y = 4.0f;
    g_fe.last = -1; g_fe.sends = 0;
    p.ProcessInput(e, out);
    if (expect < 0) return g_fe.sends != 0;
    if (g_fe.sends != 1 || g_fe.last != expect) return 1;
    if (action == 0x47 && g_fe.MouseMovement.Y != 4.0f) return 1;
    return 0;
}
int main()
{
    int bad = 0;
    CTCTavernGame game; game.calls = 0; CTCGamePlayer player; player.game = &game;
    for (int i = 0; i < 8; ++i) g_hero.m_tcMask[i] = 0;
    g_hero.m_tcMask[6] = 0x10000; g_hero.player = &player;
    bad += check(1, 0x19, 0x1a); bad += check(1, 0x4c, 0x1b); bad += check(1, 0x58, -1);
    bad += check(2, 0x19, 0x1c); bad += check(2, 0x4c, 0x1d); bad += check(0, 0x47, 0x19); bad += check(0, 1, -1);
    if (game.calls != 7) bad++;
    player.game = 0;
    bad += check(1, 0x19, 0x1a);
    if (game.calls != 7) bad++;
    if (bad) { std::printf("00690830_TEST FAIL\n"); return 1; }
    std::printf("00690830_TEST PASS\n");
    return 0;
}