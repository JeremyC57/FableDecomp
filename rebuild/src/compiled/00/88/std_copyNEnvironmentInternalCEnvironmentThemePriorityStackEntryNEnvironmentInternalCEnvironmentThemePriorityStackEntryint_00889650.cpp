// std::__copy(NEnvironmentInternal::CEnvironmentThemePriorityStackEntry* first, NEnvironmentInternal::CEnvironmentThemePriorityStackEntry* last, NEnvironmentInternal::CEnvironmentThemePriorityStackEntry* result, std::random_access_iterator_tag const&, int*) -- STLport 4.6 _algobase.h: random-access counted forward copy `for (n = last - first; n > 0; --n) *result++ = *first++`, return the advanced result
// Leaf: ??$__copy@PAVCEnvironmentThemePriorityStackEntry@NEnvironmentInternal@@PAV12@H@std@@YIPAVCEnvironmentThemePriorityStackEntry@NEnvironmentInternal@@PAV12@00ABUrandom_access_iterator_tag@0@PAH@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00889650, 111 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 0xc`.
// Identity: caller 00889ab0 std::vector<NEnvironmentInternal::CEnvironmentThemePriorityStackEntry, std::allocator<NEnvironmentInternal::CEnvironmentThemePriorityStackEntry>>::erase; callee 00888ee0 std::vector::operator=; binder ??$__copy@PAVCEnvironmentThemePriorityStackEntry@NEnvironmentInternal@@PAV12@H@std@@YIPAVCEnvironmentThemePriorityStackEntry@NEnvironmentInternal@@PAV12@00ABUrandom_access_iterator_tag@0@PAH@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over NEnvironmentInternal::CEnvironmentThemePriorityStackEntry.
#include "stlport/fable_containers.h"
struct SMember0C_8 {                             // 12 bytes, operator= @ 0x00888EE0
    long m_words[3];
    SMember0C_8& operator=(const SMember0C_8& other);
};
namespace NEnvironmentInternal {
class CEnvironmentThemePriorityStackEntry {
public:                           // 48 bytes
    long          m_key;                         // +0x00
    bool          m_flag;                        // +0x04
    SMember0C_8   m_list;                        // +0x08
    long          m_value14;                     // +0x14
    long          m_value18;                     // +0x18
    bool          m_enabled;                     // +0x1C
    double        m_time;                        // +0x20
    long          m_value28;                     // +0x28
};
}
template NEnvironmentInternal::CEnvironmentThemePriorityStackEntry* std::__copy<NEnvironmentInternal::CEnvironmentThemePriorityStackEntry*,NEnvironmentInternal::CEnvironmentThemePriorityStackEntry*,int >(NEnvironmentInternal::CEnvironmentThemePriorityStackEntry*, NEnvironmentInternal::CEnvironmentThemePriorityStackEntry*, NEnvironmentInternal::CEnvironmentThemePriorityStackEntry*, std::random_access_iterator_tag const&, int*);
