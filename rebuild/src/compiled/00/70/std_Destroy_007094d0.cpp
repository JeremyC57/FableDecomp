// std::_Destroy(SValueVector* first, SValueVector* last) -- destroy a range of 12-byte vectors of plain words (free each block), from vector reassignment
// Leaf: ?_Destroy@std@@YIXPAU?$CRetailVector@K@FableDestroy_007094d0@@0@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007094D0, length 41 bytes, plain `ret`.
// Element: a 12-byte STLport vector whose own elements are trivially destructible: destroying it only frees
// a non-null block. The vector's value type is not established (caller 0x00709660, manifest "CFeatDef_VectorReassign",
// a vector<vector<...> >::operator=): descriptive names.
// Callees: free @ 0x00BFEA14.
// Shape: STLport 4.x `_Destroy(first, last)` emitted out of line from a /Gr TU (`__fastcall`: first in ecx, last in
// edx, plain `ret`: no tag argument): `__destroy(first, last, value_type)` -> `__destroy_aux(first, last,
// __false_type())` inlined, with the element's implicit destructor inlined in the loop.
// Label correction: the catalog's `_Uninit_copy<_Vector_const_iterator<_Vector_val<unsigned int,allocator<unsigned int> > >,
// unsigned int*,allocator<unsigned int> >` is a Dinkumware name propagated over all 40 rows of this chunk; retail's STL
// is STLport 4.x and the plain `ret` (two register arguments, no destination), the element stride and the inline frees name this body.
extern "C" void __cdecl free(void* p);                  // 0x00BFEA14
struct __false_type { __false_type() { } };
namespace FableDestroy_007094d0 {
template <class T>
inline void DestroyRange(T* first, T* last)             // STLport _Destroy(first, last) -> __destroy_aux, inlined
{
    for (; first != last; ++first)
        first->~T();
}
template <class T>
struct CRetailVector {                                  // STLport vector<T>: 12 bytes (start, finish, end of storage)
    T* _M_start;                                        // +0
    T* _M_finish;                                       // +4
    T* _M_end_of_storage;                               // +8
    ~CRetailVector()
    {
        DestroyRange(_M_start, _M_finish);
        if (_M_start)
            free(_M_start);                             // _Vector_base: deallocate through the malloc allocator
    }
};
typedef CRetailVector<unsigned long> SValueVector;
}
namespace std {
template <class T> inline void _Destroy(T* p) { p->~T(); }
template <class T> inline void __destroy_aux(T* first, T* last, const __false_type&)
{
    for (; first != last; ++first)
        _Destroy(&*first);
}
void __fastcall _Destroy(FableDestroy_007094d0::SValueVector* first, FableDestroy_007094d0::SValueVector* last)
{
    __destroy_aux(first, last, __false_type());
}
}