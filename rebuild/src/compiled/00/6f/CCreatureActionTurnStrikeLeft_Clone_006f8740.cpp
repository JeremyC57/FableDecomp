// CCreatureAction_TurnStrikeLeft::Clone() const -- `return new CCreatureAction_TurnStrikeLeft(*this);` with the implicit copy constructor inlined (CCombatActionBase copy constructor, vftable store)
// Leaf: ?Clone@CCreatureAction_TurnStrikeLeft@@UBEPAVCCreatureActionBase@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x006F8740, 47 bytes: operator new(0x114) 0x00BFEA1A, null test, CCombatActionBase(const CCombatActionBase&) 0x0062F3B0, C's vftable store;
//   a public const virtual __thiscall returning CCreatureActionBase* (0 when the allocation fails).
// Identity (NF425, 2026-11-25): self ?Clone@CCreatureAction_TurnStrikeLeft@@UBEPAVCCreatureActionBase@@XZ: RTTI placement +
//   Ego_r/FableWin; base copy constructor ??0CCombatActionBase@@QAE@ABV0@@Z 0x0062F3B0 (Ego_r/FableWin; MELEE_COPYCTOR / COPYCTOR
//   opt-in).
// Respell (NF425; NF427 retail override set): was the stand-in self ?Clone@CCreatureAction_TurnStrikeLeft@@QAEPAVCCombatActionBase@@XZ (non-virtual, non-const);
//   now the real virtual member, so the RTTI-placed name is defined.
// Overrides (NF427): no engine/CCreatureAction_TurnStrikeLeft.h exists, so the local class declares the class's retail override set:
//   every slot of retail vftable 0x01263544 that differs from CCombatActionBase's (evidence/slotdiff.tsv), each RTTI-placed and in a
//   PDB name set; the emitted ??_7CCreatureAction_TurnStrikeLeft@@6B@ names all 81 of retail's slots (0 DIFF, 0 WRONG).
// Header: no engine header defines CCreatureAction_TurnStrikeLeft (header debt); a local class over its RTTI direct base
//   CCombatActionBase (engine/CCombatActionBase.h).
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

class CCreatureActionBase;
class CCreatureAction_TurnStrikeLeft : public CCombatActionBase
{
public:
    virtual CCreatureActionBase* Clone() const;                       // slot 11 ?Clone@CCreatureAction_TurnStrikeLeft@@UBEPAVCCreatureActionBase@@XZ (egor+fablewin)
    virtual void Initialise();                                        // slot 12 ?Initialise@CCreatureAction_TurnStrikeLeft@@UAEXXZ (egor+fablewin)
};

CCreatureActionBase* CCreatureAction_TurnStrikeLeft::Clone() const
{
    return new CCreatureAction_TurnStrikeLeft(*this);
}
