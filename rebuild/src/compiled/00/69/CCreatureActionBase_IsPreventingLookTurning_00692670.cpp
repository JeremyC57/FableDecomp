// CCreatureActionBase::IsPreventingLookTurning() const -- an action that prevents normal movement also prevents look turning
// Leaf: ?IsPreventingLookTurning@CCreatureActionBase@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00692670, 14 bytes: `mov eax,[ecx] ; call [eax+0C4h] ; test al,al ; setne al ; ret` (vtable slot 49 =
//   CCreatureActionBase::IsPreventingNormalMovement, retail 0x00692A20 in vftable 0x01259D24).
// Identity: Ego_r.pdb 0x00518FD0 + FableWin.pdb 0x01DB9450 ?IsPreventingLookTurning@CCreatureActionBase@@UBE_NXZ (public virtual const);
//   retail CCreatureActionBase vftable 0x01259D24 slot 50 (+0xC8) = 0x00692670, bound under this spelling by the landed vtable emitters.
// NF259 (2026-11-22, q12730): re-spelled from the free __fastcall stand-in
//   ?CCreatureActionBase_IsPreventingLookTurning@@YI_NPAVCCreatureActionBase@@@Z (a vtable-array call through __vftable) onto the shared
//   engine/CCreatureActionBase.h (retail branch, the define block of the landed sibling 0x006929A0).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE_FULL_MEMBERS
#include "fable_string.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CCountedPointer.h"
#include "stlport/fable_containers.h"
#include "engine/CCreatureActionBase.h"

bool CCreatureActionBase::IsPreventingLookTurning() const
{
    return IsPreventingNormalMovement() ? true : false;
}
