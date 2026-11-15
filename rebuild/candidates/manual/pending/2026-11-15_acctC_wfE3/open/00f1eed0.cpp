// NScript::CQ_ArenaScript::PlayWave() -- one arena wave: keeper countdown (3, 2, 1, go) unless the wave is short or the first one, spawn every creature def's creatures (big ones from round 5 at the big spawn markers, the rest alternating up and down the "ArenaSpawn" markers from the one furthest from the hero), track the kills on the quest counters until the wave is cleared or the hero leaves the arena; after the round's last wave the crowd reward object, the fanfare, the prize and the end-of-round question
// Grade: DIFFER(9253v9262) with /d2inlT54 (T53 same; default 11534) -- wfE3 hand draft. Scoped inline_depth(0) round the two small-spawn `CScriptThing NextCreature;` declarations, `CScriptThing CreatedObject;` and the pause object (retail CALLS 0x006E7B40 at 0x00F1F7D8 / 0x00F1FB72 / 0x00F20D1A and 0x00CBE09A at 0x00F20F5C; it expands the big-spawn NextCreature ctor at 0x00F1F4DF). Left: (1) block layout of the quest-counter loop (ours right after the first-wave branch, retail after the creature loops) and the expanded / called exit pads around it; (2) the FadeScreenOut CRGBColour ctor is called at T54 (retail expands it, 0x00F21107); (3) the first countdown exit 0x00F1F059 (retail expands ~CTimer there).
// Flags: /d2inlT54
// Leaf: ?PlayWave@CQ_ArenaScript@NScript@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00F1EED0, 9262 bytes, thiscall, plain `ret`, no EH frame.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CRGBCOLOUR
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_OUTOFLINE_DTOR
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_OUTOFLINE_ASSIGN
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_OUTOFLINE_CTOR
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_OUTOFLINE_CTOR
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_OUTOFLINE_DTOR
#define FABLE_RETAIL_CDEFSTRING_OUTOFLINE_CONVERSION
extern "C" int __cdecl rand();                                          // 0x00BFEB16 (MSVCR71)
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/C3DVector.h"
#include "engine/CScriptThing.h"
#include "engine/CRGBColour.h"
#include "engine/EMusicSetType.h"
#include "engine/ECutsceneBehaviour.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CQ_SunnyvaleMasterData.h"
#include "engine/CEntityScriptBase.h"
#include "engine/CQ_ArenaScript.h"
#include "engine/CArenaRoundDef.h"
#include "engine/CArenaWaveDef.h"
#include "engine/CArenaCreatureDef.h"
#include "engine/CScriptDef.h"
#include "engine/CDefPointer.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
class CScriptGameResourceObjectMovieBase;
template<> CCountedPointer<CScriptGameResourceObjectMovieBase>::~CCountedPointer();   // 0x006E7AB0
#include "engine/CScriptGameResourceObjectMovieBase.h"
#include "engine/CWideScreenMagicPauseEntities.h"
#include "engine/CTimer.h"
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };
extern CGameScriptInterfaceBase* pDSTGame;   // 0x0143E8F8
extern CDefPointer<const CScriptDef> pGlobalScriptDef;   // 0x0143E90C
long GFCharStringToInt(const CCharString&);   // 0x0099E7F0
inline CScriptThing::CScriptThing() { }
inline CScriptThing::~CScriptThing() { }
inline CCharString CScriptThing::GetDataString() const { if (!PImp.Data) return NullString; return PImp.Data->GetDataString(); }
inline const C3DVector& CScriptThing::GetPos() const { if (!PImp.Data) return NullCoord; return PImp.Data->GetPos(); }
inline CTimer::CTimer() : TimerIndex(pDSTGame->RegisterTimer()) {}
inline CTimer::~CTimer() { pDSTGame->DeregisterTimer(TimerIndex); }
FABLE_FORCEINLINE CTimer& CTimer::operator=(long time) { pDSTGame->SetTimer(TimerIndex, time); return *this; }
FABLE_FORCEINLINE long CTimer::GetTime() const { return pDSTGame->GetTimer(TimerIndex); }
#define T IsActiveThreadTerminating()
#define NSF Game->NewScriptFrame()
#define AR_START(THING,HANDLE) \
    if (HANDLE.IsValid()) HANDLE.Release(); \
    while (!Game->StartScriptingEntity(THING,HANDLE,SCRIPT_AI_PRIORITY_HIGHEST)) { \
        NSF; \
        if (T) return; \
    } \
    if (T) return;
#define AR_KEEPER(TEXT) \
    { \
        long conversation=Game->AddNewConversation(Game->GetHero(),false,false); \
        Game->AddLineToConversation(conversation,TEXT,false,Game->GetHero(),Game->GetHero()); \
    }
