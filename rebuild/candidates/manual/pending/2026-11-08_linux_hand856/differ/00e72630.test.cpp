// Behaviour test for 00e72630 NScript::CV_DemonDoorsScript::CDF_ThreeSuits::Main(): the row body verbatim over a C++98 mock of the script engine (event log); prints 00e72630_TEST PASS
#include <cstdio>
#include <cstdlib>
#include <string>
#include <vector>
#include <map>
static int g_frame = 0, g_limit = 400, g_fail = 0;
static std::vector<std::string> g_ev;
static void ev(const std::string& s) { g_ev.push_back(s); }
static void check(bool c, const char* what) { if (!c) { std::printf("FAIL %s\n", what); ++g_fail; } }
static bool has(const std::string& s) { for (size_t i = 0; i < g_ev.size(); ++i) if (g_ev[i] == s) return true; return false; }
static int count(const std::string& s) { int n = 0; for (size_t i = 0; i < g_ev.size(); ++i) if (g_ev[i] == s) ++n; return n; }
static int pos(const std::string& s) { for (size_t i = 0; i < g_ev.size(); ++i) if (g_ev[i] == s) return (int)i; return -1; }
static std::string num(long v) { char b[32]; std::sprintf(b, "%ld", v); return b; }
static bool in(const std::vector<int>& v, int f) { for (size_t i = 0; i < v.size(); ++i) if (v[i] == f) return true; return false; }
// scenario knobs (each test sets them)
static std::vector<int> g_talk, g_hit, g_ability, g_heal;
static int g_busy_until = 0, g_task_len = 2, g_start_fail = 0, g_starts = 0, g_movies = 0, g_pauses = 0, g_killed = 0;
static float g_health = 1.0f;
static bool g_cutscene = false;
static int (*g_near)(int frame) = 0;      // hero-near predicate
enum ETextGroupSelectionMethod { GROUP_SELECT_NONE=0, GROUP_SELECT_RANDOM=1, GROUP_SELECT_RANDOM_NO_REPEAT=2 };
enum EScriptEntityMoveType { MOVE_WALK=0, MOVE_RUN=1 };
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };
enum EHeroAbility { HERO_ABILITY_NULL=0, HERO_ABILITY_HEAL_LIFE_SPELL=14 };
class TRetVoid { public: void Return() { } };
class CCharString { public:
    std::string s;
    CCharString() { }
    CCharString(const char* t) : s(t) { }
    CCharString(const std::string& t) : s(t) { }
    operator const char*() const { return s.c_str(); }
    bool operator<(const CCharString& o) const { return s < o.s; }
};
static CCharString operator+(const char* a, const CCharString& b) { return CCharString(std::string(a) + b.s); }
static CCharString operator+(const CCharString& a, const char* b) { return CCharString(a.s + b); }
static CCharString GFIntToCharString(long v) { return CCharString(num(v)); }
struct C3DVector { float X, Y, Z; C3DVector() : X(0), Y(0), Z(0) { } C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { } };
enum { ID_NONE = 0, ID_ME = 1, ID_HERO = 2 };
class CScriptThing { public:
    int Id; std::string Name;
    CScriptThing() : Id(ID_NONE) { }
    CScriptThing(int id, const std::string& n) : Id(id), Name(n) { }
    bool operator==(const CScriptThing& o) const { return Id == o.Id && Name == o.Name; }
    bool MsgIsTalkedToBy(const CCharString& w) const { check(w.s == "SCRIPT_NAME_HERO", "talk name"); return Id == ID_ME && in(g_talk, g_frame); }
    bool MsgIsHitBy(const CCharString& w) const { check(w.s == "SCRIPT_NAME_HERO", "hit name"); return Id == ID_ME && in(g_hit, g_frame); }
    bool MsgIsHitByAnySpecialAbilityFrom(const CCharString& w) const { check(w.s == "SCRIPT_NAME_HERO", "ability name"); return Id == ID_ME && (in(g_ability, g_frame) || in(g_heal, g_frame)); }
    bool MsgIsHitBySpecialAbilityFrom(EHeroAbility a, const CCharString& w) const { check(a == HERO_ABILITY_HEAL_LIFE_SPELL && w.s == "SCRIPT_NAME_HERO", "heal query"); return in(g_heal, g_frame); }
    CCharString GetDataString() const { return CCharString("GUARD1"); }
    C3DVector GetPos() const { return C3DVector((float)Name.size(), 0, 0); }
    bool IsAlive() const { return Id != ID_NONE; }
    void SetFriendsWithEverythingFlag(bool b) { ev(std::string("friends:") + (b ? "1" : "0")); }
};
class CScriptGameResourceObjectScriptedThingBase { public:
    bool Valid; int Owner;
    CScriptGameResourceObjectScriptedThingBase() : Valid(false), Owner(ID_NONE) { }
    bool IsValid() const { return Valid; }
    void Release() { check(Valid, "release valid"); Valid = false; ev("release"); }
    CScriptThing GetScriptThing() const { return CScriptThing(Owner, "seh"); }
    bool IsPerformingScriptTask() const { return g_frame < g_busy_until; }
    void Speak(const CScriptThing& to, const char* text, ETextGroupSelectionMethod m, bool a, bool b, bool c) {
        check(to.Id == ID_HERO && !a && b && !c, "speak args"); check(g_pauses == 1 && g_movies >= 1, "speak under paused movie");
        ev(std::string("say:") + text + (m ? ":" + num(m) : std::string(""))); g_busy_until = g_frame + g_task_len; }
    void MoveToPosition(const C3DVector&, float d, EScriptEntityMoveType t, bool a, bool b) { ev("moveto:" + num((long)t) + ":" + num((long)d)); check(!a && b, "move args"); g_busy_until = g_frame + g_task_len; }
    void MoveToThing(const CScriptThing& t, float d, EScriptEntityMoveType m, bool a, bool b, bool c, bool e) { ev("movething:" + t.Name); check(d == 1.0f && m == MOVE_WALK && !a && b && !c && e, "movething args"); g_busy_until = g_frame + g_task_len; }
    void PlayAnimation(const CCharString& n, bool, bool, bool, bool, bool, bool, bool) { ev("anim:" + n.s); }
};
class CScriptGameResourceObjectMovieBase { public:
    CScriptGameResourceObjectMovieBase() { ++g_movies; }
    ~CScriptGameResourceObjectMovieBase() { check(g_pauses == 0, "movie outlives pause"); --g_movies; }
};
struct CScriptDefStub { long GSI_MazeCallsOverDistance; float BowerstoneIntroPoshGuardProximity; };
static CScriptDefStub g_def = { 6, 5.0f };
static CScriptDefStub* pGlobalScriptDef = &g_def;
namespace NGlobalConsole { static bool GDefaultToUsingPhysicsInCutsceneAnimations = true; }
class CGameScriptInterfaceBase { public:
    CScriptThing Hero; long Conv; std::map<long, bool> Active;
    CGameScriptInterfaceBase() : Hero(ID_HERO, "hero"), Conv(0) { }
    void NewScriptFrame() { ++g_frame; }
    bool StartScriptingEntity(const CScriptThing& t, CScriptGameResourceObjectScriptedThingBase& h, EScriptAIPriority p) {
        ++g_starts; if (g_starts <= g_start_fail) return false; h.Valid = true; h.Owner = t.Id; ev("start:" + num(t.Id) + ":" + num((long)p)); return true; }
    const CScriptThing& GetHero() { return Hero; }
    CScriptThing GetThingWithScriptName(const CCharString& n) { return CScriptThing(10 + (int)n.s.size(), n.s); }
    CScriptThing GetNearestWithDefName(const CScriptThing& t, const CCharString& n) { check(t.Id == ID_ME, "nearest of me"); return CScriptThing(9, n.s); }
    void StartMovieSequence(const CCharString& n, CScriptGameResourceObjectMovieBase&) { check(n.s.empty(), "movie name"); ev("movie"); }
    void PauseAllNonScriptedEntities(bool b) { g_pauses += b ? 1 : -1; check(g_pauses == 0 || g_pauses == 1, "pause depth"); }
    void FixMovieSequenceCamera(bool b) { ev(std::string("fixcam:") + (b ? "1" : "0")); }
    float GetHealth(const CScriptThing&) { return g_health; }
    void EntitySetFacingAngleTowardsThing(const CScriptThing& a, const CScriptThing& b, bool snap) { ev("face:" + b.Name + (snap ? ":1" : ":0")); check(a.Id == ID_ME, "face me"); }
    void SetThingHasInformation(const CScriptThing&, bool a, bool b, bool c) { ev(std::string("info:") + (a?"1":"0") + (b?"1":"0") + (c?"1":"0")); }
    void ClearThingHasInformation(const CScriptThing&) { ev("clearinfo"); }
    long AddNewConversation(const CScriptThing& t, bool a, bool b) { check(t.Id == ID_ME && !a && !b, "conv args"); ++Conv; Active[Conv] = true; return Conv; }
    void AddPersonToConversation(long c, const CScriptThing& t) { check(c == Conv && t.Id == ID_HERO, "conv person"); }
    void AddLineToConversation(long c, const CCharString& l, bool b, const CScriptThing& s, const CScriptThing& t) { check(c == Conv && !b && s.Id == ID_ME && t.Id == ID_HERO, "conv line"); ev("line:" + l.s); }
    bool IsConversationActive(long c) { return c != 0 && Active[c]; }
    void EntitySetAsScared(const CScriptThing&, bool b) { ev(std::string("scared:") + (b ? "1" : "0")); }
    bool IsInCutscene() { return g_cutscene; }
    void EntitySetThingAsAllyOfThing(const CScriptThing& a, const CScriptThing& b) { ev("ally:" + num(a.Id) + num(b.Id)); }
    void EntityUnsetThingAsAllyOfThing(const CScriptThing& a, const CScriptThing& b) { ev("unally:" + num(a.Id) + num(b.Id)); }
    void GiveThingBestEnemyTarget(const CScriptThing& a, const CScriptThing& b) { ev("enemy:" + num(a.Id) + num(b.Id)); }
    void ClearThingBestEnemyTarget(const CScriptThing&) { ev("clearenemy"); }
    void SetThingPersistent(const CScriptThing&, bool b) { ev(std::string("persistent:") + (b?"1":"0")); }
    void EntitySetAsDamageable(const CScriptThing&, bool b) { ev(std::string("damageable:") + (b?"1":"0")); }
    void EntitySetAsToAddToComboMultiplierWhenHit(const CScriptThing&, bool b) { ev(std::string("combo:") + (b?"1":"0")); }
    void EntitySetAsKillable(const CScriptThing&, bool a, bool b) { ev(std::string("killable:") + (a?"1":"0") + (b?"1":"0")); }
    void SetRegionExitAsActive(const CScriptThing& t, bool b) { ev("exit:" + t.Name + (b ? ":1" : ":0")); }
    void EntitySetAsOpinionSource(const CScriptThing&, const CCharString& n) { ev("opinion:" + n.s); }
    void MiniMapAddMarker(const CScriptThing&, const CCharString& n) { ev("marker:" + n.s); }
    void SetIsPushableByHero(CScriptThing t, bool b) { check(t.Id == ID_ME, "pushable by value"); ev(std::string("pushable:") + (b?"1":"0")); }
    CCharString GetActiveQuestName() { return CCharString("Q_GuardianSister"); }
    void SetQuestAsCompleted(const CCharString& q, bool, bool, bool) { ev("complete:" + q.s); }
    void DeactivateQuestLater(const CCharString& q, long d) { ev("deactivate:" + q.s + ":" + num(d)); }
    void RemoveThing(const CScriptThing& t, bool a, bool b) { check(t.Id == ID_ME && !a && b, "remove args"); ev("remove"); g_killed = 1; }
};
class CWideScreenMagicPauseEntities { CGameScriptInterfaceBase& G; public:
    CWideScreenMagicPauseEntities(CGameScriptInterfaceBase& g) : G(g) { G.PauseAllNonScriptedEntities(true); }
    ~CWideScreenMagicPauseEntities() { G.PauseAllNonScriptedEntities(false); } };
