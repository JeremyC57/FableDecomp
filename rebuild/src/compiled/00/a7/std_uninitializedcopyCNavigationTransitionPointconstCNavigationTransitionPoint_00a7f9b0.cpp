// std::__uninitialized_copy(CNavigationTransitionPoint const* first, CNavigationTransitionPoint const* last, CNavigationTransitionPoint* result, std::__false_type const&) -- STLport 4.6 _uninitialized.h: copy-construct [first, last) into raw storage at result (placement new per element), return the end
// Leaf: ??$__uninitialized_copy@PBVCNavigationTransitionPoint@@PAV1@@std@@YIPAVCNavigationTransitionPoint@@PBV1@0PAV1@ABU__false_type@0@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A7F9B0, 56 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 8`.
// Identity: caller 00a81690 std::vector<CNavigationTransitionPoint, std::allocator<CNavigationTransitionPoint>>::operator=; caller 00a81690 std::vector<CNavigationTransitionPoint, std::allocator<CNavigationTransitionPoint>>::operator=; binder ??$__uninitialized_copy@PBVCNavigationTransitionPoint@@PAV1@@std@@YIPAVCNavigationTransitionPoint@@PBV1@0PAV1@ABU__false_type@0@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over CNavigationTransitionPoint.
#include "stlport/fable_containers.h"
class CNavigationTransitionPoint {
public:                        // dword members: the copy is memberwise (a byte array copies through pointer registers)
    long m_00;
    long m_04;
    long m_08;
    long m_0c;
    CNavigationTransitionPoint(const CNavigationTransitionPoint& o) : m_00(o.m_00), m_04(o.m_04), m_08(o.m_08), m_0c(o.m_0c) { }   // memberwise copy: direct dword moves, not a block copy through pointer registers
};
template CNavigationTransitionPoint* std::__uninitialized_copy<CNavigationTransitionPoint const*,CNavigationTransitionPoint* >(CNavigationTransitionPoint const*, CNavigationTransitionPoint const*, CNavigationTransitionPoint*, std::__false_type const&);
