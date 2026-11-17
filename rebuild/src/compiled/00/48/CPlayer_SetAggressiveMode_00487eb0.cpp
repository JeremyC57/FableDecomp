// CPlayer::SetAggressiveMode(bool on) -- when aggressive mode is available and the state changes, store it and set the controls up for it
// Leaf: ?SetAggressiveMode@CPlayer@@QAEX_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00487EB0, 64 bytes, thiscall `ret 4`.
// Identity: Ego_r.pdb 0x00493750 / FableWin.pdb 0x01AD0860 ?SetAggressiveMode@CPlayer@@QAEX_N@Z.
// Callees: CPlayer::SetUpControlsBasedOnAggressiveMode 0x00487DE0 (private, both PDBs); data 0x013B85F3 g_bAggressiveModeAvailable.
// NF83 (round 7, q11040 binder): ported onto engine/CPlayer.h (the 0x00487DE0 define set + NF83 opt-in FABLE_RETAIL_CPLAYER_AGGRESSIVEMODE);
//   the CPlayerDonor renaming-macro include and its derived local CPlayer are gone, so the callee binds the private AAE spelling.
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
#define FABLE_RETAIL_CPLAYER_AGGRESSIVEMODE
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
extern unsigned char g_bAggressiveModeAvailable;   // 0x013B85F3

void CPlayer::SetAggressiveMode(bool on)
{
    if (g_bAggressiveModeAvailable) {
        if (!AggressiveMode && on) {
            AggressiveMode = true;
            SetUpControlsBasedOnAggressiveMode();
        } else if (AggressiveMode && !on) {
            AggressiveMode = on;
            SetUpControlsBasedOnAggressiveMode();
        }
    }
}
