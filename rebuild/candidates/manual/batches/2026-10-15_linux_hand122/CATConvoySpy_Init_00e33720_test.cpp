// Behaviour test for CAT_ConvoySpy::Init (0x00E33720).
#include <cstdio>

struct SCountBlock { long m_count; };
struct CScriptThing {
    int id; SCountBlock* PImp_Info;
    CScriptThing(int i, SCountBlock* b) : id(i), PImp_Info(b) {}
    CScriptThing(const CScriptThing& o) : id(o.id), PImp_Info(o.PImp_Info) { if (PImp_Info) ++PImp_Info->m_count; }
    ~CScriptThing() { if (PImp_Info) --PImp_Info->m_count; }
};

struct CGameScriptInterface {
    CScriptThing hero; int info[4]; int allies[4][2]; int nAllies; int pushable;
    CGameScriptInterface() : hero(1, 0), nAllies(0), pushable(-1) {}
    const CScriptThing& GetHero() { return hero; }
    void SetThingHasInformation(const CScriptThing& t, bool a, bool b, bool c) { info[0] = t.id; info[1] = a; info[2] = b; info[3] = c; }
    void EntitySetThingAsAllyOfThing(const CScriptThing& t, const CScriptThing& ally) { allies[nAllies][0] = t.id; allies[nAllies][1] = ally.id; ++nAllies; }
    void SetIsPushableByHero(CScriptThing t, bool p) { pushable = p ? t.id : -t.id; }
};

struct CQ_AmbushTradersScript { bool ConvoySpyActive; int resets; void ResetRandomSpeechTime() { ++resets; } };

struct CAT_ConvoySpy {
    CGameScriptInterface* GSI; CScriptThing Thing; CQ_AmbushTradersScript* ParentClass; long SpeechCounterA, SpeechCounterB;
    CAT_ConvoySpy(CGameScriptInterface* g, SCountBlock* b) : GSI(g), Thing(7, b) {}
    void Init()
    {
        ParentClass->ConvoySpyActive = false;
        ParentClass->ResetRandomSpeechTime();
        SpeechCounterA = 10;
        SpeechCounterB = 10;
        GSI->SetThingHasInformation(Thing, false, true, false);
        GSI->EntitySetThingAsAllyOfThing(Thing, GSI->GetHero());
        GSI->EntitySetThingAsAllyOfThing(GSI->GetHero(), Thing);
        GSI->SetIsPushableByHero(Thing, false);
    }
};

int main()
{
    SCountBlock block; block.m_count = 1;
    CGameScriptInterface gsi;
    CQ_AmbushTradersScript parent; parent.ConvoySpyActive = true; parent.resets = 0;
    CAT_ConvoySpy spy(&gsi, &block); spy.ParentClass = &parent;
    spy.Init();
    if (parent.ConvoySpyActive || parent.resets != 1 || spy.SpeechCounterA != 10 || spy.SpeechCounterB != 10
        || gsi.info[0] != 7 || gsi.info[1] || !gsi.info[2] || gsi.info[3] || gsi.nAllies != 2
        || gsi.allies[0][0] != 7 || gsi.allies[0][1] != 1 || gsi.allies[1][0] != 1 || gsi.allies[1][1] != 7
        || gsi.pushable != -7 || block.m_count != 1) {
        std::printf("00e33720_TEST FAIL\n");
        return 1;
    }
    std::printf("00e33720_TEST PASS\n");
    return 0;
}
