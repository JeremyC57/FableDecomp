#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "engine/TRetVoid.h"
#if defined(_MSC_VER)
#include "stlport/fable_containers.h"
#else
#include <vector>
#endif

static void check(bool value, const char* description) {
    if (!value) {
        fprintf(stderr, "00f05a10_TEST FAIL %s\n", description);
        exit(1);
    }
}

struct ThingState {
    ThingState(char id, bool alive) : id(id), alive(alive), handles(0) {}
    char id;
    bool alive;
    int handles;
};

// Count each held handle. Removing stale vector entries and destroying the local spy must release them.
class CScriptThing {
public:
    explicit CScriptThing(ThingState* state = 0) : state(state) { acquire(); }
    CScriptThing(const CScriptThing& source) : state(source.state) { acquire(); }
    CScriptThing& operator=(const CScriptThing& source) {
        if (this != &source) {
            release();
            state = source.state;
            acquire();
        }
        return *this;
    }
    ~CScriptThing() { release(); }
    bool IsAlive() const { return state && state->alive; }
    char id() const { return state ? state->id : '-'; }
private:
    ThingState* state;
    void acquire() { if (state) ++state->handles; }
    void release() { if (state) --state->handles; }
};

class GameFixture {
public:
    GameFixture() : spy('s',true), hero('h',true), old('o',true), saved('p',true),
        dead('!',false), banditA('a',true), banditB('b',true), convoyA('c',true),
        convoyB('d',true), sentry('t',true), heroHandle(&hero),
        nullSpy(false), populate(true), stopAt(0), polls(0), position(0) { trace[0]=0; }

    CScriptThing GetThingWithScriptName(const char* name) {
        check(strcmp(name,"AT_ConvoySpy")==0,"spy query name");
        append("S");
        return CScriptThing(nullSpy ? 0 : &spy);
    }

    long GetAllThingsWithScriptName(const char* name, std::vector<CScriptThing>& out) {
        check(old.handles==0,"obsolete bandit and convoy entries released before first query");
        if (strcmp(name,"AT_BanditAlly")==0) {
            check(out.size()==0,"bandit vector emptied");
            append("B");
            if (populate) add(out,banditA,banditB);
        } else if (strcmp(name,"AT_ConvoyMember")==0) {
            check(out.size()==0,"convoy vector emptied");
            append("C");
            if (populate) add(out,convoyA,convoyB);
        } else {
            check(strcmp(name,"AT_Sentry")==0,"sentry query name");
            check(out.size()==1 && out[0].id()=='p',"existing sentry preserved");
            append("T");
            if (populate) {
                out.push_back(CScriptThing(&dead));
                out.push_back(CScriptThing(&sentry));
            }
        }
        return static_cast<long>(out.size());
    }

    bool terminating() {
        append("|");
        ++polls;
        return polls==stopAt;
    }
    void EntitySetThingAsAllyOfThing(const CScriptThing& a,const CScriptThing& b) { record('+',a,b); }
    void EntityUnsetThingAsAllyOfThing(const CScriptThing& a,const CScriptThing& b) { record('-',a,b); }
    void EntitySetThingAsEnemyOfThing(const CScriptThing& a,const CScriptThing& b) { record('!',a,b); }
    CScriptThing& GetHero() { append("H");return heroHandle; }
    void MiniMapRemoveMarker(const CScriptThing& a) { char x[3]={'M',a.id(),0};append(x); }

    ThingState spy, hero, old, saved, dead, banditA, banditB, convoyA, convoyB, sentry;
    CScriptThing heroHandle;
    bool nullSpy, populate;
    int stopAt, polls;
    char trace[512];
private:
    size_t position;
    void add(std::vector<CScriptThing>& out,ThingState& a,ThingState& b) {
        out.push_back(CScriptThing(&dead));
        out.push_back(CScriptThing(&a));
        out.push_back(CScriptThing());
        out.push_back(CScriptThing(&b));
    }
    void append(const char* fragment) {
        size_t length=strlen(fragment);
        check(position+length<sizeof(trace),"trace capacity");
        memcpy(trace+position,fragment,length+1);
        position+=length;
    }
    void record(char op,const CScriptThing& a,const CScriptThing& b) {
        char token[4]={op,a.id(),b.id(),0};
        append(token);
    }
};

namespace NScript {
class CQ_AmbushTradersScript {
public:
    explicit CQ_AmbushTradersScript(GameFixture& game) : Game(game),SpyContactMade(false) {}
    void RefreshSpyAllegiences();
    bool IsActiveThreadTerminating() { return Game.terminating(); }
    GameFixture& Game;
    bool SpyContactMade;
    std::vector<CScriptThing> BanditsVector,ConvoyMembersVector,SentriesVector;
};
}

