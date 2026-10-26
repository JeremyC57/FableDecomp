// Behaviour test for CPlayerGui::GetButtonAlpha (0x00439AB0): during a normal/special swap the alpha is |scale|
// (special) or 1 - |scale|; otherwise a context-sensitive request without button alpha asks the player character's
// inventory (1.0 when there is no live character with an inventory); with button alpha the result is multiplied by
// the ability (special) or quick-access alpha of the button.
#include <cstdio>
#include <cmath>

class CTCInventory { public: float fade; float GetContextSensitiveFadeAlpha(long b) { return fade + (float)b; } };
class CThing { public: bool Dead; CTCInventory* inv; bool HasTC(long t) const { return t == 0x11 && inv; } CTCInventory* GetTC(long) const { return inv; } };
class CPlayer { public: CThing* c; CThing* GetPlayerCharacter() { return c; } };
class CPlayerManager { public: CPlayer* p; CPlayer* GetMainPlayer() { return p; } };
template <class T> struct vector { T* _M_start; T& operator[](long n) { return *(_M_start + n); } };
class CPlayerGui { public:
    CPlayerManager* PlayerManager;
    vector<float> AbilityButtonsAlpha, QuickAccessButtonsAlpha;
    float AbilityButtonScale; bool NormalToSpecial, SpecialToNormal;
    float GetButtonAlpha(long button, bool applyButtonAlpha, bool special, bool contextSensitive);
};
float CPlayerGui::GetButtonAlpha(long button, bool applyButtonAlpha, bool special, bool contextSensitive)
{
    float alpha = 1.0f;
    if (NormalToSpecial || SpecialToNormal) {
        alpha = (float)std::fabs(AbilityButtonScale);
        if (!special)
            alpha = 1.0f - alpha;
    } else if (contextSensitive && !applyButtonAlpha) {
        CThing* character = PlayerManager->GetMainPlayer()->GetPlayerCharacter();
        if (character && !character->Dead && character->HasTC(0x11))
            return character->GetTC(0x11)->GetContextSensitiveFadeAlpha(button);
        goto done;
    }
    if (applyButtonAlpha) {
        if (special)
            return alpha * AbilityButtonsAlpha[button];
        return alpha * QuickAccessButtonsAlpha[button];
    }
done:
    return alpha;
}
static bool Near(float a, float b) { return std::fabs(a - b) < 1e-5f; }
int main()
{
    int bad = 0;
    float ab[3] = { 0.5f, 0.25f, 0.125f }, qa[3] = { 0.75f, 0.2f, 0.1f };
    CTCInventory inv = { 0.25f }; CThing ch = { false, &inv }; CPlayer pl = { &ch }; CPlayerManager pm = { &pl };
    CPlayerGui g; g.PlayerManager = &pm; g.AbilityButtonsAlpha._M_start = ab; g.QuickAccessButtonsAlpha._M_start = qa;
    g.AbilityButtonScale = -0.4f; g.NormalToSpecial = true; g.SpecialToNormal = false;
    if (!Near(g.GetButtonAlpha(1, false, true, false), 0.4f) || !Near(g.GetButtonAlpha(1, false, false, false), 0.6f)) bad = 1;
    if (!Near(g.GetButtonAlpha(1, true, true, true), 0.4f * 0.25f) || !Near(g.GetButtonAlpha(2, true, false, false), 0.6f * 0.1f)) bad = 1;
    g.NormalToSpecial = false;
    if (!Near(g.GetButtonAlpha(1, false, false, true), 1.25f)) bad = 1;           // inventory fade
    ch.Dead = true;
    if (!Near(g.GetButtonAlpha(1, false, false, true), 1.0f)) bad = 1;            // no live character
    if (!Near(g.GetButtonAlpha(0, true, true, true), 0.5f) || !Near(g.GetButtonAlpha(0, true, false, false), 0.75f)) bad = 1;
    if (!Near(g.GetButtonAlpha(0, false, false, false), 1.0f)) bad = 1;
    if (bad) { std::printf("00439ab0_TEST FAIL\n"); return 1; }
    std::printf("00439ab0_TEST PASS\n");
    return 0;
}