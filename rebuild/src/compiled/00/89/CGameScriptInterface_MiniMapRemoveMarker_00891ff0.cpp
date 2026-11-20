// CGameScriptInterface::MiniMapRemoveMarker(const CScriptThing& thing) const -- when the script thing is valid, remove its thing's mini-map marker (the player GUI's mini-map display, 0x00437CE0, then 0x0082A380)
// Leaf: ?MiniMapRemoveMarker@CGameScriptInterface@@UBEXABVCScriptThing@@@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00891FF0, length 49 bytes.
// Callees: the script thing validated through its virtual slot 75 (+0x12c, CScriptThing::IsAlive), the thing fetched through slot 11 (+0x2c,
//   GetPThing), CPlayerGui::GetMiniMap 0x00437CE0 on the player GUI singleton (CTBaseSingletonNoDefaultAlloc<CPlayerGui>::PClass 0x013B8790),
//   CMiniMapDisplay::RemoveMarker 0x0082A380. Shape: the script thing in esi, the thing pushed before the display is fetched, `ret 4`.
// NF171 (2026-11-20, q5131): ported onto the shared headers (engine/CGameScriptInterface.h NSCRIPT, engine/CPlayerGui.h with
//   FABLE_RETAIL_CPLAYERGUI_GETMINIMAP, engine/CMiniMapDisplay.h): 0x0082A380 binds ?RemoveMarker@CMiniMapDisplay@@QAEXABVCThing@@@Z (Ego_r.pdb,
//   the landed self row; was ?RemoveMarker@CMiniMapDisplay_Retail@@QAEXPAUCThing_Retail@@@Z), 0x00437CE0 binds Ego_r.pdb / FableWin.pdb
//   ?GetMiniMap@CPlayerGui@@QAEAAVCMiniMapDisplay@@XZ (was ?GetMiniMapDisplay@CGameUI_Retail@@QAEPAUCMiniMapDisplay_Retail@@XZ) and 0x013B8790
//   the established PClass spelling (was the free g_pGameUI_013b8790). The thing is the real CThing& from GetPThing() (no cast); the unused
//   local CThing_Retail / CCharString / CWideString / CGameUI_Retail / CMainGameComponent_Retail models of the old row are gone.
// Inferred: the named `pthing` reference: retail reads the singleton word (`mov ecx,[0x013B8790]`, 0x0089200A) only after the GetPThing
//   vcall (0x00892007); the one-expression spelling `Get().GetMiniMap().RemoveMarker(*thing.GetPThing())` hoists that load into edi before
//   the vcall (DIFFER 53v49, push/pop edi). The local changes no behaviour.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CPLAYERGUI
#define FABLE_RETAIL_CPLAYERGUI_GETMINIMAP
#define FABLE_RETAIL_CMINIMAPDISPLAY
#define FABLE_RETAIL_NSCRIPT
#include "stlport/fable_containers.h"
#include "fable_string.h"
#include "engine/CMiniMapDisplay.h"
#include "engine/CPlayerGui.h"
#include "engine/CScriptThing.h"
#include "engine/CGameScriptInterface.h"

void CGameScriptInterface::MiniMapRemoveMarker(const CScriptThing& thing) const
{
    if (thing.IsAlive()) {
        const CThing& pthing = *thing.GetPThing();
        CTBaseSingletonNoDefaultAlloc<CPlayerGui>::Get().GetMiniMap().RemoveMarker(pthing);
    }
}
