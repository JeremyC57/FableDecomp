// CRenderManagerCore::IsRendering() -- whether the render manager is between BeginRender and EndRender (its Rendering flag).
// Leaf: ?IsRendering@CRenderManagerCore@@QAE_NXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A0AA60, 7 bytes (`mov al,[ecx+3A54h] ; ret`).
// Naming (NF172, 2026-11-20, q11976): Ego_r.pdb 0x00B74010 + FableWin.pdb 0x030EB270 ?IsRendering@CRenderManagerCore@@QAE_NXZ, declared
//   in engine/CRenderManagerCore.h (Rendering +0x3A54, header static assert); in Ego_r order directly before SetAWindow 0x00A0AA80. Was
//   the free stand-in FableGetCanItemBeQuickAssigned_00A0AA60 on a local CTCInventoryBaseQuickAssignView view (catalog
//   _global::GetMember_byte0x3a54_00a0aa60). The sole landed caller, 0x00499880 CProgressDisplay::RenderProgress, binds this spelling in the
//   same hand-back.
#define FABLE_RETAIL_CRENDERMANAGERCORE
#define FABLE_RETAIL_C2DBOXF
#include "fable_compiler.h"
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/C2DBoxF.h"
#include "engine/CRenderManagerCore.h"

bool CRenderManagerCore::IsRendering()
{
    return Rendering;
}
