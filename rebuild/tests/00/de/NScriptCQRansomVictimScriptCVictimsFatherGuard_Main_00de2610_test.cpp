// Behaviour fixture: exact selected CVictimsFatherGuard::Main body below.
// Deterministic interface stubs drive all phases; no exceptions are required.
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
#include <cassert>
struct Scenario {
    int frame, stopAt, assistAt, chiefAt, waitAt, rescueAt, helperAt, spokenAt;
    int talkAt, hits[3], spellAt, healAt, startDelay, taskDelay;
    int movieLive, pauseLive, movies, pauses, allies, enemies, follows, stops;
    int lastRegion, response, starts, releases, followDistance, colour;
    bool helperAlive;
    float health;
    std::vector<std::string> lines;
    Scenario() : frame(0),stopAt(12),assistAt(100),chiefAt(-1),waitAt(-1),rescueAt(-1),helperAt(-1),spokenAt(-1),talkAt(-1),spellAt(-1),healAt(-1),startDelay(0),taskDelay(0),movieLive(0),pauseLive(0),movies(0),pauses(0),allies(0),enemies(0),follows(0),stops(0),lastRegion(-1),response(-1),starts(0),releases(0),followDistance(0),colour(0),helperAlive(true),health(1.0f) { hits[0]=hits[1]=hits[2]=-1; }
};
static Scenario* g;
class CScriptThing { public:
    int id;
    CScriptThing(int n=0):id(n){}
    bool MsgIsTalkedToBy(const char*) const { return g->frame==g->talkAt; }
    bool MsgIsHitBy(const char*) const { return g->frame==g->hits[0] || g->frame==g->hits[1] || g->frame==g->hits[2]; }
    bool MsgIsHitByAnySpecialAbilityFrom(const char*) const { return g->frame==g->spellAt || g->frame==g->healAt; }
    bool MsgIsHitBySpecialAbilityFrom(int,const char*) const { return g->frame==g->healAt; }
    bool IsAlive()const{return g->helperAlive;}
    bool IsDead()const{return !g->helperAlive;}
};
class CCharString { public:
    const char* text;
    CCharString():text(""){}
    CCharString& operator=(const char* t){text=t;return *this;}
    operator const char*()const{return text;}
};
class CScriptGameResourceObjectMovieBase { public:
    CScriptGameResourceObjectMovieBase(){++g->movieLive;++g->movies;}
    ~CScriptGameResourceObjectMovieBase(){--g->movieLive;}
};
class CScriptGameResourceObjectScriptedThingBase { public:
    bool valid;
    CScriptGameResourceObjectScriptedThingBase():valid(false){}
    bool IsValid()const{return valid;}
    void Release(){valid=false;++g->releases;}
    CScriptThing GetScriptThing()const{return CScriptThing(2);}
    void Speak(CScriptThing,const char* line,int,bool,bool,bool){g->lines.push_back(line);}
    bool IsPerformingScriptTask()const{return g->taskDelay>0;}
};
class CRGBColour { public: int r,green,b,a; CRGBColour(int v,int w,int x,int y):r(v),green(w),b(x),a(y){} };
class CGameScriptInterfaceBase;
struct CWideScreenMagicPauseEntities {
    CWideScreenMagicPauseEntities(CGameScriptInterfaceBase&){++g->pauseLive;++g->pauses;}
    ~CWideScreenMagicPauseEntities(){--g->pauseLive;}
};
struct TRetVoid {void Return(){}};
enum {SCRIPT_AI_PRIORITY_HIGHEST=4,GROUP_SELECT_NONE=0,GROUP_SELECT_RANDOM_NO_REPEAT=2,HERO_ABILITY_HEAL_LIFE_SPELL=14,CUTSCENE_BEHAVIOUR_PAUSED=1,CUTSCENE_BEHAVIOUR_NOT_PAUSED=2};
namespace NScript {
class CIsThingAlive { public:CScriptThing Thing;CIsThingAlive(const CScriptThing& t):Thing(t){}};
class CQ_RansomVictimScript { public:
    bool RescuedVictim,RansomHelperStarted,RansomHelperSpoken,ExtraAssistanceGiven,ToldGuardToWaitOutside;
    long HitChief;
    CScriptGameResourceObjectScriptedThingBase seh_Guard;
    CQ_RansomVictimScript():RescuedVictim(false),RansomHelperStarted(false),RansomHelperSpoken(false),ExtraAssistanceGiven(false),ToldGuardToWaitOutside(false),HitChief(0){}
    struct CVictimsFatherGuard {
        CGameScriptInterfaceBase* Game;
        CScriptThing Me;
        CQ_RansomVictimScript& ParentClass;
        long TimesHit;
        CVictimsFatherGuard(CGameScriptInterfaceBase* game,CQ_RansomVictimScript& p):Game(game),Me(2),ParentClass(p),TimesHit(0){}
        bool IsActiveThreadTerminating()const{return g->frame>=g->stopAt;}
        void SetPredicate(const CIsThingAlive& p){assert(p.Thing.id==2);}
        void Main();
    };
};
}
class CGameScriptInterfaceBase { public:
    NScript::CQ_RansomVictimScript& parent;
    CGameScriptInterfaceBase(NScript::CQ_RansomVictimScript& p):parent(p){}
    void NewScriptFrame(){
        ++g->frame; assert(g->frame<1000);
        if(g->taskDelay>0)--g->taskDelay;
        if(g->frame==g->assistAt)parent.ExtraAssistanceGiven=true;
        if(g->frame==g->chiefAt)parent.HitChief=3;
        if(g->frame==g->waitAt)parent.ToldGuardToWaitOutside=true;
        if(g->frame==g->rescueAt)parent.RescuedVictim=true;
        if(g->frame==g->helperAt)parent.RansomHelperStarted=true;
        if(g->frame==g->spokenAt)parent.RansomHelperSpoken=true;
    }
    bool StartScriptingEntity(CScriptThing,CScriptGameResourceObjectScriptedThingBase& h,int priority){assert(priority==4);++g->starts;if(g->startDelay>0){--g->startDelay;return false;}h.valid=true;return true;}
    void StartMovieSequence(const char*,CScriptGameResourceObjectMovieBase&){}
    float GetHealth(CScriptThing){return g->health;}
    CScriptThing GetHero(){return CScriptThing(1);}
    void EntityUnsetThingAsAllyOfThing(CScriptThing,CScriptThing){++g->enemies;}
    void EntitySetThingAsAllyOfThing(CScriptThing,CScriptThing){++g->allies;}
    void GiveThingBestEnemyTarget(CScriptThing,CScriptThing){}
    long AddNewConversation(CScriptThing,bool,bool){return 3;}
    void AddPersonToConversation(long,CScriptThing){}
    void AddLineToConversation(long,const char* line,bool,CScriptThing,CScriptThing){g->lines.push_back(line);}
    void MiniMapAddMarker(CScriptThing,const char*){}
    void DisplayQuestInfo(bool){}
    void AddQuestInfoBarHealth(CScriptThing,CRGBColour c,const char*,float v){assert(v==1.0f);g->colour=c.r+c.green+c.b+c.a;}
    void EntityFollowThing(CScriptThing,CScriptThing,float distance,bool){++g->follows;g->followDistance=(int)distance;}
    void SetEntityAsRegionFollowing(CScriptThing hero,CScriptThing guard,bool v){assert(hero.id==1&&guard.id==2);g->lastRegion=v?1:0;}
    void EntitySetInFaction(CScriptThing,const char*){}
    void EntitySetCutsceneBehaviour(CScriptThing,int){}
    CScriptThing GetNearestWithScriptName(CScriptThing,const char*){return CScriptThing(3);}
    void EntityStopFollowing(CScriptThing){++g->stops;}
    void EntitySetAsRespondingToFollowAndWaitExpressions(CScriptThing,bool v){g->response=v?1:0;}
};

