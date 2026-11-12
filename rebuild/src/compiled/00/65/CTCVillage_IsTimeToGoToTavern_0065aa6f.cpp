#include "fable_compiler.h"
#pragma optimize("s", on)
// CTCVillage::IsTimeToGoToTavern(CThingAICreature* creature, const CWorld& world, bool run, float speed) -- true when it is
//   already tavern time, or when the creature's live work building is far enough away that, at the given speed (or, for
//   speed == -1, its maximum running / walking speed towards the point in front of the village thing), it must set off now to
//   arrive by its village's tavern start time; false when the work building is dead, the creature is in no village, or the
//   village has no tavern times.
// Leaf: ?IsTimeToGoToTavern@CTCVillage@@QAE_NPAVCThingAICreature@@ABVCWorld@@_NM@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0065AA6F, 415 bytes (`leave ; ret 10h`); size TU (unaligned entry, ebp frame,
//   `push 23h ; pop edi`): `optimize("s", on)` as the landed twin CTCVillage::IsTimeToGoToSchool 0x0065A8EE.
// Identity: Ego_r.pdb + FableWin.pdb ?IsTimeToGoToTavern@CTCVillage@@QAE_NPAVCThingAICreature@@ABVCWorld@@_NM@Z.
// Callees: CTCVillage::IsTavernTime 0x00658C0A, CBaseIntelligentPointer::GetPItem 0x00A01B50 (twice, the inline
//   CThingAICreature::GetWorkBuilding, WorkBuilding +0x170: retail `lea esi,[ebx+170h] ; mov ecx,esi ; call 0x00A01B50` at
//   0x0065AA8D and again `mov ecx,esi ; call 0x00A01B50` at 0x0065AAC3, no call to an out-of-line GetWorkBuilding, so its body
//   was inline; definition copied from the landed row CAIStateGroupMinionWorkInInn::GetFreeBench 0x0090BC60),
//   ??$IS_THING_ALIVE@PAVCThingBuilding@@@@YI_NABQAVCThingBuilding@@@Z 0x0065D624 (`__fastcall`, the out-of-line COMDAT of
//   this TU; landed today under the made-up `_global::IsThingAlive_ptr_0065d624`, see NAME_FIXES.tsv), CThing::GetPos 0x004C73D0 (three times), std::bitset<274>::test 0x00410DE0 and
//   CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound 0x0040F020 (inside the inline CThing::GetTC), CTCVillageMember::GetPVillage
//   0x007EE540, CTCVillage::GetTavernTimes(float&, float&) const 0x00658C5B, CTCPhysicsBase::GetFacingVector() const (virtual,
//   slot 70, +0x118; 0x006B1410), CThingAICreature::GetMaxRunningSpeed (virtual, slot 81, +0x144; 0x0082FED0) /
//   GetMaxWalkingSpeed (virtual, slot 79, +0x13C; 0x00830000). Data: CMainGameComponent::ConstantFPS 0x01375550 (through the
//   static inline GetConstantFPS), -1.0f 0x0122DEE0, 1.6f 0x0125A9A0.
// Shape (what each retail detail needed):
//   - Frame 0x3C: position [ebp-18h], target [ebp-24h], the subtraction temporary [ebp-30h], the work position / facing
//     temporary [ebp-3Ch]. target has its own slot beside position, so it is a function-scope local; only `start` / `end` sit in
//     an inner block (start shares the dead `creature` argument slot [ebp+8] with the IS_THING_ALIVE temporary and the GetTC key),
//     and the scaled start time leaves that block in a plain float (it is spilled back into [ebp+8]). With target inside the
//     block (the school twin's layout) VC7.1 overlays it on position: frame 0x30.
//   - `mov edi,[this+4]` once for both the physics call and GetPos: the parent thing is held in a reference local (the virtual
//     call would otherwise force a reload of this->ParentThing).
//   - The facing vector is called before GetPos and added as `GetPos() + facing` (member operator+, argument first).
//   - The component lookup is spelled GetTC(TCI_VILLAGE_MEMBER, &member): with the one-argument GetTC<CTCVillageMember>(&member)
//     (the FableWin.pdb instantiation; one more inline level in the shared header) VC7.1 commutes the X addition of the target
//     (`fld [eax] ; fadd [esi]` v retail `fld [esi] ; fadd [eax]`), the only difference.
//   - `fld speed ; fld -1.0f ; fld st(1) ; fucompp ; test ah,44h ; jp`: `if (speed == -1.0f)` with speed kept on the x87 stack;
//     the if / else on `run` shares the pushed &target.
//   - `if (startFrames > ...) return false; return true;` gives `test ah,41h ; je` to the shared `xor al,al` block.
// Layout: retail CThingAICreature WorkBuilding +0x170; CThing UsingComponents +0x20, Components +0x44, PhysicsTC +0x60;
//   CTCBase ParentThing +0x04; CWorld PGameTimeManager +0x1C; CGameTimeManager TimeStep +0x10 (read through the inline
//   GetDaySpeed, FableWin.pdb ?GetDaySpeed@CGameTimeManager@@QBEMXZ).
// Header opt-ins added for this row: FABLE_RETAIL_CTCVILLAGE_TAVERNTIMES (engine/CTCVillage.h),
//   FABLE_RETAIL_CGAMETIMEMANAGER_DAYSPEED (engine/CGameTimeManager.h).
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_ORIGINAL_VTABLE
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTHINGAICREATURE_HOMEBUILDING
#define FABLE_RETAIL_CTHINGAICREATURE_SAVE
#define FABLE_RETAIL_CTHINGAICREATURE_WORKBUILDING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_IS_THING_ALIVE_NOINLINE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCVILLAGE
#define FABLE_RETAIL_CTCVILLAGE_TAVERNTIMES
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CGAMETIMEMANAGER
#define FABLE_RETAIL_CGAMETIMEMANAGER_DAYSPEED
#include "rebuild_abi.h"
#include <math.h>
#pragma intrinsic(sqrt)
#include "engine/C3DVector.h"
#include "engine/CThingAICreature.h"
#include "engine/CThingBuilding.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CTCVillageMember.h"
#include "engine/CTCVillage.h"
#include "engine/CWorld.h"
#include "engine/CGameTimeManager.h"
#include "engine/CMainGameComponent.h"
#include "engine/ThingAlive.h"
// CThingAICreature::GetWorkBuilding (FableWin.pdb ?GetWorkBuilding@CThingAICreature@@QBEPAVCThingBuilding@@XZ): the same line as
//   the landed row rebuild/src/compiled/00/90/CAIStateGroupMinionWorkInInn_GetFreeBench_0090bc60.cpp. The shared header
//   (FABLE_RETAIL_CTHINGAICREATURE_WORKBUILDING) only declares it: the body needs CThingBuilding complete, so it lives in the
//   includer after engine/CThingBuilding.h. Inline in retail: `lea esi,[ebx+170h] ; call 0x00A01B50` (0x0065AA8D / 0x0065AA95).
inline CThingBuilding* CThingAICreature::GetWorkBuilding() const { return WorkBuilding.Get(); }

