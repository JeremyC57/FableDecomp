// std::__uninitialized_copy(SCameraTagEntry const* first, SCameraTagEntry const* last, SCameraTagEntry* result, std::__false_type const&) -- STLport 4.6 _uninitialized.h: copy-construct [first, last) into raw storage at result (placement new per element), return the end
// Leaf: ??$__uninitialized_copy@PBUSCameraTagEntry@@PAU1@@std@@YIPAUSCameraTagEntry@@PBU1@0PAU1@ABU__false_type@0@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x0040FC50, 74 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 8`.
// Identity: caller 00411b90 source (stdvector_operator_00411b90.cpp) names SCameraTagEntry; binder ??$__uninitialized_copy@PBUSCameraTagEntry@@PAU1@@std@@YIPAUSCameraTagEntry@@PBU1@0PAU1@ABU__false_type@0@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over SCameraTagEntry.
#include "stlport/fable_containers.h"
struct SCameraTagEntry {                   // 28 bytes (0x1c), dword members
    long m_00;
    long m_04;
    long m_08;
    long m_0c;
    long m_10;
    long m_14;
    long m_18;
    SCameraTagEntry(const SCameraTagEntry& o) : m_00(o.m_00), m_04(o.m_04), m_08(o.m_08), m_0c(o.m_0c), m_10(o.m_10), m_14(o.m_14), m_18(o.m_18) { }
};
template SCameraTagEntry* std::__uninitialized_copy<SCameraTagEntry const*,SCameraTagEntry* >(SCameraTagEntry const*, SCameraTagEntry const*, SCameraTagEntry*, std::__false_type const&);