#define AR_COUNT(N) \
    DoorTimer=N; \
    while (DoorTimer.GetTime()>0) { \
        NSF; \
        if (T) return; \
    } \
    if (T) return;
#define AR_CREATURE(I) Rounds[ArenaRound].Waves[ArenaRoundWave].Creatures[I]
void NScript::CQ_ArenaScript::PlayWave()
{
    CScriptThing SmallEnemySpawnPoint=Game->GetThingWithScriptName("ARENA_SpawnPoint");
    CScriptThing BigEnemySpawnPoint=Game->GetThingWithScriptName("BigCreatureSpawnPoint");
    CTimer DoorTimer;
    PauseCrowdChecker=false;
    if (!Rounds[ArenaRound].Waves[ArenaRoundWave].ShortWave && (ArenaRound!=0 || ArenaRoundWave!=0)) {
        if (T) return;
        AR_KEEPER("TEXT_QST_005_V2_ARENA_KEEPER_COUNTDOWN_THREE")
        AR_COUNT(2)
        AR_KEEPER("TEXT_QST_005_V2_ARENA_KEEPER_COUNTDOWN_TWO")
        AR_COUNT(2)
        AR_KEEPER("TEXT_QST_005_V2_ARENA_KEEPER_COUNTDOWN_ONE")
        AR_COUNT(2)
        AR_KEEPER("TEXT_QST_005_V2_ARENA_KEEPER_COUNTDOWN_GO")
    }
    std::vector<CScriptThing> AllBigSpawnMarkers;
    long MultipleBigEnemySpawnPoints=Game->GetAllThingsWithScriptName("MultipleBigCreatureSpawnPoint",AllBigSpawnMarkers);
    std::vector<CScriptThing> AllSpawnMarkers;
    long NumSpawnMarkers=Game->GetAllThingsWithScriptName("ArenaSpawn",AllSpawnMarkers);
    CScriptThing FurthestArenaSpawn=Game->GetFurthestWithScriptName(Game->GetHero(),"ArenaSpawn");
    long NextSpawnUp=GFCharStringToInt(FurthestArenaSpawn.GetDataString());
    long NextSpawnDown=NextSpawnUp-1;
    bool SpawnUp=true;
    if (NextSpawnDown==-1) {
        if (T) return;
        NextSpawnDown=NumSpawnMarkers-1;
    }
    std::vector<CScriptThing> CurrentEnemies;
    Game->GetAllThingsWithScriptName("ArenaEnemy",CurrentEnemies);
    if (ArenaRound==0 && ArenaRoundWave==0 && CurrentEnemies.size()!=0) {
        if (T) return;
        TotalCreatures[0]=CurrentEnemies.size();
    }
    else {
        if (T) return;
        for (long i=0; i<Rounds[ArenaRound].Waves[ArenaRoundWave].NumWaveCreatures; ++i) {
            if (T) return;
            TotalCreatures[i]=0;
            for (long j=0; j<AR_CREATURE(i).NumCreatures; ++j) {
                if (T) return;
                if (ArenaRound>4) {
                    if (T) return;
                    CScriptThing NextCreature;
                    if (NumSpawnMarkers==1) {
                        if (T) return;
                        Game->SetCreatureCreationDelayFrames(1);
                        NextCreature=Game->CreateCreatureNearby(AR_CREATURE(i).CreatureType,BigEnemySpawnPoint.GetPos(),1.0f,"ArenaEnemy",false);
                        Game->ResetCreatureCreationDelayFrames();
                    }
                    else {
                        if (T) return;
                        for (long k=0; k<MultipleBigEnemySpawnPoints; ++k) {
                            if (GFCharStringToInt(AllBigSpawnMarkers[k].GetDataString())==j) {
                                if (T) return;
                                Game->SetCreatureCreationDelayFrames(1);
                                NextCreature=Game->CreateCreatureNearby(AR_CREATURE(i).CreatureType,AllBigSpawnMarkers[k].GetPos(),1.0f,"ArenaEnemy",false);
                                Game->ResetCreatureCreationDelayFrames();
                            }
                        }
                    }
                    Game->EntitySetCutsceneBehaviour(NextCreature,CUTSCENE_BEHAVIOUR_PAUSED);
                    Game->GiveThingBestEnemyTarget(NextCreature,Game->GetHero());
                }
                else {
                    if (T) return;
                    if (SpawnUp) {
                        if (T) return;
                        for (long k=0; k<NumSpawnMarkers; ++k) {
                            if (GFCharStringToInt(AllSpawnMarkers[k].GetDataString())==NextSpawnUp) {
                                if (T) return;
#pragma inline_depth(0)
                                CScriptThing NextCreature;
#pragma inline_depth()
                                if (ArenaRound==3) {
                                    if (T) return;
                                    Game->SetCreatureCreationDelayFrames(1);
                                    NextCreature=Game->CreateCreature(AR_CREATURE(i).CreatureType,AllSpawnMarkers[k].GetPos(),"ArenaEnemy",true);
                                    Game->ResetCreatureCreationDelayFrames();
                                }
                                else {
                                    if (T) return;
                                    Game->SetCreatureCreationDelayFrames(1);
                                    NextCreature=Game->CreateCreatureNearby(AR_CREATURE(i).CreatureType,AllSpawnMarkers[k].GetPos(),1.0f,"ArenaEnemy",false);
                                    Game->ResetCreatureCreationDelayFrames();
                                }
                                Game->EntitySetCutsceneBehaviour(NextCreature,CUTSCENE_BEHAVIOUR_PAUSED);
                                if (Game->GetThingWithScriptName("WhisperAlly").IsAlive()) {
                                    if (T) return;
                                    if (rand()%2==0) {
                                        if (T) return;
                                        Game->GiveThingBestEnemyTarget(NextCreature,Game->GetThingWithScriptName("WhisperAlly"));
                                    }
                                    else {
                                        if (T) return;
                                        Game->GiveThingBestEnemyTarget(NextCreature,Game->GetHero());
                                    }
                                }
                                else {
                                    if (T) return;
                                    Game->GiveThingBestEnemyTarget(NextCreature,Game->GetHero());
                                }
                                ArenaSpawnNeeded[NextSpawnUp]=true;
                            }
                        }
                        NextSpawnUp=(NextSpawnUp+1)%NumSpawnMarkers;
                        SpawnUp=false;
                    }
                    else {
                        if (T) return;
                        for (long k=0; k<NumSpawnMarkers; ++k) {
                            if (GFCharStringToInt(AllSpawnMarkers[k].GetDataString())==NextSpawnDown) {
                                if (T) return;
#pragma inline_depth(0)
                                CScriptThing NextCreature;
#pragma inline_depth()
                                if (ArenaRound==3) {
                                    if (T) return;
                                    Game->SetCreatureCreationDelayFrames(1);
                                    NextCreature=Game->CreateCreature(AR_CREATURE(i).CreatureType,AllSpawnMarkers[k].GetPos(),"ArenaEnemy",true);
                                    Game->ResetCreatureCreationDelayFrames();
                                }
                                else {
                                    if (T) return;
                                    Game->SetCreatureCreationDelayFrames(1);
                                    NextCreature=Game->CreateCreatureNearby(AR_CREATURE(i).CreatureType,AllSpawnMarkers[k].GetPos(),1.0f,"ArenaEnemy",false);
                                    Game->ResetCreatureCreationDelayFrames();
                                }
                                Game->EntitySetCutsceneBehaviour(NextCreature,CUTSCENE_BEHAVIOUR_PAUSED);
                                if (Game->GetThingWithScriptName("WhisperAlly").IsAlive()) {
                                    if (T) return;
                                    if (rand()%2==0) {
                                        if (T) return;
                                        Game->GiveThingBestEnemyTarget(NextCreature,Game->GetThingWithScriptName("WhisperAlly"));
                                    }
                                    else {
                                        if (T) return;
                                        Game->GiveThingBestEnemyTarget(NextCreature,Game->GetHero());
                                    }
                                }
                                else {
                                    if (T) return;
                                    Game->GiveThingBestEnemyTarget(NextCreature,Game->GetHero());
                                }
                                ArenaSpawnNeeded[NextSpawnDown]=true;
                            }
                        }
                        --NextSpawnDown;
                        if (NextSpawnDown==-1) {
                            if (T) return;
                            NextSpawnDown=NumSpawnMarkers-1;
                        }
                        SpawnUp=true;
                    }
                }
                ++TotalCreatures[i];
            }
            if (T) return;
        }
        if (T) return;
    }
    long CreatureCounter[3];
    long MaxCreatures[3];
    for (long i=0; i<3; ++i) {
        if (T) return;
        MaxCreatures[i]=TotalCreatures[i];
        CreatureCounter[i]=-1;
        if (TotalCreatures[i]>0) {
            if (T) return;
            CreatureCounter[i]=Game->AddQuestInfoCounterList(AR_CREATURE(i).HUDType,TotalCreatures[i],1.0f);
        }
        if (MaxCreatures[i]>0) {
            if (T) return;
            Game->UpdateQuestInfoCounterList(CreatureCounter[i],TotalCreatures[i],-1);
        }
    }
    if (T) return;
    Game->DisplayQuestInfo(true);
    long CreaturesLeft=0;
    for (long i=0; i<3; ++i) {
        if (T) return;
        CreaturesLeft+=TotalCreatures[i];
    }
    if (T) return;
    while (CreaturesLeft>0) {
        NSF;
        if (T) return;
        CreaturesLeft=0;
        for (long i=0; i<3; ++i) {
            if (T) return;
            CreaturesLeft+=TotalCreatures[i];
            if (MaxCreatures[i]>0) {
                if (T) return;
                Game->UpdateQuestInfoCounterList(CreatureCounter[i],TotalCreatures[i],-1);
            }
        }
        if (T) return;
        if (ExtraCreatures!=0) {
            if (T) return;
            Game->RemoveQuestInfoElement(CreatureCounter[0]);
            MaxCreatures[0]+=ExtraCreatures;
            CreatureCounter[0]=Game->AddQuestInfoCounterList(AR_CREATURE(0).HUDType,MaxCreatures[0],1.0f);
            ExtraCreatures=0;
        }
        if (!Game->IsLevelLoaded("Arena")) {
            if (T) return;
            PlayerLeaving=true;
            for (long i=0; i<3; ++i) {
                if (T) return;
                TotalCreatures[i]=0;
            }
            if (T) return;
        }
    }
    if (T) return;
    for (long i=0; i<3; ++i) {
        if (T) return;
        if (MaxCreatures[i]>0) {
            if (T) return;
            Game->RemoveQuestInfoElement(CreatureCounter[i]);
        }
    }
    if (T) return;
    Game->DisplayQuestInfo(false);
    ++ArenaRoundWave;
    PauseCrowdChecker=true;
    if (ArenaRoundWave==Rounds[ArenaRound].NumWaves) {
        if (T) return;
        ++ArenaRound;
        ArenaRoundWave=0;
        long CrowdLevel=NewCrowdPoints/100;
        if (CrowdLevel>5) {
            if (T) return;
            CrowdLevel=5;
        }
        if (CrowdLevel>0) {
            if (T) return;
            long reward=rand()%pGlobalScriptDef->RewardObjects.size();
#pragma inline_depth(0)
            CScriptThing CreatedObject;
#pragma inline_depth()
            C3DVector random_vector(14.0f,0.0f,5.0f);
            random_vector.RotateXY((float)(rand()%360)*(1.0f/360.0f));
            C3DVector spawn_pos=random_vector+Game->GetThingWithScriptName("ARENA_CentrePoint").GetPos();
            CreatedObject=Game->CreateObject(pGlobalScriptDef->RewardObjects[reward],spawn_pos,"");
        }
        if (T) return;
        Game->Pause(1.0f);
        Game->OverrideMusic(GetFanfareMusic(rand()%10),false,false);
        Game->Pause(5.0f);
        Game->StopOverrideMusic(false);
        GivePrizeFund();
        while (!Game->IsHeroControlledByPlayer()) {
            NSF;
            if (T) return;
        }
        if (T) return;
        CScriptGameResourceObjectMovieBase speech_movie;
        Game->StartMovieSequence("",speech_movie);
#pragma inline_depth(0)
        CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
#pragma inline_depth()
        CScriptGameResourceObjectScriptedThingBase seh_Hero;
        AR_START(Game->GetHero(),seh_Hero)
        if (ArenaRound<8) {
            if (T) return;
            Game->GiveHeroYesNoQuestion(GetEndRoundQuestion(),"TEXT_QST_005_V2_ARENA_END_ROUND_QUESTION_YES","TEXT_QST_005_V2_ARENA_END_ROUND_QUESTION_NO","",true);
            long answer;
            while ((answer=Game->MsgIsQuestionAnsweredYesOrNo())<0) {
                NSF;
                if (T) return;
            }
            if (T) return;
            if (answer==1) {
                if (T) return;
                ++GoldMultiplier;
            }
            else {
                if (T) return;
                PlayerLeaving=true;
                Game->GiveHeroGold(GoldTotal);
                GoldTotal=0;
                long conversation=Game->AddNewConversation(Game->GetHero(),false,false);
                Game->AddLineToConversation(conversation,"TEXT_QST_005_V2_ARENA_KEEPER_RETURN",false,Game->GetHero(),Game->GetHero());
                while (Game->IsConversationActive(conversation)) {
                    NSF;
                    if (T) return;
                }
                if (T) return;
            }
        }
        Game->FadeScreenOut(0.5f,0.5f,CRGBColour(0,0,0,255));
        Game->Pause(0.5f);
    }
}
