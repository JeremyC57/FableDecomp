// std::__uninitialized_copy(NInventory::CItem const* first, NInventory::CItem const* last, NInventory::CItem* result, std::__false_type const&) -- STLport 4.6 _uninitialized.h: copy-construct [first, last) into raw storage at result (placement new per element), return the end
// Leaf: ??$__uninitialized_copy@PBVCItem@NInventory@@PAV12@@std@@YIPAVCItem@NInventory@@PBV12@0PAV12@ABU__false_type@0@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x005C10CC, 43 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 8`.
// Identity: caller 005c144a std::vector<NInventory::CItem, std::allocator<NInventory::CItem>>::_M_allocate_and_copy<NInventory::CItem const *>; caller 005c1f42 std::vector<NInventory::CItem>::operator=; callee 005c08e6 NInventory::CItem::CItem; binder ??$__uninitialized_copy@PBVCItem@NInventory@@PAV12@@std@@YIPAVCItem@NInventory@@PBV12@0PAV12@ABU__false_type@0@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over NInventory::CItem.
#pragma optimize("s",on)
#include "stlport/fable_containers.h"
namespace NInventory {
class CItem {
public:
    unsigned char _body[0x14];
    CItem(const CItem& other);           // 0x005C08E6
};
}
template NInventory::CItem* std::__uninitialized_copy<NInventory::CItem const*,NInventory::CItem* >(NInventory::CItem const*, NInventory::CItem const*, NInventory::CItem*, std::__false_type const&);
