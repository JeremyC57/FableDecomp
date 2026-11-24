// CTCTavernGameSpotTheAddition::GetWinPhrase() -- the slot-42 override: the spot-the-addition game has no win phrase (returns L"")
// Leaf: ?GetWinPhrase@CTCTavernGameSpotTheAddition@@UAE?AVCWideString@@XZ
// Retail: Fable.exe @ 0x007D7C10, 23 bytes (+ int3 padding), `ret 4`. Callees: CWideString::CWideString(const wchar_t*) @ 0x0099B6B0,
// empty literal @ 0x0122D70C. Shape: a member returning a class by value: the hidden result pointer is the first stack argument
// (`mov esi,[esp+8]`), `this` is unused, the temporary is constructed in place.
// NF238 (2026-11-22, q12671): renamed from the propagated manifest label CUserProfileManager::GetAutoSaveFileName (self leaf
//   ?GetAutoSaveFileName@CUserProfileManager@@QAE?AVCWideString@@XZ, a name whose real address is 0x00406690) and ported off its row-local
//   CUserProfileManager model onto engine/CTCTavernGameSpotTheAddition.h (the FindTarget 0x007D7C40 prelude). Evidence:
//   (1) the only reference to 0x007D7C10 is slot 42 of the CTCTavernGameSpotTheAddition vtable 0x0126F814 (absrefs); slot 42 sits between
//       Farewell (41) and GetBestScore (43) and is GetWinPhrase in every tavern-game vtable (CTCTavernGame 0x005EA100 in the base and four
//       games, CTCTavernGameCardBlackjack::GetWinPhrase 0x007DA3A0 in its own; engine/CTCTavernGame.h slot list);
//   (2) Ego_r.pdb 0x007D4EC0 / FableWin.pdb 0x02753360 ?GetWinPhrase@CTCTavernGameSpotTheAddition@@UAE?AVCWideString@@XZ, the only
//       CWideString getter of the class not already at slots 43-46; Ego_r places it 0x70 after EndRound (0x007D4E50), as retail 0x007D7C10
//       sits 0x70 after EndRound 0x007D7BA0, and gives it a 0x20 slot (this 23-byte body).
//   No landed row binds 0x007D7C10 (the ctor 0x007D8DA0 emits a one-slot local vtable model), so the self rename is free.
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

CWideString CTCTavernGameSpotTheAddition::GetWinPhrase()
{
    return CWideString(L"");
}
