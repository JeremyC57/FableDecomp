// CDisplayManager::GetRenderTargetDimensions() -- return a copy of the render target's pixel extents
// Leaf: ?GetRenderTargetDimensions@CDisplayManager@@QAE?BVC2DExtentsI@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x009BEDC0, 24 bytes, thiscall with the hidden C2DExtentsI result pointer, `ret 4`.
//   Ego_r.pdb 0x00B242A0 / FableWin.pdb 0x03022ED0 ?GetRenderTargetDimensions@CDisplayManager@@QAE?BVC2DExtentsI@@XZ (public, non-const,
//   returns const C2DExtentsI by value). Reads RenderTargetDimensions (+0x194 X, +0x198 Y; Y loaded first) into the result.
// NF139 (2026-11-20, q4291): moved off the CDisplayManagerRenderTargetDimensionsView stand-in (const, local C2DExtentsI model) onto
//   engine/CDisplayManager.h + engine/C2DExtentsI.h, the spelling 54 landed callers already bind.
#define FABLE_RETAIL_CDISPLAYMANAGER
#define FABLE_RETAIL_SCOPED_PTR
#include "fable_compiler.h"
#include "engine/Win32Types.h"
#include "engine/Direct3D9Types.h"
#include "engine/C2DExtentsI.h"
#include "engine/CDisplayManager.h"

const C2DExtentsI CDisplayManager::GetRenderTargetDimensions()
{
    C2DExtentsI dimensions;
    dimensions.Y = RenderTargetDimensions.Y;
    dimensions.X = RenderTargetDimensions.X;
    return dimensions;
}
