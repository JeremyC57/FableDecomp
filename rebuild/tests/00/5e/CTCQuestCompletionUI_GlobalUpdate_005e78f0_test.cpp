// Whole selected body, external engine calls instrumented. No shared main receipt writes.
#include <cstdio>
#include <cmath>
#include <string>
#include <vector>
#include <list>
#include <limits>
#include <cstring>
static int failures=0;static void check(bool v){if(!v)++failures;}
static bool samef(float a,float b){return(a!=a&&b!=b)||a==b||std::fabs(double(a)-b)<0.00001*(1+std::fabs(double(b)));}
typedef std::string CCharString;
struct CWideString {long value;CWideString():value(0){}void Sprintf(const wchar_t*,long n){value=n;}};
static CWideString GFIntToWideString(long n){CWideString t;t.value=n;return t;}
struct C3DVector{float X,Y,Z;C3DVector(float x,float y,float z):X(x),Y(y),Z(z){}};
enum EMessageEventType{MESSAGE_EVENT_LEAVING_QUEST_START_SCREEN=73,MESSAGE_EVENT_QUEST_COMPLETED=51,MESSAGE_EVENT_QUEST_FAILED=52};enum EPlayerMode{PLAYER_MODE_QUEST_COMPLETION_UI=37};
struct Trace {int scene,start,open,group,exitSound,messages,music,activate,hero,ready,removeBoasts,clearObservers,clearActions,clearChild,clearList,removeMode,cutscene,deleted,region,boastIndex;bool sceneValue;unsigned long groups[3];float gains[3];int eventType;const void*eventParent;long eventData;std::vector<unsigned long>stopped;std::vector<int>textIDs;std::vector<long>textValues;Trace():scene(0),start(0),open(0),group(0),exitSound(0),messages(0),music(0),activate(0),hero(0),ready(0),removeBoasts(0),clearObservers(0),clearActions(0),clearChild(0),clearList(0),removeMode(0),cutscene(0),deleted(0),region(0),boastIndex(0),sceneValue(false),eventType(0),eventParent(0),eventData(0){} };static Trace tr;
namespace NUISystem {
struct CComponent{int id,state,changes;bool changed;CComponent(int n=0):id(n),state(0),changes(0),changed(false){}virtual~CComponent(){++tr.deleted;}bool ChangedState(){return changed;}int GetCurrentStateIndex(){return state;}void ChangeState(int s){++changes;state=s;}};
struct CText:public CComponent{CText(int n):CComponent(n){}void SetValue(const CWideString&t,bool a,bool b){check(a&&b);tr.textIDs.push_back(id);tr.textValues.push_back(t.value);}};
struct CUIMiscThingsDef{CCharString SoundExit;float DigitCountTime;};static CUIMiscThingsDef misc;
struct MiscPtr{CUIMiscThingsDef*operator->()const{return &misc;}};
struct CManager {static CManager*GetInstance();MiscPtr GetMiscThingsDef(){return MiscPtr();}void PlaySoundW(CCharString&s,long n){check(s=="exit"&&n==1);++tr.exitSound;}void ClearObservers(){++tr.clearObservers;}void ClearActionParams(){++tr.clearActions;}void ClearBastardChild(){++tr.clearChild;}};static CManager manager;CManager*CManager::GetInstance(){return &manager;}
struct CList{static void ClearPreviousSelected(){++tr.clearList;}};
}
struct CASoundEmitter{void StopSpecificSample(unsigned long h){tr.stopped.push_back(h);}};
struct CASoundManager{CASoundEmitter emitter;CASoundEmitter&Get2DEmitter(){return emitter;}void SetGainFader(unsigned long g,float v,float t){check(tr.group<3&&t==0);tr.groups[tr.group]=g;tr.gains[tr.group++]=v;}};static CASoundManager sound;static CASoundManager*PSoundManager=&sound;
struct CUserProfileManager{static CUserProfileManager&Get();float GetSoundVolume()const{return 0.7f;}float GetDialogueVolume()const{return 0.4f;}};static CUserProfileManager profile;CUserProfileManager&CUserProfileManager::Get(){return profile;}
struct CPlayerGui{void Open(){++tr.open;}};static CPlayerGui gui;
template<class T>struct CTBaseSingletonNoDefaultAlloc{static T*instance;static T&Get(){return*instance;}};template<class T>T*CTBaseSingletonNoDefaultAlloc<T>::instance=0;
struct CMusicManager{void ClearUIOverridingMusicSet(){++tr.music;}};
struct CThing;struct CMessageEvent{};
struct CMessageEventManager{CMessageEvent*AddMessage(EMessageEventType e,unsigned long f,const C3DVector&p,CThing*from,CThing*to,float a,float b,long data){check(f==0x32&&p.X==0&&p.Y==0&&p.Z==0&&to==0&&a==0&&b==0);++tr.messages;tr.eventType=e;tr.eventParent=from;tr.eventData=data;return 0;}};
struct CWorld{bool loading;CMusicManager music;CMessageEventManager messages;void SetAsStarting3DRendering(){++tr.start;}bool IsPerformingRegionLoad()const{++tr.region;return loading;}CMusicManager&GetMusicManager(){return music;}CMessageEventManager&GetMessageEventManager(){return messages;}};
struct CPlayer{void RemoveMode(EPlayerMode m){check(m==PLAYER_MODE_QUEST_COMPLETION_UI);++tr.removeMode;}};
struct CPlayerManager{CPlayer player;CPlayer&GetMainPlayer(){return player;}};
struct CIEngine{void Enable3DSceneRendering(bool b){++tr.scene;tr.sceneValue=b;}void CutscenePreload(bool a,bool b){check(!a&&!b);++tr.cutscene;}};
struct CDisplayEngine{CIEngine engine;CIEngine&Get3DEngine(){return engine;}};
struct CMainGameComponent{CWorld world;CDisplayEngine display;static CMainGameComponent*Get();static long GetConstantFPS();CDisplayEngine&GetDisplayEngine(){return display;}CWorld&GetWorld(){return world;}};static CMainGameComponent game;static long fps;CMainGameComponent*CMainGameComponent::Get(){return &game;}long CMainGameComponent::GetConstantFPS(){return fps;}
struct CTCHeroStats{long completed,money,reward;long GetNumBoastsCompleted()const{return completed;}void SetNumBoastsCompleted(long n){completed=n;}void AddMoney(long n,bool flag){check(flag);money+=n;}void AddRewardMoney(long n){reward+=n;}};
struct CThing{CTCHeroStats*stats;template<class T>bool GetTC(T**p){*p=stats;return stats!=0;}};
struct CQuestManager{
 struct CBoast{CCharString ScriptName;bool Failed;long Cost,Reward;const CCharString&GetScriptName()const{return ScriptName;}bool IsFailed()const{return Failed;}long GetCost()const{return Cost;}long GetReward()const{return Reward;}};
 std::list<CBoast>boasts;bool allReady;bool AreAllBoastsProcessed(CCharString&){++tr.ready;return allReady;}std::list<CBoast>&GetQuestBoasts(){return boasts;}void RemoveAcceptedBoastsForScript(CCharString){++tr.removeBoasts;}long GetIndexFromRegisteredQuest(const CCharString&){++tr.boastIndex;return 12345;}
};static CQuestManager quests;
struct CTCQuestCompletionUI{
 enum CompletionType{QUEST_START=0,QUEST_SUCCESS=1,QUEST_FAILURE=2,HERO_DIED=3};
 bool Active,ReallyActive,Quitting,WaitingForRegionLoad,WaitingForPlayerControl,WaitingForBoastProcessing,QuittingFadeOut,controlled;
 unsigned int Frame;int FrameStartedBoastProcessing;CompletionType Completion;std::vector<NUISystem::CComponent*>Components;NUISystem::CComponent*PFrame,*PBlack;NUISystem::CText*PRenownText,*PGoldText,*PExperienceText;
 CCharString ScriptName;std::list<CQuestManager::CBoast>Boasts;long TargetGoldGained,InitialGold,TargetExperienceGained,InitialExperience,TargetRenownGained,InitialRenown;float GoldGainedDisplayed,ExperienceGainedDisplayed,RenownGainedDisplayed;unsigned long ExperienceCountingSound,GoldCountingSound,RenownCountingSound;CThing*ParentThing;CPlayerManager players;
 CWorld&GetWorld(){return game.world;}CThing&GetParentThing(){return*ParentThing;}CPlayerManager&GetPlayerManager(){return players;}bool IsHeroControlledByPlayer()const{++tr.hero;return controlled;}void ActivateReal(){++tr.activate;ReallyActive=true;}
 void GlobalUpdate(bool paused);
};
void CTCQuestCompletionUI::GlobalUpdate(bool paused)
{
    if (Quitting) {
        CMainGameComponent::Get()->GetDisplayEngine().Get3DEngine().Enable3DSceneRendering(true);
        GetWorld().SetAsStarting3DRendering();
        Active = false;
        Quitting = false;
        ReallyActive = false;
        delete PBlack;
        PBlack = 0;
        CTBaseSingletonNoDefaultAlloc<CPlayerGui>::Get().Open();
        if (PSoundManager && Completion != QUEST_START && Completion != QUEST_SUCCESS) {
            PSoundManager->SetGainFader(0x10, CUserProfileManager::Get().GetSoundVolume(), 0.0f);
            PSoundManager->SetGainFader(0x40, CUserProfileManager::Get().GetDialogueVolume(), 0.0f);
            PSoundManager->SetGainFader(0x80, CUserProfileManager::Get().GetSoundVolume(), 0.0f);
        }
        if (ExperienceCountingSound) {
            PSoundManager->Get2DEmitter().StopSpecificSample(ExperienceCountingSound);
            ExperienceCountingSound = 0;
        }
        if (GoldCountingSound) {
            PSoundManager->Get2DEmitter().StopSpecificSample(GoldCountingSound);
            GoldCountingSound = 0;
        }
        if (RenownCountingSound) {
            PSoundManager->Get2DEmitter().StopSpecificSample(RenownCountingSound);
            RenownCountingSound = 0;
        }
        NUISystem::CManager::GetInstance()->PlaySoundW(NUISystem::CManager::GetInstance()->GetMiscThingsDef()->SoundExit, 1);
        if (Completion == QUEST_START) {
            GetWorld().GetMessageEventManager().AddMessage(MESSAGE_EVENT_LEAVING_QUEST_START_SCREEN, 0x32, C3DVector(0.0f, 0.0f, 0.0f), &GetParentThing(), 0, 0.0f, 0.0f, 0);
        } else if (Completion == QUEST_SUCCESS) {
            if (ScriptName != "")
                GetWorld().GetMessageEventManager().AddMessage(MESSAGE_EVENT_QUEST_COMPLETED, 0x32, C3DVector(0.0f, 0.0f, 0.0f), 0, 0, 0.0f, 0.0f,
                    CTBaseSingletonNoDefaultAlloc<CQuestManager>::Get().GetIndexFromRegisteredQuest(ScriptName));
        } else if (Completion == QUEST_FAILURE) {
            if (ScriptName != "")
                GetWorld().GetMessageEventManager().AddMessage(MESSAGE_EVENT_QUEST_FAILED, 0x32, C3DVector(0.0f, 0.0f, 0.0f), 0, 0, 0.0f, 0.0f,
                    CTBaseSingletonNoDefaultAlloc<CQuestManager>::Get().GetIndexFromRegisteredQuest(ScriptName));
        }
        if (Completion != QUEST_START && Completion != QUEST_SUCCESS)
            GetWorld().GetMusicManager().ClearUIOverridingMusicSet();
        return;
    }
    if (!Active)
        return;
    ++Frame;
    if (WaitingForRegionLoad) {
        if (Frame > 1 && !CMainGameComponent::Get()->GetWorld().IsPerformingRegionLoad())
            WaitingForRegionLoad = false;
        return;
    }
    if (WaitingForPlayerControl && IsHeroControlledByPlayer())
        WaitingForPlayerControl = false;
    if (WaitingForPlayerControl)
        return;
    if (WaitingForBoastProcessing) {
        if (FrameStartedBoastProcessing == -1)
            FrameStartedBoastProcessing = Frame;
        if (CTBaseSingletonNoDefaultAlloc<CQuestManager>::Get().AreAllBoastsProcessed(ScriptName)) {
            Boasts.clear();
            WaitingForBoastProcessing = false;
            std::list<CQuestManager::CBoast>& boasts = CTBaseSingletonNoDefaultAlloc<CQuestManager>::Get().GetQuestBoasts();
            for (std::list<CQuestManager::CBoast>::iterator it = boasts.begin(); it != boasts.end(); ) {
                if (it->GetScriptName() == ScriptName) {
                    CTCHeroStats* ptc_hero_stats;
                    if (GetParentThing().GetTC(&ptc_hero_stats)) {
                        if (!it->IsFailed()) {
                            ptc_hero_stats->SetNumBoastsCompleted(ptc_hero_stats->GetNumBoastsCompleted()+1);
                            if (Completion == QUEST_SUCCESS) {
                                ptc_hero_stats->AddMoney(it->GetReward(), true);
                                ptc_hero_stats->AddRewardMoney(it->GetReward());
                            }
                        } else if (Completion == QUEST_SUCCESS) {
                            ptc_hero_stats->AddMoney(-it->GetCost(), true);
                            ptc_hero_stats->AddRewardMoney(-it->GetCost());
                        }
                    }
                    Boasts.push_back(*it);
                    it = boasts.erase(it);
                } else {
                    ++it;
                }
            }
        }
        if (Frame - FrameStartedBoastProcessing > (unsigned long)CMainGameComponent::GetConstantFPS()) {
            CTBaseSingletonNoDefaultAlloc<CQuestManager>::Get().RemoveAcceptedBoastsForScript(ScriptName);
            WaitingForBoastProcessing = false;
        }
        return;
    }
    if (PBlack->ChangedState() && PBlack->GetCurrentStateIndex() == 1 && !ReallyActive) {
        PBlack->ChangeState(0);
        if (!ReallyActive)
            ActivateReal();
        return;
    }
    if (QuittingFadeOut) {
        if (PBlack->ChangedState() && PBlack->GetCurrentStateIndex() == 1) {
            PBlack->ChangeState(0);
            for (unsigned long count = 0; count < Components.size(); ++count)
                delete Components[count];
            Components.clear();
            PFrame = 0;
            NUISystem::CManager::GetInstance()->ClearObservers();
            NUISystem::CManager::GetInstance()->ClearActionParams();
            NUISystem::CList::ClearPreviousSelected();
            NUISystem::CManager::GetInstance()->ClearBastardChild();
            GetPlayerManager().GetMainPlayer().RemoveMode(PLAYER_MODE_QUEST_COMPLETION_UI);
            CMainGameComponent::Get()->GetDisplayEngine().Get3DEngine().CutscenePreload(false, false);
            return;
        }
        if (PBlack->ChangedState() && PBlack->GetCurrentStateIndex() == 0) {
            Quitting = true;
            QuittingFadeOut = false;
        }
        return;
    }
    if (PBlack->ChangedState() && PBlack->GetCurrentStateIndex() == 1) {
        PBlack->ChangeState(0);
    } else if (PBlack->ChangedState() && PBlack->GetCurrentStateIndex() == 0) {
    // Original PDB block1f5cc, donor022c58b2..022c5c53, owns count_time.
    if (PRenownText)
        PRenownText->SetValue(GFIntToWideString((long)RenownGainedDisplayed), true, true);
    if (PGoldText)
        PGoldText->SetValue(GFIntToWideString((long)GoldGainedDisplayed), true, true);
    if (PExperienceText)
        PExperienceText->SetValue(GFIntToWideString((long)ExperienceGainedDisplayed), true, true);
    float count_time = NUISystem::CManager::GetInstance()->GetMiscThingsDef()->DigitCountTime;
    RenownGainedDisplayed += ((float)(TargetRenownGained - InitialRenown)) / (CMainGameComponent::GetConstantFPS() * count_time);
    ExperienceGainedDisplayed += ((float)(TargetExperienceGained - InitialExperience)) / (CMainGameComponent::GetConstantFPS() * count_time);
    GoldGainedDisplayed += ((float)(TargetGoldGained - InitialGold)) / (CMainGameComponent::GetConstantFPS() * count_time);
    if (RenownGainedDisplayed > TargetRenownGained)
        RenownGainedDisplayed = TargetRenownGained;
    if (GoldGainedDisplayed > TargetGoldGained)
        GoldGainedDisplayed = TargetGoldGained;
    if (ExperienceGainedDisplayed > TargetExperienceGained)
        ExperienceGainedDisplayed = TargetExperienceGained;
    if (ExperienceCountingSound && ExperienceGainedDisplayed == TargetExperienceGained) {
        PSoundManager->Get2DEmitter().StopSpecificSample(ExperienceCountingSound);
        ExperienceCountingSound = 0;
    }
    if (GoldCountingSound && GoldGainedDisplayed == TargetGoldGained) {
        PSoundManager->Get2DEmitter().StopSpecificSample(GoldCountingSound);
        GoldCountingSound = 0;
    }
    if (RenownCountingSound && RenownGainedDisplayed == TargetRenownGained) {
        PSoundManager->Get2DEmitter().StopSpecificSample(RenownCountingSound);
        RenownCountingSound = 0;
    }
    }
}

