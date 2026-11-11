// CTCDoor::UpdateOpenStatusTriggerTypeScript() -- when the script changed the open state, open (touching the appearance's DOOR_OPEN animation in the main mesh bank) or close the door, then latch OldOpen
// Leaf: ?UpdateOpenStatusTriggerTypeScript@CTCDoor@@IAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007310C0, 163 bytes (+ int3 padding), plain `ret`. Ego_r.pdb / FableWin.pdb
//   ?UpdateOpenStatusTriggerTypeScript@CTCDoor@@IAEXXZ (protected).
// Callees (PDB spellings): CTCDoor::Open(bool) 0x00730C00, CTCDoor::Close() 0x00730DD0, CDefClassBase::GetSubDef<CAppearanceDef> 0x0042B0A2
//   (on the thing's base def read in place; result ignored), CAnimationSet::GetAnim 0x005DC340, CTCBase::GetWorld 0x00686A70,
//   CMeshDataBank::PeekAnimation 0x00A242C0 (through CWorld::GetMeshBank, PMainMeshBank +0x3C; result unused), the def's virtual release.
// Globals: the CCharString "DOOR_OPEN" @ 0x013BAD34 (dynamic initializer 0x0121ED00; no PDB data name on disk: the spelling the landed
//   Open 0x00730C00 binds). Filter temporary: CAnimFilterNoGroup (vtable 0x0122FFDC; base CAnimFilterBase 0x0122FFCC).
// Layout: engine/CTCDoor.h (CurrentOpen +0x10, OldOpen +0x11); CThing PBaseDef +0x70; CAppearanceDef Animation +0x34.
// Name fix (agentNF12 2026-11-11): moved off the local CTCDoor_Methods / CDefPointer / CAppearanceDef holder / CWorld / CMeshDataBank
// stand-ins (and a same-TU stand-in GetSubDefRaw body) onto the shared headers. 0x0042B0A2 binds GetSubDef<CAppearanceDef> (was
// ??$GetSubDefRaw@VCAppearanceDef@@@CDefClassBase@@...), 0x00686A70 ?GetWorld@CTCBase@@IBEAAVCWorld@@XZ (was QAE returning a pointer);
// the class is CTCDoor (was CTCDoor_Methods) and the row's own symbol the PDB protected IAEXXZ.
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_BODY
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTCDOOR
#define FABLE_RETAIL_CTCDOOR_OPENCLOSE
#define FABLE_RETAIL_CANIMFILTER
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLD_MESH_BANK
#define FABLE_RETAIL_CMESHDATABANK_PEEKANIMATION
#include "fable_string.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CDefClassBase.h"
#include "engine/CThing.h"
#include "engine/CThingBaseDef.h"
#include "engine/CDefPointer.h"
#include "engine/CAppearanceDef.h"
#include "engine/CAnimationEntry.h"
#include "engine/CAnimFilterNoGroup.h"
#include "engine/CDefinitionManager.h"
#include "engine/CWorld.h"
#include "engine/CMeshDataBank.h"
#include "engine/CTCDoor.h"

extern const CCharString g_DoorOpenAnimName;                   // 0x013BAD34 ("DOOR_OPEN")

void CTCDoor::UpdateOpenStatusTriggerTypeScript()
{
    if (CurrentOpen != OldOpen) {
        if (CurrentOpen) {
            Open(true);
            CDefPointer<const CAppearanceDef> appearance;
            ParentThing->PeekBaseDef().GetSubDef(&appearance);
            const CAnimationEntry& entry = appearance->Animation.GetAnim(g_DoorOpenAnimName, 0, CAnimFilterNoGroup());
            GetWorld().GetMeshBank()->PeekAnimation(entry.BankIndex);
        } else {
            Close();
        }
        OldOpen = CurrentOpen;
    }
}
