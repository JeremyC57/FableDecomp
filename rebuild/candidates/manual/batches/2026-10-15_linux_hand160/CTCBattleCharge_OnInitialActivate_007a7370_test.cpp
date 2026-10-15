// Behaviour test for CTCBattleCharge::OnInitialActivate (0x007A7370): mode 0x23 added when the owner has a mode manager; a live
// player creature gets its animation suspended, anything else resets the charge.
#include <cstdio>
static unsigned int g_mode; static int g_suspend, g_reset;
struct Modes { bool AddMode(unsigned int m) { g_mode = m; return true; } };
struct Thing {
    unsigned char m_type; unsigned int bits[9]; Modes* modes; unsigned char m_dead;
    bool GetInterface(Modes** out, int type) { if (bits[type >> 5] & (1u << (type & 31))) { *out = modes; return true; } return false; }
    void SetSuspendAnimation(bool s, bool i) { if (s && !i) ++g_suspend; }
};
struct Charge {
    Thing* ParentThing;
    void Reset() { ++g_reset; }
    void OnInitialActivate()
    {
        Thing* thing = ParentThing; Modes* modes;
        if (thing->GetInterface(&modes, 0x31)) modes->AddMode(0x23);
        if (!thing->m_dead && thing->m_type == 1) ParentThing->SetSuspendAnimation(true, false); else Reset();
    }
};
int main()
{
    Modes m; Thing t; for (int i = 0; i < 9; ++i) t.bits[i] = 0; t.bits[1] = 1u << 17; t.modes = &m; t.m_type = 1; t.m_dead = 0;
    Charge c; c.ParentThing = &t; c.OnInitialActivate();
    if (g_mode != 0x23 || g_suspend != 1 || g_reset != 0) { std::printf("007a7370_TEST FAIL\n"); return 1; }
    t.m_type = 2; t.bits[1] = 0; g_mode = 0; c.OnInitialActivate();
    if (g_mode != 0 || g_reset != 1) { std::printf("007a7370_TEST FAIL\n"); return 1; }
    std::printf("007a7370_TEST PASS\n");
    return 0;
}
