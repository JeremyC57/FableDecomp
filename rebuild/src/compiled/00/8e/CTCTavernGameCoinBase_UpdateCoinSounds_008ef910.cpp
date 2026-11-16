// CTCTavernGameCoinBase::UpdateCoinSounds(bool) -- forward the flag to every coin's UpdateSound.
// Leaf: ?UpdateCoinSounds@CTCTavernGameCoinBase@@QAEX_N@Z
// NF37 (2026-11-16): moved off the local CTavernCoin / CTCTavernGameCoinBase stand-ins onto the shared engine/CTCTavernGameCoinBase.h
// (Coins +0xFC, std::vector<Coin>) and engine/Coin.h. Ego_r.pdb 0x009152C0 / FableWin.pdb ?UpdateCoinSounds@CTCTavernGameCoinBase@@QAEX_N@Z (bool,
// the landed caller 0x007CF9A0 binds it so); callee Coin::UpdateSound(bool) 0x008EF400 (Ego_r 0x00914D50 / FableWin 0x02AEA420, Ego_r order
// Coin::Update / ctor / UpdateSound). Include chain = the preamble of the landed 0x007CF9A0.
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
#define FABLE_RETAIL_CTCTAVERNGAME_GETGAMESTATE
#define FABLE_RETAIL_CGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_PACKED
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C2DVECTOR_NORMALISE
#define FABLE_RETAIL_CRIGHTHANDEDSET
#define FABLE_RETAIL_INPUTPROCESS
#define FABLE_RETAIL_INPUTPROCESS_ZTARGET
#define FABLE_RETAIL_INPUTPROCESS_MOVEMENT
#define FABLE_RETAIL_INPUTPROCESS_EVENTINQUEUE
#define FABLE_RETAIL_CINPUTPROCESSTAVERNGAME_PRESSED_ACCESS
#define FABLE_RETAIL_CINPUTPROCESSTAVERNGAME_EVENTINQUEUE_ACCESS
#define FABLE_RETAIL_CPROCESSEDINPUT_PERFORMED
#include "stlport/fable_containers.h"
#include "engine/C3DVector.h"
#include "engine/C2DVector.h"
#include "engine/CRightHandedSet.h"
#include "engine/CTavernGameDef.h"
#include "engine/CTavernGameShoveHaPennyDef.h"
#include "engine/CDefinitionManager.h"
#include "engine/CTCTavernGame.h"
#include "engine/CTCTavernGameCoinBase.h"
#include "engine/CTCTavernGameShoveHaPenny.h"
#include "fable_game.h"
#include "engine/EGameAction.h"
#include "engine/CInputEvent.h"
#include "engine/CProcessedInput.h"
#include "engine/CInputProcessGameBase.h"
#include "engine/CInputProcessTavernGame.h"
#include "engine/Coin.h"

void CTCTavernGameCoinBase::UpdateCoinSounds(bool on)
{
    for (std::vector<Coin>::iterator coin = Coins.begin(); coin != Coins.end(); ++coin)
        coin->UpdateSound(on);
}
