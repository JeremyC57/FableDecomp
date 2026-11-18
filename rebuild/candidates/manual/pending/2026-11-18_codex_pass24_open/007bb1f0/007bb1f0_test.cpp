#include <cassert>
#include <cstdio>
#include <cstring>
#include <vector>

struct C3DVector { float X,Y,Z; };
struct CThing;
struct CThingAICreature {};
struct CTCVolumeContainmentTracker {
    CThing* Inside;
    CThing* GetPThingInside() const { return Inside; }
};
struct CTCTrespassable { bool Trespassable,IsBeingTrespassedIn; };
enum ETCInterfaceType { LocalTracking=0x53 };
enum EOpinionDeedType { Petty=19,Ignored=23,Caught=35,PublicVisit=0x4d };
enum EOpinionPostDeedType { RecordImmediately=2 };
struct Deed {
    EOpinionDeedType Type;
    CThing* Building;
    bool Private;
};
struct CTCHeroOpinionDeedLog {
    std::vector<Deed> Calls;
    long PostDeed(EOpinionDeedType type,const C3DVector& pos,CThing* building,int related,
                  EOpinionPostDeedType mode,bool private_post,bool notify,int category,
                  bool ignore,int reason)
    {
        assert(pos.X==1 && pos.Y==2 && pos.Z==3 && related==0);
        assert(mode==RecordImmediately && notify && category==0 && !ignore && reason==0);
        assert((type!=PublicVisit)==private_post);
        assert(private_post==(building!=0));
        Deed deed={type,building,private_post};
        Calls.push_back(deed);
        return 100+Calls.size();
    }
};
struct CThing {
    bool Alive,HasTrespassComponent,HasTracker,HasDeedLog,HasLocalTracking;
    unsigned LocalTrackingQueries;
    CTCTrespassable Trespass;
    CTCVolumeContainmentTracker Tracker;
    CTCHeroOpinionDeedLog DeedLog;
    CThing(bool alive=true,bool has_trespass=false,bool private_building=false):
        Alive(alive),HasTrespassComponent(has_trespass),HasTracker(false),HasDeedLog(false),
        HasLocalTracking(false),LocalTrackingQueries(0)
    {
        Trespass.Trespassable=private_building;
        Trespass.IsBeingTrespassedIn=false;
        Tracker.Inside=0;
    }
    bool GetTC(CTCTrespassable** component)
    {
        if (HasTrespassComponent) *component=&Trespass;
        return HasTrespassComponent;
    }
    bool GetTC(CTCVolumeContainmentTracker** component)
    {
        if (HasTracker) *component=&Tracker;
        return HasTracker;
    }
    bool GetTC(CTCHeroOpinionDeedLog** component)
    {
        if (HasDeedLog) *component=&DeedLog;
        return HasDeedLog;
    }
    bool IsDead() const { return !Alive; }
    bool HasTC(ETCInterfaceType type)
    {
        assert(type==LocalTracking);
        ++LocalTrackingQueries;
        return HasLocalTracking;
    }
    C3DVector GetPos() const { C3DVector pos={1,2,3}; return pos; }
};
template<class T> class CIntelligentPointer {
    T* Pointer;
public:
    explicit CIntelligentPointer(T* pointer=0):Pointer(pointer) {}
    T* Get() const { return Pointer; }
    T* operator->() const { return Pointer; }
};
static bool IS_THING_ALIVE(const CIntelligentPointer<CThing>& ref)
{
    return ref.Get() && !ref->IsDead();
}
struct CThingFilter_HasBuildingAsHomeOrWork {
    explicit CThingFilter_HasBuildingAsHomeOrWork(CThing* building):Building(building) {}
    CThing* Building;
};
struct CThingSearchTools {
    unsigned Calls;
    CThing* Building;
    CThingAICreature Occupant;
    CThingSearchTools():Calls(0),Building(0) {}
    template<class T> void GetAllThings(const CThingFilter_HasBuildingAsHomeOrWork& filter,std::vector<T*>& results)
    {
        assert(filter.Building->Trespass.IsBeingTrespassedIn);
        ++Calls;
        Building=filter.Building;
        results.push_back(&Occupant);
    }
};
static CThingSearchTools Search;
static CThingSearchTools& PeekThingSearchTools() { return Search; }
struct COpinionReactionManager {
    mutable unsigned Calls;
    mutable CThing* Parent;
    COpinionReactionManager():Calls(0),Parent(0) {}
    long AttachOpinionSource(CThing& parent,const char* source,int flags) const
    {
        assert(std::strcmp(source,"OPINION_SOURCE_TRESPASSER")==0 && flags==0);
        ++Calls;
        Parent=&parent;
        return 73;
    }
};
struct CWorld {
    static unsigned long Frame;
    COpinionReactionManager Opinions;
    static unsigned long GetFrame() { return Frame; }
    static long GetFramesPerSecond() { return 10; }
    const COpinionReactionManager& PeekOpinionReactionManager() const { return Opinions; }
};
unsigned long CWorld::Frame=0;
static CWorld World;
static CWorld& GetWorld() { return World; }
struct CTCTrespasser {
    CThing* ParentThing;
    CIntelligentPointer<CThing> TrespassingThing;
    std::vector<CThingAICreature*> OccupantsOfCurrentTrespassingThing;
    unsigned long FrameSomeoneWasInformedOfLastDeed,FramePlayerLeft;
    long AttachedOpinionSource,InsideBuildingNotTrespassingDeedHandle,CurrentDeedHandleIndex,
         OccupantsKnowOfDeedHandleIndex,DeedHandles[3];
    unsigned Leaves,KnowledgeQueries;
    bool OccupantsKnow;
    explicit CTCTrespasser(CThing& parent):ParentThing(&parent),FrameSomeoneWasInformedOfLastDeed(0),
        FramePlayerLeft(0),AttachedOpinionSource(-1),InsideBuildingNotTrespassingDeedHandle(-1),
        CurrentDeedHandleIndex(0),OccupantsKnowOfDeedHandleIndex(-1),Leaves(0),KnowledgeQueries(0),
        OccupantsKnow(false)
    {
        for (unsigned i=0; i<3; ++i) DeedHandles[i]=-1;
    }
    void LeavingRegion() { ++Leaves; }
    EOpinionDeedType GetDeedType(long index)
    {
        static const EOpinionDeedType types[]={Petty,Ignored,Caught};
        assert(index>=0 && index<3);
        return types[index];
    }
    bool SeeIfAnyOccupantsKnowAboutDeed(long index,EOpinionDeedType deed)
    {
        assert(index==CurrentDeedHandleIndex-1 && deed==GetDeedType(index));
        ++KnowledgeQueries;
        return OccupantsKnow;
    }
    void FrameUpdate();
};

