// std::__copy_backward_ptrs(_Tp* first, _Tp* last, _Tp* result, const __false_type&) -- STLport __copy_backward_ptrs(first, last, result, __false_type): forward to the out-of-line __copy_backward with a random_access_iterator_tag and a null Distance* (std::set<unsigned long>)
// Leaf: ??$__copy_backward_ptrs@V?$set@KU?$less@K@std@@V?$allocator@K@2@@std@@@std@@YIPAV?$set@KU?$less@K@std@@V?$allocator@K@2@@0@PAV10@00ABU__false_type@0@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00518f60, 20 bytes, __fastcall (first in ecx, last in edx, result on the stack), `ret 8`.
// Identity (probable): its callee 0x00517430 is the 12-byte __copy_backward that vector::_M_fill_insert 0x0051C920 uses, whose element assignment 0x00512CE0 is the set<unsigned long> _Rb_tree::operator= (landed std::fill<std::set<unsigned long>*> 0x00517060 calls the same operator=); no caller (unreferenced COMDAT).
// Callee: std::__copy_backward<std::set<unsigned long>*, std::set<unsigned long>*, int> 0x00517430 (landed under the stand-in CopyBackward_CElement12_00512ce0_00517430; no binding).
// Shape (family L002): `push 0 ; lea eax,[esp+8] ; push eax ; push result ; call` -- the empty tag temporary is homed on the
//   result argument's own slot (its address is pushed, nothing is stored), ecx/edx pass through untouched.
#include <stddef.h>
namespace std {
template <class _Tp> class allocator { public: allocator() { } };
struct __false_type { __false_type() { } };
struct random_access_iterator_tag { random_access_iterator_tag() { } };
}
namespace std {
template <class _Tp> struct less { };
template <class _Key, class _Compare = less<_Key>, class _Alloc = allocator<_Key> >
class set {                                                // STLport 4.6: one 12-byte _Rb_tree
public:
    void* _M_t[3];
    set& operator=(const set& __x);                        // inline _M_t = __x._M_t (_Rb_tree::operator= 0x00512CE0)
};
}
typedef std::set<unsigned long> _Elem;
namespace std {
template <class _BidirectionalIter1, class _BidirectionalIter2, class _Distance>       // 0x00517430
_BidirectionalIter2 __fastcall __copy_backward(_BidirectionalIter1 __first, _BidirectionalIter1 __last,
                                              _BidirectionalIter2 __result, const random_access_iterator_tag&, _Distance*);
template <class _Tp>
inline _Tp* __fastcall __copy_backward_ptrs(_Tp* __first, _Tp* __last, _Tp* __result, const __false_type&)
{
    return __copy_backward(__first, __last, __result, random_access_iterator_tag(), (ptrdiff_t*)0);
}
}
template _Elem* __fastcall std::__copy_backward_ptrs(_Elem*, _Elem*, _Elem*, const std::__false_type&);