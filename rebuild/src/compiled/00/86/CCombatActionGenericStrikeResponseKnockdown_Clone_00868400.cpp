// CCombatAction_GenericStrikeResponseKnockdown::Clone() const -- `return new CCombatAction_GenericStrikeResponseKnockdown(*this);` through the out-of-line copy constructor
// Leaf: ?Clone@CCombatAction_GenericStrikeResponseKnockdown@@EBEPAVCCreatureActionBase@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00868400, 34 bytes: operator new(0x13C) 0x00BFEA1A, null test, then the copy constructor 0x00868430 on `*this`;
//   a private const virtual __thiscall returning CCreatureActionBase* (0 when the allocation fails).
// Identity (NF425, 2026-11-25): self ?Clone@CCombatAction_GenericStrikeResponseKnockdown@@EBEPAVCCreatureActionBase@@XZ: RTTI placement
//   + Ego_r/FableWin; callee ??0CCombatAction_GenericStrikeResponseKnockdown@@QAE@ABV0@@Z: Ego_r/FableWin name set; retail 0x00868430
//   stores CCombatAction_GenericStrikeResponseKnockdown's vftable 0x01277344 first; the landed row at 0x00868430 defines it.
// Respell (NF425): was the stand-in self ?Clone@CCombatAction_GenericStrikeResponseKnockdown@@QAEPAVCCreatureActionBase@@XZ
//   (non-virtual, non-const); now the real virtual member, so the RTTI-placed name is defined.
// Header: engine header engine/CCombatAction_GenericStrikeResponseKnockdown.h does not give the class at sizeof 0x13C with Clone
//   declared in the prelude tried (header debt); a local class over its RTTI direct base CCombatActionBase engine/CCombatActionBase.h.
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

class CCombatAction_GenericStrikeResponseKnockdown : public CCombatActionBase
{
public:
    CCombatAction_GenericStrikeResponseKnockdown(const CCombatAction_GenericStrikeResponseKnockdown& other);   // 0x00868430
private:
    virtual CCreatureActionBase* Clone() const;
public:
    unsigned char _body[0x13C - sizeof(CCombatActionBase)];   // CCombatAction_GenericStrikeResponseKnockdown's own members (not modelled: the copy constructor is out of line); sizeof 0x13C
};

CCreatureActionBase* CCombatAction_GenericStrikeResponseKnockdown::Clone() const
{
    return new CCombatAction_GenericStrikeResponseKnockdown(*this);
}
