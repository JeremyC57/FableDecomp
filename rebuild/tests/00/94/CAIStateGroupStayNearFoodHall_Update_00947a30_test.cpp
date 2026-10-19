#include <cstdio>
#ifndef _MSC_VER
#define __fastcall
// Host adapters expose the fields used by the exact candidate. VC7.1 uses the real headers below.
#define FABLE_ENGINE_CAISTATEGROUPBASE_H
#define FABLE_ENGINE_CAIBRAIN_H
struct CThingAICreature;
struct CAIBrain { CThingAICreature* Creature; };
struct CAIStateGroupBase { CAIBrain* PBrain; bool Terminated; };
#endif
// BEGIN EXACT CANDIDATE
// CAIStateGroup_StayNearFoodHall::Update() -- repeatedly walk to the food hall and wait while cooking
// Retail: 0x00947a30, length 244. Uses the generated CAIStateGroupBase header for the +0x14 base end;
// local derived members model Position (+0x14) and its intelligent pointer (+0x20), as retail accesses them there.
// Callees: GetPItem @ 0x00A01B50, IsInWorkHours @ 0x00947840, MoveToPosProximity @ 0x0097E890,
// LowerBound @ 0x004365B0, GFGetVillage @ 0x007EE540, IsCookTime @ 0x00658BB9, Terminate @ 0x008FCF40.
// The loop continues only while the move action reports 4 and the hall remains live and cooking time continues.
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
struct CThing;
struct C3DVector { float X,Y,Z; C3DVector() : X(0),Y(0),Z(0) {} C3DVector(const C3DVector& v) : X(v.X),Y(v.Y),Z(v.Z) {} };
struct CInterfaceMap { struct Entry { long type; void* component; }; Entry *first,*last; Entry* LowerBound(const long*); };
struct CThing { unsigned char pad0[0x20]; unsigned long flags20; unsigned long flags24; unsigned char pad28[0x1c]; CInterfaceMap interfaces; unsigned char pad4c[0x45]; unsigned char dead; };
struct CBaseIntelligentPointer { virtual void v00(); void* Handle; CThing* GetPItem(); };
struct CTCVillager;
struct CTCVillage { bool IsCookTime(); };
struct CAIStateGroup_StayNearFoodHall : CAIStateGroupBase {
    C3DVector Position;
    CBaseIntelligentPointer PFoodHall;
    bool IsInWorkHours();
    void Update();
    long AIAction_MoveToPosProximity(long, C3DVector, float, bool*, long, bool, bool, bool, CThing*, bool, float);
    void Terminate();
};
CTCVillage* __fastcall GFGetVillage(CTCVillager*); // 0x007ee540
void CAIStateGroup_StayNearFoodHall::Update()
{
    CThing* foodHall;
    CThing* creature;
    CInterfaceMap::Entry* it;
    CInterfaceMap* map;
    CTCVillage* village;
    bool reached;
    long result;
    long key;
    if (!PFoodHall.GetPItem()) goto done;
    foodHall = PFoodHall.GetPItem();
    if (foodHall->dead & 1) goto done;
    if (!IsInWorkHours()) goto done;
    do {
        result = AIAction_MoveToPosProximity(2, Position, 4.0f, &reached, 3, true, false, true, 0, true, 0.0f);
        if (!PFoodHall.GetPItem()) goto done;
        foodHall = PFoodHall.GetPItem();
        if (foodHall->dead & 1) goto done;
        creature = (CThing*)PBrain->Creature;
        if (!(creature->flags24 & 8)) goto done;
        key = 0x23;
        map = &creature->interfaces;
        it = map->LowerBound(&key);
        if (it == map->last || it->type > 0x23) it = map->last;
        village = GFGetVillage((CTCVillager*)it->component);
        if (!village || !village->IsCookTime()) goto done;
    } while (result == 4);
done:
    Terminate();
}
// END EXACT CANDIDATE
struct CTCVillager { int identity; };
static int errors, scenario, pointerReads, workChecks, actions, mapCalls, villageCalls, cookChecks, terminations;
static CThing hall, creature, replacement;
static CAIBrain brain;
static CInterfaceMap::Entry entries[2], replacementEntries[2];
static CTCVillager normalVillager, sentinelVillager, replacementVillager;
static CTCVillage village;
static CAIStateGroup_StayNearFoodHall* group;
static C3DVector expectedPosition;
static void check(bool okay) { if(!okay) ++errors; }
void CBaseIntelligentPointer::v00() {}
CThing* CBaseIntelligentPointer::GetPItem() {
    check(this==&group->PFoodHall); ++pointerReads; return (CThing*)Handle;
}
bool CAIStateGroup_StayNearFoodHall::IsInWorkHours() {
    check(this==group); ++workChecks; return scenario!=3;
}
long CAIStateGroup_StayNearFoodHall::AIAction_MoveToPosProximity(long repeat, C3DVector position, float radius, bool* reached, long move, bool a, bool b, bool c, CThing* target, bool d, float time) {
    check(this==group && repeat==2 && radius==4.0f && reached!=0 && move==3 && a && !b && c && target==0 && d && time==0.0f);
    check(position.X==expectedPosition.X && position.Y==expectedPosition.Y && position.Z==expectedPosition.Z);
    ++actions; *reached=true;
    if(scenario==4) PFoodHall.Handle=0;
    if(scenario==5) hall.dead=1;
    if(scenario==11) brain.Creature=(CThingAICreature*)&replacement;
    if(actions>3) { ++errors; return 2; }
    if(scenario==0 && actions==1) {
        expectedPosition.X=-7.0f; expectedPosition.Y=11.5f; expectedPosition.Z=0.25f;
        Position=expectedPosition;
        return 4;
    }
    return scenario==12 ? 1 : 2;
}
CInterfaceMap::Entry* CInterfaceMap::LowerBound(const long* key) {
    check(this==(scenario==11?&replacement.interfaces:&creature.interfaces));
    check(key && *key==0x23); ++mapCalls;
    if(scenario==7) return last;
    return first;
}
CTCVillage* __fastcall GFGetVillage(CTCVillager* item) {
    ++villageCalls;
    check(item==(scenario==7 || scenario==8 ? &sentinelVillager : scenario==11 ? &replacementVillager : &normalVillager));
    return scenario==9 ? 0 : &village;
}
bool CTCVillage::IsCookTime() { check(this==&village); ++cookChecks; return scenario!=10; }
void CAIStateGroup_StayNearFoodHall::Terminate() { check(this==group); ++terminations; Terminated=true; }
static void run(int which) {
    scenario=which; pointerReads=workChecks=actions=mapCalls=villageCalls=cookChecks=terminations=0;
    hall.dead=0; creature.dead=0; replacement.dead=0;
    creature.flags24=which==6?0:8; replacement.flags24=8;
    entries[0].type=which==8?0x24:0x23; entries[0].component=&normalVillager;
    entries[1].type=0x7fffffff; entries[1].component=&sentinelVillager;
    replacementEntries[0].type=0x23; replacementEntries[0].component=&replacementVillager;
    replacementEntries[1].type=0x7fffffff; replacementEntries[1].component=&sentinelVillager;
    creature.interfaces.first=entries; creature.interfaces.last=entries+1;
    replacement.interfaces.first=replacementEntries; replacement.interfaces.last=replacementEntries+1;
    CAIStateGroup_StayNearFoodHall state; group=&state; state.PBrain=&brain; state.Terminated=false;
    brain.Creature=(CThingAICreature*)&creature;
    state.PFoodHall.Handle=which==1?0:&hall;
    if(which==2) hall.dead=1;
    expectedPosition.X=1.25f; expectedPosition.Y=-2.0f; expectedPosition.Z=3.5f;
    state.Position=expectedPosition;
    state.Update();
    int wantedActions=which==0?2:which<=3?0:1;
    int wantedMaps=which==0?2:which<=6?0:1;
    int wantedGets=which==0?6:which==1?1:which==2 || which==3?2:which==4?3:4;
    check(terminations==1 && state.Terminated);
    check(workChecks==(which==1 || which==2?0:1));
    check(actions==wantedActions && pointerReads==wantedGets);
    check(mapCalls==wantedMaps && villageCalls==wantedMaps);
    check(cookChecks==(which==9?0:wantedMaps));
}
int main() {
    for(int i=0;i<=12;++i) run(i);
    std::printf("00947a30_TEST %s\n",errors?"FAIL":"PASS");
    return errors?1:0;
}