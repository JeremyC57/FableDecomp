// std::__uninitialized_fill_n(NInventory::CItem* first, unsigned int n, NInventory::CItem const& x, std::__false_type const&) -- STLport 4.6 _uninitialized.h: copy-construct n copies of x into raw storage at first, return the end
// Leaf: ??$__uninitialized_fill_n@PAVCItem@NInventory@@IV12@@std@@YIPAVCItem@NInventory@@PAV12@IABV12@ABU__false_type@0@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x005C0E1B, 38 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 8`.
// Identity: caller 005c276c std::vector<NInventory::CItem, std::allocator<NInventory::CItem>>::_M_insert_overflow; caller 005c2831 std::vector<NInventory::CItem>::_M_fill_insert; callee 005c08e6 NInventory::CItem::CItem; binder ??$__uninitialized_fill_n@PAVCItem@NInventory@@IV12@@std@@YIPAVCItem@NInventory@@PAV12@IABV12@ABU__false_type@0@@Z
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
template NInventory::CItem* std::__uninitialized_fill_n<NInventory::CItem*,unsigned int,NInventory::CItem >(NInventory::CItem*, unsigned int, NInventory::CItem const&, std::__false_type const&);
