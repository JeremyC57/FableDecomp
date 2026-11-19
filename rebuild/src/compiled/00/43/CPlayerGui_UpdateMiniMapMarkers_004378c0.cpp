// CPlayerGui::UpdateMiniMapMarkers(bool) -- forward to the mini-map drawer's UpdateMarkers through the counted pointer PDrawMiniMap (+0x160)
// Leaf: ?UpdateMiniMapMarkers@CPlayerGui@@QAEX_N@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004378C0, 11 bytes: `mov ecx,[ecx+160h] ; jmp 0x00643E9A` (the bool passes through).
// NF104 (q4761, 2026-11-19): moved off its `T::Run()` pad forwarder stand-in (file CPlayerGui_Refresh) onto the shared engine/CPlayerGui.h
//   (FABLE_RETAIL_CPLAYERGUI + _MEMBERS: PDrawMiniMap +0x160, UpdateMiniMapMarkers(bool) declared) and engine/CDrawMiniMap.h
//   (FABLE_RETAIL_NPLAYERGUI_CDRAWMINIMAP_CLASS, NF104 overlay opt-in FABLE_RETAIL_NPLAYERGUI_CDRAWMINIMAP_UPDATEMARKERS), the defines of the
//   landed same-class forwarder 0x00437870 IsMiniMapDrawing. Names: Ego_r.pdb 0x0044A560 + FableWin.pdb 0x01A7FC60
//   ?UpdateMiniMapMarkers@CPlayerGui@@QAEX_N@Z (31 landed rows bind it); the callee 0x00643E9A is Ego_r.pdb
//   ?UpdateMarkers@CDrawMiniMap@NPlayerGui@@QAEX_N@Z (was bound as the stand-in Sub::Run_00643e9a).
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CPLAYERGUI
#define FABLE_RETAIL_CPLAYERGUI_MEMBERS
#define FABLE_RETAIL_NHEROINFORMATIONSCREENS_CMANAGER
#define FABLE_RETAIL_NHEROINFORMATIONSCREENS_CMANAGER_LAYOUT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CENGINEPRIMITIVEHANDLE
#define FABLE_RETAIL_CTEXTURE
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_NPLAYERGUI_CDRAWMINIMAP_CLASS
#define FABLE_RETAIL_NPLAYERGUI_CDRAWMINIMAP_UPDATEMARKERS
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/CPlayerGui.h"
#include "engine/CDrawMiniMap.h"

void CPlayerGui::UpdateMiniMapMarkers(bool update)
{
    PDrawMiniMap->UpdateMarkers(update);
}
