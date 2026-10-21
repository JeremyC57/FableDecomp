// Behaviour test for CThing::SetAsDrawable (0x004C8E50): a changed flag is stored and the graphic appearance (when
// present) is flagged changed with reasons 15; an unchanged flag leaves both alone; each present component among
// thing-owner (0x21), carrying (0x46), emote icon (0x45), particle tracker (0xD8) and particle emitter (0x4B) gets the
// flag in that order, absent ones are skipped.
#include <cstdio>
#include <string>

static std::string g_log;
struct Appearance { unsigned long reasons; int calls; };
struct Thing {
    bool drawable; Appearance* appearance; bool has[5];
    void SetAsDrawable(bool d)
    {
        if (drawable != d) { drawable = d; if (appearance) { appearance->reasons = 15; ++appearance->calls; } }
        static const char* names[5] = { "O", "C", "E", "T", "P" };
        for (int i = 0; i < 5; ++i) if (has[i]) { g_log += names[i]; g_log += d ? "1" : "0"; }
    }
};

int main()
{
    int ok = 1;
    Appearance a = { 0, 0 };
    Thing t = { false, &a, { true, false, true, true, false } };
    t.SetAsDrawable(true);
    ok &= t.drawable && a.calls == 1 && a.reasons == 15 && g_log == "O1E1T1";
    g_log = ""; t.SetAsDrawable(true);
    ok &= a.calls == 1 && g_log == "O1E1T1";
    Thing u = { true, 0, { false, true, false, false, true } };
    g_log = ""; u.SetAsDrawable(false);
    ok &= !u.drawable && g_log == "C0P0";
    std::printf(ok ? "004c8e50_TEST PASS\n" : "004c8e50_TEST FAIL\n");
    return ok ? 0 : 1;
}