#include <stdio.h>
#include <string>
typedef std::string CCharString;
class CWorld {public:static long GetFrame(){return 42;}};
struct CTCSleep {
    bool ready,fast;long woke;
    void SetIsNotReadyToSleep(){ready=false;}
    void SetFastTrack(bool f){fast=f;}
    void SetLastFrameWokeUp(long f){woke=f;}
};
template<class T>class CDefPointer {public:T* p;T* operator->()const{return p;}};
struct CBedDef {CCharString GetUpFromBedFastAnimName,GetUpFromBedAnimName;};
struct CTCBed {
    CDefPointer<const CBedDef> def;int starts,clears;long anim;
    CDefPointer<const CBedDef> GetDefinition()const{return def;}
    void StartGetOutOfBedAnimation(long n){++starts;anim=n;}void Clear(){++clears;}
};
struct CThing {
    bool alive,provide;CTCBed* bed;
    void PeekTC(const CTCBed** out)const{*out=bed;}
    bool GetTC(CTCBed** out){*out=provide?bed:0;return provide;}
};
struct BedRef{CThing* thing;CThing* operator->()const{return thing;}CThing* Get()const{return thing;}};
static bool IS_THING_ALIVE(const BedRef& p){return p.thing&&p.thing->alive;}
namespace NCreatureMode{enum EMode{MODE_OTHER=1,MODE_SLEEP_BED=13};}
struct CTCCreatureModeManager {
    struct CModeBase {NCreatureMode::EMode mode;NCreatureMode::EMode GetType()const{return mode;}} current;
    int removes;unsigned long removed;
    CModeBase& GetCurrentMode(){return current;}void RemoveMode(unsigned long m){++removes;removed=m;}
};
struct Creature{CTCSleep* sleep;CTCCreatureModeManager* modes;
    bool GetTC(CTCSleep** p){*p=sleep;return sleep!=0;}
    bool GetTC(CTCCreatureModeManager** p){*p=modes;return true;}
};
struct Brain{bool succeeds,bed_dies;unsigned calls;CCharString played;};
namespace NCreatureMode {
    bool GetUpFromSleeping(Creature&,CThing& bed,const CCharString& anim,Brain* brain){
        ++brain->calls;brain->played=anim;if(brain->bed_dies)bed.alive=false;return brain->succeeds;
    }
}
class CAIStateGroup_GoToBed {
public:
    int replaced;void OnReplace(){++replaced;}
};
class CAIStateGroup_MinionSleep:public CAIStateGroup_GoToBed {
public:
    Creature creature;BedRef PBed;Brain* PBrain;long BedGetOutOfBedFastAnim;bool PlayEmergencyGetOutOfBed;
    Creature& GetCreature(){return creature;}const CWorld& PeekWorld(){static CWorld w;return w;}
    void OnReplace();
};
void CAIStateGroup_MinionSleep::OnReplace()
{
    CTCSleep* tc_sleep;
    if (GetCreature().GetTC(&tc_sleep)) {
        tc_sleep->SetIsNotReadyToSleep();
        tc_sleep->SetFastTrack(false);
        tc_sleep->SetLastFrameWokeUp(PeekWorld().GetFrame());
    }
    CTCCreatureModeManager* ptc_mode_manager;
    GetCreature().GetTC(&ptc_mode_manager);
    bool needToRemoveMode = ptc_mode_manager->GetCurrentMode().GetType() == NCreatureMode::MODE_SLEEP_BED;
    if (needToRemoveMode) {
        if (IS_THING_ALIVE(PBed)) {
            const CTCBed* ptcbed = 0;
            PBed->PeekTC(&ptcbed);
            CDefPointer<const CBedDef> pbed_def = ptcbed->GetDefinition();
            CCharString getUpAnim = pbed_def->GetUpFromBedFastAnimName;
            if (getUpAnim == "")
                getUpAnim = pbed_def->GetUpFromBedAnimName;
            if (NCreatureMode::GetUpFromSleeping(GetCreature(), *PBed.Get(), getUpAnim, PBrain)) {
                CTCBed* ptc_bed;
                if (IS_THING_ALIVE(PBed) && PBed->GetTC(&ptc_bed) && BedGetOutOfBedFastAnim)
                    ptc_bed->StartGetOutOfBedAnimation(BedGetOutOfBedFastAnim);
                needToRemoveMode = false;
            }
        }
        if (needToRemoveMode) {
            ptc_mode_manager->RemoveMode(NCreatureMode::MODE_SLEEP_BED);
            CTCBed* ptc_bed;
            if (IS_THING_ALIVE(PBed) && PBed->GetTC(&ptc_bed))
                ptc_bed->Clear();
        }
    }
    CAIStateGroup_GoToBed::OnReplace();
    PlayEmergencyGetOutOfBed = false;
}

int main(){
    for(unsigned i=0;i<512;++i){
        bool hasSleep=(i&1)!=0,inBed=(i&2)!=0,bedExists=(i&4)!=0,alive=(i&8)!=0;
        bool fast=(i&16)!=0,succeeds=(i&32)!=0,dies=(i&64)!=0,provides=(i&128)!=0,hasAnim=(i&256)!=0;
        CTCSleep sleep={true,true,-1};
        CTCCreatureModeManager modes={{inBed?NCreatureMode::MODE_SLEEP_BED:NCreatureMode::MODE_OTHER},0,0};
        CBedDef def={fast?"fast":"","normal"};CTCBed bed={{&def},0,0,0};
        CThing thing={alive,provides,&bed};Brain brain={succeeds,dies,0,""};
        CAIStateGroup_MinionSleep g;g.replaced=0;g.PlayEmergencyGetOutOfBed=true;
        g.creature.sleep=hasSleep?&sleep:0;g.creature.modes=&modes;g.PBed.thing=bedExists?&thing:0;
        g.PBrain=&brain;g.BedGetOutOfBedFastAnim=hasAnim?31:0;
        g.OnReplace();
        const bool wake=inBed&&bedExists&&alive;
        const bool remove=inBed&&!(wake&&succeeds);
        const bool available=bedExists&&alive&&!(wake&&dies)&&provides;
        const bool start=wake&&succeeds&&available&&hasAnim;
        if(sleep.ready!=!hasSleep||sleep.fast!=!hasSleep||sleep.woke!=(hasSleep?42:-1)||
           brain.calls!=unsigned(wake)||(wake&&brain.played!=(fast?"fast":"normal"))||
           modes.removes!=int(remove)||(remove&&modes.removed!=13)||bed.starts!=int(start)||
           (start&&bed.anim!=31)||bed.clears!=int(remove&&available)||g.replaced!=1||g.PlayEmergencyGetOutOfBed)return 1;
    }
    puts("0092cda0_TEST PASS");
}