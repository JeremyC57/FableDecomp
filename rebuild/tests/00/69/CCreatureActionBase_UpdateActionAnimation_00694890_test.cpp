#include <stdio.h>
#include <stdlib.h>
#include <math.h>
static unsigned trial;static int code;
#define CHECK(x) do{if(!(x)){printf("failed line %d trial %u\n",__LINE__,trial);exit(1);}}while(0)
#include <string.h>
struct CCharString {
 const char* value;
 CCharString(const char* v=""):value(v){}
 bool operator==(const CCharString& other)const{return strcmp(value,other.value)==0;}
 bool StartsWith(const CCharString& other)const{return strncmp(value,other.value,strlen(other.value))==0;}
};
namespace std {
 template<class T>struct vector {
  typedef T* iterator;typedef const T* const_iterator;T data[12];int n;
  vector():n(0){} void push_back(const T& v){CHECK(n<12);data[n++]=v;}
  iterator begin(){return data;}iterator end(){return data+n;}int size()const{return n;}
 };
 template<class K,class V>struct pair{K first;V second;pair(K k,V v):first(k),second(v){}};
 template<class K,class V>pair<K,V> make_pair(K k,V v){return pair<K,V>(k,v);}
 template<class K,class V>struct map{
  K keys[12];V values[12];int n;map():n(0){}
  void insert(const pair<K,V>& p){for(int i=0;i<n;++i)if(keys[i]==p.first)return;CHECK(n<12);keys[n]=p.first;values[n++]=p.second;}
  int size()const{return n;}bool empty()const{return n==0;}
  const V& operator[](K k)const{for(int i=0;i<n;++i)if(keys[i]==k)return values[i];CHECK(false);return values[0];}
 };
}
static const CCharString PERFORM_ACTION("PERFORM_ACTION"),CUT_SCENE_EVENT_PREFIX("CUT_");
template<class T,class L,class U> void GFLimit(T&x,const L&lo,const U&hi){if(x<lo)x=lo;else if(x>hi)x=hi;}
struct CAnimFrameInfo{float Current,Last;CAnimFrameInfo(float c=0,float l=0):Current(c),Last(l){}};
struct CAnimSet{CAnimFrameInfo f;int sets;CAnimFrameInfo GetFrame()const{return f;}void SetFrame(const CAnimFrameInfo& v){f=v;++sets;}};
enum EAnimUpdateMode{ANIM_UPDATE_NONE=0};struct CAnimInit{int flags;long index;CAnimInit(int type,unsigned long mask,long a,EAnimUpdateMode mode,bool loop,long v,long prio):flags(v),index(a){CHECK(type==1&&!mask&&!loop&&prio==100&&mode==ANIM_UPDATE_NONE);}};
struct CAnimateThing{int finished,set,comb,removed,flags;long transition;
 void SetAsFinished(){++finished;}void SetMainAnim(const CAnimInit&a,long t,long b){CHECK(!b&&a.index==17);++set;flags=a.flags;transition=t;}
 void AddCombinationAnim(const CAnimInit&a,long t,long b,bool x,bool y){CHECK(!b&&a.index==17&&x&&!y);++comb;flags=a.flags;transition=t;}
 void RemoveCombinationAnim(CAnimSet* p){CHECK(p);++removed;}
};static CAnimateThing animation;
struct CTCAnimationComplex{CAnimateThing& GetAnimation(){return animation;}}static tc;
struct C3DAnimationInfo{bool loading,loaded;int began;bool IsLoading()const{return loading;}bool IsLoaded()const{return loaded;}void BeginLoading(bool b){CHECK(!b);++began;}}static info;
struct CMeshDataBank{C3DAnimationInfo*PeekAnimation(long a){CHECK(a==17);return &info;}}static bank;
struct CWorld{CMeshDataBank*PeekMeshBank(){return &bank;}}static world;
struct Appearance{bool IsResourcesLoaded(bool x)const{CHECK(x);return (trial&1)!=0;}}static appearance;
struct CThingCreatureBase{void GetTC(CTCAnimationComplex**p){*p=&tc;}
 bool IsAnimCombinationAnim(long a)const{CHECK(a==17);return (trial&2)!=0;}
 Appearance* GetTCGraphicAppearance(){return &appearance;}
};static CThingCreatureBase creature;
struct CAnimationEntry{long BankIndex;int calls;long GetFlags(){++calls;return trial>>10;}};
template<class T>struct Ptr{T*p;T*Get()const{return p;}T*operator->()const{return p;}T&operator*()const{return *p;}};
struct CMainGameComponent{static long GetConstantFPS(){return 20;}};
namespace NAnimationEvents{struct CManager{
 bool AnimHasEvent(long a,const CCharString&n)const{CHECK(a==23&&n==PERFORM_ACTION);return (trial&4)!=0;}
 float GetEventTimeInSeconds(long a,const CCharString&n)const{CHECK(a==23);return n==PERFORM_ACTION?(float)(code-1)*0.25f:n=="CUT_b"?.4f:.2f;}
 void GetEventNames(long a,std::vector<CCharString>&v)const{CHECK(a==23);v.push_back("walk");v.push_back("CUT_a");v.push_back("CUT_b");v.push_back("CUT_duplicate");}
};static CManager mgr;CManager&GetGameEventManager(){return mgr;}}
static CAnimSet sets[3];static int count,chosen;
static void GetActionAnimSetList(std::vector<CAnimSet*>&out,CThingCreatureBase&c,long a){CHECK(&c==&creature&&a==17);for(int i=0;i<count;++i)out.push_back(sets+i);}
static CAnimSet* GetPActionAnimSet(CThingCreatureBase&c,long a){CHECK(&c==&creature&&a==17);return chosen?sets:0;}
struct CCreatureActionBase{Ptr<CAnimationEntry>PAnimEntry;Ptr<CThingCreatureBase>PCreature;bool AddedActionAnim;long ActionTime,TotalTime;float RunningTimeSeconds;
 std::map<long,CCharString> CutSceneEvents;int steps,loaded;
 bool WaitForMeshToLoad()const{return (trial&8)!=0;}CWorld& PeekWorld(){return world;}
 void InitialiseActionPause(){CHECK(steps==0);++steps;}void InitialiseActionTime(){CHECK(steps==1);++steps;}void InitialiseActionEvents(){CHECK(steps==2);++steps;}
 long GetAnimationIndex()const{return 23;}long GetAnimDelayTime()const{return code-2;}
 long GetAnimationTransitionInTime()const{return 13;}long GetAnimExtraFlags()const{return 32;}
 bool CanAnimationTransitionOut()const{return (trial&16)!=0;}bool AllowsCombinationAnims()const{return (trial&32)!=0;}bool OverrideFaceWithDefault()const{return (trial&64)!=0;}
 void OnAnimationLoaded(){++loaded;}void UpdateActionAnimation();
};
void CCreatureActionBase::UpdateActionAnimation()
{
    if (!PAnimEntry.Get() || !PAnimEntry->BankIndex)
        return;
    long const_fps = CMainGameComponent::GetConstantFPS();
    CTCAnimationComplex* ptc_anim;
    PCreature->GetTC(&ptc_anim);
    CAnimateThing& anim = ptc_anim->GetAnimation();
    bool combination_anim = PCreature->IsAnimCombinationAnim(PAnimEntry->BankIndex);
    if (!combination_anim && !AddedActionAnim)
        anim.SetAsFinished();
    if (!AddedActionAnim) {
        bool waitForMesh = false;
        if (WaitForMeshToLoad() && !PCreature->GetTCGraphicAppearance()->IsResourcesLoaded(true))
            waitForMesh = true;
        if (!waitForMesh && PeekWorld().PeekMeshBank()->PeekAnimation(PAnimEntry->BankIndex)->IsLoaded()) {
            InitialiseActionPause();
            InitialiseActionTime();
            InitialiseActionEvents();
            const NAnimationEvents::CManager& game_events_manager = NAnimationEvents::GetGameEventManager();
            long anim_index = GetAnimationIndex();
            if (game_events_manager.AnimHasEvent(anim_index, PERFORM_ACTION)) {
                ActionTime = (long)(game_events_manager.GetEventTimeInSeconds(anim_index, PERFORM_ACTION) * const_fps);
                ActionTime += GetAnimDelayTime();
                GFLimit(ActionTime, 0, TotalTime);
            }
            std::vector<CCharString> anim_event_names;
            game_events_manager.GetEventNames(anim_index, anim_event_names);
            if (false) {} else for (std::vector<CCharString>::const_iterator event_it = anim_event_names.begin();
                 event_it != anim_event_names.end(); ++event_it) {
                const CCharString& event_name = *event_it;
                if (event_name.StartsWith(CUT_SCENE_EVENT_PREFIX)) {
                    long event_frame = (long)(game_events_manager.GetEventTimeInSeconds(anim_index, event_name) *
                                              const_fps + GetAnimDelayTime());
                    CutSceneEvents.insert(std::make_pair(event_frame, event_name));
                }
            }
            long trans_time = GetAnimationTransitionInTime();
            long flags = GetAnimExtraFlags();
            if (combination_anim) {
                CAnimInit anim_init(1, 0, PAnimEntry->BankIndex, ANIM_UPDATE_NONE, false, flags, 100);
                anim.AddCombinationAnim(anim_init, trans_time, 0, true, false);
            } else {
                long anim_flags = PAnimEntry->GetFlags();
                if (!CanAnimationTransitionOut())
                    flags |= 2;
                if (!AllowsCombinationAnims())
                    flags |= 1;
                if (OverrideFaceWithDefault())
                    flags |= 4;
                CAnimInit anim_init(1, 0, PAnimEntry->BankIndex, ANIM_UPDATE_NONE, false, flags, 100);
                anim.SetMainAnim(anim_init, trans_time, 0);
            }
            std::vector<CAnimSet*> anim_sets;
            GetActionAnimSetList(anim_sets, *PCreature, PAnimEntry->BankIndex);
            if (anim_sets.size() == 1) {
                AddedActionAnim = true;
                OnAnimationLoaded();
            } else {
                if (false) {} else for (std::vector<CAnimSet*>::iterator it = anim_sets.begin(); it != anim_sets.end(); ++it) {
                    CAnimSet* panim_set = *it;
                    anim.RemoveCombinationAnim(panim_set);
                }
            }
        } else if (!PeekWorld().PeekMeshBank()->PeekAnimation(PAnimEntry->BankIndex)->IsLoading()) {
            PeekWorld().PeekMeshBank()->PeekAnimation(PAnimEntry->BankIndex)->BeginLoading(false);
        }
    } else {
        CAnimSet* panim_set = GetPActionAnimSet(*PCreature, PAnimEntry->BankIndex);
        if (panim_set) {
            const CAnimFrameInfo& frame_info = panim_set->GetFrame();
            float delay_time = (float)GetAnimDelayTime() / const_fps;
            float total_time = (float)TotalTime / const_fps;
            float assumed_running_time = RunningTimeSeconds;
            float cur_frame = (assumed_running_time - delay_time) / (total_time - delay_time);
            GFLimit(cur_frame, 0.0f, 1.0f);
            float last_frame = frame_info.Current;
            panim_set->SetFrame(CAnimFrameInfo(cur_frame, last_frame));
        }
    }
}
int main(){
 for(trial=0;trial<32768;++trial){
  CCreatureActionBase c;CAnimationEntry entry;code=(trial>>7)%8;count=(trial>>12)%4;chosen=(trial%3)!=0;
  entry.BankIndex=trial%13==0?0:17;entry.calls=0;c.PAnimEntry.p=trial%17==0?0:&entry;c.PCreature.p=&creature;
  c.AddedActionAnim=(trial&1024)!=0;c.steps=c.loaded=0;c.ActionTime=-5;c.TotalTime=12;c.RunningTimeSeconds=(float)((int)(trial%13)-3)*.1f;
  info.loaded=(trial&2048)!=0;info.loading=trial%5==0;info.began=0;animation=CAnimateThing();
  sets[0].sets=0;sets[0].f=CAnimFrameInfo(.61f,.32f);
  const bool valid=c.PAnimEntry.p&&entry.BankIndex!=0,added=c.AddedActionAnim;
  const bool ready=valid&&!added&&(!(trial&8)||(trial&1))&&info.loaded;
  c.UpdateActionAnimation();
  CHECK(animation.finished==(valid&&!added&&!(trial&2)));
  CHECK(c.steps==(ready?3:0)&&c.loaded==(ready&&count==1));
  CHECK(c.AddedActionAnim==(added||(ready&&count==1)));
  CHECK(info.began==(valid&&!added&&!ready&&!info.loading));
  CHECK(animation.set==(ready&&!(trial&2))&&animation.comb==(ready&&(trial&2)));
  CHECK(entry.calls==animation.set&&animation.removed==(ready&&count!=1?count:0));
  if(ready){int f=32;if(!(trial&2)){if(!(trial&16))f|=2;if(!(trial&32))f|=1;if(trial&64)f|=4;}
   CHECK(animation.flags==f&&animation.transition==13);
   CHECK(c.CutSceneEvents.size()==2&&c.CutSceneEvents[4+(code-2)]=="CUT_a"&&c.CutSceneEvents[8+(code-2)]=="CUT_b");
  }else CHECK(c.CutSceneEvents.empty());
  long t=-5;if(ready&&(trial&4)){t=(code-1)*5+code-2;if(t<0)t=0;if(t>12)t=12;}
  CHECK(c.ActionTime==t);CHECK(sets[0].sets==(valid&&added&&chosen));
  if(sets[0].sets){double delay=(code-2)/20.0;double f=(c.RunningTimeSeconds-delay)/(.6-delay);if(f<0)f=0;if(f>1)f=1;
   CHECK(fabs(sets[0].f.Current-f)<.00001&&sets[0].f.Last==.61f);}
 }
 puts("00694890_TEST PASS");return 0;
}