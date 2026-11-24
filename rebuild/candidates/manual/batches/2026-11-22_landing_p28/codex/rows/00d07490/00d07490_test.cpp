// Standalone fixture with the reconstructed function body and deterministic engine substitutes.
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <string>
#include <vector>

typedef std::string CCharString;
struct TRetVoid { void Return() {} };
enum EHeroAbility { HERO_ABILITY_HEAL_LIFE_SPELL = 14 };
enum ETextGroupSelectionMethod { GROUP_SELECT_NONE };
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_HIGH = 3, SCRIPT_AI_PRIORITY_HIGHEST = 4 };
enum EScriptEntityMoveType { ENTITY_MOVE_WALK, ENTITY_MOVE_RUN };
struct C3DVector {
    float X, Y, Z;
    C3DVector(float x = 0.0f): X(x), Y(0), Z(0) {}
};
struct CRGBColour {
    unsigned char r, g, b, a;
    CRGBColour(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha): r(red), g(green), b(blue), a(alpha) {}
};

struct ScriptFixture {
    std::vector<CCharString> events;
    int frames, stopFrame, calls, stopCheck, starts, startFailures, tasks, near, aware, hit, special, healed, talked;
    int distanceOver, threat, randomValue, following, traderAlive, exp, firstEffectAlive, secondEffectAlive, activeTimers, nextTimer;
    int gateClose, changeRegion, gateLineExpiry, firstGuardComment, advanceGate, forcefieldSpawns, timeReads;
    long remaining[4];
    float health;
    bool* field;
    bool* guardField;
    bool* banditAlert;
    bool* guildWarning;
    CCharString enemyName;
    ScriptFixture(): frames(0), stopFrame(8), calls(0), stopCheck(0), starts(0), startFailures(0), tasks(0),
        near(0), aware(0), hit(0), special(0), healed(0), talked(0), distanceOver(0), threat(0),
        randomValue(1), following(0), traderAlive(1), exp(0), firstEffectAlive(1), secondEffectAlive(1),
        activeTimers(0), nextTimer(0), gateClose(0), changeRegion(0), gateLineExpiry(0), firstGuardComment(0), advanceGate(0),
        forcefieldSpawns(0), timeReads(0), health(1.0f), field(0), guardField(0), banditAlert(0), guildWarning(0),
        enemyName("Enemy") { for (int i=0; i<4; ++i) remaining[i]=0; }
    void record(const CCharString& s) { events.push_back(s); }
    unsigned count(const CCharString& s) const {
        unsigned n=0;
        for (unsigned i=0; i<events.size(); ++i)
            if (events[i]==s) ++n;
        return n;
    }
    bool terminating() { ++calls; return frames >= stopFrame || (stopCheck && calls >= stopCheck); }
} fixture;

enum { HERO=1, ENEMY=2, TRADER=3, LOCATION=4, EFFECT_ONE=5, EFFECT_TWO=6, DOOR=7, SOUND=8 };
struct CScriptThing {
    int id;
    C3DVector position;
    struct { const CScriptThing* Data; } PImp;
    explicit CScriptThing(int which=0): id(which), position(7.0f) { PImp.Data=0; }
    const C3DVector& GetPos() const { return position; }
    float GetAngleXY() const { return id ? 1.5f : 0.0f; }
    bool IsAlive() const {
        if (id==TRADER) return fixture.traderAlive != 0;
        if (id==EFFECT_ONE) return fixture.firstEffectAlive != 0;
        if (id==EFFECT_TWO) return fixture.secondEffectAlive != 0;
        return id != 0;
    }
    bool IsAwareOfHero() const { return fixture.aware != 0; }
    bool MsgIsHitBy(const CCharString&) const { return fixture.hit && fixture.frames==fixture.hit; }
    bool MsgIsHitByAnySpecialAbilityFrom(const CCharString&) const { return fixture.special && fixture.frames==fixture.special; }
    bool MsgIsHitBySpecialAbilityFrom(EHeroAbility ability, const CCharString&) const {
        return fixture.healed && ability==HERO_ABILITY_HEAL_LIFE_SPELL;
    }
    bool MsgIsTalkedToBy(const CCharString&) const { return fixture.talked && fixture.frames>=fixture.talked; }
    void SetFriendsWithEverythingFlag(bool yes) const { if (yes) fixture.record("friendly"); }
    const CCharString& GetName() const { return fixture.enemyName; }
    bool operator==(const CScriptThing& other) const { return id==other.id; }
};

