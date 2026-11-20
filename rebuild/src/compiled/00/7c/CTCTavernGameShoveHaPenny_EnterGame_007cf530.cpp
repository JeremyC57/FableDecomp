// CTCTavernGameShoveHaPenny::EnterGame() -- coin-base EnterGame, put the camera on the def's named camera point, rounds played reset
// Leaf: ?EnterGame@CTCTavernGameShoveHaPenny@@UAEXXZ
// Retail: Fable.exe @ 0x007CF530, 123 bytes; CTCTavernGameShoveHaPenny vtable 0x0126F2F4 slot 29 (EnterGame; the catalog's
//         "vfunc_29").
// Callees: CTCTavernGameCoinBase::EnterGame() 0x008EF390 (non-virtual base call); CTCTavernGame::GetDef(CDefPointer<const CTavernGameDef>*)
//          0x005EA000; CDefStringTable::GetString(long) 0x009D49B0 on GDefStringTable 0x013CA828 (CCharString by hidden
//          pointer); CTCTavernGame::CameraUseCameraPoint(const CCharString&) 0x005EBFA0; CCharString::~CCharString 0x0099EAE0;
//          the def's slot 1 on release.
// Layout: CTavernGameDef::CameraName (CDefString, a string-table id) +0x90 (PDB offset holds); retail
//         CTCTavernGameShoveHaPenny::RoundsPlayed +0x148 (PDB +0x160).
// Shape: the camera name is a named local in its own block (`lea ecx ; push ecx`, destroyed before the RoundsPlayed store);
//        null-tested CDefPointer dtor with the def's inline ReleaseTemporary().
// Naming (NF159, 2026-11-20, q5079): ported onto engine/CTCTavernGameShoveHaPenny.h (retail branch) from the local CoinBase / def / string
//   stand-ins: ?GetDef@CTCTavernGame@@QAEXPAV?$CDefPointer@$$CBVCTavernGameDef@@@@@Z (Ego_r.pdb / FableWin.pdb; was
//   ?GetTavernGameDef@CTCTavernGameCoinBase@@QAEXPAUCDefPointer@@@Z), ?CameraUseCameraPoint@CTCTavernGame@@QAEXABVCCharString@@@Z (the
//   landed 0x007D7FD0 spelling; was on CoinBase), the base call ?EnterGame@CTCTavernGameCoinBase@@UAEXXZ (virtual, both PDBs; was QAE; its other
//   binder 0x007D34A0 moves with it). The camera name is CDefString::GetString (GDefStringTable.GetString(TablePos) in place, as 0x007D7FD0).
//   Own leaf: the virtual override (vtable 0x0126F2F4 slot 29; was QAE).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CDEFSTRING_FORCEINLINE_GETSTRING
#define FABLE_RETAIL_CTCTAVERNGAME
#define FABLE_RETAIL_CTCTAVERNGAME_GETDEF_OUTOFLINE
#define FABLE_RETAIL_CTCTAVERNGAME_CAMERAPOINT
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_PACKED
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_CRIGHTHANDEDSET
#include "fable_compiler.h"
#include "fable_string.h"
#include "engine/CDefClassBase.h"
#include "engine/CDefPointer.h"
#include "engine/CTavernGameDef.h"
#include "engine/CTCTavernGameShoveHaPenny.h"

void CTCTavernGameShoveHaPenny::EnterGame()
{
    CTCTavernGameCoinBase::EnterGame();
    CDefPointer<const CTavernGameDef> def;
    GetDef(&def);
    {
        CCharString camera = def->CameraName.GetString();
        CameraUseCameraPoint(camera);
    }
    RoundsPlayed = 0;
}
