// NUISystem::CComponent::IsZoomIndependent() const -- a component ignores its parent's zoom exactly when it is independent (virtual)
// Leaf: ?IsZoomIndependent@CComponent@NUISystem@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0052F1B0, 8 bytes: `mov eax,[ecx] ; jmp [eax+194h]` (tail call of virtual slot 101), thiscall.
// Identity (two sources): slot 103 of CComponent's primary vtable 0x0124608C (rtti-vtables.tsv; the catalog's "vfunc_103"), which
//   engine/NUISystem_CComponent.h names IsZoomIndependent (slot 101 IsIndependent, 102 IsPositionIndependent, landed 0x0052F1A0 with
//   the same bytes); Ego_r.pdb emits ?IsZoomIndependent@CComponent@NUISystem@@UBE_NXZ right behind IsPositionIndependent.
// Extent: the catalog row (553 bytes) ran on through 21 CSprite / CComponent virtuals (0x0052F1C0..0x0052F3C0); see SPLITS.tsv.
// Callees: virtual NUISystem::CComponent::IsIndependent() const (slot 101).
#include "engine/NUISystem_CComponent.h"
namespace NUISystem {
bool CComponent::IsZoomIndependent() const
{
    return IsIndependent();
}
}