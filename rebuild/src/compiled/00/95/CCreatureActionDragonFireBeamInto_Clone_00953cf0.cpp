// CCreatureAction_DragonFireBeamInto::Clone() const -- new CCreatureAction_DragonFireBeamInto(*this) with the implicit copy constructor inlined
//   (base copy + the Timer member at +0x114)
// Leaf: ?Clone@CCreatureAction_DragonFireBeamInto@@UBEPAVCCreatureActionBase@@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00953CF0, oracle length 59 bytes.
//
// Retail bytes:  push esi ; push edi ; push 0x118 ; mov edi,ecx ; call operator new ; mov esi,eax ; add esp,4 ; test esi,esi ; je N ; push edi ; mov ecx,esi ; call CCombatActionBase::CCombatActionBase(const CCombatActionBase&) ; mov dword ptr [esi],offset CCreatureAction_DragonFireBeamInto::`vftable' ; mov eax,[edi+0x114] ; mov [esi+0x114],eax ; pop edi ; mov eax,esi ; pop esi ; ret ; N: pop edi ; xor eax,eax ; pop esi ; ret
// Virtual clone: the implicit copy constructor is inlined as the base copy constructor @ 0x0062F3B0, this class's vtable store
// (0x01293E54, RTTI .?AVCCreatureAction_DragonFireBeamInto@@) and the Timer copy. sizeof = 0x118.
// Names (agentNF255, 2026-11-22, q12720, with its ctor 0x009535B0): own leaf now the PDB
//   ?Clone@CCreatureAction_DragonFireBeamInto@@UBEPAVCCreatureActionBase@@XZ (Ego_r.pdb 0x00978640 + FableWin.pdb 0x02BE2150: public virtual
//   const, returns CCreatureActionBase*; retail vftable 0x01293E54 slot 11), was the non-virtual stand-in
//   ?Clone@CCreatureAction_DragonFireBeamInto@@QAEPAUCBase_0062f3b0@@XZ on a row-local CBase_0062f3b0 model; the base copy constructor
//   is now ??0CCombatActionBase@@QAE@ABV0@@Z (MELEE_COPYCTOR, Ego_r 0x00545FD0 + FableWin 0x01D919A0; the landed 0x00956960 binds the same).
//   The 0x00956960 (CCreatureAction_ThrowShotObject::Clone) recipe on the shared headers.
// Vtable: the prelude text of the ctor row 0x009535B0 (see there), so this object's ??_7CCreatureAction_DragonFireBeamInto@@6B@ COMDAT is
//   the same 81-slot copy.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_PACKED
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE_FULL_MEMBERS
#define FABLE_RETAIL_MAZE_DEFENSIVE_MELEE_PASS27
#define FABLE_RETAIL_CCOMBATACTIONBASE_MELEE
#define FABLE_RETAIL_CCOMBATACTIONBASE_MELEE_COPYCTOR
#define FABLE_RETAIL_CCOMBATACTIONBASE_FRAMEUPDATE
#define FABLE_RETAIL_CCOMBATACTIONBASE_INITIALISE
#define FABLE_RETAIL_CCOMBATACTIONBASE_FINISHACTION
#define FABLE_RETAIL_CCOMBATACTIONBASE_ONANIMATIONLOADED
#define FABLE_RETAIL_CCOMBATACTIONBASE_MELEE_OWN_VIRTUALS
#define FABLE_RETAIL_CCOMBATACTIONBASE_SOUNDCTOR
#define FABLE_RETAIL_CCOMBATACTIONBASE_TARGETCTOR
#define FABLE_RETAIL_JACKDRAGON_HOVER_PASS27
#define FABLE_RETAIL_CCREATUREACTION_DRAGONFIREBEAMINTO_INITIALISE
#include "fable_string.h"
#include "engine/fable_ai_action_enums.h"
#include "engine/C3DVector.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CCountedPointer.h"
#include "stlport/fable_containers.h"
#include "engine/CCreatureActionBase.h"
#include "engine/CCombatActionBase.h"
#include "engine/CSoundPair.h"
#include "engine/CCreatureAction_DragonFireBeamInto.h"

CCreatureActionBase* CCreatureAction_DragonFireBeamInto::Clone() const
{
    return new CCreatureAction_DragonFireBeamInto(*this);
}