void CTCTrespasser::FrameUpdate()
{
    CTCVolumeContainmentTracker* ptc_volume_containment_tracker;
    if (!ParentThing->GetTC(&ptc_volume_containment_tracker))
        return;
    CThing* thing_in = ptc_volume_containment_tracker->GetPThingInside();
    CTCTrespassable* trespassable;
    bool state_changed = thing_in != TrespassingThing.Get();
    if (state_changed || (thing_in && !thing_in->IsDead() && thing_in->GetTC(&trespassable)
                       && trespassable->Trespassable && !trespassable->IsBeingTrespassedIn)) {
        LeavingRegion();
        CTCTrespassable* ptc_trespassable;
        if (thing_in && !thing_in->IsDead() && thing_in->GetTC(&ptc_trespassable)) {
            if (ptc_trespassable->Trespassable) {
                ptc_trespassable->IsBeingTrespassedIn = true;
                PeekThingSearchTools().GetAllThings<CThingAICreature>
                    (CThingFilter_HasBuildingAsHomeOrWork(thing_in), OccupantsOfCurrentTrespassingThing);
                FrameSomeoneWasInformedOfLastDeed = 0;
                const COpinionReactionManager& manager = GetWorld().PeekOpinionReactionManager();
                AttachedOpinionSource = manager.AttachOpinionSource(*ParentThing, "OPINION_SOURCE_TRESPASSER", 0);
            } else {
                CTCHeroOpinionDeedLog* ptc_deed_log;
                if (ParentThing->GetTC(&ptc_deed_log))
                    InsideBuildingNotTrespassingDeedHandle = ptc_deed_log->PostDeed(
                        (EOpinionDeedType)0x4d, ParentThing->GetPos(), 0, 0, (EOpinionPostDeedType)2, false, true, 0, false, 0);
            }
        }
    }
    TrespassingThing = CIntelligentPointer<CThing>(thing_in);
    CTCTrespassable* ptc_trespassable;
    if (IS_THING_ALIVE(TrespassingThing) && TrespassingThing->GetTC(&ptc_trespassable) && ptc_trespassable->IsBeingTrespassedIn) {
        long last_deed_handle_index = CurrentDeedHandleIndex - 1;
        bool there_is_no_last_deed = last_deed_handle_index < 0 || DeedHandles[last_deed_handle_index] == -1;
        if (CurrentDeedHandleIndex < 3) {
            if (!there_is_no_last_deed && OccupantsKnowOfDeedHandleIndex != last_deed_handle_index &&
                !SeeIfAnyOccupantsKnowAboutDeed(last_deed_handle_index, GetDeedType(last_deed_handle_index))) {
                if (TrespassingThing->HasTC((ETCInterfaceType)0x53))
                    CurrentDeedHandleIndex = 3;
            } else if ((unsigned long)CWorld::GetFrame() - FrameSomeoneWasInformedOfLastDeed > (unsigned long)(CWorld::GetFramesPerSecond() * 5))
                ++CurrentDeedHandleIndex;
        }
        CTCHeroOpinionDeedLog* ptc_deed_log;
        if (ParentThing->GetTC(&ptc_deed_log)) {
            for (long c1 = 0; c1 < CurrentDeedHandleIndex && c1 < 3; ++c1) {
                if (DeedHandles[c1] == -1)
                    DeedHandles[c1] = ptc_deed_log->PostDeed(GetDeedType(c1), ParentThing->GetPos(),
                         ptc_volume_containment_tracker->GetPThingInside(), 0, (EOpinionPostDeedType)2, true, true, 0, false, 0);
            }
        }
    }
    if (!IS_THING_ALIVE(TrespassingThing) && CurrentDeedHandleIndex > 0) {
        long frames_between_forgetting_posting_deeds = CWorld::GetFramesPerSecond() * 10;
        if ((unsigned long)CWorld::GetFrame() - FramePlayerLeft > (unsigned long)frames_between_forgetting_posting_deeds) {
            --CurrentDeedHandleIndex;
            FramePlayerLeft += frames_between_forgetting_posting_deeds;
        }
    }
}

