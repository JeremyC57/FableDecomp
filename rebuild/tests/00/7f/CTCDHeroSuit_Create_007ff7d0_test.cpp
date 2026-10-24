// Behaviour test for 0x007FF7D0 CTCDHeroSuit::Create: activate a thing that has the hero-suit driver bit, kill one without it.
#include <cstdio>
struct C3DVector { float X, Y, Z; };
class CTCDHeroSuit;
static int g_activations = 0, g_kills = 0, g_lastPlayer = -1, g_lastMap = -1; static bool g_lastPersistent = false;
class CThing {
public:
    unsigned int UsingComponents[9];
    void* PTCDriver;
    bool activates;
    static CThing* s_next;
    static CThing* Create(long defIndex, long player, long map, bool levelPersistent)
    { g_lastPlayer = player; g_lastMap = map; g_lastPersistent = levelPersistent; return defIndex ? s_next : 0; }
    bool DoActivationSequence() { ++g_activations; return activates; }
    void Kill(bool) { ++g_kills; }
    bool GetTCDriver(CTCDHeroSuit** out)
    {
        if (UsingComponents[2] & 0x20000000) { *out = (CTCDHeroSuit*)PTCDriver; return true; }
        return false;
    }
};
CThing* CThing::s_next = 0;
class CTCDHeroSuit {
public:
    static CThing* Create(long defIndex, const C3DVector& position)
    {
        CThing* thing = CThing::Create(defIndex, 4, 0, true);
        CThing* result = 0;
        if (thing) {
            CTCDHeroSuit* suit;
            if (thing->GetTCDriver(&suit)) {
                if (thing->DoActivationSequence())
                    result = thing;
            }
            else
                thing->Kill(false);
        }
        return result;
    }
};
int main()
{
    C3DVector pos = { 1.0f, 2.0f, 3.0f };
    CThing t; for (int i = 0; i < 9; ++i) t.UsingComponents[i] = 0; t.PTCDriver = 0; t.activates = true;
    CThing::s_next = &t;
    if (CTCDHeroSuit::Create(0, pos) != 0) { std::printf("007ff7d0_TEST FAIL\n"); return 1; }
    if (CTCDHeroSuit::Create(7, pos) != 0 || g_kills != 1 || g_activations != 0) { std::printf("007ff7d0_TEST FAIL\n"); return 1; }
    t.UsingComponents[2] = 0x20000000;
    if (CTCDHeroSuit::Create(7, pos) != &t || g_activations != 1 || g_lastPlayer != 4 || g_lastMap != 0 || !g_lastPersistent) { std::printf("007ff7d0_TEST FAIL\n"); return 1; }
    t.activates = false;
    if (CTCDHeroSuit::Create(7, pos) != 0 || g_kills != 1) { std::printf("007ff7d0_TEST FAIL\n"); return 1; }
    std::printf("007ff7d0_TEST PASS\n");
    return 0;
}