// Behaviour test for CInputProcessFishing::ProcessInput @ 0x00690B20 (copy of the model with stub callees).
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
    bool handled; int events; long type; unsigned char data[8]; int len;
    void SetHandled() { handled = true; }
    void AddGameEvent(const CGameEvent& e) { ++events; type = e.Type; len = e.len; std::memcpy(data, e.buf, 8); }
};
struct CTCFishing { int reel, cast; void ReelInButtonPressed() { ++reel; } void CastButtonPressed() { ++cast; } };
struct CThing {
    unsigned long m_tcMask[8]; CTCFishing* fishing;
    unsigned long HasTC(long kind) const { return m_tcMask[kind >> 5] & (1UL << (kind & 31)); }
    CTCFishing* GetTC(long) { return fishing; }
};
static CThing g_hero;
struct CInputProcessFishing {
    bool IsEventButtonPressed(const CInputEvent& e) { return e.pressed != 0; }
    bool IsEventGameAction(long a, const CInputEvent& e) { return e.action == a; }
    void ProcessInput(const CInputEvent& event, CProcessedInput& processed)
    {
        if (!IsEventButtonPressed(event)) return;
        CThing* hero = &g_hero;
        CTCFishing* fishing = 0;
        if (hero->HasTC(0xb1)) fishing = hero->GetTC(0xb1);
        if (IsEventGameAction(0x2b, event)) { fishing->ReelInButtonPressed(); processed.SetHandled(); }
        else if (IsEventGameAction(0x2c, event)) { fishing->CastButtonPressed(); processed.SetHandled(); }
        else if (IsEventGameAction(3, event) && hero->HasTC(0xc6)) {
            CGameEvent gameEvent(0x1a); gameEvent.AddLong(0x17); gameEvent.AddBool(true); processed.AddGameEvent(gameEvent);
        }
    }
};
static void run(int pressed, long action, CProcessedInput& out)
{
    CInputProcessFishing p; CInputEvent e; e.pressed = pressed; e.action = action;
    out.handled = false; out.events = 0; out.len = 0; p.ProcessInput(e, out);
}
int main()
{
    int bad = 0; CProcessedInput out;
    CTCFishing f; f.reel = f.cast = 0;
    for (int i = 0; i < 8; ++i) g_hero.m_tcMask[i] = 0;
    g_hero.m_tcMask[5] = 0x20000; g_hero.fishing = &f;
    run(1, 0x2b, out); if (!out.handled || f.reel != 1) bad++;
    run(1, 0x2c, out); if (!out.handled || f.cast != 1) bad++;
    run(0, 0x2b, out); if (out.handled || f.reel != 1) bad++;
    run(1, 3, out); if (out.events != 0) bad++;
    g_hero.m_tcMask[6] = 0x40;
    run(1, 3, out); if (out.events != 1 || out.type != 0x1a || out.len != 5 || out.data[0] != 0x17 || out.data[4] != 1 || out.handled) bad++;
    run(1, 4, out); if (out.events != 0 || out.handled) bad++;
    if (bad) { std::printf("00690b20_TEST FAIL\n"); return 1; }
    std::printf("00690b20_TEST PASS\n");
    return 0;
}