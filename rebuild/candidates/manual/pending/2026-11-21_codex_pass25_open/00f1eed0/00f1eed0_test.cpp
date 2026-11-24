// Exact selected PlayWave body with deterministic dependency doubles. C++03 /Od /W3, no /EHsc.
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <cassert>
#if defined(_MSC_VER) && _MSC_VER < 1400
#define for if (0) {} else for
#endif
static int liveThings=0,liveTimers=0,liveMovies=0,livePauses=0,liveHandles=0;
struct CCharString {std::string value;CCharString(const char*s=""):value(s){}operator const char*()const{return value.c_str();}};
struct C3DVector {float X,Y,Z;C3DVector(float x=0,float y=0,float z=0):X(x),Y(y),Z(z){}C3DVector operator+(const C3DVector&o)const{return C3DVector(X+o.X,Y+o.Y,Z+o.Z);}void RotateXY(float a){assert(a>=0&&a<1);}};
struct CRGBColour {unsigned char R,G,B,A;CRGBColour(unsigned char r,unsigned char g,unsigned char b,unsigned char a=255):R(r),G(g),B(b),A(a){}};
struct CScriptThing {int id;C3DVector pos;CCharString data;bool alive;CScriptThing(int n=0):id(n),alive(n!=0){++liveThings;}CScriptThing(const CScriptThing&o):id(o.id),pos(o.pos),data(o.data),alive(o.alive){++liveThings;}CScriptThing&operator=(const CScriptThing&o){id=o.id;pos=o.pos;data=o.data;alive=o.alive;return *this;}~CScriptThing(){--liveThings;}bool IsAlive()const{return alive;}const C3DVector&GetPos()const{return pos;}CCharString GetDataString()const{return data;}};
struct TRetVoid {void Return(){}};
enum {CUTSCENE_BEHAVIOUR_PAUSED=1,SCRIPT_AI_PRIORITY_HIGHEST=4};
struct CScriptGameResourceObjectScriptedThingBase {bool valid;CScriptGameResourceObjectScriptedThingBase():valid(false){++liveHandles;}~CScriptGameResourceObjectScriptedThingBase(){--liveHandles;}bool IsValid()const{return valid;}void Release(){valid=false;}};
struct CScriptGameResourceObjectMovieBase {CScriptGameResourceObjectMovieBase(){++liveMovies;}~CScriptGameResourceObjectMovieBase(){--liveMovies;}};
struct FakeGame;
struct CWideScreenMagicPauseEntities {CWideScreenMagicPauseEntities(FakeGame&){++livePauses;}~CWideScreenMagicPauseEntities(){--livePauses;}};
struct CTimer {mutable long value;CTimer():value(0){++liveTimers;}~CTimer(){--liveTimers;}CTimer&operator=(long n){value=n;return *this;}long GetTime()const{return value--;}private:CTimer(const CTimer&);};
// The timer dependency returns a descending countdown on each poll; production receives these ticks through NewScriptFrame.
struct CArenaCreatureDef {long NumCreatures;CCharString CreatureType,HUDType;CArenaCreatureDef():NumCreatures(1),CreatureType("creature"),HUDType("hud") {}};
struct CArenaWaveDef {bool ShortWave;long NumWaveCreatures;std::vector<CArenaCreatureDef>Creatures;CArenaWaveDef():ShortWave(false),NumWaveCreatures(1),Creatures(3){}};
struct CArenaRoundDef {long NumWaves;std::vector<CArenaWaveDef>Waves;CArenaRoundDef():NumWaves(2),Waves(3){}};
struct CScriptDef {std::vector<CCharString>RewardObjects;CScriptDef():RewardObjects(3,CCharString("reward")) {}};
static CScriptDef GlobalDef;static CScriptDef*pGlobalScriptDef=&GlobalDef;
static long GFCharStringToInt(const CCharString&s){return std::atoi(s.value.c_str());}
namespace NScript {struct CQ_ArenaScript;}
struct FakeGame {
 NScript::CQ_ArenaScript* owner;CScriptThing hero;long markerCount,bigCount,existing,answer;int answerWait,controlWait,startWait,conversationWait;
 bool whisper,levelLoaded,injectExtra,injected,questVisible;long checks,terminateAt,frames,nearby,exact,targets,whisperTargets,objects,conversations,lines,countAdds,countUpdates,countRemoves,goldPaid,questions,fades,movies,delayDepth;
 FakeGame():owner(0),hero(1),markerCount(3),bigCount(3),existing(0),answer(1),answerWait(1),controlWait(1),startWait(1),conversationWait(1),whisper(false),levelLoaded(true),injectExtra(false),injected(false),questVisible(false),checks(0),terminateAt(-1),frames(0),nearby(0),exact(0),targets(0),whisperTargets(0),objects(0),conversations(0),lines(0),countAdds(0),countUpdates(0),countRemoves(0),goldPaid(0),questions(0),fades(0),movies(0),delayDepth(0){}
 CScriptThing GetThingWithScriptName(const CCharString&name){CScriptThing s(2);if(name.value=="WhisperAlly"){s.id=7;s.alive=whisper;}return s;}
 const CScriptThing&GetHero()const{return hero;}
 long AddNewConversation(const CScriptThing&,bool,bool){return ++conversations;}
 void AddLineToConversation(long,const CCharString&,bool,const CScriptThing&,const CScriptThing&){++lines;}
 long GetAllThingsWithScriptName(const CCharString&name,std::vector<CScriptThing>&out){long count=name.value=="MultipleBigCreatureSpawnPoint"?bigCount:name.value=="ArenaEnemy"?existing:markerCount;for(long i=0;i<count;++i){CScriptThing s(10+i);char str[20];std::sprintf(str,"%ld",i);s.data=CCharString(str);out.push_back(s);}return count;}
 CScriptThing GetFurthestWithScriptName(const CScriptThing&,const CCharString&){CScriptThing s(3);s.data=CCharString("0");return s;}
 void SetCreatureCreationDelayFrames(long n){assert(n==1);++delayDepth;}
 void ResetCreatureCreationDelayFrames(){assert(delayDepth>0);--delayDepth;}
 CScriptThing CreateCreatureNearby(const CCharString&,const C3DVector&,float,const CCharString&,bool){++nearby;return CScriptThing(4);}
 CScriptThing CreateCreature(const CCharString&,const C3DVector&,const CCharString&,bool){++exact;return CScriptThing(4);}
 void EntitySetCutsceneBehaviour(const CScriptThing&,int b){assert(b==1);}
 void GiveThingBestEnemyTarget(const CScriptThing&,const CScriptThing&t){++targets;if(t.id==7)++whisperTargets;}
 long AddQuestInfoCounterList(const CCharString&,long n,float){assert(n>0);return ++countAdds;}
 void UpdateQuestInfoCounterList(long,long,long){++countUpdates;}
 void RemoveQuestInfoElement(long){++countRemoves;}
 void DisplayQuestInfo(bool v){questVisible=v;}
 void NewScriptFrame();
 bool IsLevelLoaded(const CCharString&n){assert(n.value=="Arena");return levelLoaded;}
 CScriptThing CreateObject(const CCharString&,const C3DVector&,const CCharString&){++objects;return CScriptThing(5);}
 void Pause(float){}
 void OverrideMusic(int,bool,bool){}
 void StopOverrideMusic(bool){}
 bool IsHeroControlledByPlayer(){return controlWait--<=0;}
 void StartMovieSequence(const CCharString&,CScriptGameResourceObjectMovieBase&){++movies;}
 bool StartScriptingEntity(const CScriptThing&,CScriptGameResourceObjectScriptedThingBase&h,int p){assert(p==4);bool yes=startWait--<=0;if(yes)h.valid=true;return yes;}
 void GiveHeroYesNoQuestion(const CCharString&,const CCharString&,const CCharString&,const CCharString&,bool){++questions;}
 long MsgIsQuestionAnsweredYesOrNo(){return answerWait--<=0?answer:-1;}
 void GiveHeroGold(long n){goldPaid+=n;}
 bool IsConversationActive(long){return conversationWait-- >0;}
 void FadeScreenOut(float,float,const CRGBColour&c){assert(c.A==255);++fades;}
};
namespace NScript {
struct CQ_ArenaScript {
 FakeGame&Game;std::vector<CArenaRoundDef>Rounds;long ArenaRound,ArenaRoundWave,TotalCreatures[3],ExtraCreatures,NewCrowdPoints,GoldMultiplier,GoldTotal;bool PauseCrowdChecker,PlayerLeaving,ArenaSpawnNeeded[16];int prizes;
 CQ_ArenaScript(FakeGame&g):Game(g),Rounds(10),ArenaRound(0),ArenaRoundWave(0),ExtraCreatures(0),NewCrowdPoints(300),GoldMultiplier(1),GoldTotal(500),PauseCrowdChecker(true),PlayerLeaving(false),prizes(0){g.owner=this;for(int i=0;i<3;++i)TotalCreatures[i]=0;for(int i=0;i<16;++i)ArenaSpawnNeeded[i]=false;}
 bool IsActiveThreadTerminating(){++Game.checks;return Game.checks==Game.terminateAt;}
 int GetFanfareMusic(int x){return x;}
 CCharString GetEndRoundQuestion(){return CCharString("question");}
 void GivePrizeFund(){++prizes;}
 void PlayWave();
};
}
void FakeGame::NewScriptFrame(){++frames;if(questVisible){for(int i=0;i<3;++i)owner->TotalCreatures[i]=0;if(injectExtra&&!injected){owner->ExtraCreatures=2;injected=true;}}assert(frames<100);}
#define T IsActiveThreadTerminating()
#define NSF Game.NewScriptFrame()
#define AR_KEEPER(TEXT) Conversation=Game.AddNewConversation(Game.GetHero(),false,false);Game.AddLineToConversation(Conversation,TEXT,false,Game.GetHero(),Game.GetHero());
#define AR_CREATURE(I) Rounds[ArenaRound].Waves[ArenaRoundWave].Creatures[I]

