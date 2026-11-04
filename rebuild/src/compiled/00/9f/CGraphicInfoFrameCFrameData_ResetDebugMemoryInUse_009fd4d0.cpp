// CGraphicInfoFrame::CFrameData::ResetDebugMemoryInUse() -- forget which mipmap levels were touched (debug memory accounting)
// Leaf: ?ResetDebugMemoryInUse@CFrameData@CGraphicInfoFrame@@UAEXXZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x009FD4D0, 5 bytes: `mov byte [ecx+3Ah],7Fh ; ret`.
// Identity (two sources): (1) it is slot 9 of vtable 0x0129C89C (RTTI .?AVCFrameData@CGraphicInfoFrame@@ : CManagedTextureOwner :
//   CResource), and CResource slot 9 is ResetDebugMemoryInUse (engine/CResource.h, base body 0x009FBF10); Ego_r.pdb has the override
//   ?ResetDebugMemoryInUse@CFrameData@CGraphicInfoFrame@@UAEXXZ; (2) Ego_r.pdb CFrameData puts the signed-char TouchedMipmapLevel at
//   +0x3A (between LoadingMipmapLevel +0x39 and TouchedFrame +0x3C), the byte the body sets to SCHAR_MAX (no level touched).
//   The catalog "CGraphicInfoFrame::CFrameData::...::vfunc_9" is the slot label.
// Callees: none.
// Header: engine/CGraphicInfoFrame.h (new), engine/CManagedTextureOwner.h (new), engine/CResource.h (FABLE_RETAIL_DEF_CLASS_BASE),
//   engine/CIVCountedPointeeBase.h.
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CGraphicInfoFrame.h"
void CGraphicInfoFrame::CFrameData::ResetDebugMemoryInUse()
{
    TouchedMipmapLevel = 127;
}