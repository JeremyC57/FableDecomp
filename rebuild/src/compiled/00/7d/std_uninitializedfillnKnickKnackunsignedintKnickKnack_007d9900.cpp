// std::__uninitialized_fill_n(KnickKnack* first, unsigned int n, KnickKnack const& x, std::__false_type const&) -- STLport 4.6 _uninitialized.h: copy-construct n copies of x into raw storage at first, return the end
// Leaf: ??$__uninitialized_fill_n@PAUKnickKnack@@IU1@@std@@YIPAUKnickKnack@@PAU1@IABU1@ABU__false_type@0@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x007D9900, 52 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 8`.
// Identity: caller 007d9c90 std::vector<KnickKnack, std::allocator<KnickKnack>>::_M_insert_overflow; binder ??$__uninitialized_fill_n@PAUKnickKnack@@IU1@@std@@YIPAUKnickKnack@@PAU1@IABU1@ABU__false_type@0@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over KnickKnack.
#include "stlport/fable_containers.h"
struct KnickKnack {
    long m_0; long m_4; long m_8; long m_c;
    KnickKnack(const KnickKnack& other) : m_0(other.m_0), m_4(other.m_4), m_8(other.m_8), m_c(other.m_c) { }   // memberwise user copy: one register per dword
};
template KnickKnack* std::__uninitialized_fill_n<KnickKnack*,unsigned int,KnickKnack >(KnickKnack*, unsigned int, KnickKnack const&, std::__false_type const&);
