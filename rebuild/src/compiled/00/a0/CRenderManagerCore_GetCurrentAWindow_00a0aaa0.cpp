// CRenderManagerCore::GetCurrentAWindow() const -- the current absolute window: the display manager's float viewport box
// Leaf: ?GetCurrentAWindow@CRenderManagerCore@@QBEABVC2DBoxF@@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A0AAA0, `mov eax,[ecx+3A3Ch] ; add eax,204h ; ret`.
// Identity: Ego_r.pdb 0x00B74050 / FableWin.pdb 0x030EB340 ?GetCurrentAWindow@CRenderManagerCore@@QBEABVC2DBoxF@@XZ, Ego_r directly after the
//   SetAWindow pair (0x00B74030) as retail 0x00A0AAA0 after the landed SetAWindow 0x00A0AA80 (same 0x20 offset); declared in
//   engine/CRenderManagerCore.h; the landed CRenderManager2D Add* rows and 0x00BC6DB0 call it so. The manifest's
//   "CVertexBufferWin32::DoSizeof" is an identical-shape label, not this function.
// Body: DisplayManager (+0x3A3C) -> FableWin.pdb ?GetViewportFloat@CDisplayManager@@QBEABVC2DBoxF@@XZ (inline, ViewportBoxFloat +0x204).
#define FABLE_RETAIL_CRENDERMANAGERCORE
#define FABLE_RETAIL_CDISPLAYMANAGER_GETVIEWPORTFLOAT
#define FABLE_RETAIL_C2DBOXF
#include "fable_compiler.h"
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/C2DBoxF.h"
#include "engine/CRenderManagerCore.h"
#include "engine/Direct3D9Types.h"
#include "engine/CDisplayManager.h"

const C2DBoxF& CRenderManagerCore::GetCurrentAWindow() const
{
    return DisplayManager->GetViewportFloat();
}