namespace NScript { struct CIsThingAlive { explicit CIsThingAlive(const CScriptThing&) {} }; }
struct CScriptGameResourceObjectMovieBase { ~CScriptGameResourceObjectMovieBase() { fixture.record("movie end"); } };
struct CScriptGameResourceObjectScriptedThingBase {
    bool valid;
    CScriptGameResourceObjectScriptedThingBase(): valid(false) {}
    bool IsValid() const { return valid; }
    void Release() { valid=false; fixture.record("release"); }
    CScriptThing GetScriptThing() const { return CScriptThing(ENEMY); }
    bool IsPerformingScriptTask() const {
        if (fixture.tasks) { --fixture.tasks; return true; }
        return false;
    }
    void Speak(const CScriptThing&, const char* s, ETextGroupSelectionMethod, bool, bool, bool) { fixture.record(s); }
    void MoveToPosition(const C3DVector& pos, float range, EScriptEntityMoveType policy, bool, bool) {
        assert(pos.X==7.0f && range > 0.0f);
        fixture.record(policy==ENTITY_MOVE_RUN ? "run" : "walk");
    }
    void FollowThing(const CScriptThing&, float, bool) { fixture.record("follow"); }
    void ClearCommands() { fixture.record("clear"); }
};
struct FakeGame {
    void NewScriptFrame() const {
        ++fixture.frames;
        fixture.record("frame");
        assert(fixture.frames<30);
        if (fixture.field && fixture.frames==2) *fixture.field=true;
        if (fixture.field && fixture.frames==4) *fixture.field=false;
        if (fixture.guardField && fixture.frames==fixture.advanceGate) *fixture.guardField=true;
        if (fixture.guildWarning && fixture.frames==3) *fixture.guildWarning=false;
        if (fixture.banditAlert && fixture.frames==2) *fixture.banditAlert=true;
        if (fixture.frames==fixture.gateLineExpiry) fixture.remaining[1]=0;
        if (fixture.firstGuardComment && fixture.frames>fixture.firstGuardComment) fixture.remaining[0]=0;
    }
    CScriptThing GetHero() const { return CScriptThing(HERO); }
    bool StartScriptingEntity(const CScriptThing&, CScriptGameResourceObjectScriptedThingBase& handle, EScriptAIPriority priority) const {
        fixture.record(priority==SCRIPT_AI_PRIORITY_HIGHEST ? "control highest" : "control high");
        if (++fixture.starts<=fixture.startFailures) return false;
        handle.valid=true;
        return true;
    }
    void SetQuestCardObjective(const char* quest, const char*, const char*, const char* region) const {
        assert(strcmp(quest,"Q_BanditCamp")==0 && strcmp(region,"BanditCampMain")==0);
        fixture.record("objective");
    }
    void GiveThingBestEnemyTarget(const CScriptThing&, const CScriptThing& target) const { fixture.record(target.id==TRADER ? "target trader" : "target hero"); }
    void ClearThingHasInformation(const CScriptThing&) const { fixture.record("clear info"); }
    long AddNewConversation(const CScriptThing&, bool, bool) const { fixture.record("conversation"); return 11; }
    void AddPersonToConversation(long index, const CScriptThing& person) const { assert(index==11 && person.id==HERO); }
    void AddLineToConversation(long index, const char* line, bool, const CScriptThing&, const CScriptThing& listener) const {
        assert(index==11 && listener.id==HERO);
        fixture.record(line);
    }
    void StartMovieSequence(const char*, CScriptGameResourceObjectMovieBase&) const { fixture.record("movie start"); }
    void PauseAllNonScriptedEntities(bool pause) const { fixture.record(pause ? "pause" : "unpause"); }
    float GetHealth(const CScriptThing&) const { return fixture.health; }
    CScriptThing GetNearestWithScriptName(const CScriptThing&, const char* name) const {
        assert(strcmp(name,"TraderToRescue")==0); return CScriptThing(TRADER);
    }
    bool IsEntityFollowingHero(const CScriptThing&) const { return fixture.following!=0; }
    bool IsThingAwareOfOtherThingInAnyWay(const CScriptThing&, const CScriptThing& target) const { return (fixture.aware & target.id)!=0; }
    void MiniMapAddMarker(const CScriptThing&, const char* marker) const { assert(strcmp(marker,"HUD_ORB_RED_SMALL")==0); fixture.record("mark"); }
    CScriptThing GetThingWithScriptName(const char* name) const {
        fixture.record(name);
        if (strcmp(name,"DoorSoundMarker")==0) return CScriptThing(SOUND);
        if (strcmp(name,"OakValeBanditRaidGate")==0) return CScriptThing(DOOR);
        if (strcmp(name,"DarkwoodRockTroll")==0) return CScriptThing(ENEMY);
        return CScriptThing(LOCATION);
    }
    CScriptThing CreateEffect(const char* name, const C3DVector& position, const char*, float angle, bool, bool) const {
        assert(position.X==7.0f && (angle==0.0f || angle==1.5f));
        fixture.record(name);
        ++fixture.forcefieldSpawns;
        return CScriptThing(strstr(name,"_02") ? EFFECT_TWO : EFFECT_ONE);
    }
    void RemoveThing(const CScriptThing& thing, bool first, bool second) const {
        assert(!first && second);
        fixture.record(thing.id==EFFECT_ONE ? "remove 1" : thing.id==EFFECT_TWO ? "remove 2" : "remove me");
    }
    void DisplayQuestInfo(bool shown) const { fixture.record(shown ? "show quest info" : "hide quest info"); }
    long AddQuestInfoBar(float full, float empty, const CRGBColour& green, const CRGBColour& red, const char* icon, const char*, float scale) const {
        assert(full==30.0f && empty==0.0f && green.g==255 && red.r==255 && scale==1.0f && strcmp(icon,"HUD_CLOCK_ICON")==0);
        fixture.record("create bar"); return 42;
    }
    void UpdateQuestInfoBar(long bar, float, float min, float max) const { assert(bar==42 && min==-1 && max==-1); fixture.record("update bar"); }
    void RemoveQuestInfoElement(long bar) const { assert(bar==42); fixture.record("remove bar"); }
    bool IsRegionLoaded(const char*) const { return !fixture.changeRegion || fixture.frames!=fixture.changeRegion; }
    void OpenDoor(const CScriptThing& door) const { assert(door.id==DOOR); fixture.record("open door"); }
    void EntitySetAsKillable(const CScriptThing&, bool killable, bool last) const { assert(!killable && last); fixture.record("unkillable"); }
    void EntitySetInFaction(const CScriptThing&, const char* faction) const { assert(strcmp(faction,"FACTION_BANDITS")==0); fixture.record("faction"); }
    void EntitySetFacingAngleTowardsThing(const CScriptThing&, const CScriptThing&, bool) const { fixture.record("face hero"); }
    long GetHeroRoughExperienceLevel() const { return fixture.exp; }
} game;
class CWideScreenMagicPauseEntities {
    FakeGame& game;
public:
    explicit CWideScreenMagicPauseEntities(FakeGame& g): game(g) { game.PauseAllNonScriptedEntities(true); }
    ~CWideScreenMagicPauseEntities() { game.PauseAllNonScriptedEntities(false); }
};
class CTimer {
    int index;
public:
    CTimer(): index(fixture.nextTimer++ % 4) { ++fixture.activeTimers; fixture.remaining[index]=0; }
    ~CTimer() { --fixture.activeTimers; }
    CTimer& operator=(long value) { SetTime(value); return *this; }
    void SetTime(long value) {
        fixture.remaining[index]=value;
        fixture.record(value==30 ? "set 30" : value==8 ? "set 8" : value==10 ? "set 10" : value==4 ? "set 4" : "set timer");
    }
    long GetTime() const {
        ++fixture.timeReads;
        if (fixture.gateClose && index==0 && fixture.frames>=fixture.gateClose) return 0;
        return fixture.remaining[index];
    }
};
struct CScriptDef {
    long TollBanditPayDistance, TollBanditTauntTimer;
    float TollBanditCloseToExit, TollBanditTimeToFade;
} scriptDef={12,10,3.0f,2.0f};
CScriptDef* pGlobalScriptDef=&scriptDef;
float g_fZero=0.0f;
bool IsDistanceBetweenThingsUnder(const CScriptThing& a, const CScriptThing& b, float distance) {
    if (distance==15.0f) return a.id && b.id && ((fixture.near & b.id)!=0);
    if (distance==8.0f && fixture.firstGuardComment && fixture.frames>fixture.firstGuardComment) fixture.remaining[0]=0;
    return fixture.near!=0;
}
bool IsDistanceBetweenThingsOver(const CScriptThing&, const CScriptThing&, float) { return fixture.distanceOver!=0; }
bool IsDistanceFromThingToPositionOver(const CScriptThing&, const C3DVector&, float distance) {
    assert(distance==5.0f);
    if (!fixture.distanceOver) return false;
    --fixture.distanceOver;
    return true;
}
void FadeOutAndKillEntity(const CScriptThing&, float seconds, bool kill, bool remove) { assert(seconds==2.0f && kill && remove); fixture.record("fade"); }
int IsPlayerThreateningEntity(const CScriptThing&) { return fixture.threat && fixture.frames < fixture.threat; }
int FakeRand() { return fixture.randomValue; }


