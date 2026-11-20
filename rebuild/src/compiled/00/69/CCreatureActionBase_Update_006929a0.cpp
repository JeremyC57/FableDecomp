// CCreatureActionBase::Update() -- a finished action clears itself from its creature (CThingCreatureBase::ClearCurrentAction); otherwise it runs its FrameUpdate
// Leaf: ?Update@CCreatureActionBase@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x006929A0: `mov al,[ecx+61h] ; test al,al ; je` -> `add ecx,8 ; call GetPItem 0x00A01B50 ; mov ecx,eax ;
//   jmp ClearCurrentAction 0x006633A0`, else `mov eax,[ecx] ; jmp [eax+0E0h]` (FrameUpdate, slot 56).
// Identity: Ego_r.pdb 0x00519310 + FableWin.pdb 0x01DB6560 ?Update@CCreatureActionBase@@QAEXXZ; Ego_r places it directly after
//   PeekThingSearchTools 0x00519300 (= retail 0x00692990, landed), as retail 0x006929A0 follows it; the landed caller 0x006643C0
//   CThingCreatureBase::UpdateAction already binds this spelling.
// NF175 (2026-11-20, q11935): re-spelled from the free stand-in ?CCreatureActionBase_Update@@YIXPAVCCreatureActionBase@@@Z (whose extern "C"
//   getSub / runSub bound GetPItem 0x00A01B50 and ClearCurrentAction 0x006633A0 as @runSub@4, and whose FrameUpdate was a vtable-array
//   call) onto the shared engine/CCreatureActionBase.h (retail, FULL_MEMBERS: PCreature +0x08, Finished +0x61; NF175 UPDATE opt-in),
//   engine/CIntelligentPointer.h and engine/CThingCreatureBase.h (CLEARCURRENTACTION opt-in).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGCREATUREBASE_CLEARCURRENTACTION
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE_FULL_MEMBERS
#define FABLE_RETAIL_CCREATUREACTIONBASE_UPDATE
#include "fable_string.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CCountedPointer.h"
#include "stlport/fable_containers.h"
#include "engine/CCreatureActionBase.h"

void CCreatureActionBase::Update()
{
    if (Finished)
        PCreature->ClearCurrentAction();
    else
        FrameUpdate();
}
