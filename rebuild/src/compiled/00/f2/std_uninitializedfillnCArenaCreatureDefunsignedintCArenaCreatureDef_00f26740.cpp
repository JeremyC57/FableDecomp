// std::uninitialized_fill_n(CArenaCreatureDef* first, unsigned int n, CArenaCreatureDef const& x) -- STLport 4.6 _uninitialized.h: copy-construct n copies of x into raw storage at first (the __false_type helper inlined), return the end
// Leaf: ??$uninitialized_fill_n@PAVCArenaCreatureDef@@IV1@@std@@YIPAVCArenaCreatureDef@@PAV1@IABV1@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00F26740, 42 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 4`.
// Identity: caller 00f26570 std::vector<CArenaCreatureDef, std::allocator<CArenaCreatureDef>>::_M_fill_insert; callee 00f25dd0 CVtable_012fb358::CVtable_012fb358; binder ??$uninitialized_fill_n@PAVCArenaCreatureDef@@IV1@@std@@YIPAVCArenaCreatureDef@@PAV1@IABV1@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over CArenaCreatureDef.
#include "stlport/fable_containers.h"
class CArenaCreatureDef {
public:
    unsigned char _body[0x38];
    CArenaCreatureDef(const CArenaCreatureDef& other);           // 0x00F25DD0
};
template CArenaCreatureDef* std::uninitialized_fill_n<CArenaCreatureDef*,unsigned int,CArenaCreatureDef >(CArenaCreatureDef*, unsigned int, CArenaCreatureDef const&);