class CTimer { long Set, At; public: CTimer() : Set(0), At(0) { }
    CTimer& operator=(long t) { Set = t; At = g_frame; ev("timer:" + num(t)); return *this; }
    long GetTime() const { long r = Set - (g_frame - At); return r < 0 ? 0 : r; } };
static bool IsDistanceFromThingToPositionOver(const CScriptThing&, const C3DVector&, float d) { static int n = 0; check(d == 2.0f, "walk radius"); return ++n % 3 != 0; }
static bool IsDistanceBetweenThingsUnder(const CScriptThing& a, const CScriptThing& b, float d) { (void)a; (void)b; ev("dist:" + num((long)d)); return g_near ? g_near(g_frame) != 0 : false; }
static void FadeOutAndKillEntity(const CScriptThing& t, float f, bool a, bool b) { check(t.Id == ID_ME && f == 1.0f && a && b, "fade args"); ev("fadekill"); g_killed = 1; }
typedef std::map<CCharString, CScriptGameResourceObjectScriptedThingBase> CastMap;
static void RunCutsceneMacro(const CCharString& n, CastMap& m, void* f, void* g, bool a, bool b) {
    check(!f && !g && !a && b, "cutscene args"); check(g_pauses == 1, "cutscene paused");
    std::string cast; for (CastMap::iterator i = m.begin(); i != m.end(); ++i) cast += i->first.s + "=" + num(i->second.Owner) + ",";
    ev("cutscene:" + n.s + ":" + cast); }
