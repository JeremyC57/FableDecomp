#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "engine/TRetVoid.h"
static int tick,nearTick,endTick,loadedTick,waveDeath,duration,reg,dis,created,attached,queued,armed,startTick,firstTick,aliveTo,defCalls;
static bool *siegeStart,*secondWave;
static void ck(bool ok){if(!ok)exit(1);}
struct CScriptThing{int id;int GetPos()const{return id;}};
struct Def{long BS_NoOfWavesInFirstAttack,BS_NoOfBanditsInWave;const char* BS_BanditTypeInFirstAttack;};
static Def def;static Def* pGlobalScriptDef=&def;
struct CTimer {CTimer(){++reg;duration=0;}~CTimer(){++dis;}bool operator==(int v)const{return duration==v;}
 CTimer& operator=(long v){ck(v==2);duration=v;return *this;}};
struct FixtureGame{
 void NewScriptFrame(){if(*secondWave&&firstTick<0)firstTick=tick;++tick;if(duration>0)--duration;if(tick>=startTick)*siegeStart=true;}
 void GetAllThingsWithScriptName(const char* name,std::vector<CScriptThing>& out){if(!strcmp(name,"MB_BanditSpawn")){CScriptThing m={11};out.push_back(m);m.id=12;out.push_back(m);}else{ck(!strcmp(name,"BanditFirstWave"));++queued;CScriptThing b={20};out.push_back(b);}}
 CScriptThing GetThingWithScriptName(const char* name){ck(!strcmp(name,"MK_BS_INTRO_LEADER"));CScriptThing x={21};return x;}
 CScriptThing GetHero(){CScriptThing x={31};return x;}
 CScriptThing CreateCreatureNearby(const char* name,int pos,float range,const char* script,bool flag){ck(!strcmp(name,"Bandit") && (pos==11||pos==12) && range==5 && !flag && !strcmp(script,"BanditFirstWave"));++created;aliveTo=tick+waveDeath;CScriptThing x={50};return x;}
 const char* GetActiveQuestName(){return "Siege";}
 void EntityAttachToScript(CScriptThing x,const char* s){ck(x.id==50&&!strcmp(s,"Siege"));++attached;}
 bool IsRegionLoaded(const char* name){ck(!strcmp(name,"KnotholeGlade"));return tick>=loadedTick;}
};
bool IsDistanceBetweenThingsUnder(CScriptThing a,CScriptThing b,float d){ck(a.id==21&&b.id==31&&d==20);return tick>=nearTick;}
bool AreAllThingsInVectorDead(const std::vector<CScriptThing>& v){ck(v.size()==1&&v[0].id==20);return tick>=aliveTo;}
namespace NScript {struct CQ_BreakSiegeScript{
 FixtureGame Game;bool BanditSiegeStart,HasFirstWaveAttacked,StartSecondWave;
 bool IsActiveThreadTerminating(){return tick>=endTick;}
 void WatchForOpenGates();};}
