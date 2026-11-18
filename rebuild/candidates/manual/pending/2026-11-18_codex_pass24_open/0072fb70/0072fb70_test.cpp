#include <math.h>
#include <stdio.h>
#include <list>
#include <vector>

const long Unsched=0x7fffffff;
struct C3DVector {
    float X,Y,Z;
    C3DVector(float x=0,float y=0,float z=0):X(x),Y(y),Z(z) {}
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X-o.X,Y-o.Y,Z-o.Z); }
    float GetMagnitude() const { return (float)sqrt(X*X+Y*Y+Z*Z); }
    void AccurateNormalise()
    {
        float length=GetMagnitude();
        if(length>0) { X/=length; Y/=length; Z/=length; }
    }
};
struct CRightHandedSet {
    C3DVector Up,Forward;
    CRightHandedSet(const C3DVector& up,const C3DVector& dir):Up(up),Forward(dir) {}
};
struct CThingGraphic {
    C3DVector Forward;
    unsigned Orientations;
    CThingGraphic():Orientations(0) {}
    void SetRHSet(const CRightHandedSet& set) { ++Orientations;Forward=set.Forward; }
};
struct CTCVillageMember;
struct CThing {
    unsigned Flags91;
    long DefIndex;
    C3DVector Position;
    CTCVillageMember* Member;
    CThingGraphic Visual;
    CThingGraphic* Graphic;
    bool PassesMarkerFilter;
    CThing():Flags91(0),DefIndex(107),Member(0),Graphic(&Visual),PassesMarkerFilter(true) {}
    const C3DVector& GetPos() const { return Position; }
    unsigned long GetDefGlobalIndex() const { return DefIndex; }
    CThingGraphic* GetPhysicsTC() { return Graphic; }
    bool GetTC(CTCVillageMember** out) { *out=Member; return Member!=0; }
};
template<class T> struct CIntelligentPointer {
    T* Ptr;
    CIntelligentPointer(T* ptr=0):Ptr(ptr) {}
    T* Get() const { return Ptr; }
    T* operator->() const { return Ptr; }
};
typedef CThing CThingMarker;
struct CTCVillage {
    CThing* ParentThing;
    std::list<CIntelligentPointer<CThingMarker> > VillageMarkers;
    CThing& GetParentThing() { return *ParentThing; }
};
struct CTCVillageMember {
    CTCVillage* Village;
    CTCVillage* GetPVillage() { return Village; }
};
bool IS_THING_ALIVE(const CThing* thing) { return thing && !(thing->Flags91&1); }
bool IS_THING_ALIVE(const CIntelligentPointer<CThing>& thing) { return IS_THING_ALIVE(thing.Get()); }
struct CThingFilter_IsNotBeingExclusivelyUsed {
    bool operator()(const CThing& thing) const { return thing.PassesMarkerFilter; }
};
namespace std {
void random_shuffle(std::vector<CThing*>::iterator,std::vector<CThing*>::iterator) {}
}
struct CClock {
    long Day;
    long GetDayCount() const { return Day; }
};
struct CCamera {
    bool IsPosInViewingFrustrum(const C3DVector& position) const { return position.Y>0; }
    bool IsLineOfSightToPos(const C3DVector& position) const { return position.Y>1; }
};
struct CWorld {
    CClock Time;
    CClock* Clock;
    CCamera View;
    CCamera* Camera;
    long Frame;
    CWorld():Clock(&Time),Camera(&View),Frame(10) { Time.Day=4; }
    long GetFrame() const { return Frame; }
    const CClock& PeekGameTimeManager() const { return Time; }
    CCamera* GetGameCamera() { return Camera; }
};
long GFRandom(long range,unsigned long* random) { return range ? (long)(*random%range) : 0; }
template<class T> struct CDefPointer {
    T* Object;
    CDefPointer():Object(0) {}
    ~CDefPointer() { if(Object) Object->Release(); }
    T* operator->() const { return Object; }
};
struct CCreatureDef {
    long RespawnWaitInDaysMin,RespawnWaitInDaysMax,RespawnWaitInFramesMin,RespawnWaitInFramesMax;
    mutable unsigned Releases;
    CCreatureDef():RespawnWaitInDaysMin(0),RespawnWaitInDaysMax(0),RespawnWaitInFramesMin(3),RespawnWaitInFramesMax(3),Releases(0) {}
    void Release() const { ++Releases; }
};
struct CThingBaseDef {
    CCreatureDef CreatureDef;
    mutable unsigned Releases;
    CThingBaseDef():Releases(0) {}
    bool GetSubDef(CDefPointer<const CCreatureDef>* out) const { out->Object=&CreatureDef;return true; }
    void Release() const { ++Releases; }
};
struct CDefinitionManager {
    CThingBaseDef ThingDef;
    bool GetDef(long,CDefPointer<const CThingBaseDef>* out) { out->Object=&ThingDef;return true; }
    long GetDefGlobalIndexFromName(const char*) { return 107; }
};
struct CGameDefinitionManager {
    static CDefinitionManager& Get() { static CDefinitionManager definition;return definition; }
};
typedef CThing CThingPlayerCreature;
struct CPlayer {
    CThing Hero;
    CThing* GetPControlledCreature() { return &Hero; }
};
struct CPlayerManager {
    CPlayer Player;
    CPlayer& GetMainPlayer() { return Player; }
};
struct CTCDummyVillager {
    CThing* ParentThing;
    CIntelligentPointer<CThing> Creature;
    long DefType,DayToBeRecreated,FrameToBeRecreated;
    CDefinitionManager Definitions;
    CWorld World;
    CPlayerManager PlayerManager;
    unsigned long Random;
    bool CanRecreate,ValidSpawn;
    long Respawns;
    CThing Spawned;
    CTCDummyVillager(CThing& parent):ParentThing(&parent),DefType(44),DayToBeRecreated(Unsched),
        FrameToBeRecreated(Unsched),CanRecreate(true),ValidSpawn(false),Respawns(0) { Random=0; }
    CDefinitionManager& GetDefinitionManager() { return Definitions; }
    CWorld& GetWorld() { return World; }
    unsigned long* GetPWorldSeed() { return &Random; }
    CPlayerManager& GetPlayerManager() { return PlayerManager; }
    bool CanBeRecreated() { return CanRecreate; }
    void CreateVillager(CTCVillage& village,const C3DVector& pos,bool valid)
    {
        ++Respawns;Spawned.Position=pos;Creature.Ptr=&Spawned;
        ValidSpawn=&village==ParentThing->Member->Village&&valid;
    }
    CThing& GetParentThing() { return *ParentThing; }
    void FrameUpdate();
};