void NScript::CQ_RansomVictimScript::CVictimsFatherGuard::Main()
{
    // Fixture adapter: bind the original reference to the configured dependency.
    ::CGameScriptInterfaceBase& Game = *this->Game;
{
    TRetVoid _Fail_Val;
    SetPredicate(CIsThingAlive(Me));
    Game.NewScriptFrame();
    if (IsActiveThreadTerminating())
        return _Fail_Val.Return();
    { CQ_RansomVictimScript& parent = ParentClass; if (parent.seh_Guard.IsValid()) parent.seh_Guard.Release(); }
    while (!Game.StartScriptingEntity(Me, ParentClass.seh_Guard, SCRIPT_AI_PRIORITY_HIGHEST)) {
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating())
            return _Fail_Val.Return();
    }
    if (IsActiveThreadTerminating())
        return _Fail_Val.Return();
    while (!ParentClass.ExtraAssistanceGiven) {
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating())
            return _Fail_Val.Return();
        if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            if (IsActiveThreadTerminating())
                return _Fail_Val.Return();
            CScriptGameResourceObjectMovieBase speech_movie;
            Game.StartMovieSequence("", speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
            if (Game.GetHealth(ParentClass.seh_Guard.GetScriptThing()) > 0.0f) {
                ParentClass.seh_Guard.Speak(Game.GetHero(), "TEXT_QST_B06_CHIEFSGUARD_PROTECTING_CHIEF", GROUP_SELECT_RANDOM_NO_REPEAT, false, true, false);
                while (ParentClass.seh_Guard.IsPerformingScriptTask()) {
                    Game.NewScriptFrame();
                    if (IsActiveThreadTerminating())
                        return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating())
                    return _Fail_Val.Return();
            }
        }
        if (Me.MsgIsHitBy("SCRIPT_NAME_HERO") || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL, "SCRIPT_NAME_HERO"))) {
            if (IsActiveThreadTerminating())
                return _Fail_Val.Return();
            ++TimesHit;
            CCharString line;
            switch (TimesHit) {
            case 1: line = "TEXT_QST_B06_CHIEFSGUARD_ON_HIT_10"; break;
            case 2: line = "TEXT_QST_B06_CHIEFSGUARD_ON_HIT_20"; break;
            case 3: line = "TEXT_QST_B06_CHIEFSGUARD_ON_HIT_30"; break;
            default: line = "TEXT_QST_B06_CHIEFSGUARD_ON_HIT_30"; break;
            }
            {
                CScriptGameResourceObjectMovieBase speech_movie;
                Game.StartMovieSequence("", speech_movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
                if (Game.GetHealth(ParentClass.seh_Guard.GetScriptThing()) > 0.0f) {
                    ParentClass.seh_Guard.Speak(Game.GetHero(), line, GROUP_SELECT_NONE, false, true, false);
                    while (ParentClass.seh_Guard.IsPerformingScriptTask()) {
                        Game.NewScriptFrame();
                        if (IsActiveThreadTerminating())
                            return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating())
                        return _Fail_Val.Return();
                }
            }
            if (TimesHit >= 3) {
                if (IsActiveThreadTerminating())
                    return _Fail_Val.Return();
                Game.EntityUnsetThingAsAllyOfThing(Me, Game.GetHero());
                Game.EntityUnsetThingAsAllyOfThing(Game.GetHero(), Me);
                Game.GiveThingBestEnemyTarget(Me, Game.GetHero());
                { CQ_RansomVictimScript& parent = ParentClass; if (parent.seh_Guard.IsValid()) parent.seh_Guard.Release(); }
                for (;;) {
                    Game.NewScriptFrame();
                    if (IsActiveThreadTerminating())
                        return _Fail_Val.Return();
                }
            }
            if (IsActiveThreadTerminating())
                return _Fail_Val.Return();
            Game.EntitySetThingAsAllyOfThing(Me, Game.GetHero());
            Game.EntitySetThingAsAllyOfThing(Game.GetHero(), Me);
        }
        if (ParentClass.HitChief > 2) {
            if (IsActiveThreadTerminating())
                return _Fail_Val.Return();
            long conversation = Game.AddNewConversation(Me, false, false);
            Game.AddPersonToConversation(conversation, Game.GetHero());
            Game.AddLineToConversation(conversation, "TEXT_QST_B06_CHIEFSGUARD_HIT_CHIEF_THIRD_TIME_10", false, Me, Game.GetHero());
            Game.EntityUnsetThingAsAllyOfThing(Me, Game.GetHero());
            Game.EntityUnsetThingAsAllyOfThing(Game.GetHero(), Me);
            Game.GiveThingBestEnemyTarget(Me, Game.GetHero());
            { CQ_RansomVictimScript& parent = ParentClass; if (parent.seh_Guard.IsValid()) parent.seh_Guard.Release(); }
            for (;;) {
                Game.NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return _Fail_Val.Return();
            }
        }
    }
    if (IsActiveThreadTerminating())
        return _Fail_Val.Return();
    Game.MiniMapAddMarker(Me, "HUD_ORB_GREEN_SMALL");
    Game.DisplayQuestInfo(true);
    Game.AddQuestInfoBarHealth(Me, CRGBColour(0, 255, 0, 255), "HUD_QUEST_ICON_GUARD", 1.0f);
    Game.EntityFollowThing(Me, Game.GetHero(), 6.0f, true);
    Game.SetEntityAsRegionFollowing(Game.GetHero(), Me, true);
    Game.EntitySetInFaction(Me, "FACTION_HERO");
    { CQ_RansomVictimScript& parent = ParentClass; if (parent.seh_Guard.IsValid()) parent.seh_Guard.Release(); }
    Game.EntitySetCutsceneBehaviour(Me, CUTSCENE_BEHAVIOUR_PAUSED);
    long FollowState = 0;
    if (IsActiveThreadTerminating())
        return _Fail_Val.Return();
    for (;;) {
        if (ParentClass.RansomHelperStarted && !ParentClass.RansomHelperSpoken) {
            if (IsActiveThreadTerminating())
                return _Fail_Val.Return();
            { CQ_RansomVictimScript& parent = ParentClass; if (parent.seh_Guard.IsValid()) parent.seh_Guard.Release(); }
            while (!Game.StartScriptingEntity(Me, ParentClass.seh_Guard, SCRIPT_AI_PRIORITY_HIGHEST)) {
                Game.NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating())
                return _Fail_Val.Return();
            while (!ParentClass.RansomHelperSpoken) {
                Game.NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating())
                return _Fail_Val.Return();
            { CQ_RansomVictimScript& parent = ParentClass; if (parent.seh_Guard.IsValid()) parent.seh_Guard.Release(); }
            Game.EntitySetCutsceneBehaviour(Me, CUTSCENE_BEHAVIOUR_PAUSED);
        }
        if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            if (IsActiveThreadTerminating())
                return _Fail_Val.Return();
            long conversation = Game.AddNewConversation(Me, false, false);
            Game.AddPersonToConversation(conversation, Game.GetHero());
            if (ParentClass.ToldGuardToWaitOutside && !ParentClass.RescuedVictim && Game.GetNearestWithScriptName(Me, "RansomHelper").IsAlive()) {
                if (IsActiveThreadTerminating())
                    return _Fail_Val.Return();
                Game.AddLineToConversation(conversation, "TEXT_QST_B06_CHIEFSGUARD_WAITIING_OUTSIDE_10", false, Me, Game.GetHero());
            } else if (ParentClass.ToldGuardToWaitOutside && !ParentClass.RescuedVictim && Game.GetNearestWithScriptName(Me, "RansomHelper").IsDead()) {
                if (IsActiveThreadTerminating())
                    return _Fail_Val.Return();
                Game.AddLineToConversation(conversation, "TEXT_QST_B06_CHIEFSGUARD_WAITING_OUTSIDE_KILLED_GUARDS_10", false, Me, Game.GetHero());
            } else {
                if (IsActiveThreadTerminating())
                    return _Fail_Val.Return();
                Game.AddLineToConversation(conversation, "TEXT_QST_B06_CHIEFSGUARD_ON_TALK_10", false, Me, Game.GetHero());
            }
        }
        if (Me.MsgIsHitBy("SCRIPT_NAME_HERO") || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL, "SCRIPT_NAME_HERO"))) {
            if (IsActiveThreadTerminating())
                return _Fail_Val.Return();
            ++TimesHit;
            CCharString line;
            switch (TimesHit) {
            case 1: line = "TEXT_QST_B06_CHIEFSGUARD_ON_HIT_10"; break;
            case 2: line = "TEXT_QST_B06_CHIEFSGUARD_ON_HIT_20"; break;
            case 3: line = "TEXT_QST_B06_CHIEFSGUARD_ON_HIT_30"; break;
            default: line = "TEXT_QST_B06_CHIEFSGUARD_ON_HIT_30"; break;
            }
            { CQ_RansomVictimScript& parent = ParentClass; if (parent.seh_Guard.IsValid()) parent.seh_Guard.Release(); }
            while (!Game.StartScriptingEntity(Me, ParentClass.seh_Guard, SCRIPT_AI_PRIORITY_HIGHEST)) {
                Game.NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating())
                return _Fail_Val.Return();
            Game.EntitySetCutsceneBehaviour(Me, CUTSCENE_BEHAVIOUR_NOT_PAUSED);
            {
                CScriptGameResourceObjectMovieBase speech_movie;
                Game.StartMovieSequence("", speech_movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
                if (Game.GetHealth(ParentClass.seh_Guard.GetScriptThing()) > 0.0f) {
                    ParentClass.seh_Guard.Speak(Game.GetHero(), line, GROUP_SELECT_NONE, false, true, false);
                    while (ParentClass.seh_Guard.IsPerformingScriptTask()) {
                        Game.NewScriptFrame();
                        if (IsActiveThreadTerminating())
                            return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating())
                        return _Fail_Val.Return();
                }
                { CQ_RansomVictimScript& parent = ParentClass; if (parent.seh_Guard.IsValid()) parent.seh_Guard.Release(); }
                Game.EntitySetCutsceneBehaviour(Me, CUTSCENE_BEHAVIOUR_PAUSED);
            }
            if (TimesHit >= 3) {
                if (IsActiveThreadTerminating())
                    return _Fail_Val.Return();
                Game.EntityUnsetThingAsAllyOfThing(Me, Game.GetHero());
                Game.EntityUnsetThingAsAllyOfThing(Game.GetHero(), Me);
                Game.EntityStopFollowing(Me);
                Game.SetEntityAsRegionFollowing(Game.GetHero(), Me, false);
                Game.GiveThingBestEnemyTarget(Me, Game.GetHero());
                { CQ_RansomVictimScript& parent = ParentClass; if (parent.seh_Guard.IsValid()) parent.seh_Guard.Release(); }
                for (;;) {
                    Game.NewScriptFrame();
                    if (IsActiveThreadTerminating())
                        return _Fail_Val.Return();
                }
            }
            if (IsActiveThreadTerminating())
                return _Fail_Val.Return();
            Game.EntitySetThingAsAllyOfThing(Me, Game.GetHero());
            Game.EntitySetThingAsAllyOfThing(Game.GetHero(), Me);
        }
        if (FollowState == 0 && ParentClass.ToldGuardToWaitOutside && !ParentClass.RescuedVictim) {
            if (IsActiveThreadTerminating())
                return _Fail_Val.Return();
            Game.EntityStopFollowing(Me);
            Game.SetEntityAsRegionFollowing(Game.GetHero(), Me, false);
            Game.EntitySetAsRespondingToFollowAndWaitExpressions(Me, false);
            FollowState = 1;
        }
        if (FollowState == 1 && ParentClass.RescuedVictim) {
            if (IsActiveThreadTerminating())
                return _Fail_Val.Return();
            Game.EntityFollowThing(Me, Game.GetHero(), 1.0f, true);
            Game.SetEntityAsRegionFollowing(Game.GetHero(), Me, true);
            Game.EntitySetAsRespondingToFollowAndWaitExpressions(Me, true);
            FollowState = 2;
        }
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating())
            return _Fail_Val.Return();
    }
}
}