static void Advance(CTCTrespasser& trespasser,CThing* building,unsigned long frame)
{
    trespasser.ParentThing->Tracker.Inside=building;
    CWorld::Frame=frame;
    trespasser.FrameUpdate();
}

int main()
{
    CThing parent;
    CThing private_building(true,true,true), public_building(true,true,false);
    CThing absent_interface, dead_building(false,true,true);
    CTCTrespasser trespasser(parent);
    parent.HasDeedLog=true;
    Advance(trespasser,&private_building,60);
    assert(trespasser.Leaves==0 && !trespasser.TrespassingThing.Get());

    parent.HasTracker=true;
    Advance(trespasser,&absent_interface,60);
    assert(trespasser.Leaves==1 && Search.Calls==0);
    Advance(trespasser,&dead_building,60);
    assert(trespasser.Leaves==2 && !dead_building.Trespass.IsBeingTrespassedIn);
    Advance(trespasser,&public_building,60);
    assert(trespasser.Leaves==3 && parent.DeedLog.Calls.size()==1);
    assert(parent.DeedLog.Calls[0].Type==PublicVisit && trespasser.InsideBuildingNotTrespassingDeedHandle==101);
    Advance(trespasser,&public_building,61);
    assert(trespasser.Leaves==3 && parent.DeedLog.Calls.size()==1);

    parent.HasDeedLog=false;
    trespasser.FrameSomeoneWasInformedOfLastDeed=40;
    Advance(trespasser,&private_building,50);
    assert(trespasser.Leaves==4 && Search.Calls==1 && Search.Building==&private_building);
    assert(World.Opinions.Calls==1 && World.Opinions.Parent==&parent && trespasser.AttachedOpinionSource==73);
    assert(trespasser.TrespassingThing.Get()==&private_building && private_building.Trespass.IsBeingTrespassedIn);
    assert(trespasser.FrameSomeoneWasInformedOfLastDeed==0 && trespasser.CurrentDeedHandleIndex==0);
    assert(trespasser.OccupantsOfCurrentTrespassingThing.size()==1);
    Advance(trespasser,&private_building,51);
    assert(trespasser.CurrentDeedHandleIndex==1 && trespasser.DeedHandles[0]==-1);

    parent.HasDeedLog=true;
    Advance(trespasser,&private_building,52);
    assert(trespasser.CurrentDeedHandleIndex==2 && parent.DeedLog.Calls.size()==3);
    assert(parent.DeedLog.Calls[1].Type==Petty && parent.DeedLog.Calls[2].Type==Ignored);
    assert(parent.DeedLog.Calls[1].Building==&private_building && parent.DeedLog.Calls[2].Building==&private_building);
    Advance(trespasser,&private_building,53);
    assert(trespasser.CurrentDeedHandleIndex==2 && trespasser.KnowledgeQueries==1);
    assert(private_building.LocalTrackingQueries==1 && parent.DeedLog.Calls.size()==3);
    trespasser.OccupantsKnow=true;
    Advance(trespasser,&private_building,53);
    assert(trespasser.CurrentDeedHandleIndex==3 && trespasser.KnowledgeQueries==2);
    assert(parent.DeedLog.Calls.size()==4 && parent.DeedLog.Calls[3].Type==Caught);
    Advance(trespasser,&private_building,80);
    assert(trespasser.CurrentDeedHandleIndex==3 && parent.DeedLog.Calls.size()==4);

    Advance(trespasser,0,100);
    assert(trespasser.CurrentDeedHandleIndex==3 && !trespasser.TrespassingThing.Get());
    Advance(trespasser,0,101);
    assert(trespasser.CurrentDeedHandleIndex==2 && trespasser.FramePlayerLeft==100);
    Advance(trespasser,0,201);
    assert(trespasser.CurrentDeedHandleIndex==1 && trespasser.FramePlayerLeft==200);
    trespasser.OccupantsKnow=false;
    private_building.HasLocalTracking=true;
    Advance(trespasser,&private_building,203);
    assert(trespasser.CurrentDeedHandleIndex==3 && private_building.LocalTrackingQueries==2);
    assert(parent.DeedLog.Calls.size()==4);

    CTCTrespasser resume(parent);
    resume.TrespassingThing=CIntelligentPointer<CThing>(&private_building);
    private_building.Trespass.IsBeingTrespassedIn=false;
    const unsigned searches_before_resume=Search.Calls;
    Advance(resume,&private_building,30);
    assert(resume.Leaves==1 && private_building.Trespass.IsBeingTrespassedIn);
    assert(Search.Calls==searches_before_resume+1);
    resume.CurrentDeedHandleIndex=1;
    resume.DeedHandles[0]=135;
    resume.OccupantsKnowOfDeedHandleIndex=0;
    Advance(resume,&private_building,60);
    assert(resume.KnowledgeQueries==0 && resume.CurrentDeedHandleIndex==2);
    Advance(resume,&dead_building,101);
    assert(resume.CurrentDeedHandleIndex==1 && resume.FramePlayerLeft==100);
    std::puts("007bb1f0_TEST PASS");
    return 0;
}
