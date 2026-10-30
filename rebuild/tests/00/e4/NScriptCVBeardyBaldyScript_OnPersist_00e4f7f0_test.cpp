// Behaviour test for NScript::CV_BeardyBaldyScript::OnPersist (0x00E4F7F0): every persisted member is transferred once, in
// retail's key order, under its member name, with false / 0 / the empty string as the default; a load overwrites each member.
#include <cstdio>
#include <cstring>

static const char* gKeys[32];
static int gCount;
static bool gLoading;

struct Str { const char* p; Str() : p("") { } };

struct Context {
    void Transfer(const char* name, bool& v, const bool& def) { gKeys[gCount++] = name; if (gLoading) v = !def; }
    void Transfer(const char* name, long& v, const long& def) { gKeys[gCount++] = name; if (gLoading) v = def + 7; }
    void Transfer(const char* name, Str& v) { gKeys[gCount++] = name; if (gLoading) v.p = name; }
};

struct BeardyBaldy {
    bool QuestFinished, QuestComplete, QuestActivated;
    long QuestPhase;
    bool InitialisePhase, IntroComplete, Phase1RequirementsComplete, Phase2RequirementsComplete, Phase3RequirementsComplete;
    Str RequiredHairdo, RequiredBeard, RequiredTash;
    bool AcquiredNewHairdo, AcquiredNewBeard, AcquiredNewTash, AllHairChanged, BeardyBaldyKilled, BarberLadyKilled, BeardyBaldyLeft;
    long IncorrectHairComboCount;
    bool AttackedByHero;
    void OnPersist(Context& context)
    {
        context.Transfer("QuestFinished", QuestFinished, false);
        context.Transfer("QuestActivated", QuestActivated, false);
        context.Transfer("QuestComplete", QuestComplete, false);
        context.Transfer("QuestPhase", QuestPhase, 0L);
        context.Transfer("InitialisePhase", InitialisePhase, false);
        context.Transfer("IntroComplete", IntroComplete, false);
        context.Transfer("Phase1RequirementsComplete", Phase1RequirementsComplete, false);
        context.Transfer("Phase2RequirementsComplete", Phase2RequirementsComplete, false);
        context.Transfer("Phase3RequirementsComplete", Phase3RequirementsComplete, false);
        context.Transfer("RequiredHairdo", RequiredHairdo);
        context.Transfer("RequiredBeard", RequiredBeard);
        context.Transfer("RequiredTash", RequiredTash);
        context.Transfer("BeardyBaldyKilled", BeardyBaldyKilled, false);
        context.Transfer("BarberLadyKilled", BarberLadyKilled, false);
        context.Transfer("AcquiredNewHairdo", AcquiredNewHairdo, false);
        context.Transfer("AcquiredNewBeard", AcquiredNewBeard, false);
        context.Transfer("AcquiredNewTash", AcquiredNewTash, false);
        context.Transfer("AllHairChanged", AllHairChanged, false);
        context.Transfer("IncorrectHairComboCount", IncorrectHairComboCount, 0L);
        context.Transfer("AttackedByHero", AttackedByHero, false);
    }
};

int main()
{
    static const char* expected[20] = { "QuestFinished", "QuestActivated", "QuestComplete", "QuestPhase", "InitialisePhase",
        "IntroComplete", "Phase1RequirementsComplete", "Phase2RequirementsComplete", "Phase3RequirementsComplete",
        "RequiredHairdo", "RequiredBeard", "RequiredTash", "BeardyBaldyKilled", "BarberLadyKilled", "AcquiredNewHairdo",
        "AcquiredNewBeard", "AcquiredNewTash", "AllHairChanged", "IncorrectHairComboCount", "AttackedByHero" };
    BeardyBaldy b;
    std::memset(&b, 0, sizeof(b));
    Context c;
    gCount = 0; gLoading = true;
    b.OnPersist(c);
    if (gCount != 20) { std::printf("00e4f7f0_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 20; ++i)
        if (std::strcmp(gKeys[i], expected[i]) != 0) { std::printf("00e4f7f0_TEST FAIL\n"); return 1; }
    if (!b.QuestFinished || !b.AttackedByHero || b.QuestPhase != 7 || b.IncorrectHairComboCount != 7 || b.BeardyBaldyLeft
        || std::strcmp(b.RequiredTash.p, "RequiredTash") != 0) {
        std::printf("00e4f7f0_TEST FAIL\n"); return 1;
    }
    std::printf("00e4f7f0_TEST PASS\n");
    return 0;
}