static bool Has(const Scenario& s,const char* suffix){for(unsigned i=0;i<s.lines.size();++i)if(s.lines[i].find(suffix)!=std::string::npos)return true;return false;}
static long Run(Scenario& s){g=&s;NScript::CQ_RansomVictimScript p;CGameScriptInterfaceBase game(p);NScript::CQ_RansomVictimScript::CVictimsFatherGuard guard(&game,p);guard.Main();assert(s.frame==s.stopAt);assert(s.movieLive==0&&s.pauseLive==0);return guard.TimesHit;}
int main(){
    bool ok=true;
    Scenario a;a.assistAt=5;a.hits[0]=3;a.talkAt=7;a.waitAt=8;a.rescueAt=10;
    ok=Run(a)==1&&Has(a,"ON_HIT_10")&&Has(a,"ON_TALK_10")&&a.follows==2&&a.stops==1&&a.followDistance==1&&a.lastRegion==1&&a.response==1&&a.colour==510&&ok;
    Scenario b;b.assistAt=5;b.hits[0]=3;b.hits[1]=4;b.hits[2]=7;b.talkAt=2;b.waitAt=6;
    ok=Run(b)==3&&Has(b,"PROTECTING_CHIEF")&&Has(b,"ON_HIT_30")&&b.enemies==2&&b.allies==4&&b.lastRegion==0&&ok;
    Scenario c;c.chiefAt=4;
    ok=Run(c)==0&&Has(c,"HIT_CHIEF_THIRD_TIME")&&c.enemies==2&&c.follows==0&&ok;
    Scenario d;d.healAt=3;d.spellAt=5;
    ok=Run(d)==1&&d.allies==2&&d.lines.size()==1&&ok;
    Scenario e;e.assistAt=3;e.waitAt=4;e.talkAt=5;
    ok=Run(e)==0&&Has(e,"WAITIING_OUTSIDE")&&ok;
    Scenario f;f.assistAt=3;f.waitAt=4;f.talkAt=5;f.helperAlive=false;
    ok=Run(f)==0&&Has(f,"WAITING_OUTSIDE_KILLED_GUARDS")&&ok;
    Scenario h;h.hits[0]=2;h.hits[1]=3;h.hits[2]=4;
    ok=Run(h)==3&&h.enemies==2&&h.follows==0&&ok;
    Scenario i;i.talkAt=2;i.health=0.0f;
    ok=Run(i)==0&&i.lines.empty()&&i.movies==1&&ok;
    Scenario j;j.assistAt=3;j.helperAt=4;j.spokenAt=7;
    ok=Run(j)==0&&j.starts==2&&j.releases==2&&ok;
    for(int stop=1;stop<=10;++stop){Scenario k;k.stopAt=stop;k.talkAt=2;k.taskDelay=5;k.assistAt=6;k.startDelay=1;Run(k);ok=k.movieLive==0&&k.pauseLive==0&&ok;}
    std::printf(ok?"00de2610_TEST PASS\n":"00de2610_TEST FAIL\n");return ok?0:1;
}