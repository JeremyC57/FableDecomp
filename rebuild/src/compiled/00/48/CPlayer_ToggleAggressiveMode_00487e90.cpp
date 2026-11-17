// CPlayer::ToggleAggressiveMode() -- when aggressive mode is available, flip AggressiveMode (+0x217) and set the controls up for it
// Leaf: ?ToggleAggressiveMode@CPlayer@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00487E90, 32 bytes, thiscall.
// Identity: Ego_r.pdb 0x00493730 / FableWin.pdb 0x01AD0810 ?ToggleAggressiveMode@CPlayer@@QAEXXZ (donor order: after
//   SetUpControlsBasedOnAggressiveMode 0x00493670 = retail 0x00487DE0, before SetAggressiveMode 0x00493750 = retail 0x00487EB0).
// Callees: CPlayer::SetUpControlsBasedOnAggressiveMode 0x00487DE0 (private, both PDBs); data 0x013B85F3 g_bAggressiveModeAvailable
//   (DESCRIPTIVE, no PDB data name; the landed rows' spelling).
// NF83 (round 7, q11040 binder + own name): ported onto engine/CPlayer.h (the 0x00487DE0 define set + NF83 opt-in
//   FABLE_RETAIL_CPLAYER_AGGRESSIVEMODE); the local CPlayer model, the free __fastcall stand-in and its callee stand-in
//   CPlayer_OnAggressiveChanged are gone, and the enable byte takes the one spelling the other rows use.
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

void CPlayer::ToggleAggressiveMode()
{
    if (g_bAggressiveModeAvailable) {
        AggressiveMode = !AggressiveMode;
        SetUpControlsBasedOnAggressiveMode();
    }
}
