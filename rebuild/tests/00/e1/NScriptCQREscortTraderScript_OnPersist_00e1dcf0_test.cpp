// Behaviour test for NScript::CQR_EscortTraderScript::OnPersist (0x00E1DCF0): the quest stage and the two region names go
// through the two-argument Transfer (default from GetDefaultVal<T>), every other member through Transfer with an explicit
// zero / false default, 22 keys in retail order.
#include <cstdio>
#include <cstring>

static const char* gKeys[32];
static int gCount;
static bool gLoad;

class CCharString { public: const char* Text; };

class CPersistContext { public:
    template <class T> static T GetDefaultVal() { T v; std::memset(&v, 0, sizeof(v)); return v; }
    template <class T> void Transfer(const char* name, T& value, const T& defaultValue)
    {
        if (gCount < 32) gKeys[gCount] = name;
        ++gCount;
        if (gLoad) value = defaultValue;
    }
    template <class T> void Transfer(const char* name, T& value) { Transfer(name, value, GetDefaultVal<T>()); }
};

struct CQR_EscortTraderScript {
    enum EscortTraderQuestStage { STAGE_NONE, STAGE_ESCORT };
    EscortTraderQuestStage QuestStage;
    CCharString PreviousLoadedRegion, CurrentLoadedRegion;
    bool GreetedHero, HiredByTrader, TraderDeparted, TraderIntroDone, TraderDepartedLost, DoneExpressions, TraderHit,
         TraderCreated, WrongTurnDetected, TheBlackGateIsClosed, ExpressionDetectedFollow, ExpressionDetectedWait,
         AddedQuestInfo, GoneWrongWay, TotallyLost, AddedBonusObject;
    long TraderHitCount, TraderHealthBarID;
    float TraderInitialHealth;
    void OnPersist(CPersistContext& context)
    {
        context.Transfer("QuestStage", QuestStage);
        context.Transfer("GreetedHero", GreetedHero, false);
        context.Transfer("PreviousLoadedRegion", PreviousLoadedRegion);
        context.Transfer("CurrentLoadedRegion", CurrentLoadedRegion);
        context.Transfer("HiredByTrader", HiredByTrader, false);
        context.Transfer("TraderDeparted", TraderDeparted, false);
        context.Transfer("TraderIntroDone", TraderIntroDone, false);
        context.Transfer("TraderDepartedLost", TraderDepartedLost, false);
        context.Transfer("DoneExpressions", DoneExpressions, false);
        context.Transfer("TraderHit", TraderHit, false);
        context.Transfer("TraderHitCount", TraderHitCount, 0L);
        context.Transfer("TraderCreated", TraderCreated, false);
        context.Transfer("WrongTurnDetected", WrongTurnDetected, false);
        context.Transfer("TraderInitialHealth", TraderInitialHealth, 0.0f);
        context.Transfer("TraderHealthBarID", TraderHealthBarID, 0L);
        context.Transfer("TheBlackGateIsClosed", TheBlackGateIsClosed, false);
        context.Transfer("ExpressionDetectedFollow", ExpressionDetectedFollow, false);
        context.Transfer("ExpressionDetectedWait", ExpressionDetectedWait, false);
        context.Transfer("AddedQuestInfo", AddedQuestInfo, false);
        context.Transfer("GoneWrongWay", GoneWrongWay, false);
        context.Transfer("TotallyLost", TotallyLost, false);
        context.Transfer("AddedBonusObject", AddedBonusObject, false);
    }
};

int main()
{
    CQR_EscortTraderScript s;
    s.QuestStage = CQR_EscortTraderScript::STAGE_ESCORT; s.CurrentLoadedRegion.Text = "Bowerstone"; s.TraderHitCount = 4;
    s.TraderInitialHealth = 80.0f; s.AddedBonusObject = true;
    CPersistContext c;
    gCount = 0; gLoad = false;
    s.OnPersist(c);
    if (gCount != 22 || std::strcmp(gKeys[0], "QuestStage") != 0 || std::strcmp(gKeys[3], "CurrentLoadedRegion") != 0
        || std::strcmp(gKeys[13], "TraderInitialHealth") != 0 || std::strcmp(gKeys[21], "AddedBonusObject") != 0
        || s.TraderHitCount != 4) { std::printf("00e1dcf0_TEST FAIL\n"); return 1; }
    gCount = 0; gLoad = true;
    s.OnPersist(c);
    if (gCount != 22 || s.QuestStage != CQR_EscortTraderScript::STAGE_NONE || s.CurrentLoadedRegion.Text != 0
        || s.TraderHitCount != 0 || s.TraderInitialHealth != 0.0f || s.AddedBonusObject) { std::printf("00e1dcf0_TEST FAIL\n"); return 1; }
    std::printf("00e1dcf0_TEST PASS\n");
    return 0;
}