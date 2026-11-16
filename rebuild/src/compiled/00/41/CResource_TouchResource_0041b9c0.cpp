// CResource::TouchResource() -- once per frame, move the resource to the most-recently-used end of its resource list: unlink it (ListRemove
// 0x009FBFB0), insert it before the list head (ListInsert 0x009FBF90), and record the list's current frame in LastUsedFrame
// Leaf: ?TouchResource@CResource@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0041B9C0, 54 bytes, thiscall; no caller (every use expands it inline).
// NF39 (2026-11-16): moved off its free-__fastcall `CTCCameraPointScriptedSpline::EditCyclePreviousKeyCamera` stand-in (manifest label, wrong)
//   onto the shared engine/CResource.h + engine/CResourceList.h (opt-in FABLE_RETAIL_CRESOURCE_TOUCHRESOURCE: the header's body, which names
//   0x0041B9C0 as retail's only out-of-line copy). Evidence: FableWin.pdb ?TouchResource@CResource@@QAEXXZ; the body is exactly TouchResource
//   (CResource ResourceList +0x08 / LastUsedFrame +0x18, CResourceList Head +0x04 / CurrentFrame +0x2C, the private Ego_r.pdb / FableWin.pdb
//   ListRemove / ListInsert), expanded identically in RenderSnow 0x00B54F00.. and the landed CGraphicInfoFrame::GetTexture 0x00B66C10.
// The header body is forced inline (every retail use expands it); taking its address below makes the TU emit the out-of-line COMDAT (the
//   emitter is not part of the graded body).
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CMOVABLERESOURCE
#define FABLE_RETAIL_CRESOURCE_TOUCHRESOURCE
#include "engine/CResource.h"
#include "engine/CResourceList.h"

void (CResource::*Use_0041b9c0)() = &CResource::TouchResource;
