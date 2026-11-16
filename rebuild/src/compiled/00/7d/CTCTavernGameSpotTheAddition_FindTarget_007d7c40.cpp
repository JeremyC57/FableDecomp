// CTCTavernGameSpotTheAddition::FindTarget(float range) -- the Thing of the selected knick-knack (KnickKnacks[Selected]), or 0 when Selected is out of range;
// the range argument is never read
// Leaf: ?FindTarget@CTCTavernGameSpotTheAddition@@AAEPAVCThingObject@@M@Z
// Retail: Fable.exe @ 0x007D7C40, 50 bytes (`ret 4`: one argument, never read), no callees.
// NF39 (2026-11-16): moved off its local CTCTavernGameSpotTheAddition / CRetailVector<STargetEntry> model onto the shared
//   engine/CTCTavernGameSpotTheAddition.h: Ego_r.pdb + FableWin.pdb name set ?FindTarget@CTCTavernGameSpotTheAddition@@AAEPAVCThingObject@@M@Z (private,
//   float range, CThingObject*); KnickKnacks (+0x110, 16-byte KnickKnack {Pos, Thing +0x0C}) and Selected (+0x144) per Ego_r.pdb at the retail offsets.
// Shape: the range check is `Selected < size()` with size() an unsigned element count (`sar edx,4` then `jae`) followed by `Selected >= 0`
//   (`test ; jl`); the element is reached through operator[] (`*(begin() + n)`), which re-loads the start after the checks.
// Defines: the landed 0x007D8570 ProcessInput preamble (the class header's chain).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTCTAVERNGAME
#define FABLE_RETAIL_CTCTAVERNGAME_GETDEF_OUTOFLINE
#define FABLE_RETAIL_CTCTAVERNGAME_POSITIONMESH
#define FABLE_RETAIL_CTCTAVERNGAME_GETGAMESTATE
#define FABLE_RETAIL_CPLAYERGUI
#define FABLE_RETAIL_CSUBTITLERENDERER
#define FABLE_RETAIL_CSUBTITLERENDERER_CLEAR
#define FABLE_RETAIL_CGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_PACKED
#define FABLE_RETAIL_C3DVECTOR_DOT_GROUPED
#define FABLE_RETAIL_CRIGHTHANDEDSET
#define FABLE_RETAIL_CRIGHTHANDEDSET_GETRIGHT_OUTOFLINE
#define FABLE_RETAIL_INPUTPROCESS
#define FABLE_RETAIL_INPUTPROCESS_ZTARGET
#define FABLE_RETAIL_INPUTPROCESS_MOVEMENT
#define FABLE_RETAIL_CINPUTPROCESSTAVERNGAME_PRESSED_ACCESS
#define FABLE_RETAIL_CPROCESSEDINPUT_PERFORMED
#include "stlport/fable_containers.h"
#include "engine/C3DVector.h"
#include "engine/C2DVector.h"
#include "engine/CRightHandedSet.h"
#include "engine/CTavernGameDef.h"
#include "engine/CTavernGameSpotTheAdditionDef.h"
#include "engine/CDefinitionManager.h"
#include "engine/CTCTavernGame.h"
#include "engine/CTCTavernGameSpotTheAddition.h"
#include "engine/CPlayerGui.h"
#include "engine/CWorld.h"
#include "fable_game.h"
#include "engine/CRGBColour.h"
#include "engine/EGameAction.h"
#include "engine/CInputEvent.h"
#include "engine/CProcessedInput.h"
#include "engine/CInputProcessGameBase.h"
#include "engine/CInputProcessTavernGame.h"

CThingObject* CTCTavernGameSpotTheAddition::FindTarget(float)
{
    if (Selected < KnickKnacks.size() && Selected >= 0)
        return KnickKnacks[Selected].Thing;
    return 0;
}
