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
struct CQ_OakValeBanditRaidScript {
    FakeGame* Game;
    bool GateClosed, BanditsAwareOfPlayerAgain, OpenGates, NoPersistence, BanditsKilled;
    CQ_OakValeBanditRaidScript(): Game(&game), GateClosed(false), BanditsAwareOfPlayerAgain(false), OpenGates(false), NoPersistence(false), BanditsKilled(false) {}
    bool IsActiveThreadTerminating() { return fixture.terminating(); }
    bool PopulateLevel() { fixture.record("populate"); return true; }
    void WatchForGateTimer();
};
}

void NScript::CQ_OakValeBanditRaidScript::WatchForGateTimer()
{
    TRetVoid _Fail_Val;
    CScriptThing Gate = Game->GetThingWithScriptName("OakValeBanditRaidGate");
    CScriptThing SoundMarker = Game->GetThingWithScriptName("DoorSoundMarker");
    for (;; Game->NewScriptFrame()) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        while (!GateClosed) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return _Fail_Val.Return();
        }
        if (IsActiveThreadTerminating())
            return _Fail_Val.Return();
        CTimer GateTimer;
        CTimer ShoutTimer;
        Game->DisplayQuestInfo(true);
        bool LeftRegion = false;
        bool Shouted = false;
        GateTimer = 30;
        ShoutTimer = 0;
        long BanditTimer = Game->AddQuestInfoBar(30.0f, 0.0f, CRGBColour(0, 255, 0, 255), CRGBColour(255, 0, 0, 255), "HUD_CLOCK_ICON", "", 1.0f);
        while (GateTimer.GetTime() > 0 && GateClosed && !LeftRegion) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return _Fail_Val.Return();
            long Fred = GateTimer.GetTime();
            Game->UpdateQuestInfoBar(BanditTimer, (float)(30 - GateTimer.GetTime()), -1.0f, -1.0f);
            if (!Game->IsRegionLoaded("BanditCampPathEntrance")) {
                if (IsActiveThreadTerminating())
                    return _Fail_Val.Return();
                LeftRegion = true;
            }
            if (BanditsAwareOfPlayerAgain == true) {
                if (IsActiveThreadTerminating())
                    return _Fail_Val.Return();
                BanditsAwareOfPlayerAgain = false;
                GateTimer = 30;
            }
            if (IsDistanceBetweenThingsUnder(Game->GetHero(), SoundMarker, 16.0f)) {
                if (IsActiveThreadTerminating())
                    return _Fail_Val.Return();
                GateTimer = 30;
                if (ShoutTimer.GetTime() <= 0) {
                    if (IsActiveThreadTerminating())
                        return _Fail_Val.Return();
                    long HLStyleConversationIndex = Game->AddNewConversation(SoundMarker, false, false);
                    Game->AddPersonToConversation(HLStyleConversationIndex, Game->GetHero());
                    Game->AddLineToConversation(HLStyleConversationIndex, "TEXT_QST_047_HERO_AT_GATE", false, SoundMarker, Game->GetHero());
                    ShoutTimer = 8;
                }
            }
        }
        if (IsActiveThreadTerminating())
            return _Fail_Val.Return();
        Game->DisplayQuestInfo(false);
        Game->RemoveQuestInfoElement(BanditTimer);
        NoPersistence = false;
        if (LeftRegion) {
            if (IsActiveThreadTerminating())
            {
                return _Fail_Val.Return();
            }
            while (!Game->IsRegionLoaded("BanditCampPathEntrance")) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating())
                {
                    return _Fail_Val.Return();
                }
            }
            if (IsActiveThreadTerminating())
            {
                return _Fail_Val.Return();
            }
            NoPersistence = true;
            if (!BanditsKilled) {
                if (IsActiveThreadTerminating())
                {
                    return _Fail_Val.Return();
                }
                PopulateLevel();
                Game->OpenDoor(Game->GetThingWithScriptName("OakValeBanditRaidGate"));
                GateClosed = false;
            } else {
                if (IsActiveThreadTerminating())
                {
                    return _Fail_Val.Return();
                }
                OpenGates = true;
            }
            BanditsKilled = false;
        } else {
            if (IsActiveThreadTerminating())
            {
                return _Fail_Val.Return();
            }
            OpenGates = true;
        }
    }
}

static void Run(bool closed, bool killed, bool expectOpened, bool expectReplicate)
{
    NScript::CQ_OakValeBanditRaidScript quest;
    quest.GateClosed=closed;
    quest.BanditsKilled=killed;
    fixture.banditAlert=&quest.BanditsAwareOfPlayerAgain;
    quest.WatchForGateTimer();
    fixture.banditAlert=0;
    assert(!fixture.activeTimers);
    if (!fixture.stopCheck) {
        assert(quest.OpenGates==expectOpened);
        assert((fixture.count("open door")>0)==expectReplicate);
    }
}
int main()
{
    fixture.gateClose=3; fixture.stopFrame=5;
    Run(true,false,true,false);
    assert(fixture.count("create bar")>=1 && fixture.count("remove bar")>=1);
    fixture=ScriptFixture(); fixture.gateClose=5; fixture.stopFrame=5; fixture.changeRegion=2; fixture.near=1; fixture.gateLineExpiry=2;
    Run(true,false,false,true);
    assert(fixture.count("populate")==1 && fixture.count("TEXT_QST_047_HERO_AT_GATE")>=1);
    assert(fixture.count("set 30")>=3);
    fixture=ScriptFixture(); fixture.changeRegion=1; fixture.stopFrame=4; fixture.gateClose=5;
    Run(true,true,true,false);
    fixture=ScriptFixture();
    Run(false,false,false,false);
    assert(!fixture.count("create bar"));
    for (int stop=1; stop<=30; ++stop) {
        fixture=ScriptFixture(); fixture.stopCheck=stop; fixture.near=1;
        fixture.changeRegion=stop%2 ? 1 : 0; fixture.gateClose=4; fixture.gateLineExpiry=3;
        Run(true,stop%3!=0,false,false);
    }
    puts("00dc08c0_TEST PASS");
    return 0;
}