// NUISystem::CUIScreenUnitConverter::ResetDimensions() -- refresh the cached back-buffer size from the display manager's render target
// Leaf: ?ResetDimensions@CUIScreenUnitConverter@NUISystem@@SIXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0040F0E0, static __fastcall, no arguments: GFGetSystemManager 0x009A4EC0, then
//   CDisplayManager::GetRenderTargetDimensions 0x009BEDC0 (hidden C2DExtentsI result) on PDisplayManager (+0x60), the two longs
//   converted to float into BackBufferWidth 0x013B876C / BackBufferHeight 0x013B8770.
// Identity: Ego_r.pdb 0x0040C590 + FableWin.pdb 0x01884760 ?ResetDimensions@CUIScreenUnitConverter@NUISystem@@SIXXZ (the spelling the
//   landed caller 0x0042EC7C CNewFrontendGameComponent::Run binds); the statics are the private ?BackBufferWidth / ?BackBufferHeight
//   @CUIScreenUnitConverter@NUISystem@@0MA (NF114: Ego_d.pdb PUB32 + FableWin.pdb LF_STMEMBER; landed 0x004299A8 binds them).
// Naming (NF142, 2026-11-20, q4291): moved off the Device::GetDims / Dims / local CSystemManager stand-ins and the free
//   @NUISystem_CUIScreenUnitConverter_ResetDimensions@0 / ?g_dimW / ?g_dimH spellings onto engine/NUISystem_CUIScreenUnitConverter.h,
//   engine/CSystemManager.h, engine/CDisplayManager.h and engine/C2DExtentsI.h (the GFABoxToVBox 0x009E2020 form).
#define FABLE_RETAIL_CDISPLAYMANAGER
#define FABLE_RETAIL_CSYSTEMMANAGER
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CUISCREENUNITCONVERTER_RESETDIMENSIONS
#define FABLE_RETAIL_CUISCREENUNITCONVERTER_ORIGINAL_DIMENSIONS
#include "fable_compiler.h"
#include "engine/Win32Types.h"
#include "engine/Direct3D9Types.h"
#include "engine/C2DExtentsI.h"
#include "engine/CDisplayManager.h"
#include "engine/CSystemManager.h"
#include "engine/NUISystem_CUIScreenUnitConverter.h"

namespace NUISystem {

void __fastcall CUIScreenUnitConverter::ResetDimensions()
{
    C2DExtentsI dimensions = GFGetSystemManager()->GetDisplayManager()->GetRenderTargetDimensions();
    BackBufferWidth = (float)dimensions.X;
    BackBufferHeight = (float)dimensions.Y;
}

}