class CEntityScriptBase { public:
    CGameScriptInterfaceBase* Game; CScriptThing Me; int Predicates;
    CEntityScriptBase() : Game(0), Me(ID_ME, "me"), Predicates(0) { }
    bool IsActiveThreadTerminating() const { return g_frame >= g_limit || g_killed; }
    template <class P> void SetPredicate(const P&) { ++Predicates; ev("predicate"); }
};
namespace NScript { class CIsThingAlive { public: CScriptThing Thing; CIsThingAlive(const CScriptThing& t) : Thing(t) { check(t.Id == ID_ME, "alive of me"); } }; }
using NScript::CIsThingAlive;
static bool g_knight = false, g_mage = false, g_bandit = false;
namespace NScript {
class CV_DemonDoorsScript { public:
    void FadeOutFaceAndOpenDoor(const CScriptThing& t, const CCharString& d) { check(t.Id == ID_ME, "door face"); ev("opendoor:" + d.s); }
    class CDF_ThreeSuits; };
class CV_DemonDoorsScript::CDF_ThreeSuits : public CEntityScriptBase { public:
    CV_DemonDoorsScript& ParentClass; void* PMasterData;
    CDF_ThreeSuits(CV_DemonDoorsScript& p) : ParentClass(p), PMasterData(0) { }
    bool DressedAsBandit() { return g_bandit; }
    bool DressedAsGoodKnight() { return g_knight; }
    bool DressedAsEvilMage() { return g_mage; }
    void Main(); };
}

