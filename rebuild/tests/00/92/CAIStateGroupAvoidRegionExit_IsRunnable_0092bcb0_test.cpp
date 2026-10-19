#include <cstdio>
#ifndef _MSC_VER
#define __fastcall
#define __forceinline inline
#define FABLE_ENGINE_CAISTATEGROUPBASE_H
#define FABLE_ENGINE_CAIBRAIN_H
#define FABLE_ENGINE_CTHINGAICREATURE_H
#define FABLE_ENGINE_CTCVILLAGE_H
#define FABLE_ENGINE_CAISTATEGROUP_AVOIDREGIONEXIT_H
struct CThingAICreature;
struct CTCVillage;
struct CAIBrain { CThingAICreature* Creature; };
struct CAIStateGroupBase { CAIBrain* PBrain; };
#endif
// CAIStateGroup_AvoidRegionExit::IsRunnable() -- recover the live entrance position unless walk-to-marker disallows it.
// Grade: RELOCATION_MATCH -- compiler-generated vector assignment preserves the retail shared destination-address adjustment (207 bytes).
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
#include "engine/CThingAICreature.h"
#include "engine/CTCVillage.h"
struct CTCVillager;
struct CRetailInterfaceMap {
    struct Entry { long Type; void* Component; };
    Entry* First;
    Entry* Last;
    Entry* LowerBound(const long*); // 0x0040F020
};
// Retail CThing prefix: PDB UsingComponents name, compact component map; Dead is +0x91 vs donor +0xa1.
struct CRetailThingView {
    unsigned char _prefix[0x20];
    unsigned long UsingComponents[9];
    CRetailInterfaceMap Components;
    unsigned char _beforeDead[0x91-0x4c];
    unsigned char Dead:1;
    bool HasTC(long type) const { if (UsingComponents[type>>5] & (1UL<<(type&31))) return true; return false; }
    void* GetTC(long type) {
      long key=type; CRetailInterfaceMap* map=&Components;
      CRetailInterfaceMap::Entry* it=map->LowerBound(&key);
      if(it==map->Last || type<it->Type) it=map->Last;
      return it->Component;
    }
    template<class T> __forceinline bool GetInterface(T** out, long type) {
        if (UsingComponents[type >> 5] & (1UL << (type & 31))) {
            long key = type;
            CRetailInterfaceMap* map = &Components;
            CRetailInterfaceMap::Entry* it = map->LowerBound(&key);
            if (it == map->Last || type < it->Type) it = map->Last;
            *out = (T*)it->Component;
            return true;
        }
        return false;
    }
};
struct CIntelligentPointerView {
    void* __vftable;
    CThingAICreature* Object;
    CThingAICreature* GetPItem() const; // 0x00A01B50
};
struct CVillagePersonNode {
    CVillagePersonNode* Next;
    CVillagePersonNode* Previous;
    CIntelligentPointerView Value;
};
// The engine village header retains donor 12-byte lists (+0x34 VillagePeople).
// Retail lists are one head pointer, so VillagePeople is +0x2c; no header is redefined.
struct CVillagePeopleList { CVillagePersonNode* Head; };
struct CRetailVillagePeopleView {
    unsigned char _beforePeople[0x2c];
    CVillagePeopleList VillagePeople;
};
CTCVillage* __fastcall GFGetPVillageInterface(CTCVillager*); // 0x007EE540
inline CRetailThingView* ThingView(CThingAICreature* p) { return (CRetailThingView*)p; }


#include "engine/CAIStateGroup_AvoidRegionExit.h"
struct C3DVector {
 float X,Y,Z;
 C3DVector() {}
 C3DVector(const C3DVector& o):X(o.X),Y(o.Y),Z(o.Z) {}
};
// Retail CTCWalkToMarker bool fields; donor header is not available.
struct CTCWalkToMarkerView {
 unsigned char _before68[0x68]; bool Flag68;
 unsigned char _before7b[0x12]; bool Flag7b;
};
struct CTCAvoidRegionExitView {
 unsigned char _base[0xc]; CIntelligentPointerView EntranceMarker;
 C3DVector GetPosition(); // 0092BD80
};
struct CAvoidStateView : CAIStateGroupBase { C3DVector Position; bool IsRunnable(); };
bool CAvoidStateView::IsRunnable() {
 CRetailThingView* creature=ThingView(PBrain->Creature);
 if(creature->HasTC(0xaa)) {
  CTCWalkToMarkerView* walk=(CTCWalkToMarkerView*)creature->GetTC(0xaa);
  if(!walk->Flag7b || walk->Flag68) return false;
 }
 if(creature->HasTC(0x105)) {
  CTCAvoidRegionExitView* avoid=(CTCAvoidRegionExitView*)creature->GetTC(0x105);
  if(avoid->EntranceMarker.GetPItem() && !ThingView(avoid->EntranceMarker.GetPItem())->Dead) {
   Position=avoid->GetPosition();
   return true;
  }
 }
 return false;
}

