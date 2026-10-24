// std::__destroy_aux(SValueVector* first, SValueVector* last, const __false_type&) -- destroy a range of 12-byte vectors of plain words (free each block)
// Leaf: ?__destroy_aux@std@@YIXPAU?$CRetailVector@K@FableDestroy_00770670@@0ABU__false_type@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00770670, length 43 bytes, `ret 4`.
// Element: a 12-byte STLport vector of trivially destructible values: destroying it frees a non-null block. The
// value type is not established (its only caller is the inline clear at 0x007898CC, landed as
// CActiveFile::OnReadFinished with a stand-in model): descriptive names.
// Callees: free @ 0x00BFEA14.
// Shape: STLport 4.x `__destroy_aux(first, last, const __false_type&)` from a /Gr TU (`__fastcall`: first in ecx, last
// in edx, the empty tag reference on the stack, `ret 4`): `for (; first != last; ++first) _Destroy(&*first);` with the
// element's implicit destructor inlined.
// Label correction: the catalog's `_Uninit_copy<_Vector_const_iterator<_Vector_val<unsigned int,allocator<unsigned int> > >,
// unsigned int*,allocator<unsigned int> >` is a Dinkumware name propagated over all 40 rows of this chunk; retail's STL
// is STLport 4.x and the `ret 4` (no destination), the element stride and the inline destructor calls name this body.
extern "C" void __cdecl free(void* p);                  // 0x00BFEA14
struct __false_type { __false_type() { } };
namespace FableDestroy_00770670 {
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
void __fastcall __destroy_aux(FableDestroy_00770670::SValueVector* first, FableDestroy_00770670::SValueVector* last, const __false_type&)
{
    for (; first != last; ++first)
        _Destroy(&*first);
}
}