// Behaviour test for CInputProcessOracleMinigame::ProcessInput @ 0x00691680 (copy of the model with stub callees).
#include <cstdio>
#include <cstring>
struct CInputEvent { int pressed; long action; };
struct CMemoryBuffer { unsigned char buf[8]; int len; void Copy(unsigned long n, const void* d) { std::memcpy(buf + len, d, n); len += (int)n; } };
struct CGameEvent : CMemoryBuffer {
    long Type;
    CGameEvent(long type) : Type(type) { len = 0; }
    void AddLong(long value) { int v = (int)value; Copy(4, &v); }
    void AddBool(bool value) { Copy(1, &value); }
};
struct CProcessedInput {
    bool handled; int events; long type; int len;
    void SetHandled() { handled = true; }
    void AddGameEvent(const CGameEvent& e) { ++events; type = e.Type; len = e.len; }
};
struct CTCOracleMinigame { long last; int quits; void PushSymbolToQueue(long s) { last = s; } void Quit() { ++quits; } };
struct CThing {
    unsigned long m_tcMask[9]; CTCOracleMinigame* oracle;
    unsigned long HasTC(long kind) const { return m_tcMask[kind >> 5] & (1UL << (kind & 31)); }
    CTCOracleMinigame* GetTC(long) { return oracle; }
};
static CThing g_hero;
struct CInputProcessOracleMinigame {
    bool IsEventButtonPressed(const CInputEvent& e) { return e.pressed != 0; }
    bool IsEventGameAction(long a, const CInputEvent& e) { return e.action == a; }
    void ProcessInput(const CInputEvent& event, CProcessedInput& processed)
    {
        if (!IsEventButtonPressed(event)) return;
        CThing* hero = &g_hero;
        CTCOracleMinigame* oracle = 0;
        if (hero->HasTC(0x107)) oracle = hero->GetTC(0x107);
        if (IsEventGameAction(0x42, event)) oracle->PushSymbolToQueue(0);
        else if (IsEventGameAction(0x43, event)) oracle->PushSymbolToQueue(2);
        else if (IsEventGameAction(0x44, event)) oracle->PushSymbolToQueue(3);
        else if (IsEventGameAction(0x45, event)) oracle->PushSymbolToQueue(1);
        else if (IsEventGameAction(0x46, event)) oracle->Quit();
        else if (IsEventGameAction(3, event) && hero->HasTC(0xc6)) {
            CGameEvent gameEvent(0x1a); gameEvent.AddLong(0x17); gameEvent.AddBool(true); processed.AddGameEvent(gameEvent);
        }
        processed.SetHandled();
    }
};
static CTCOracleMinigame g_oracle;
static int sym(long action, long expect)
{
    CInputProcessOracleMinigame p; CInputEvent e; CProcessedInput out; e.pressed = 1; e.action = action;
    out.handled = false; out.events = 0; g_oracle.last = -1; p.ProcessInput(e, out);
    return !out.handled || g_oracle.last != expect;
}
int main()
{
    int bad = 0;
    for (int i = 0; i < 9; ++i) g_hero.m_tcMask[i] = 0;
    g_hero.m_tcMask[8] = 0x80; g_hero.oracle = &g_oracle; g_oracle.quits = 0;
    bad += sym(0x42, 0); bad += sym(0x43, 2); bad += sym(0x44, 3); bad += sym(0x45, 1); bad += sym(0x46, -1); bad += sym(0x99, -1);
    if (g_oracle.quits != 1) bad++;
    CInputProcessOracleMinigame p; CInputEvent e; CProcessedInput out;
    e.pressed = 0; e.action = 0x42; out.handled = false; g_oracle.last = -1; p.ProcessInput(e, out);
    if (out.handled || g_oracle.last != -1) bad++;
    e.pressed = 1; e.action = 3; out.events = 0; p.ProcessInput(e, out); if (out.events != 0 || !out.handled) bad++;
    g_hero.m_tcMask[6] = 0x40; out.events = 0; p.ProcessInput(e, out);
    if (out.events != 1 || out.type != 0x1a || out.len != 5) bad++;
    if (bad) { std::printf("00691680_TEST FAIL\n"); return 1; }
    std::printf("00691680_TEST PASS\n");
    return 0;
}