void NScript::CQ_ArenaScript::PlayWave()
{
    TRetVoid _Fail_Val;
    CScriptThing SmallEnemySpawnPoint = Game.GetThingWithScriptName("ARENA_SpawnPoint");
    CScriptThing BigEnemySpawnPoint = Game.GetThingWithScriptName("BigCreatureSpawnPoint");
    CTimer DoorTimer;
    PauseCrowdChecker = false;

    if (!Rounds[ArenaRound].Waves[ArenaRoundWave].ShortWave && !(ArenaRound == 0 && ArenaRoundWave == 0)) {
        if (T)
            return _Fail_Val.Return();
        long Conversation;
        AR_KEEPER("TEXT_QST_005_V2_ARENA_KEEPER_COUNTDOWN_THREE")
        DoorTimer = 2;
        while (DoorTimer.GetTime() > 0) {
            NSF;
            if (T)
                return _Fail_Val.Return();
        }
        if (T)
            return _Fail_Val.Return();
        AR_KEEPER("TEXT_QST_005_V2_ARENA_KEEPER_COUNTDOWN_TWO")
        DoorTimer = 2;
        while (DoorTimer.GetTime() > 0) {
            NSF;
            if (T)
                return _Fail_Val.Return();
        }
        if (T)
            return _Fail_Val.Return();
        AR_KEEPER("TEXT_QST_005_V2_ARENA_KEEPER_COUNTDOWN_ONE")
        DoorTimer = 2;
        while (DoorTimer.GetTime() > 0) {
            NSF;
            if (T)
                return _Fail_Val.Return();
        }
        if (T)
            return _Fail_Val.Return();
        AR_KEEPER("TEXT_QST_005_V2_ARENA_KEEPER_COUNTDOWN_GO")
    }
    std::vector<CScriptThing> AllBigSpawnMarkers;
    long MultipleBigEnemySpawnPoints =
        Game.GetAllThingsWithScriptName("MultipleBigCreatureSpawnPoint", AllBigSpawnMarkers);
    std::vector<CScriptThing> AllSpawnMarkers;
    long NumSpawns = Game.GetAllThingsWithScriptName("ArenaSpawn", AllSpawnMarkers);
    CScriptThing FurthestArenaSpawn = Game.GetFurthestWithScriptName(Game.GetHero(), "ArenaSpawn");
    long PositiveSpawn = GFCharStringToInt(FurthestArenaSpawn.GetDataString());
    long NegativeSpawn = PositiveSpawn - 1;
    bool NextSpawnPositive = true;
    if (NegativeSpawn == -1) {
        if (T)
            return _Fail_Val.Return();
        NegativeSpawn = NumSpawns - 1;
    }
    std::vector<CScriptThing> CurrentEnemies;
    Game.GetAllThingsWithScriptName("ArenaEnemy", CurrentEnemies);
    if (ArenaRound == 0 && ArenaRoundWave == 0 && CurrentEnemies.size() != 0) {
        if (T)
            return _Fail_Val.Return();
        TotalCreatures[0] = CurrentEnemies.size();
    } else {
        if (T)
            return _Fail_Val.Return();
        for (long i = 0; i < Rounds[ArenaRound].Waves[ArenaRoundWave].NumWaveCreatures; ++i) {
            if (T)
                return _Fail_Val.Return();
            TotalCreatures[i] = 0;
            for (long j = 0; j < AR_CREATURE(i).NumCreatures; ++j) {
                if (T)
                    return _Fail_Val.Return();
                if (ArenaRound > 4) {
                    if (T)
                        return _Fail_Val.Return();
                    CScriptThing NextCreature;
                    if (NumSpawns == 1) {
                        if (T)
                            return _Fail_Val.Return();
                        Game.SetCreatureCreationDelayFrames(1);
                        NextCreature =
                            Game.CreateCreatureNearby(AR_CREATURE(i).CreatureType,
                                                      BigEnemySpawnPoint.GetPos(), 1.0f, "ArenaEnemy", false);
                        Game.ResetCreatureCreationDelayFrames();
                    } else {
                        if (T)
                            return _Fail_Val.Return();
                        for (long idx = 0; idx < MultipleBigEnemySpawnPoints; ++idx) {
                            if (GFCharStringToInt(AllBigSpawnMarkers[idx].GetDataString()) == j) {

                                if (T)
                                    return _Fail_Val.Return();

                                Game.SetCreatureCreationDelayFrames(1);
                                NextCreature = Game.CreateCreatureNearby(AR_CREATURE(i).CreatureType,
                                                                         AllBigSpawnMarkers[idx].GetPos(),
                                                                         1.0f, "ArenaEnemy", false);
                                Game.ResetCreatureCreationDelayFrames();
                            }
                        }
                    }
                    Game.EntitySetCutsceneBehaviour(NextCreature, CUTSCENE_BEHAVIOUR_PAUSED);
                    Game.GiveThingBestEnemyTarget(NextCreature, Game.GetHero());
                } else {
                    if (T)
                        return _Fail_Val.Return();
                    if (NextSpawnPositive) {

                        if (T)
                            return _Fail_Val.Return();

                        for (long idx = 0; idx < NumSpawns; ++idx) {
                            if (GFCharStringToInt(AllSpawnMarkers[idx].GetDataString()) == PositiveSpawn) {

                                if (T)
                                    return _Fail_Val.Return();

#ifdef _MSC_VER
// Inline-budget: retail calls this constructor at this source site.
#pragma inline_depth(0)
#endif
                                CScriptThing NextCreature;
#ifdef _MSC_VER
#pragma inline_depth()
#endif
                                if (ArenaRound == 3) {
                                    if (T)
                                        return _Fail_Val.Return();
                                    Game.SetCreatureCreationDelayFrames(1);
                                    NextCreature = Game.CreateCreature(AR_CREATURE(i).CreatureType,
                                                                       AllSpawnMarkers[idx].GetPos(),
                                                                       "ArenaEnemy", true);
                                    Game.ResetCreatureCreationDelayFrames();
                                } else {
                                    if (T)
                                        return _Fail_Val.Return();
                                    Game.SetCreatureCreationDelayFrames(1);
                                    NextCreature = Game.CreateCreatureNearby(AR_CREATURE(i).CreatureType,
                                                                             AllSpawnMarkers[idx].GetPos(),
                                                                             1.0f, "ArenaEnemy", false);
                                    Game.ResetCreatureCreationDelayFrames();
                                }
                                Game.EntitySetCutsceneBehaviour(NextCreature, CUTSCENE_BEHAVIOUR_PAUSED);
                                if (Game.GetThingWithScriptName("WhisperAlly").IsAlive()) {
                                    if (T)
                                        return _Fail_Val.Return();
                                    if (rand() % 2 == 0) {
                                        if (T)
                                            return _Fail_Val.Return();
                                        Game.GiveThingBestEnemyTarget(
                                            NextCreature, Game.GetThingWithScriptName("WhisperAlly"));
                                    } else {
                                        if (T)
                                            return _Fail_Val.Return();
                                        Game.GiveThingBestEnemyTarget(NextCreature, Game.GetHero());
                                    }
                                } else {
                                    if (T)
                                        return _Fail_Val.Return();
                                    Game.GiveThingBestEnemyTarget(NextCreature, Game.GetHero());
                                }
                                ArenaSpawnNeeded[PositiveSpawn] = true;
                            }
                        }
                        PositiveSpawn = (PositiveSpawn + 1) % NumSpawns;
                        NextSpawnPositive = false;
                    } else {

                        if (T)
                            return _Fail_Val.Return();

                        for (long idx = 0; idx < NumSpawns; ++idx) {
                            if (GFCharStringToInt(AllSpawnMarkers[idx].GetDataString()) == NegativeSpawn) {

                                if (T)
                                    return _Fail_Val.Return();

#ifdef _MSC_VER
// Inline-budget: retail calls this constructor at this source site.
#pragma inline_depth(0)
#endif
                                CScriptThing NextCreature;
#ifdef _MSC_VER
#pragma inline_depth()
#endif
                                if (ArenaRound == 3) {
                                    if (T)
                                        return _Fail_Val.Return();
                                    Game.SetCreatureCreationDelayFrames(1);
                                    NextCreature = Game.CreateCreature(AR_CREATURE(i).CreatureType,
                                                                       AllSpawnMarkers[idx].GetPos(),
                                                                       "ArenaEnemy", true);
                                    Game.ResetCreatureCreationDelayFrames();
                                } else {
                                    if (T)
                                        return _Fail_Val.Return();
                                    Game.SetCreatureCreationDelayFrames(1);
                                    NextCreature = Game.CreateCreatureNearby(AR_CREATURE(i).CreatureType,
                                                                             AllSpawnMarkers[idx].GetPos(),
                                                                             1.0f, "ArenaEnemy", false);
                                    Game.ResetCreatureCreationDelayFrames();
                                }
                                Game.EntitySetCutsceneBehaviour(NextCreature, CUTSCENE_BEHAVIOUR_PAUSED);
                                if (Game.GetThingWithScriptName("WhisperAlly").IsAlive()) {
                                    if (T)
                                        return _Fail_Val.Return();
                                    if (rand() % 2 == 0) {
                                        if (T)
                                            return _Fail_Val.Return();
                                        Game.GiveThingBestEnemyTarget(
                                            NextCreature, Game.GetThingWithScriptName("WhisperAlly"));
                                    } else {
                                        if (T)
                                            return _Fail_Val.Return();
                                        Game.GiveThingBestEnemyTarget(NextCreature, Game.GetHero());
                                    }
                                } else {
                                    if (T)
                                        return _Fail_Val.Return();
                                    Game.GiveThingBestEnemyTarget(NextCreature, Game.GetHero());
                                }
                                ArenaSpawnNeeded[NegativeSpawn] = true;
                            }
                        }
                        --NegativeSpawn;
                        if (NegativeSpawn == -1) {

                            if (T)
                                return _Fail_Val.Return();

                            NegativeSpawn = NumSpawns - 1;
                        }
                        NextSpawnPositive = true;
                    }
                }
                ++TotalCreatures[i];
            }
            if (T)
                return _Fail_Val.Return();
        }
        if (T)
            return _Fail_Val.Return();
    }
    long CreatureCounter[3];
    long MaxCreatures[3];
    for (long i = 0; i < 3; ++i) {
        if (T)
            return _Fail_Val.Return();
        MaxCreatures[i] = TotalCreatures[i];
        CreatureCounter[i] = -1;
        if (TotalCreatures[i] > 0) {
            if (T)
                return _Fail_Val.Return();
            CreatureCounter[i] =
                Game.AddQuestInfoCounterList(AR_CREATURE(i).HUDType, TotalCreatures[i], 1.0f);
        }
        if (MaxCreatures[i] > 0) {
            if (T)
                return _Fail_Val.Return();
            Game.UpdateQuestInfoCounterList(CreatureCounter[i], TotalCreatures[i], -1);
        }
    }
    if (T)
        return _Fail_Val.Return();
    Game.DisplayQuestInfo(true);
    long AllCreatures = 0;
    for (long i = 0; i < 3; ++i) {
        if (T)
            return _Fail_Val.Return();
        AllCreatures += TotalCreatures[i];
    }
    if (T)
        return _Fail_Val.Return();
    while (AllCreatures > 0) {
        NSF;
        if (T)
            return _Fail_Val.Return();
        AllCreatures = 0;
        for (long i = 0; i < 3; ++i) {
            if (T)
                return _Fail_Val.Return();
            AllCreatures += TotalCreatures[i];
            if (MaxCreatures[i] > 0) {
                if (T)
                    return _Fail_Val.Return();
                Game.UpdateQuestInfoCounterList(CreatureCounter[i], TotalCreatures[i], -1);
            }
        }
        if (T)
            return _Fail_Val.Return();
        if (ExtraCreatures != 0) {
            if (T)
                return _Fail_Val.Return();
            Game.RemoveQuestInfoElement(CreatureCounter[0]);
            MaxCreatures[0] += ExtraCreatures;
            CreatureCounter[0] = Game.AddQuestInfoCounterList(AR_CREATURE(0).HUDType, MaxCreatures[0], 1.0f);
            ExtraCreatures = 0;
        }
        if (!Game.IsLevelLoaded("Arena")) {
            if (T)
                return _Fail_Val.Return();
            PlayerLeaving = true;
            for (long i = 0; i < 3; ++i) {
                if (T)
                    return _Fail_Val.Return();
                TotalCreatures[i] = 0;
            }
            if (T)
                return _Fail_Val.Return();
        }
    }
    if (T)
        return _Fail_Val.Return();
    for (long i = 0; i < 3; ++i) {
        if (T)
            return _Fail_Val.Return();
        if (MaxCreatures[i] > 0) {
            if (T)
                return _Fail_Val.Return();
            Game.RemoveQuestInfoElement(CreatureCounter[i]);
        }
    }
    if (T)
        return _Fail_Val.Return();
    Game.DisplayQuestInfo(false);
    ++ArenaRoundWave;
    PauseCrowdChecker = true;
    if (ArenaRoundWave == Rounds[ArenaRound].NumWaves) {
        if (T)
            return _Fail_Val.Return();
        ++ArenaRound;
        ArenaRoundWave = 0;
        long NumPresents = NewCrowdPoints / 100;
        if (NumPresents > 5) {
            if (T)
                return _Fail_Val.Return();
            NumPresents = 5;
        }
        if (NumPresents > 0) {

#ifdef _MSC_VER
// Inline-budget: retail shares an out-of-line cleanup at the cold exit.
#pragma inline_depth(0)
#endif
            if (T)
                return _Fail_Val.Return();
#ifdef _MSC_VER
#pragma inline_depth()
#endif

            unsigned long random = rand() % pGlobalScriptDef->RewardObjects.size();
#ifdef _MSC_VER
// Inline-budget: retail calls this constructor at this source site.
#pragma inline_depth(0)
#endif
            CScriptThing CreatedObject;
#ifdef _MSC_VER
#pragma inline_depth()
#endif
            C3DVector random_vector(14.0f, 0.0f, 5.0f);
            float RandAngle = (float)(rand() % 360) * (1.0f / 360.0f);
            random_vector.RotateXY(RandAngle);
            C3DVector spawn_pos = Game.GetThingWithScriptName("ARENA_CentrePoint").GetPos() + random_vector;
            CreatedObject = Game.CreateObject(pGlobalScriptDef->RewardObjects[random], spawn_pos, "");
        }
        if (T)
            return _Fail_Val.Return();
        Game.Pause(1.0f);
        Game.OverrideMusic(GetFanfareMusic(rand() % 10), false, false);
        Game.Pause(5.0f);
        Game.StopOverrideMusic(false);
        GivePrizeFund();
        while (!Game.IsHeroControlledByPlayer()) {
            NSF;

#ifdef _MSC_VER
// Inline-budget: retail shares an out-of-line cleanup at the cold exit.
#pragma inline_depth(0)
#endif
            if (T)
                return _Fail_Val.Return();
#ifdef _MSC_VER
#pragma inline_depth()
#endif
        }

        if (T)
            return _Fail_Val.Return();

        CScriptGameResourceObjectMovieBase speech_movie;
        Game.StartMovieSequence("", speech_movie);
#ifdef _MSC_VER
// Inline-budget: retail calls this constructor at this source site.
#pragma inline_depth(0)
#endif
        CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
#ifdef _MSC_VER
#pragma inline_depth()
#endif
        CScriptGameResourceObjectScriptedThingBase seh_Hero;
        if (seh_Hero.IsValid())
            seh_Hero.Release();
        while (!Game.StartScriptingEntity(Game.GetHero(), seh_Hero, SCRIPT_AI_PRIORITY_HIGHEST)) {
            NSF;

            if (T)
                return _Fail_Val.Return();
        }
        if (T)
            return _Fail_Val.Return();
        if (ArenaRound < 8) {
            if (T)
                return _Fail_Val.Return();
            Game.GiveHeroYesNoQuestion(GetEndRoundQuestion(), "TEXT_QST_005_V2_ARENA_END_ROUND_QUESTION_YES",
                                       "TEXT_QST_005_V2_ARENA_END_ROUND_QUESTION_NO", "", true);
            long answer;
            while ((answer = Game.MsgIsQuestionAnsweredYesOrNo()) < 0) {
                NSF;
                if (T)
                    return _Fail_Val.Return();
            }
            if (T)
                return _Fail_Val.Return();
            if (answer == 1) {
                if (T)
                    return _Fail_Val.Return();
                ++GoldMultiplier;
            } else {
                if (T)
                    return _Fail_Val.Return();
                PlayerLeaving = true;
                Game.GiveHeroGold(GoldTotal);
                GoldTotal = 0;
                long Conversation = Game.AddNewConversation(Game.GetHero(), false, false);
                Game.AddLineToConversation(Conversation, "TEXT_QST_005_V2_ARENA_KEEPER_RETURN", false,
                                           Game.GetHero(), Game.GetHero());
                while (Game.IsConversationActive(Conversation)) {
                    NSF;
                    if (T)
                        return _Fail_Val.Return();
                }
                if (T)
                    return _Fail_Val.Return();
            }
        }
        Game.FadeScreenOut(0.5f, 0.5f, CRGBColour(0, 0, 0));
        Game.Pause(0.5f);
    }
}
#undef T
#undef NSF
#undef AR_KEEPER
#undef AR_CREATURE
using namespace NScript;
static void Balanced(){assert(liveTimers==0&&liveMovies==0&&livePauses==0&&liveHandles==0&&liveThings==1);}
static long Run(int mode,long cancelAt){
 FakeGame game;CQ_ArenaScript script(game);script.ArenaRound=1;script.ArenaRoundWave=1;script.Rounds[1].NumWaves=2;game.terminateAt=cancelAt;
 if(mode==0){script.ArenaRound=0;script.ArenaRoundWave=0;script.Rounds[0].NumWaves=2;}
 if(mode==1){script.Rounds[1].Waves[1].ShortWave=true;}
 if(mode==2){script.ArenaRound=3;script.ArenaRoundWave=0;script.Rounds[3].NumWaves=1;game.whisper=true;}
 if(mode==3){script.ArenaRound=5;script.ArenaRoundWave=0;script.Rounds[5].NumWaves=1;game.markerCount=1;}
 if(mode==4){script.ArenaRound=5;script.ArenaRoundWave=0;script.Rounds[5].NumWaves=1;game.bigCount=2;script.Rounds[5].Waves[0].Creatures[0].NumCreatures=2;}
 if(mode==5){script.ArenaRound=0;script.ArenaRoundWave=0;game.existing=2;script.Rounds[0].NumWaves=1;}
 if(mode==6){game.injectExtra=true;game.levelLoaded=false;}
 if(mode==7){game.answer=0;script.NewCrowdPoints=600;}
 if(mode==8){script.ArenaRound=7;script.ArenaRoundWave=0;script.Rounds[7].NumWaves=1;}
 if(mode==9){script.NewCrowdPoints=0;}
 if(mode==10){script.Rounds[1].Waves[1].NumWaveCreatures=3;script.Rounds[1].Waves[1].Creatures[1].NumCreatures=0;script.Rounds[1].Waves[1].Creatures[2].NumCreatures=2;}
 if(mode==11){script.Rounds[1].Waves[1].NumWaveCreatures=0;}
 if(mode==12){script.Rounds[1].Waves[1].Creatures[0].NumCreatures=6;game.whisper=true;}
 script.PlayWave();Balanced();
 if(cancelAt<0){
  assert(script.PauseCrowdChecker&&!game.questVisible&&game.delayDepth==0);
  if(mode==0){assert(script.ArenaRound==0&&script.ArenaRoundWave==1&&game.objects==0&&script.prizes==0&&game.lines==0);}
  else{assert(script.prizes==1&&game.movies==1&&game.fades==1);if(mode==8)assert(script.ArenaRound==8&&game.questions==0);else assert(game.questions==1);}
  if(mode==1||mode==0||mode==5)assert(game.lines==(mode==5?0:0));
  else assert(game.lines==(mode==7?5:4));
  if(mode==2)assert(game.exact==1&&game.nearby==0);
  if(mode==3)assert(game.nearby==1&&game.exact==0);
  if(mode==4)assert(game.nearby==2);
  if(mode==5)assert(game.nearby==0&&game.exact==0&&game.countAdds==1);
  if(mode==6)assert(game.injected&&script.PlayerLeaving&&game.countAdds==2);
  if(mode==7)assert(script.PlayerLeaving&&script.GoldTotal==0&&game.goldPaid==500&&script.GoldMultiplier==1);
  if(mode==9)assert(game.objects==0);
  else if(mode!=0)assert(game.objects==1);
  if(mode==10)assert(game.countAdds==2&&game.nearby==3);
  if(mode==11)assert(game.countAdds==0&&game.nearby==0&&game.exact==0);
 }
 return game.checks;
}
int main(){
 for(int mode=0;mode<=12;++mode){std::srand(mode+1);Run(mode,-1);assert(liveThings==0);}
 // Every termination check reached on these independent branch paths is cancelled in turn.
 const int paths[8]={0,1,2,3,5,6,7,8};
 for(int p=0;p<8;++p){long checks=Run(paths[p],-1);for(long cancel=1;cancel<=checks;++cancel){Run(paths[p],cancel);assert(liveThings==0);}}
 std::puts("00f1eed0_TEST PASS");return 0;
}
#if defined(_MSC_VER) && _MSC_VER < 1400
#undef for
#endif
