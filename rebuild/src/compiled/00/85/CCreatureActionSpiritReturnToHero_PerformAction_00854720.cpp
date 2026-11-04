// CCreatureAction_SpiritReturnToHero::PerformAction() -- send the spirit back: its physics position is reset to the hero creature's position (its own position when the player controls no creature)
// Leaf: ?PerformAction@CCreatureAction_SpiritReturnToHero@@UAEXXZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00854720, 104 bytes, vtable slot 3 (PerformAction, pure in CCreatureActionBase).
// Callees: CPlayerManager::GetMainPlayer() 0x00449970 on CMainGameComponent::Get()->GetPlayerManager() (inline: the static
//   0x013B86A0, PPlayerManager +0x1C), CPlayer::GetPControlledCreature() const 0x00487DC0 (twice), GetPItem 0x00A01B50
//   (twice, the PCreature intelligent pointer), CThing::GetPos() const 0x004C73D0, CTCPhysicsBase::ResetPosition(const
//   C3DVector&) (virtual slot 31, +0x7C) on GetPhysicsTC() (+0x60).
// Layout: CCreatureActionBase::PCreature +0x08 (Ego_r.pdb; header overlay FABLE_RETAIL_CCREATUREACTIONBASE_PCREATURE).
// Shape: an if/else assigning a C3DVector local (the frame vector, copied before the second GetPItem); the pointer ternary
//   `(a ? a : b)->GetPos()` of the old draft swapped the this/player registers (esi/edi).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGPLAYERCREATURE
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE_PCREATURE
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CPLAYERMANAGER
#define FABLE_RETAIL_CGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_SCOPED_PTR
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CCreatureActionBase.h"
#include "engine/CPlayer.h"
#include "engine/CPlayerManager.h"
#include "fable_game.h"
#define CCreatureAction_SpiritReturnToHero DonorCCreatureAction_SpiritReturnToHero
#include "engine/CCreatureAction_SpiritReturnToHero.h"
#undef CCreatureAction_SpiritReturnToHero

class CCreatureAction_SpiritReturnToHero : public CCreatureActionBase {
public:
    virtual void PerformAction();
};

void CCreatureAction_SpiritReturnToHero::PerformAction()
{
    CPlayer& player = CMainGameComponent::Get()->GetPlayerManager().GetMainPlayer();
    C3DVector target;
    if (player.GetPControlledCreature())
        target = player.GetPControlledCreature()->GetPos();
    else
        target = PCreature->GetPos();
    PCreature->GetPhysicsTC()->ResetPosition(target);
}