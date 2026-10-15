// Behaviour test for CTCActionUseReadable::OnActionUse (0x007E80C0): base call always; a live creature gets a read action for the
// text tag only when the readable does not use the info display and the tag is not empty.
#include <cstdio>
static int g_base, g_actions; static const void* g_text;
struct Thing { unsigned char Flags91; bool IsDead() const { return (Flags91 & 1) != 0; } };
struct Action { const void* text; };
struct Creature : Thing { bool SetCurrentAction(const Action& a) { ++g_actions; g_text = a.text; return true; } };
struct WideString { const unsigned short* P; bool operator!=(const unsigned short* t) const { return !(P == 0 ? t[0] == 0 : P[0] == t[0]); } };
struct Def { bool UseInfoDisplay; };
static const unsigned short g_empty[1] = { 0 };
struct Readable {
    const Def* PDef; WideString TextTag;
    void OnActionUse(Thing& user)
    {
        ++g_base;
        if (!PDef->UseInfoDisplay && TextTag != g_empty) {
            Creature* creature = (Creature*)&user;
            if (creature && !creature->IsDead()) { Action a; a.text = &TextTag; creature->SetCurrentAction(a); }
        }
    }
};
int main()
{
    static const unsigned short text[3] = { 'h', 'i', 0 };
    Def d; d.UseInfoDisplay = false; Readable r; r.PDef = &d; r.TextTag.P = text;
    Creature c; c.Flags91 = 0;
    r.OnActionUse(c);
    if (g_base != 1 || g_actions != 1 || g_text != &r.TextTag) { std::printf("007e80c0_TEST FAIL\n"); return 1; }
    c.Flags91 = 1; r.OnActionUse(c);
    d.UseInfoDisplay = true; c.Flags91 = 0; r.OnActionUse(c);
    d.UseInfoDisplay = false; r.TextTag.P = 0; r.OnActionUse(c);
    if (g_base != 4 || g_actions != 1) { std::printf("007e80c0_TEST FAIL\n"); return 1; }
    std::printf("007e80c0_TEST PASS\n");
    return 0;
}
