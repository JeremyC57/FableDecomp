// NUISystem::CComponent::IsPositionIndependent() const -- a component is position-independent exactly when it is independent (virtual)
// Leaf: ?IsPositionIndependent@CComponent@NUISystem@@UBE_NXZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x0052F1A0, 8 bytes: `mov eax,[ecx] ; jmp [eax+194h]` (tail call of virtual slot 101).
// Identity (two sources): slot 102 of CComponent's primary vtable 0x0124608C (rtti-vtables.tsv; the catalog's "vfunc_102"), which
//   engine/NUISystem_CComponent.h names IsPositionIndependent (slot 101 IsIndependent, 103 IsZoomIndependent); Ego_r.pdb emits
//   ?IsIndependent@CComponent@NUISystem@@UBE_NXZ 0x005A8D50 then ?IsPositionIndependent@..UBE_NXZ 0x005A8D60, as retail 0x0052F190
//   (the IsIndependent flag read, +0x12C bit 6) then 0x0052F1A0; the body calls slot 101 (+0x194) = IsIndependent.
// Callees: virtual NUISystem::CComponent::IsIndependent() const (slot 101).
// Header: engine/NUISystem_CComponent.h.
#include "engine/NUISystem_CComponent.h"
namespace NUISystem {
bool CComponent::IsPositionIndependent() const
{
    return IsIndependent();
}
}