void CTCDummyVillager::FrameUpdate()
{
    if (IS_THING_ALIVE(Creature) || !CanBeRecreated())
        return;
    if (DayToBeRecreated == 0x7fffffff && FrameToBeRecreated == 0x7fffffff) {
        CDefPointer<const CThingBaseDef> pdef;
        GetDefinitionManager().GetDef(DefType, &pdef);
        CDefPointer<const CCreatureDef> creature_def;
        if (pdef->GetSubDef(&creature_def)) {
            if (creature_def->RespawnWaitInDaysMin == 0 && creature_def->RespawnWaitInDaysMax == 0) {
                DayToBeRecreated = GetWorld().PeekGameTimeManager().GetDayCount();
                FrameToBeRecreated = GFRandom(creature_def->RespawnWaitInFramesMax - creature_def->RespawnWaitInFramesMin,
                    GetPWorldSeed()) + GetWorld().GetFrame() + creature_def->RespawnWaitInFramesMin;
            } else {
                DayToBeRecreated = GFRandom(creature_def->RespawnWaitInDaysMax - creature_def->RespawnWaitInDaysMin,
                    GetPWorldSeed()) + creature_def->RespawnWaitInDaysMin + GetWorld().PeekGameTimeManager().GetDayCount();
                FrameToBeRecreated = 0x7fffffff;
            }
        }
    }
    bool recreate;
    if (FrameToBeRecreated != 0x7fffffff)
        recreate = GetWorld().GetFrame() >= FrameToBeRecreated;
    else
        recreate = GetWorld().PeekGameTimeManager().GetDayCount() >= DayToBeRecreated;
    if (!recreate)
        return;

    CTCVillageMember* village_member;
    if (!GetParentThing().GetTC(&village_member) || !village_member->GetPVillage())
        return;
    C3DVector spawn_pos;
    bool found_spawn_pos = false;
    CThingPlayerCreature* phero = GetPlayerManager().GetMainPlayer().GetPControlledCreature();
    long def_index = CGameDefinitionManager::Get().GetDefGlobalIndexFromName("GAZE_OUT_OF_BUILDING_MARKER");
    const std::list<CIntelligentPointer<CThingMarker> >& village_markers = village_member->GetPVillage()->VillageMarkers;
    std::vector<CThingMarker*> shuffled_markers;
    for (std::list<CIntelligentPointer<CThingMarker> >::const_iterator i = village_markers.begin(); i != village_markers.end(); ++i)
        shuffled_markers.push_back(i->Get());
    std::random_shuffle(shuffled_markers.begin(), shuffled_markers.end());
    for (unsigned pass = 0; pass != 2 && !found_spawn_pos; ++pass) {
        const bool test_line_of_sight = pass != 0;
        for (std::vector<CThingMarker*>::iterator i = shuffled_markers.begin(); i != shuffled_markers.end(); ++i) {
            if (!IS_THING_ALIVE(*i) || (*i)->GetDefGlobalIndex() != static_cast<unsigned long>(def_index) ||
                !CThingFilter_IsNotBeingExclusivelyUsed()(**i))
                continue;
            if (GetWorld().GetGameCamera()->IsPosInViewingFrustrum((*i)->GetPos())) {
                if (!test_line_of_sight || GetWorld().GetGameCamera()->IsLineOfSightToPos((*i)->GetPos()))
                    continue;
            }
            if (found_spawn_pos && !((spawn_pos - phero->GetPos()).GetMagnitude() > ((*i)->GetPos() - phero->GetPos()).GetMagnitude()))
                continue;
            found_spawn_pos = true;
            spawn_pos = (*i)->GetPos();
            if (test_line_of_sight)
                break;
        }
    }
    if (!found_spawn_pos) {
        if (CTCVillage* ptc_village = village_member->GetPVillage())
            spawn_pos = ptc_village->GetParentThing().GetPos();
    }
    if (CTCVillage* ptc_village = village_member->GetPVillage()) {
        CreateVillager(*ptc_village, spawn_pos, true);
        if (IS_THING_ALIVE(Creature)) {
            C3DVector facing_dir = phero->GetPos() - Creature->GetPos();
            facing_dir.AccurateNormalise();
            Creature->GetPhysicsTC()->SetRHSet(CRightHandedSet(C3DVector(0.0f, 0.0f, 1.0f), facing_dir));
        }
    }
}