void NScript::CQ_BreakSiegeScript::WatchForOpenGates()
{
    TRetVoid _Fail_Val;
    while (!BanditSiegeStart) {
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating())
            return _Fail_Val.Return();
    }
    if (IsActiveThreadTerminating())
        return _Fail_Val.Return();
    std::vector<CScriptThing> BanditSpawns;
    Game.GetAllThingsWithScriptName("MB_BanditSpawn", BanditSpawns);
    CScriptThing NewBandit;
    CScriptThing EnteredCamp = Game.GetThingWithScriptName("MK_BS_INTRO_LEADER");
    if (!HasFirstWaveAttacked) {
        if (IsActiveThreadTerminating())
            {
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
                return _Fail_Val.Return();
#ifdef _MSC_VER
#pragma inline_depth()
#endif
            }
        CTimer BanditStreamDelay;
        bool GotIntoCamp = false;
        if (false) {} else for (long i = 0; i < pGlobalScriptDef->BS_NoOfWavesInFirstAttack; ++i) {
            if (IsActiveThreadTerminating())
                {
#ifdef _MSC_VER
#pragma inline_depth(1)
#endif
                    return _Fail_Val.Return();
#ifdef _MSC_VER
#pragma inline_depth()
#endif
                }
            long BanditStream = 0;
            while (BanditStream < pGlobalScriptDef->BS_NoOfBanditsInWave) {
                Game.NewScriptFrame();
                if (IsActiveThreadTerminating())
                    {
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
                        return _Fail_Val.Return();
#ifdef _MSC_VER
#pragma inline_depth()
#endif
                    }
                if (BanditStreamDelay == 0) {
                    if (IsActiveThreadTerminating())
                        {
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
                            return _Fail_Val.Return();
#ifdef _MSC_VER
#pragma inline_depth()
#endif
                        }
#ifdef _MSC_VER
#pragma inline_depth(1)
#endif
                    NewBandit = Game.CreateCreatureNearby(pGlobalScriptDef->BS_BanditTypeInFirstAttack,
                        BanditSpawns[rand() % BanditSpawns.size()].GetPos(),
                        5.0f, "BanditFirstWave", false);
#ifdef _MSC_VER
#pragma inline_depth()
#endif
                    Game.EntityAttachToScript(NewBandit, Game.GetActiveQuestName());
                    BanditStreamDelay = 2;
                    ++BanditStream;
                }
                if (IsDistanceBetweenThingsUnder(EnteredCamp, Game.GetHero(), 20.0f)) {
                    if (IsActiveThreadTerminating())
                        {
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
                            return _Fail_Val.Return();
#ifdef _MSC_VER
#pragma inline_depth()
#endif
                        }
                    GotIntoCamp = true;
                    break;
                }
            }
            if (IsActiveThreadTerminating())
                {
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
                    return _Fail_Val.Return();
#ifdef _MSC_VER
#pragma inline_depth()
#endif
                }
            std::vector<CScriptThing> Bandits;
            Game.GetAllThingsWithScriptName("BanditFirstWave", Bandits);
            while (!GotIntoCamp && !AreAllThingsInVectorDead(Bandits)) {
                Game.NewScriptFrame();
                if (IsActiveThreadTerminating())
                    {
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
                        return _Fail_Val.Return();
#ifdef _MSC_VER
#pragma inline_depth()
#endif
                    }
                if (IsDistanceBetweenThingsUnder(EnteredCamp, Game.GetHero(), 20.0f)) {
                    if (IsActiveThreadTerminating())
                        {
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
                            return _Fail_Val.Return();
#ifdef _MSC_VER
#pragma inline_depth()
#endif
                        }
                    GotIntoCamp = true;
                }
            }
            if (IsActiveThreadTerminating())
                {
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
                    return _Fail_Val.Return();
#ifdef _MSC_VER
#pragma inline_depth()
#endif
                }
            if (GotIntoCamp) {
                if (IsActiveThreadTerminating())
                    {
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
                        return _Fail_Val.Return();
#ifdef _MSC_VER
#pragma inline_depth()
#endif
                    }
                {
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
                    break;
#ifdef _MSC_VER
#pragma inline_depth()
#endif
                }
            }
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
        }
#ifdef _MSC_VER
#pragma inline_depth()
#endif
        if (IsActiveThreadTerminating())
            {
#ifdef _MSC_VER
#pragma inline_depth(1)
#endif
                return _Fail_Val.Return();
#ifdef _MSC_VER
#pragma inline_depth()
#endif
            }
        HasFirstWaveAttacked = true;
    }
    std::vector<CScriptThing> BanditVector;
    Game.GetAllThingsWithScriptName("BanditFirstWave", BanditVector);
    if (false) {} else for (;;Game.NewScriptFrame()) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        if (AreAllThingsInVectorDead(BanditVector) && Game.IsRegionLoaded("KnotholeGlade")) {
            if (IsActiveThreadTerminating())
                {
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
                    return _Fail_Val.Return();
#ifdef _MSC_VER
#pragma inline_depth()
#endif
                }
            StartSecondWave = true;
        }
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
}

int main(){
 for(int started=0;started<=1;++started)for(int prior=0;prior<2;++prior)for(int waves=0;waves<3;++waves)for(int bandits=0;bandits<3;++bandits)for(int near=1;near<13;near+=3)for(int end=0;end<20;++end){
 NScript::CQ_BreakSiegeScript s;s.BanditSiegeStart=started!=0;s.HasFirstWaveAttacked=prior!=0;s.StartSecondWave=false;siegeStart=&s.BanditSiegeStart;secondWave=&s.StartSecondWave;startTick=2;
 def.BS_NoOfWavesInFirstAttack=waves;def.BS_NoOfBanditsInWave=bandits;def.BS_BanditTypeInFirstAttack="Bandit";tick=reg=dis=created=attached=queued=aliveTo=0;firstTick=-1;nearTick=near;endTick=end;loadedTick=4;waveDeath=3;s.WatchForOpenGates();
 int t=started?0:(end<2?(end>0?end:1):2), sp=0,done=0,deadline=0,ready=0,delay=0;
 bool enter=t<end;if(enter&&!prior){bool nearCamp=false;for(int w=0;w<waves&&t<end&&!nearCamp;++w){int count=0;while(count<bandits&&t<end){++t;if(delay>0)--delay;if(t==end)break;if(!delay){++count;++sp;deadline=t+3;delay=2;}if(t>=near){nearCamp=true;break;}}
 while(!nearCamp&&t<deadline&&t<end){++t;if(delay>0)--delay;if(t>=near)nearCamp=true;}}
 done=t<end;}
 if(enter&&(prior||done)){int eligible=t;if(eligible<deadline)eligible=deadline;if(eligible<4)eligible=4;ready=eligible<end;}
 ck(reg==int(enter&&!prior)&&reg==dis&&created==sp&&attached==sp&&s.HasFirstWaveAttacked==bool(prior||done)&&s.StartSecondWave==bool(ready));
 }
 puts("00d20a40_TEST PASS");return 0;}