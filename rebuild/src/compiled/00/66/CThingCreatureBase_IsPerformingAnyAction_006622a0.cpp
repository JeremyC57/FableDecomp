// CThingCreatureBase::IsPerformingAnyAction() const -- true when the creature has a current action that has not finished
// Leaf: ?IsPerformingAnyAction@CThingCreatureBase@@QBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006622A0, 26 bytes: `mov eax,[ecx+100h] ; test ; je ; mov cl,[eax+61h] ; test ;
//   jne ; mov eax,1 ; ret ; xor eax,eax ; ret`.
// Name fix (naming agent NF21, 2026-11-11): Ego_r.pdb and FableWin.pdb spell ?IsPerformingAnyAction@CThingCreatureBase@@QBE_NXZ
//   (const, bool); the row was a local `int IsPerformingAnyAction()` (QAEHXZ) over an invented ActionState struct. Now on the shared
//   engine/CThingCreatureBase.h (PCurrentAction CCountedPointer<CCreatureActionBase> +0x100, inline PeekPCurrentAction, the
//   ACTIONQUERY declaration) and engine/CCreatureActionBase.h (Finished +0x61, inline Ego_r ?IsFinished@CCreatureActionBase@@QBE_NXZ).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGCREATUREBASE_ACTIONQUERY
#define FABLE_RETAIL_CTHINGCREATUREBASE_ANIMATION
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE_INTERRUPTION
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/C3DVector.h"
#include "engine/CThing.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CCreatureActionBase.h"

bool CThingCreatureBase::IsPerformingAnyAction() const
{
    const CCreatureActionBase* action = PeekPCurrentAction();
    return action && !action->IsFinished();
}