void NScript::CQ_AmbushTradersScript::RefreshSpyAllegiences()
{
    TRetVoid _Fail_Val;
    CScriptThing spy = Game.GetThingWithScriptName("AT_ConvoySpy");
    while (BanditsVector.size() != 0)
        BanditsVector.pop_back();
    while (ConvoyMembersVector.size() != 0)
        ConvoyMembersVector.pop_back();
    Game.GetAllThingsWithScriptName("AT_BanditAlly", BanditsVector);
    Game.GetAllThingsWithScriptName("AT_ConvoyMember", ConvoyMembersVector);
    Game.GetAllThingsWithScriptName("AT_Sentry", SentriesVector);
    if (spy.IsAlive()) {
        if (IsActiveThreadTerminating())
            return _Fail_Val.Return();
        if (false) {} else for (long idx = 0; idx < BanditsVector.size(); idx++) {
            if (BanditsVector[idx].IsAlive()) {
                if (IsActiveThreadTerminating())
                    return _Fail_Val.Return();
                Game.EntitySetThingAsAllyOfThing(spy, BanditsVector[idx]);
                Game.EntitySetThingAsAllyOfThing(BanditsVector[idx], spy);
            }
        }
        if (SpyContactMade) {
            if (IsActiveThreadTerminating())
                return _Fail_Val.Return();
            if (false) {} else for (long idx = 0; idx < ConvoyMembersVector.size(); idx++) {
                if (ConvoyMembersVector[idx].IsAlive()) {
                    if (IsActiveThreadTerminating())
                        return _Fail_Val.Return();
                    Game.MiniMapRemoveMarker(ConvoyMembersVector[idx]);
                    Game.EntityUnsetThingAsAllyOfThing(spy, ConvoyMembersVector[idx]);
                    Game.EntityUnsetThingAsAllyOfThing(ConvoyMembersVector[idx], spy);
                    Game.EntitySetThingAsEnemyOfThing(spy, ConvoyMembersVector[idx]);
                    Game.EntitySetThingAsEnemyOfThing(ConvoyMembersVector[idx], spy);
                    Game.EntitySetThingAsEnemyOfThing(Game.GetHero(), ConvoyMembersVector[idx]);
                    Game.EntitySetThingAsEnemyOfThing(ConvoyMembersVector[idx], Game.GetHero());
                }
            }
            if (false) {} else for (long idx = 0; idx < SentriesVector.size(); idx++) {
                if (SentriesVector[idx].IsAlive()) {
                    if (IsActiveThreadTerminating())
                        return _Fail_Val.Return();
                    Game.EntityUnsetThingAsAllyOfThing(spy, SentriesVector[idx]);
                    Game.EntityUnsetThingAsAllyOfThing(SentriesVector[idx], spy);
                    Game.EntitySetThingAsEnemyOfThing(spy, SentriesVector[idx]);
                    Game.EntitySetThingAsEnemyOfThing(SentriesVector[idx], spy);
                }
            }
        } else {
            if (IsActiveThreadTerminating())
                return _Fail_Val.Return();
            if (false) {} else for (long idx = 0; idx < ConvoyMembersVector.size(); idx++) {
                if (ConvoyMembersVector[idx].IsAlive()) {
                    if (IsActiveThreadTerminating())
                        return _Fail_Val.Return();
                    Game.EntitySetThingAsAllyOfThing(spy, ConvoyMembersVector[idx]);
                    Game.EntitySetThingAsAllyOfThing(ConvoyMembersVector[idx], spy);
                }
            }
        }
    }
}

static void exercise(bool contact, bool nullSpy, bool deadSpy, bool populate, int stopAt, const char* expected) {
    GameFixture game;
    game.nullSpy=nullSpy;
    game.spy.alive=!deadSpy;
    game.populate=populate;
    game.stopAt=stopAt;
    {
        NScript::CQ_AmbushTradersScript quest(game);
        quest.SpyContactMade=contact;
        quest.BanditsVector.push_back(CScriptThing(&game.old));
        quest.BanditsVector.push_back(CScriptThing(&game.old));
        quest.ConvoyMembersVector.push_back(CScriptThing(&game.old));
        quest.SentriesVector.push_back(CScriptThing(&game.saved));
        quest.RefreshSpyAllegiences();
        check(game.spy.handles==0,"local spy released on return");
        check(game.saved.handles==1,"preserved sentry retained");
        if (strcmp(game.trace,expected)!=0) {
            fprintf(stderr,"expected %s\nreceived %s\n",expected,game.trace);
            check(false,"ordered API calls and arguments");
        }
    }
    check(game.banditA.handles==0 && game.convoyB.handles==0 && game.sentry.handles==0 &&
          game.saved.handles==0 && game.dead.handles==0,"vector handles balanced");
}

static int exerciseAllStops(bool contact,const char* expected) {
    exercise(contact,false,false,true,0,expected);
    int count=1;
    for (const char* p=expected;*p;++p) {
        if (*p!='|') continue;
        char prefix[512];
        size_t length=static_cast<size_t>(p-expected)+1;
        memcpy(prefix,expected,length);
        prefix[length]=0;
        exercise(contact,false,false,true,count,prefix);
        ++count;
    }
    return count;
}

int main() {
    const char* contact =
        "SBCT||+sa+as|+sb+bs||Mc-sc-cs!sc!csH!hcH!ch|Md-sd-ds!sd!dsH!hdH!dh"
        "|-sp-ps!sp!ps|-st-ts!st!ts";
    const char* concealed = "SBCT||+sa+as|+sb+bs||+sc+cs|+sd+ds";
    int cases=exerciseAllStops(true,contact)+exerciseAllStops(false,concealed);
    exercise(true,false,true,true,0,"SBCT");
    exercise(false,true,false,true,0,"SBCT");
    exercise(true,false,false,false,0,"SBCT|||-sp-ps!sp!ps");
    exercise(false,false,false,false,0,"SBCT||");
    check(cases+4==20,"scenario count");
    puts("00f05a10_TEST PASS");
    return 0;
}