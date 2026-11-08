#ifndef FABLE_STLPORT_ALGORITHM_H
#define FABLE_STLPORT_ALGORITHM_H
// STLport 4.x (SGI) for_each + mem_fun_ref, as the landed NPlayerGui::CDrawMiniMap::ClearPrimitives 0x00644660 and
// CDrawQuickAccessIcons rows model them (chunk 867 lifts that TU-local model into a shared header): mem_fun_ref_t is the 4-byte
// functor holding the pointer-to-member; for_each is the /Gr `__fastcall` COMDAT at 0x0043F360 (hidden return pointer in ecx, the
// functor result returned by value: `lea ecx,<dead slot> ; mov edx,begin ; push pfn ; push end ; call`). Declaration only for
// for_each: the retail instantiation is a separate COMDAT, callers only relocate to it.
#include "rebuild_abi.h"
namespace std {
template <class R, class T> class mem_fun_ref_t {
public:
    explicit mem_fun_ref_t(R (T::*pfn)()) : m_pfn(pfn) {}
    R operator()(T& object) const { return (object.*m_pfn)(); }
private:
    R (T::*m_pfn)();
};
template <class R, class T> inline mem_fun_ref_t<R, T> mem_fun_ref(R (T::*pfn)())
{
    return mem_fun_ref_t<R, T>(pfn);
}
template <class It, class F> F FABLE_FASTCALL for_each(It first, It last, F f);   // 0x0043F360 (SGI, /Gr)
}
#endif
