#ifndef FABLE_ENGINE_CAISTATEGROUP_VILLAGETASKPATROL_H
#define FABLE_ENGINE_CAISTATEGROUP_VILLAGETASKPATROL_H
// New header (p25M, row 0x0093F3C0 Update) from the PDB layouts: Ego_r.pdb CAIStateGroup_VillageTaskPatrol, sizeof 0x20
// (ghidra_out/struct_layouts_egor.tsv: CAIStateGroupBase 0x14, PTCVillageMember +0x14, PTask +0x18 = CIntelligentPointer<CVillageTaskPatrol>,
// whose own vtable is 0x0128B090); methods from FableWin.pdb ?..@CAIStateGroup_VillageTaskPatrol@@ (Initialise, Update, IsRunnable,
// OnReplace, InitCreaturePosition -- also Ego_r 0x00962D70 --, Alloc, GetName, ctor, dtor). The class vtable is 0x0128BA64 (RTTI
// .?AVCAIStateGroup_VillageTaskPatrol@@): slot 1 Initialise 0x0093EED0, 3 InitCreaturePosition 0x0093EF30, 10 OnReplace 0x0093EE40,
// 11 IsRunnable 0x0093EDF0, 12 Update 0x0093F3C0.
#include "engine/CAIStateGroupBase.h"
class CTCVillageMember; class CVillageTaskPatrol;
class CAIStateGroup_VillageTaskPatrol:public CAIStateGroupBase {
public:
 virtual void Initialise(CAIBrain & p0, CThingAICreature & p1, long p2, EScriptAIPriority p3, const CCharString & p4);
 virtual void Update();
 virtual bool IsRunnable();
 virtual void OnReplace();
 virtual bool InitCreaturePosition();
 static CAIStateGroupBase * FABLE_FASTCALL Alloc();
 static CCharString FABLE_FASTCALL GetName();
private:
 CTCVillageMember * PTCVillageMember;
 CIntelligentPointer<CVillageTaskPatrol> PTask;
public:
 CAIStateGroup_VillageTaskPatrol();
 virtual ~CAIStateGroup_VillageTaskPatrol();
};
FABLE_STATIC_ASSERT(sizeof(CAIStateGroup_VillageTaskPatrol)==32);
#endif
