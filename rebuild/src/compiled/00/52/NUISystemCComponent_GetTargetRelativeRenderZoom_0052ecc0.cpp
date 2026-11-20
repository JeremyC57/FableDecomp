// NUISystem::CComponent::GetTargetRelativeRenderZoom() const -- the component's target zoom scaled by the UI scale (no parent, or
//   zoom-independent), converted to back-buffer units when it uses relative zoom, then multiplied by the position parent's (else the
//   parent's) own relative render zoom unless it is zoom-independent
// Leaf: ?GetTargetRelativeRenderZoom@CComponent@NUISystem@@UBE?AVC2DVector@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0052ECC0; CComponent vtable 0x0124608C slot 123 (CSprite's slot 123 holds the same address).
// Identity: Ego_r.pdb S_PUB32 + FableWin.pdb ?GetTargetRelativeRenderZoom@CComponent@NUISystem@@UBE?AVC2DVector@@XZ (engine/
//   NUISystem_CComponent.h slot 123); the catalog's NUISystem::CSprite::vfunc_123 placeholder. No landed row references 0x0052ECC0.
// Callees: CManager::GetInstance 0x0041E5F2 and CManager::GetUIScale 0x0041CF47 (?GetUIScale@CManager@NUISystem@@QBE?AVC2DVector@@XZ,
//   hidden C2DVector result), CUIScreenUnitConverter::ConvertVector 0x0052E580 (static __fastcall, by-value C2DVector), the virtuals
//   IsZoomIndependent (slot 103), IsUsingRelativeZoom (slot 116) and this one (slot 123) on the parent. Members TargetZoom +0x64,
//   PParent +0xC8, PPositionParent +0x118 (engine/NUISystem_CComponent.h).
// Naming (NF156, 2026-11-20, q4778 / q4945): moved off the row-local Vec / CManager / CUIScreenUnitConverter / CComponent models
//   (?GetUIScale@CManager@NUISystem@@QBE?AUVec@@XZ, the self leaf ..UBE?AUVec@@XZ) onto engine/NUISystem_CComponent.h,
//   engine/NUISystem_CManager.h (FABLE_RETAIL_CMANAGER) and engine/NUISystem_CUIScreenUnitConverter.h (FABLE_RETAIL_CUISCREENUNITCONVERTER_VECTOR).
#define FABLE_RETAIL_CMANAGER
#define FABLE_RETAIL_CUISCREENUNITCONVERTER_VECTOR
#include "engine/C2DVector.h"
#include "engine/NUISystem_CComponent.h"
#include "engine/NUISystem_CManager.h"
#include "engine/NUISystem_CUIScreenUnitConverter.h"
namespace NUISystem {
C2DVector CComponent::GetTargetRelativeRenderZoom() const
{
    C2DVector size = TargetZoom;
    if (!PParent || IsZoomIndependent()) {
        size.X *= CManager::GetInstance()->GetUIScale().X;
        size.Y *= CManager::GetInstance()->GetUIScale().Y;
    }
    if (IsUsingRelativeZoom())
        size = CUIScreenUnitConverter::ConvertVector(size);
    CComponent* parent = PPositionParent ? PPositionParent : PParent;
    if (parent && !IsZoomIndependent()) {
        if (!IsZoomIndependent()) {
            size.X *= parent->GetTargetRelativeRenderZoom().X;
            size.Y *= parent->GetTargetRelativeRenderZoom().Y;
        }
        return C2DVector(size.X, size.Y);
    }
    return size;
}
}
