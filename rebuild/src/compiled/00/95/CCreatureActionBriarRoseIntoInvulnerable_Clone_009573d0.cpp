// CCreatureAction_BriarRoseIntoInvulnerable::Clone() const -- `return new CCreatureAction_BriarRoseIntoInvulnerable(*this);` through the out-of-line copy constructor
// Leaf: ?Clone@CCreatureAction_BriarRoseIntoInvulnerable@@UBEPAVCCreatureActionBase@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x009573D0, 34 bytes: operator new(0x11C) 0x00BFEA1A, null test, then the copy constructor 0x00957400 on `*this`;
//   a public const virtual __thiscall returning CCreatureActionBase* (0 when the allocation fails).
// Identity (NF425, 2026-11-25): self ?Clone@CCreatureAction_BriarRoseIntoInvulnerable@@UBEPAVCCreatureActionBase@@XZ: RTTI placement +
//   Ego_r/FableWin; callee ??0CCreatureAction_BriarRoseIntoInvulnerable@@QAE@ABV0@@Z: FableWin name set; retail 0x00957400 stores
//   CCreatureAction_BriarRoseIntoInvulnerable's vftable 0x01294B8C first; the landed row at 0x00957400 defines it.
// Respell (NF425): was the stand-in self ?Clone@CCreatureAction_BriarRoseIntoInvulnerable@@QAEPAVCCreatureActionBase@@XZ (non-virtual,
//   non-const); now the real virtual member, so the RTTI-placed name is defined.
// Header: engine/CCreatureAction_BriarRoseIntoInvulnerable.h, opt-in FABLE_RETAIL_BRIAR_ROSE: the polymorphic class there declares this
//   Clone and its copy constructor; prelude: the base class's opt-ins and header plus the opt-in, then this header.
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
#define FABLE_RETAIL_CTCPHYSICSBASE
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
#define FABLE_RETAIL_BRIAR_ROSE
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
#include "engine/CTCPhysicsBase.h"
#include "engine/CCreatureAction_BriarRoseIntoInvulnerable.h"

CCreatureActionBase* CCreatureAction_BriarRoseIntoInvulnerable::Clone() const
{
    return new CCreatureAction_BriarRoseIntoInvulnerable(*this);
}