namespace NScript {
struct CQ_BanditCampBossBattleScript {
    bool Gate3Open;
    struct CGate3Guard {
        CQ_BanditCampBossBattleScript& ParentClass;
        FakeGame* Game;
        CScriptThing Me;
        CGate3Guard(CQ_BanditCampBossBattleScript& q): ParentClass(q), Game(&game), Me(ENEMY) {}
        void SetPredicate(const CIsThingAlive&) { fixture.record("alive predicate"); }
        bool IsActiveThreadTerminating() { return fixture.terminating(); }
        void Main();
    };
};
}

void NScript::CQ_BanditCampBossBattleScript::CGate3Guard::Main()
{
    TRetVoid _Fail_Val;
    SetPredicate(CIsThingAlive(Me));
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return _Fail_Val.Return();
    CScriptGameResourceObjectScriptedThingBase seh_me;
    if (seh_me.IsValid())
        seh_me.Release();
    while (!Game->StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_HIGHEST)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return _Fail_Val.Return();
    }
    if (IsActiveThreadTerminating())
        return _Fail_Val.Return();
    bool TalkedTo = false;
    bool SpokenToMe = false;
    CTimer MoanTimer;
    MoanTimer = 0;
    Game->SetQuestCardObjective("Q_BanditCamp", "TEXT_QUEST_BANDIT_CAMP_OBJECTIVE_06", "", "BanditCampMain");
    while (!IsActiveThreadTerminating()) {
        if (Me.MsgIsHitBy("SCRIPT_NAME_HERO")
            || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO")
                && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL, "SCRIPT_NAME_HERO"))) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            Game->GiveThingBestEnemyTarget(Me, Game->GetHero());
            if (seh_me.IsValid())
                seh_me.Release();
            Game->ClearThingHasInformation(Me);
            while (true) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            // Inferred: the debug script generator retains this termination check after the idle loop.
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        }
        if (!ParentClass.Gate3Open) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (MoanTimer.GetTime() <= 0 && IsDistanceBetweenThingsUnder(Game->GetHero(), Me, 8.0f)) {
                if (IsActiveThreadTerminating())
                    return _Fail_Val.Return();
                long Conversation = Game->AddNewConversation(Me, false, false);
                Game->AddPersonToConversation(Conversation, Game->GetHero());
                if (!SpokenToMe) {
                    if (IsActiveThreadTerminating())
                        return _Fail_Val.Return();
                    SpokenToMe = true;
                    Game->AddLineToConversation(Conversation, "TEXT_QST_009_BANDIT3_COMMENT_FIRST", false, Me, Game->GetHero());
                } else {
                    if (IsActiveThreadTerminating())
                        return _Fail_Val.Return();
                    Game->AddLineToConversation(Conversation, "TEXT_QST_009_BANDIT3_COMMENT_SECOND", false, Me, Game->GetHero());
                }
                MoanTimer = 10;
            }
            if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
                if (IsActiveThreadTerminating())
                    return _Fail_Val.Return();
                if (!TalkedTo) {
                    if (IsActiveThreadTerminating())
                        return _Fail_Val.Return();
                    if (seh_me.IsValid())
                        seh_me.Release();
                    while (!Game->StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_HIGHEST)) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating())
                            return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating())
                        return _Fail_Val.Return();
                    do {
                        CScriptGameResourceObjectMovieBase speech_movie;
                        Game->StartMovieSequence("", speech_movie);
                        CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                        if (Game->GetHealth(seh_me.GetScriptThing()) > g_fZero) {
                            seh_me.Speak(Game->GetHero(), "TEXT_QST_009_BANDIT3_COMMENT_FIRST_CHAT", GROUP_SELECT_NONE, false, true, false);
                            while (seh_me.IsPerformingScriptTask()) {
                                Game->NewScriptFrame();
                                if (IsActiveThreadTerminating())
                                    return _Fail_Val.Return();
                            }
                            if (IsActiveThreadTerminating())
                                return _Fail_Val.Return();
                        }
                        TalkedTo = true;
                    } while (0);
                } else {
                    if (IsActiveThreadTerminating())
                        return _Fail_Val.Return();
                    if (seh_me.IsValid())
                        seh_me.Release();
                    while (!Game->StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_HIGHEST)) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating())
                            return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating())
                        return _Fail_Val.Return();
                    do {
                        CScriptGameResourceObjectMovieBase speech_movie;
                        Game->StartMovieSequence("", speech_movie);
                        CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                        if (Game->GetHealth(seh_me.GetScriptThing()) > g_fZero) {
                            seh_me.Speak(Game->GetHero(), "TEXT_QST_009_BANDIT3_COMMENT_SECOND_CHAT", GROUP_SELECT_NONE, false, true, false);
                            while (seh_me.IsPerformingScriptTask()) {
                                Game->NewScriptFrame();
                                if (IsActiveThreadTerminating())
                                    return _Fail_Val.Return();
                            }
                            if (IsActiveThreadTerminating())
                                return _Fail_Val.Return();
                        }
                    } while (0);
                }
            }
        }
        Game->NewScriptFrame();
    }
}

