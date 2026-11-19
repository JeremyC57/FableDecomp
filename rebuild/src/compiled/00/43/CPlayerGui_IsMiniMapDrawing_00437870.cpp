// CPlayerGui::IsMiniMapDrawing() const -- whether the mini-map illustrator is drawing: PDrawMiniMap (+0x160) -> CDrawBase::Drawing (+0x18)
// Leaf: ?IsMiniMapDrawing@CPlayerGui@@QBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00437870.
// NF95 (part 2, q11376): moved off its global __fastcall stand-in CTCShotDefinitionBase_CanBeHitByMeleeStrike (manifest label
//   GetMember_dword0x160_derefbyte0x18) onto the shared engine/CPlayerGui.h (opt-in FABLE_RETAIL_CPLAYERGUI_MINIMAPDRAWING, landed by
//   0x00654438) + engine/CDrawMiniMap.h (FABLE_RETAIL_NPLAYERGUI_CDRAWMINIMAP_CLASS). Names: Ego_r.pdb 0x0044A510 / FableWin.pdb
//   0x01A7F700 ?IsMiniMapDrawing@CPlayerGui@@QBE_NXZ; callers 0x004A3740 / 0x0064D404 / 0x00654438 already bind it so (xref).
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
#define FABLE_RETAIL_CPLAYERGUI_MINIMAPDRAWING
#define FABLE_RETAIL_NPLAYERGUI_CDRAWMINIMAP_CLASS
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/CPlayerGui.h"
#include "engine/CDrawMiniMap.h"

bool CPlayerGui::IsMiniMapDrawing() const
{
    return PDrawMiniMap->IsDrawing();
}
