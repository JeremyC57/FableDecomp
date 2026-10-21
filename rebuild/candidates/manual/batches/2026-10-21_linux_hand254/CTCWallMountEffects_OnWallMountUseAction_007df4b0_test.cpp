// Behaviour test for CTCWallMountEffects::OnWallMountUseAction (0x007DF4B0): false (and nothing shown) until more than
// SecondsBetweenEffectUsesAllowed * fps frames have passed since the last use; a teleport/heal mount shows its
// question with "", "Yes", "No" (colour 4, time 0, false, true), remembers the creature and waits for confirmation;
// a mount without an effect returns false and changes nothing.
#include <cstdio>
#include <cstring>
#include <string>

static long g_frame = 0;
static long g_framesPerSecond = 30;
struct CThingCreatureBase { int id; };

static std::string g_text, g_a, g_b, g_c;
static int g_shown = 0;
static void SetText(const std::string& text, const std::string& a, const std::string& b, const std::string& c,
                    unsigned long colour, float time, bool b1, bool b2)
{
    ++g_shown; g_text = text; g_a = a; g_b = b; g_c = c;
    if (colour != 4 || time != 0.0f || b1 || !b2) g_text = "BAD ARGS";
}

struct CWallMountEffectsDef { long Effect; float HealingAmount; long SecondsBetweenEffectUsesAllowed; };
struct CTCWallMountEffects {
    const CWallMountEffectsDef* PDef;
    long WaitingForConfirmation;
    CThingCreatureBase* CreaturePerformingAction;
    long FrameEffectLastUsed;
    bool IsAllowedToUseEffect() const
    {
        const CWallMountEffectsDef* def = PDef;
        long fps = g_framesPerSecond;
        return g_frame - FrameEffectLastUsed > def->SecondsBetweenEffectUsesAllowed * fps;
    }
    bool OnWallMountUseAction(CThingCreatureBase& creature)
    {
        if (!IsAllowedToUseEffect())
            return false;
        std::string question("");
        switch (PDef->Effect) {
        case 1: question = "Do you want to teleport?"; break;
        case 2: question = "Do you want to heal yourself?"; break;
        }
        if (question != "") {
            SetText(question, "", "Yes", "No", 4, 0.0f, false, true);
            CreaturePerformingAction = &creature;
            WaitingForConfirmation = 1;
            return true;
        }
        return false;
    }
};

int main()
{
    int ok = 1;
    CWallMountEffectsDef tele = { 1, 0.0f, 2 }, none = { 0, 0.0f, 0 };
    CThingCreatureBase hero = { 7 };
    CTCWallMountEffects m = { &tele, 0, 0, 100 };
    g_frame = 160;                                   // 60 frames passed, needs > 60
    ok &= !m.OnWallMountUseAction(hero) && g_shown == 0 && m.WaitingForConfirmation == 0;
    g_frame = 161;
    ok &= m.OnWallMountUseAction(hero) && g_shown == 1 && g_text == "Do you want to teleport?";
    ok &= g_a == "" && g_b == "Yes" && g_c == "No" && m.CreaturePerformingAction == &hero && m.WaitingForConfirmation == 1;
    CWallMountEffectsDef heal = { 2, 5.0f, 0 };
    CTCWallMountEffects h = { &heal, 0, 0, 0 };
    g_frame = 1;
    ok &= h.OnWallMountUseAction(hero) && g_text == "Do you want to heal yourself?";
    CTCWallMountEffects n = { &none, 0, 0, 0 };
    ok &= !n.OnWallMountUseAction(hero) && g_shown == 2 && n.CreaturePerformingAction == 0;
    std::printf(ok ? "007df4b0_TEST PASS\n" : "007df4b0_TEST FAIL\n");
    return ok ? 0 : 1;
}