void NScript::CV_DemonDoorsScript::CDF_ThreeSuits::Main()
{
    CScriptGameResourceObjectScriptedThingBase seh_me;
    TRetVoid _Fail_Val;
    SetPredicate(CIsThingAlive(Me));
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    if (seh_me.IsValid()) seh_me.Release();
    while (!Game->StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGHEST)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    Game->SetThingPersistent(Me,true);
    Game->EntitySetAsDamageable(Me,false);
    Game->EntitySetAsToAddToComboMultiplierWhenHit(Me,false);
    Game->EntitySetAsKillable(Me,false,false);
    CScriptThing region_exit=Game->GetNearestWithDefName(Me,"REGION_EXIT_POINT");
    Game->SetRegionExitAsActive(region_exit,false);
    long state=0;
    while (!IsActiveThreadTerminating()) {
        if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            CScriptGameResourceObjectMovieBase speech_movie;
            Game->StartMovieSequence("",speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
            switch (state) {
            case 0:
                if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
                    seh_me.Speak(Game->GetHero(),"TEXT_QST_088_THREE_SUITS_INTRO",GROUP_SELECT_NONE,false,true,false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                state=1;
            case 1:
                if (DressedAsGoodKnight()) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
                        seh_me.Speak(Game->GetHero(),"TEXT_QST_088_THREE_SUITS_SEEN_KNIGHT",GROUP_SELECT_NONE,false,true,false);
                        while (seh_me.IsPerformingScriptTask()) {
                            Game->NewScriptFrame();
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    state=2;
                } else {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
                        seh_me.Speak(Game->GetHero(),"TEXT_QST_088_THREE_SUITS_WANT_KNIGHT",GROUP_SELECT_NONE,false,true,false);
                        while (seh_me.IsPerformingScriptTask()) {
                            Game->NewScriptFrame();
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    break;
                }
            case 2:
                if (DressedAsEvilMage()) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
                        seh_me.Speak(Game->GetHero(),"TEXT_QST_088_THREE_SUITS_SEEN_MAGE",GROUP_SELECT_NONE,false,true,false);
                        while (seh_me.IsPerformingScriptTask()) {
                            Game->NewScriptFrame();
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    state=3;
                } else {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
                        seh_me.Speak(Game->GetHero(),"TEXT_QST_088_THREE_SUITS_WANT_MAGE",GROUP_SELECT_NONE,false,true,false);
                        while (seh_me.IsPerformingScriptTask()) {
                            Game->NewScriptFrame();
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    break;
                }
            case 3:
                if (DressedAsBandit()) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
                        seh_me.Speak(Game->GetHero(),"TEXT_QST_088_THREE_SUITS_SEEN_BANDIT",GROUP_SELECT_NONE,false,true,false);
                        while (seh_me.IsPerformingScriptTask()) {
                            Game->NewScriptFrame();
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
                        seh_me.Speak(Game->GetHero(),"TEXT_QST_088_THREE_SUITS_OPENING",GROUP_SELECT_NONE,false,true,false);
                        while (seh_me.IsPerformingScriptTask()) {
                            Game->NewScriptFrame();
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    ParentClass.FadeOutFaceAndOpenDoor(Me,"DD_ThreeSuits");
                    Game->SetRegionExitAsActive(region_exit,true);
                    return;
                } else {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
                        seh_me.Speak(Game->GetHero(),"TEXT_QST_088_THREE_SUITS_WANT_BANDIT",GROUP_SELECT_NONE,false,true,false);
                        while (seh_me.IsPerformingScriptTask()) {
                            Game->NewScriptFrame();
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                }
                break;
            }
        }
        Game->NewScriptFrame();
    }
}

int main()
{
    CGameScriptInterfaceBase game; NScript::CV_DemonDoorsScript parent;
    NScript::CV_DemonDoorsScript::CDF_ThreeSuits f(parent); f.Game = &game;
    g_talk.push_back(5); g_talk.push_back(15); g_talk.push_back(25); g_talk.push_back(35); g_talk.push_back(45);
    struct K { static void step(int) { } };
    // talk 1: intro + want knight; talk 2 (knight on): seen knight + want mage; talk 3: (mage on) seen mage + want bandit;
    // talk 4: (bandit on) seen bandit, opening, door
    g_limit = 12; f.Main();
    check(has("persistent:1") && has("damageable:0") && has("combo:0") && has("killable:00") && has("exit:REGION_EXIT_POINT:0"), "setup");
    check(has("say:TEXT_QST_088_THREE_SUITS_INTRO") && has("say:TEXT_QST_088_THREE_SUITS_WANT_KNIGHT"), "first talk");
    g_ev.clear(); g_frame = 0; g_starts = 0; g_limit = 22; g_knight = true; g_talk.clear(); g_talk.push_back(5); g_talk.push_back(15); f.Main();
    check(count("say:TEXT_QST_088_THREE_SUITS_INTRO") == 1 && has("say:TEXT_QST_088_THREE_SUITS_SEEN_KNIGHT") && has("say:TEXT_QST_088_THREE_SUITS_WANT_MAGE"), "knight seen, mage wanted");
    check(count("say:TEXT_QST_088_THREE_SUITS_SEEN_KNIGHT") == 1, "state remembered: knight seen once");
    g_ev.clear(); g_frame = 0; g_starts = 0; g_limit = 60; g_mage = true; g_talk.clear(); g_talk.push_back(5); g_talk.push_back(15); g_bandit = false; f.Main();
    check(has("say:TEXT_QST_088_THREE_SUITS_SEEN_MAGE") && has("say:TEXT_QST_088_THREE_SUITS_WANT_BANDIT") && !has("opendoor:DD_ThreeSuits"), "mage seen, bandit wanted");
    g_ev.clear(); g_frame = 0; g_starts = 0; g_limit = 60; g_bandit = true; g_talk.clear(); g_talk.push_back(5); f.Main();
    check(has("say:TEXT_QST_088_THREE_SUITS_SEEN_BANDIT") && has("say:TEXT_QST_088_THREE_SUITS_OPENING"), "bandit seen, opening");
    check(has("opendoor:DD_ThreeSuits") && has("exit:REGION_EXIT_POINT:1") && g_frame < 60, "door opened, exit active, Main returns");
    check(g_movies == 0 && g_pauses == 0, "balanced");
    if (g_fail) return 1;
    std::printf("00e72630_TEST PASS\n");
    return 0;
}