int main()
{
    CThing host,home,live,far,close,visible,mismatch;
    home.Position=C3DVector(99,0,0);
    far.Position=C3DVector(9,0,0);close.Position=C3DVector(7,0,0);
    visible.Position=C3DVector(2,1,0);
    mismatch.Position=C3DVector(1,0,0);mismatch.DefIndex=9;
    CTCVillage village;village.ParentThing=&home;
    CTCVillageMember member={&village};host.Member=&member;
    village.VillageMarkers.push_back(&far);
    village.VillageMarkers.push_back(&close);
    village.VillageMarkers.push_back(&visible);
    village.VillageMarkers.push_back(&mismatch);
    CTCDummyVillager villager(host);
    villager.Creature.Ptr=&live;
    villager.FrameUpdate();
    if(villager.Respawns||villager.FrameToBeRecreated!=Unsched)
        return 1;
    live.Flags91=1;villager.CanRecreate=false;villager.FrameUpdate();
    if(villager.Respawns||villager.FrameToBeRecreated!=Unsched)
        return 2;
    villager.CanRecreate=true;villager.FrameUpdate();
    if(villager.FrameToBeRecreated!=13||villager.Respawns||
        villager.Definitions.ThingDef.Releases!=1||villager.Definitions.ThingDef.CreatureDef.Releases!=1)
        return 3;
    villager.World.Frame=13;villager.FrameUpdate();
    if(villager.Respawns!=1||villager.Spawned.Position.X!=7||!villager.ValidSpawn||
        villager.Spawned.Visual.Orientations!=1||villager.Spawned.Visual.Forward.X!=-1.0f)
        return 4;
    villager.Creature.Ptr=0;village.VillageMarkers.clear();village.VillageMarkers.push_back(&visible);
    villager.FrameUpdate();
    if(villager.Respawns!=2||villager.Spawned.Position.X!=2)
        return 5;
    CTCDummyVillager days(host);
    days.Definitions.ThingDef.CreatureDef.RespawnWaitInDaysMin=2;
    days.Definitions.ThingDef.CreatureDef.RespawnWaitInDaysMax=2;
    days.FrameUpdate();
    if(days.DayToBeRecreated!=6||days.FrameToBeRecreated!=Unsched||days.Respawns)
        return 6;
    days.World.Time.Day=6;days.FrameUpdate();
    if(days.Respawns!=1||!days.ValidSpawn)
        return 7;
    puts("0072fb70_TEST PASS");
    return 0;
}