static int signed32(unsigned int n){return n<=0x7fffffffu?(int)n:-int(0xffffffffu-n)-1;}
static float modelCount(float shown,long target,long initial,float time){float denom=float(fps)*time;float result=float(double(shown)+double(target-initial)/denom);if(result>target)result=float(target);return result;}
static CQuestManager::CBoast boast(const CCharString&s,bool failed,long cost,long reward){CQuestManager::CBoast b;b.ScriptName=s;b.Failed=failed;b.Cost=cost;b.Reward=reward;return b;}
int main(){CTBaseSingletonNoDefaultAlloc<CPlayerGui>::instance=&gui;CTBaseSingletonNoDefaultAlloc<CQuestManager>::instance=&quests;unsigned long cases=0;const unsigned int frames[4]={0,1,31,0xfffffffeu};
 for(int gate=0;gate<6;++gate)for(int completion=0;completion<4;++completion)for(int flags=0;flags<64;++flags)for(int state=0;state<3;++state)for(int f=0;f<4;++f)for(int masks=0;masks<8;++masks){
  int soundMask=masks,textMask=(masks*5+f)&7;
  tr=Trace();CTCQuestCompletionUI u;CTCHeroStats stats;stats.completed=7;stats.money=100;stats.reward=50;CThing thing;thing.stats=(flags&4)?&stats:0;u.ParentThing=&thing;
  u.Active=gate!=1;u.ReallyActive=(flags&2)!=0;u.Quitting=gate==0;u.QuittingFadeOut=(flags&8)!=0;u.WaitingForRegionLoad=gate==2;u.WaitingForPlayerControl=gate==3;u.WaitingForBoastProcessing=gate==4;u.controlled=(flags&1)!=0;u.Completion=(CTCQuestCompletionUI::CompletionType)completion;
  u.Frame=frames[f];u.FrameStartedBoastProcessing=(flags&16)?-1:signed32(u.Frame-40u);u.ScriptName=(flags&32)?"":"quest";fps=(flags&16)?10:30;game.world.loading=(flags&1)!=0;quests.allReady=(flags&1)!=0;quests.boasts.clear();
  quests.boasts.push_back(boast(u.ScriptName,false,3,20));quests.boasts.push_back(boast("other",false,2,10));quests.boasts.push_back(boast(u.ScriptName,true,5,30));quests.boasts.push_back(boast(u.ScriptName,false,7,40));u.Boasts.push_back(boast("old",false,1,1));
  NUISystem::CComponent frame(-10);u.PFrame=&frame;u.PBlack=new NUISystem::CComponent(1000);u.PBlack->state=state;u.PBlack->changed=(flags&1)!=0;NUISystem::CText renown(0),gold(1),experience(2);u.PRenownText=(textMask&1)?&renown:0;u.PGoldText=(textMask&2)?&gold:0;u.PExperienceText=(textMask&4)?&experience:0;
  for(int i=0;i<flags%4;++i)u.Components.push_back(new NUISystem::CComponent(i));int components=(int)u.Components.size();
  NUISystem::misc.SoundExit="exit";const float times[4]={0.25f,1.0f,2.5f,0.0f};NUISystem::misc.DigitCountTime=times[f];
  u.TargetRenownGained=10+flags%9;u.InitialRenown=(flags&16)?u.TargetRenownGained+5:0;u.TargetGoldGained=30+flags%13;u.InitialGold=(flags&8)?u.TargetGoldGained:2;u.TargetExperienceGained=20+flags%7;u.InitialExperience=(flags&4)?-10:0;
  u.RenownGainedDisplayed=f==0?-2.25f:(f==1?0.0f:(f==2?5.5f:float(u.TargetRenownGained+10)));u.GoldGainedDisplayed=f==0?-3.75f:(f==1?0.0f:(f==2?15.5f:float(u.TargetGoldGained+10)));u.ExperienceGainedDisplayed=f==0?-1.25f:(f==1?0.0f:(f==2?7.5f:float(u.TargetExperienceGained+10)));
  u.ExperienceCountingSound=(soundMask&1)?101:0;u.GoldCountingSound=(soundMask&2)?102:0;u.RenownCountingSound=(soundMask&4)?103:0;PSoundManager=&sound;if(soundMask==0&&(flags&32))PSoundManager=0;
  unsigned int expectedFrame=u.Frame;int expectedStarted=u.FrameStartedBoastProcessing;bool expectedActive=u.Active,expectedReally=u.ReallyActive,expectedQuit=u.Quitting,expectedFade=u.QuittingFadeOut,expectedRegion=u.WaitingForRegionLoad,expectedPlayer=u.WaitingForPlayerControl,expectedBoast=u.WaitingForBoastProcessing;bool deleteBlack=false,deleteComponents=false,count=false;int changeBlack=0;
  float oldR=u.RenownGainedDisplayed,oldG=u.GoldGainedDisplayed,oldE=u.ExperienceGainedDisplayed,newR=oldR,newG=oldG,newE=oldE;unsigned long eSound=u.ExperienceCountingSound,gSound=u.GoldCountingSound,rSound=u.RenownCountingSound;int scene=0,open=0,msg=0,music=0,groups=0,activate=0,clear=0,ready=0,hero=0,region=0,remove=0,index=0,expectedEvent=0;long expectedMoney=100,expectedReward=50,expectedCompleted=7;std::vector<unsigned long>stops;
  if(u.Quitting){scene=1;open=1;expectedActive=expectedReally=expectedQuit=false;deleteBlack=true;groups=PSoundManager&&completion>=2?3:0;if(eSound){stops.push_back(eSound);eSound=0;}if(gSound){stops.push_back(gSound);gSound=0;}if(rSound){stops.push_back(rSound);rSound=0;}music=completion>=2?1:0;if(completion==0){msg=1;expectedEvent=73;}else if(completion<=2&&!u.ScriptName.empty()){msg=1;index=1;expectedEvent=completion==1?51:52;}}
  else if(u.Active){++expectedFrame;
   if(u.WaitingForRegionLoad){region=expectedFrame>1?1:0;if(region&&!game.world.loading)expectedRegion=false;}
   else{bool proceed=true;if(u.WaitingForPlayerControl){hero=1;if(u.controlled)expectedPlayer=false;else proceed=false;}
    if(proceed&&u.WaitingForBoastProcessing){ready=1;if(expectedStarted==-1)expectedStarted=signed32(expectedFrame);if(quests.allReady){expectedBoast=false;if(thing.stats){expectedCompleted+=2;if(completion==1){expectedMoney+=55;expectedReward+=55;}}}unsigned int elapsed=expectedFrame-(unsigned int)expectedStarted;if(elapsed>(unsigned long)fps){remove=1;expectedBoast=false;}}
    else if(proceed){bool changed=(flags&1)!=0;if(changed&&state==1&&!u.ReallyActive){changeBlack=1;activate=1;expectedReally=true;}
     else if(u.QuittingFadeOut){if(changed&&state==1){changeBlack=1;deleteComponents=true;clear=1;}else if(changed&&state==0){expectedQuit=true;expectedFade=false;}}
     else if(changed&&state==1)changeBlack=1;
     else if(changed&&state==0){count=true;newR=modelCount(oldR,u.TargetRenownGained,u.InitialRenown,times[f]);newE=modelCount(oldE,u.TargetExperienceGained,u.InitialExperience,times[f]);newG=modelCount(oldG,u.TargetGoldGained,u.InitialGold,times[f]);if(eSound&&newE==u.TargetExperienceGained){stops.push_back(eSound);eSound=0;}if(gSound&&newG==u.TargetGoldGained){stops.push_back(gSound);gSound=0;}if(rSound&&newR==u.TargetRenownGained){stops.push_back(rSound);rSound=0;}}
    }
   }
  }
  u.GlobalUpdate((flags&1)!=0);
  check(u.Frame==expectedFrame&&u.FrameStartedBoastProcessing==expectedStarted&&u.Active==expectedActive&&u.ReallyActive==expectedReally&&u.Quitting==expectedQuit&&u.QuittingFadeOut==expectedFade&&u.WaitingForRegionLoad==expectedRegion&&u.WaitingForPlayerControl==expectedPlayer&&u.WaitingForBoastProcessing==expectedBoast);
  check(tr.scene==scene&&(!scene||tr.sceneValue)&&tr.start==scene&&tr.open==open&&tr.group==groups&&tr.exitSound==(gate==0?1:0)&&tr.messages==msg&&tr.boastIndex==index&&tr.music==music&&tr.activate==activate&&tr.hero==hero&&tr.region==region&&tr.ready==ready&&tr.removeBoasts==remove);
  if(groups)check(tr.groups[0]==16&&tr.groups[1]==64&&tr.groups[2]==128&&tr.gains[0]==0.7f&&tr.gains[1]==0.4f&&tr.gains[2]==0.7f);
  if(msg)check(tr.eventType==expectedEvent&&tr.eventParent==(completion==0?&thing:0)&&tr.eventData==(completion==0?0:12345));
  check(tr.clearObservers==clear&&tr.clearActions==clear&&tr.clearChild==clear&&tr.clearList==clear&&tr.removeMode==clear&&tr.cutscene==clear);check(tr.deleted==(deleteBlack?1:0)+(deleteComponents?components:0));check((u.PBlack==0)==deleteBlack&&(u.PFrame==0)==deleteComponents&&u.Components.size()==(deleteComponents?0:(unsigned int)components));if(u.PBlack)check(u.PBlack->changes==changeBlack);
  check(stats.completed==expectedCompleted&&stats.money==expectedMoney&&stats.reward==expectedReward);bool processed=ready&&quests.allReady;check(u.Boasts.size()==(processed?3u:1u)&&quests.boasts.size()==(processed?1u:4u));
  check(samef(u.RenownGainedDisplayed,newR)&&samef(u.GoldGainedDisplayed,newG)&&samef(u.ExperienceGainedDisplayed,newE)&&u.ExperienceCountingSound==eSound&&u.GoldCountingSound==gSound&&u.RenownCountingSound==rSound&&tr.stopped==stops);
  std::vector<int>ids;std::vector<long>values;if(count){if(u.PRenownText){ids.push_back(0);values.push_back((long)oldR);}if(u.PGoldText){ids.push_back(1);values.push_back((long)oldG);}if(u.PExperienceText){ids.push_back(2);values.push_back((long)oldE);}}check(tr.textIDs==ids&&tr.textValues==values);
  delete u.PBlack;for(unsigned int i=0;i<u.Components.size();++i)delete u.Components[i];++cases;
 }
 check(cases==147456);std::printf(failures?"005e78f0_TEST FAIL\n":"005e78f0_TEST PASS\n");return failures?1:0;
}