bool CTCVillage::IsTimeToGoToTavern(CThingAICreature* creature, const CWorld& world, bool run, float speed)
{
    if (IsTavernTime())
        return true;
    if (!IS_THING_ALIVE(creature->GetWorkBuilding()))
        return false;
    C3DVector position = creature->GetPos();
    C3DVector work = creature->GetWorkBuilding()->GetPos();
    float distance = (position - work).GetMagnitude();
    CTCVillageMember* member;
    if (!creature->GetTC(TCI_VILLAGE_MEMBER, &member))
        return false;
    CTCVillage* village = member->GetPVillage();
    if (!village)
        return false;
    float startFrames;
    {
        float start;
        float end;
        if (!village->GetTavernTimes(start, end))
            return false;
        startFrames = start / world.PeekGameTimeManager().GetDaySpeed();
    }
    CThing& parent = GetParentThing();
    C3DVector target = parent.GetPos() + parent.GetPhysicsTC()->GetFacingVector();
    if (speed == -1.0f) {
        if (run)
            speed = creature->GetMaxRunningSpeed(target);
        else
            speed = creature->GetMaxWalkingSpeed(target);
    }
    if (startFrames > distance * 1.6f / (CMainGameComponent::GetConstantFPS() * speed))
        return false;
    return true;
}