static void Run(bool gateOpen)
{
    NScript::CQ_BanditCampBossBattleScript quest;
    quest.Gate3Open=gateOpen;
    fixture.guardField=&quest.Gate3Open;
    NScript::CQ_BanditCampBossBattleScript::CGate3Guard guard(quest);
    guard.Main();
    fixture.guardField=0;
    assert(fixture.activeTimers==0 && fixture.count("pause")==fixture.count("unpause"));
}

int main()
{
    fixture.near=1; fixture.talked=1; fixture.firstGuardComment=1; fixture.advanceGate=5;
    fixture.startFailures=1; fixture.tasks=1;
    Run(false);
    assert(fixture.count("alive predicate")==1 && fixture.count("objective")==1);
    assert(fixture.count("TEXT_QST_009_BANDIT3_COMMENT_FIRST")==1);
    assert(fixture.count("TEXT_QST_009_BANDIT3_COMMENT_SECOND")>=1);
    assert(fixture.count("TEXT_QST_009_BANDIT3_COMMENT_FIRST_CHAT")==1);
    assert(fixture.count("TEXT_QST_009_BANDIT3_COMMENT_SECOND_CHAT")>=1);
    fixture=ScriptFixture(); fixture.hit=1;
    Run(false);
    assert(fixture.count("clear info")==1 && fixture.count("target hero")==1);
    fixture=ScriptFixture(); fixture.special=1; fixture.healed=1;
    Run(true);
    assert(!fixture.count("clear info") && !fixture.count("conversation"));
    fixture=ScriptFixture(); fixture.special=1;
    Run(true);
    assert(fixture.count("clear info")==1);
    fixture=ScriptFixture(); fixture.talked=1; fixture.health=0;
    Run(false);
    assert(fixture.count("movie start") && !fixture.count("TEXT_QST_009_BANDIT3_COMMENT_FIRST_CHAT"));
    for (int stop=1; stop<45; ++stop) {
        fixture=ScriptFixture(); fixture.stopCheck=stop;
        fixture.talked=1; fixture.near=1; fixture.tasks=1; fixture.hit=5;
        Run(false);
    }
    puts("00d07490_TEST PASS");
    return 0;
}
