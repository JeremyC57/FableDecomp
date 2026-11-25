// CCreatureAction_CastWillReclaimUndeadFollower::Clone() const -- `return new CCreatureAction_CastWillReclaimUndeadFollower(*this);` through the out-of-line copy constructor
// Leaf: ?Clone@CCreatureAction_CastWillReclaimUndeadFollower@@UBEPAVCCreatureActionBase@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008BE7F0, 34 bytes: operator new(0x11C) 0x00BFEA1A, null test, then the copy constructor 0x008BE820 on `*this`;
//   a public const virtual __thiscall returning CCreatureActionBase* (0 when the allocation fails).
// Identity (NF425, 2026-11-25): self ?Clone@CCreatureAction_CastWillReclaimUndeadFollower@@UBEPAVCCreatureActionBase@@XZ: RTTI
//   placement + Ego_r/FableWin; callee ??0CCreatureAction_CastWillReclaimUndeadFollower@@QAE@ABV0@@Z: FableWin name set; retail
//   0x008BE820 stores CCreatureAction_CastWillReclaimUndeadFollower's vftable 0x0127D5BC first; the landed row at 0x008BE820 defines
//   it.
// Respell (NF425): was the stand-in self ?Clone@CCreatureAction_CastWillReclaimUndeadFollower@@QAEPAVCCreatureActionBase@@XZ
//   (non-virtual, non-const); now the real virtual member, so the RTTI-placed name is defined.
// Header: no engine header defines CCreatureAction_CastWillReclaimUndeadFollower; a local class over its RTTI direct base
//   CCombatActionBase engine/CCombatActionBase.h.
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

class CCreatureAction_CastWillReclaimUndeadFollower : public CCombatActionBase
{
public:
    CCreatureAction_CastWillReclaimUndeadFollower(const CCreatureAction_CastWillReclaimUndeadFollower& other);   // 0x008BE820
    virtual void PerformAction();                                   // ?PerformAction@CCreatureAction_CastWillReclaimUndeadFollower@@UAEXXZ (both PDB name sets)
public:
    virtual CCreatureActionBase* Clone() const;
    unsigned char _body[0x11C - sizeof(CCombatActionBase)];   // CCreatureAction_CastWillReclaimUndeadFollower's own members (not modelled: the copy constructor is out of line); sizeof 0x11C
};

CCreatureActionBase* CCreatureAction_CastWillReclaimUndeadFollower::Clone() const
{
    return new CCreatureAction_CastWillReclaimUndeadFollower(*this);
}