static int queries, getCalls, positionCalls, errors;
static long keys[4];
static CRetailInterfaceMap* expectedMap;
static CTCAvoidRegionExitView* expectedAvoid;
CRetailInterfaceMap::Entry* CRetailInterfaceMap::LowerBound(const long* type) {
 if(this!=expectedMap || queries>=4) { ++errors; return Last; }
 keys[queries++]=*type;
 for(Entry* i=First;i!=Last;++i) if(i->Type>=*type) return i;
 return Last;
}
CThingAICreature* CIntelligentPointerView::GetPItem() const {
 if(this!=&expectedAvoid->EntranceMarker) ++errors;
 ++getCalls;return Object;
}
C3DVector CTCAvoidRegionExitView::GetPosition() {
 if(this!=expectedAvoid || getCalls!=2) ++errors;
 ++positionCalls;C3DVector result;result.X=11.25f;result.Y=-3.5f;result.Z=9.0f;return result;
}
static bool run(bool hasWalk,bool flag7b,bool flag68,bool hasAvoid,int markerState) {
 CRetailThingView creature={},marker={};
 CTCWalkToMarkerView walk={}; CTCAvoidRegionExitView avoid={};
 CAIBrain brain; CAvoidStateView state;
 brain.Creature=(CThingAICreature*)&creature;state.PBrain=&brain;
 state.Position.X=1;state.Position.Y=2;state.Position.Z=3;
 walk.Flag7b=flag7b;walk.Flag68=flag68;marker.Dead=(markerState==2);
 avoid.EntranceMarker.Object=markerState?(CThingAICreature*)&marker:0;
 CRetailInterfaceMap::Entry entries[3];int count=0;
 if(hasWalk) { creature.UsingComponents[0xaa>>5]|=1UL<<(0xaa&31); entries[count].Type=0xaa;entries[count++].Component=&walk; }
 if(hasAvoid) { creature.UsingComponents[0x105>>5]|=1UL<<(0x105&31);entries[count].Type=0x105;entries[count++].Component=&avoid; }
 entries[count].Type=0;entries[count].Component=0;
 creature.Components.First=entries;creature.Components.Last=entries+count;
 expectedMap=&creature.Components;expectedAvoid=&avoid;
 queries=getCalls=positionCalls=errors=0;
 bool actual=state.IsRunnable();
 bool walkAllows=!hasWalk||(flag7b&&!flag68);
 bool expected=walkAllows&&hasAvoid&&markerState==1;
 int expectedQueries=(hasWalk?1:0)+(walkAllows&&hasAvoid?1:0);
 int expectedGets=walkAllows&&hasAvoid?(markerState?2:1):0;
 if(actual!=expected || queries!=expectedQueries || getCalls!=expectedGets || positionCalls!=(expected?1:0)) return false;
 if(hasWalk&&keys[0]!=0xaa) return false;
 if(walkAllows&&hasAvoid&&keys[hasWalk?1:0]!=0x105) return false;
 if(state.Position.X!=(expected?11.25f:1.0f)||state.Position.Y!=(expected?-3.5f:2.0f)||state.Position.Z!=(expected?9.0f:3.0f)) return false;
 return errors==0;
}
int main() {
 for(int w=0;w<2;++w)for(int a=0;a<2;++a)for(int b=0;b<2;++b)for(int r=0;r<2;++r)for(int m=0;m<3;++m)
  if(!run(w!=0,a!=0,b!=0,r!=0,m)) { std::printf("0092bcb0_TEST FAIL\n");return 1; }
 std::printf("0092bcb0_TEST PASS\n");return 0;
}