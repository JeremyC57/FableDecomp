// Behaviour test for CTCQuestCard::OnSerialise (0x00702720): nothing is transferred when the flag is false; otherwise the ten
// fields go through the context in order with false/0 defaults and the def's rewards as the reward defaults.
#include <cstdio>
#include <cstring>
struct CCharString { void* PStringData; };
static const char* g_names[16]; static const void* g_vals[16]; static long g_defs[16]; static int g_n = 0;
struct CPersistContext {
    void Transfer(const char* name, bool& value, const bool& defaultValue) { g_names[g_n] = name; g_vals[g_n] = &value; g_defs[g_n++] = defaultValue ? 1 : 0; }
    void Transfer(const char* name, long& value, const long& defaultValue) { g_names[g_n] = name; g_vals[g_n] = &value; g_defs[g_n++] = defaultValue; }
    void Transfer(const char* name, CCharString* value) { g_names[g_n] = name; g_vals[g_n] = value; g_defs[g_n++] = -1; }
};
struct CQuestCardDef { long RenownReward; long GoldReward; };
template <class T> struct CDefPointer {
    const T* Object;
    const T* operator->() const { return Object; }
};
struct CTCQuestCard {
    CDefPointer<CQuestCardDef> PDef;
    bool          Finished;
    bool          Replayable;
    long          FeatIndex;
    CCharString   QuestName;
    CCharString   ObjectiveName;
    CCharString   RegionName;
    CCharString   TeleporterRegionName;
    CCharString   ScriptQuestName;
    long          GoldReward;
    long          RenownReward;
    void OnSerialise(CPersistContext& context, bool flag);
};

void CTCQuestCard::OnSerialise(CPersistContext& context, bool flag)
{
    if (flag) {
        context.Transfer("Finished", Finished, false);
        context.Transfer("Replayable", Replayable, false);
        context.Transfer("FeatIndex", FeatIndex, 0);
        context.Transfer("QuestName", &QuestName);
        context.Transfer("ObjectiveName", &ObjectiveName);
        context.Transfer("RegionName", &RegionName);
        context.Transfer("TeleporterRegionName", &TeleporterRegionName);
        context.Transfer("ScriptQuestName", &ScriptQuestName);
        context.Transfer("GoldReward", GoldReward, PDef->GoldReward);
        context.Transfer("RenownReward", RenownReward, PDef->RenownReward);
    }
}

int main()
{
    int bad = 0;
    CQuestCardDef def; def.RenownReward = 30; def.GoldReward = 500;
    CTCQuestCard card; card.PDef.Object = &def;
    CPersistContext ctx;
    card.OnSerialise(ctx, false);
    if (g_n != 0) bad |= 1;
    card.OnSerialise(ctx, true);
    static const char* expected[10] = { "Finished", "Replayable", "FeatIndex", "QuestName", "ObjectiveName", "RegionName",
                                        "TeleporterRegionName", "ScriptQuestName", "GoldReward", "RenownReward" };
    const void* fields[10] = { &card.Finished, &card.Replayable, &card.FeatIndex, &card.QuestName, &card.ObjectiveName,
                               &card.RegionName, &card.TeleporterRegionName, &card.ScriptQuestName, &card.GoldReward, &card.RenownReward };
    if (g_n != 10) bad |= 2;
    for (int i = 0; i < 10 && i < g_n; ++i)
        if (std::strcmp(g_names[i], expected[i]) != 0 || g_vals[i] != fields[i]) bad |= 4;
    if (g_defs[0] != 0 || g_defs[1] != 0 || g_defs[2] != 0 || g_defs[3] != -1 || g_defs[8] != 500 || g_defs[9] != 30) bad |= 8;
    if (bad) { std::printf("00702720_TEST FAIL\n"); return 1; }
    std::printf("00702720_TEST PASS\n");
    return 0;
}