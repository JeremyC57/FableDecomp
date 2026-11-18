// CPlayer::SetUpControlsBasedOnAggressiveMode() -- pick the PC control scheme (normal or aggressive) from the global def and, when leaving aggressive mode while the controlled creature is alive and carrying a weapon, make it sheathe the weapon back into the inventory
// Leaf: ?SetUpControlsBasedOnAggressiveMode@CPlayer@@AAEXXZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00487DE0, length 176 bytes, plain `ret`.
// Identity: Ego_r.pdb 0x00493670 + FableWin.pdb 0x01AD04D0 ?SetUpControlsBasedOnAggressiveMode@CPlayer@@AAEXXZ (private; callers
//   SetAggressiveMode 0x00487EB0, ToggleAggressiveMode 0x00487E90).
// Callees: CGamePlayerInterface::SetControlScheme(long) 0x00446EF0, CBaseIntelligentPointer::GetPItem 0x00A01B50 (x4),
//   CTCCarrying::IsThingCarryingWeapon(const CThing&) 0x006A1540, CCreatureAction_SheatheItemToInventory ctor 0x0069FFD0 / its implicit
//   virtual destructor's COMDAT 0x0048C950, CThingCreatureBase::SetCurrentAction 0x006644F0.
// Shape: the global def copied into the local CDefPointer pglobal_def (`inc [edi+4]` behind a null test; released at the end), the scheme
//   picked into control_index (Ego_d.pdb locals pglobal_def, control_index); the sheathe action a 0xB0-byte temporary.
// NF79 (round 7, q10972): ported onto the shared headers (CPlayer.h FABLE_RETAIL_PLAYER + my opt-in CPLAYER_SETUPCONTROLSBASEDONAGGRESSIVEMODE,
//   CGameDefinitionManager.h DEFPOINTERS, CGlobalDef.h, CGamePlayerInterface.h + my opt-in CGAMEPLAYERINTERFACE_SETCONTROLSCHEME, the retail CThing / CThingCreatureBase chain, CTCCarrying.h,
//   CCreatureAction_SheatheItemToInventory.h with NF77's IMPLICIT_DTOR): binds the destructor 0x0048C950 as ??1..@@UAE@XZ (was the local
//   non-virtual QAE) and the ctor 0x0069FFD0 with its reference parameter; self leaf private (was QAE). Was a local model of CPlayer /
//   CThing / CThingCreatureBase / the action / CDefPointer / CGlobalDef / CGameDefinitionManager / CGamePlayerInterface.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGPLAYERCREATURE
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTION_SHEATHEITEMTOINVENTORY_IMPLICIT_DTOR
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER_DEFPOINTERS
#define FABLE_RETAIL_CGAMEPLAYERINTERFACE_SETCONTROLSCHEME
#define FABLE_RETAIL_CTCCARRYING
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CPLAYER_SETUPCONTROLSBASEDONAGGRESSIVEMODE
#define FABLE_RETAIL_STLP_LIST
#include "stlport/fable_containers.h"
#include "fable_string.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CThing.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/CCreatureActionBase.h"
#include "engine/CCreatureAction_SheatheItemToInventory.h"
#include "engine/CTCCarrying.h"
#include "engine/CDefPointer.h"
#include "engine/CGlobalDef.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CGamePlayerInterface.h"
#include "engine/CPlayer.h"

// Lionhead's IS_THING_ALIVE on a thing pointer (one fetch: retail calls GetPItem once for the test), as the landed rows spell it.
inline bool IS_THING_ALIVE(const CThing* thing)
{
    if (thing != 0) {
        if (!thing->IsDead())
            return true;
    }
    return false;
}

void CPlayer::SetUpControlsBasedOnAggressiveMode()
{
    CDefPointer<const CGlobalDef> pglobal_def = DefinitionManager->PGlobalDef;
    long control_index;
    if (AggressiveMode)
        control_index = pglobal_def->ControlSchemePCAggressive;
    else
        control_index = pglobal_def->ControlSchemePC;
    PlayerInterface->SetControlScheme(control_index);
    if (!AggressiveMode && IS_THING_ALIVE(ControlledCreature.Get()) && CTCCarrying::IsThingCarryingWeapon(*ControlledCreature.Get()))
        ControlledCreature.Get()->SetCurrentAction(CCreatureAction_SheatheItemToInventory(*ControlledCreature.Get(), 0, 100));
}
