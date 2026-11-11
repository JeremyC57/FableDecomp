// std::__uninitialized_copy(std::list<C2DLineF,std::allocator<C2DLineF> >* first, std::list<C2DLineF,std::allocator<C2DLineF> >* last, std::list<C2DLineF,std::allocator<C2DLineF> >* result, std::__false_type const&) -- STLport 4.6 _uninitialized.h: copy-construct [first, last) into raw storage at result (placement new per element), return the end
// Leaf: ??$__uninitialized_copy@PAV?$list@VC2DLineF@@V?$allocator@VC2DLineF@@@std@@@std@@PAV12@@std@@YIPAV?$list@VC2DLineF@@V?$allocator@VC2DLineF@@@std@@@0@PAV10@00ABU__false_type@0@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A831D0, 87 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 8`.
// Identity: caller 00a84270 std::vector<std::list<C2DLineF, std::allocator<C2DLineF>>, std::allocator<std::list<C2DLineF, std::allocator<C2DLineF>>>>::_M_insert_overflow; caller 00a84270 std::vector<std::list<C2DLineF, std::allocator<C2DLineF>>, std::allocator<std::list<C2DLineF, std::allocator<C2DLineF>>>>::_M_insert_overflow; caller 00a84690 std::vector<std::list<C2DLineF, std::allocator<C2DLineF>>, std::allocator<std::list<C2DLineF, std::allocator<C2DLineF>>>>::_M_fill_insert; binder ??$__uninitialized_copy@PAV?$list@VC2DLineF@@V?$allocator@VC2DLineF@@@std@@@std@@PAV12@@std@@YIPAV?$list@VC2DLineF@@V?$allocator@VC2DLineF@@@std@@@0@PAV10@00ABU__false_type@0@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over std::list<C2DLineF,std::allocator<C2DLineF>>.
#include "stlport/fable_containers.h"
#include "engine/C2DLineF.h"
extern "C" void* __cdecl malloc(unsigned int size);         // 0x00BFEA0E

template <class T> struct allocator { allocator() { } };
template <class T> struct _STLP_alloc_proxy : allocator<T> {
    T* _M_data;
    _STLP_alloc_proxy(const allocator<T>& a, T* p) : allocator<T>(a), _M_data(p) { }
};
template std::list<C2DLineF,std::allocator<C2DLineF> >* std::__uninitialized_copy<std::list<C2DLineF,std::allocator<C2DLineF> >*,std::list<C2DLineF,std::allocator<C2DLineF> >* >(std::list<C2DLineF,std::allocator<C2DLineF> >*, std::list<C2DLineF,std::allocator<C2DLineF> >*, std::list<C2DLineF,std::allocator<C2DLineF> >*, std::__false_type const&);
