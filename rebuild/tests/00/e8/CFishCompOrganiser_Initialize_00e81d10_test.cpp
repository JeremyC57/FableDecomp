// 00e81d10 behaviour test: CFishCompOrganiser::Initialize (match.cpp body: operator[] as *(begin() + n),
// script-def rod/fish defs as vectors), host stand-ins. C++03, g++ -w; also VC7.1 /Od.
#include <stdio.h>
#include <string>
#include <vector>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { printf("FAIL line %d: %s\n", __LINE__, #c); g_fail = 1; } } while (0)
static std::string g_log;
static void logs(const std::string& s) { g_log += s; g_log += ";"; }

struct CCharString {
    std::string S;
    CCharString() { }
    CCharString(const char* s) : S(s) { }
};
struct CScriptThing { std::string Name; };
struct CGameScriptInterface {
    void SetThingHasInformation(const CScriptThing& thing, bool a, bool b, bool c)
    { logs("info " + thing.Name + (a ? " 1" : " 0") + (b ? "1" : "0") + (c ? "1" : "0")); }
    void SetThingPersistent(const CScriptThing& thing, bool persistent) { logs("persistent " + thing.Name + (persistent ? " 1" : " 0")); }
    void SetIsPushableByHero(CScriptThing thing, bool pushable) { logs("pushable " + thing.Name + (pushable ? " 1" : " 0")); }
    void EntitySetAsUseMovementInActions(const CScriptThing& thing, bool use) { logs("movement " + thing.Name + (use ? " 1" : " 0")); }
    void EntityAttachToScript(const CScriptThing& thing, const CCharString& script) { logs("attach " + thing.Name + " " + script.S); }
    CCharString GetActiveQuestName() { return CCharString("Q_FISHING"); }
};
template <class T> struct vector {
    std::vector<T> V;
    T* _M_start;
    vector() : _M_start(0) { }
    void resize(unsigned int n) { V.resize(n); _M_start = n ? &V[0] : 0; }
    T* begin() { return _M_start; }
    T& operator[](unsigned int n) { return *(begin() + n); }
};
struct CScriptDef {
    long FishTypesInGame;
    long FishingRodsInGame;
    long FishingPrizes;
    vector<CCharString> RodDefs;
    vector<CCharString> FishDefs;
};
static CScriptDef* g_pScriptDef;

static int g_doneCalls = 0, g_doneAt = -1;
class CFishCompOrganiser {
public:
    CGameScriptInterface* Game;
    CScriptThing          Me;
    bool                  CommentHitByHero;
    long                  Conversation;
    long                  FishingRodsInGame;
    long                  FishTypesInGame;
    long                  FishingPrizes;
    vector<CCharString>   RodDef;
    vector<CCharString>   FishDef;
    bool IsDone() { return ++g_doneCalls == g_doneAt; }
    void Initialize();
};

void CFishCompOrganiser::Initialize()
{
    Conversation = -1;
    CommentHitByHero = false;
    FishingPrizes = g_pScriptDef->FishingPrizes;
    FishTypesInGame = g_pScriptDef->FishTypesInGame;
    FishingRodsInGame = g_pScriptDef->FishingRodsInGame;
    RodDef.resize(FishingRodsInGame);
    FishDef.resize(FishTypesInGame);
    for (long i = 0; i < FishingRodsInGame; ++i) {
        if (IsDone())
            return;
        RodDef[i] = g_pScriptDef->RodDefs[i];
    }
    if (IsDone())
        return;
    for (long j = 0; j < FishTypesInGame; ++j) {
        if (IsDone())
            return;
        FishDef[j] = g_pScriptDef->FishDefs[j];
    }
    if (IsDone())
        return;
    Game->SetThingHasInformation(Me, false, true, false);
    Game->SetThingPersistent(Me, true);
    Game->SetIsPushableByHero(Me, false);
    Game->EntitySetAsUseMovementInActions(Me, false);
    Game->EntityAttachToScript(Me, Game->GetActiveQuestName());
}

static void reset(CFishCompOrganiser& o) { g_log = ""; g_doneCalls = 0; g_doneAt = -1; o.Conversation = 5; o.CommentHitByHero = true; }

int main()
{
    CScriptDef def;
    def.FishTypesInGame = 3; def.FishingRodsInGame = 2; def.FishingPrizes = 4;
    def.RodDefs.resize(2); def.RodDefs[0] = "ROD_A"; def.RodDefs[1] = "ROD_B";
    def.FishDefs.resize(3); def.FishDefs[0] = "FISH_A"; def.FishDefs[1] = "FISH_B"; def.FishDefs[2] = "FISH_C";
    g_pScriptDef = &def;
    CGameScriptInterface game;
    CFishCompOrganiser o; o.Game = &game; o.Me.Name = "ORG";

    // full run: counts and both def lists copied, then the five set-up calls
    reset(o); o.Initialize();
    CHECK(o.Conversation == -1 && !o.CommentHitByHero);
    CHECK(o.FishingPrizes == 4 && o.FishTypesInGame == 3 && o.FishingRodsInGame == 2);
    CHECK(o.RodDef.V.size() == 2 && o.RodDef[0].S == "ROD_A" && o.RodDef[1].S == "ROD_B");
    CHECK(o.FishDef.V.size() == 3 && o.FishDef[0].S == "FISH_A" && o.FishDef[2].S == "FISH_C");
    CHECK(g_log == "info ORG 010;persistent ORG 1;pushable ORG 0;movement ORG 0;attach ORG Q_FISHING;");
    CHECK(g_doneCalls == 2 + 1 + 3 + 1);

    // done during the rod copy: the second rod is not copied, the lists are already sized, nothing set up
    CFishCompOrganiser p; p.Game = &game; p.Me.Name = "ORG2";
    reset(p); g_doneAt = 2; p.Initialize();
    CHECK(p.RodDef[0].S == "ROD_A" && p.RodDef[1].S == "" && p.FishDef.V.size() == 3 && p.FishDef[0].S == "");
    CHECK(g_log == "" && p.Conversation == -1);

    // done between the loops, then inside the fish loop, then just before the set-up
    CFishCompOrganiser q; q.Game = &game; q.Me.Name = "ORG3";
    reset(q); g_doneAt = 3; q.Initialize();
    CHECK(q.RodDef[1].S == "ROD_B" && q.FishDef[0].S == "" && g_log == "");
    reset(q); q.FishDef[0].S = ""; g_doneAt = 5; q.Initialize();
    CHECK(q.FishDef[0].S == "FISH_A" && q.FishDef[1].S == "" && g_log == "");
    reset(q); g_doneAt = 7; q.Initialize();
    CHECK(q.FishDef[2].S == "FISH_C" && g_log == "");

    // no rods and no fish: only the two between/after checks, then set-up
    def.FishTypesInGame = 0; def.FishingRodsInGame = 0;
    CFishCompOrganiser r; r.Game = &game; r.Me.Name = "ORG4";
    reset(r); r.Initialize();
    CHECK(g_doneCalls == 2 && r.RodDef.V.empty() && g_log.find("attach ORG4 Q_FISHING;") != std::string::npos);

    if (!g_fail) printf("00e81d10_TEST PASS\n");
    return g_fail;
}