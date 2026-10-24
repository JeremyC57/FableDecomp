// std::__destroy_aux(SRecord30Vector* first, SRecord30Vector* last, const __false_type&) -- destroy a range of vectors of 48-byte records (each record frees its inner block at +8)
// Leaf: ?__destroy_aux@std@@YIXPAU?$CRetailVector@USRecord30@FableDestroy_006b6f50@@@FableDestroy_006b6f50@@0ABU__false_type@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006B6F50, length 84 bytes, `ret 4`.
// Element: a 12-byte STLport vector of 48-byte records, each holding a vector of plain values at +8 (freed per
// record) among trivially destructible words. The record is not established (callers 0x006B6FE6 / 0x006B7036, small
// vector helpers after it, 0x006B7130 and 0x006B7380, manifest "CGameWorld_Destructor"): descriptive names.
// Callees: free @ 0x00BFEA14.
// Shape: STLport 4.x `__destroy_aux(first, last, const __false_type&)` from a /Gr TU (`__fastcall`: first in ecx, last
// in edx, the empty tag reference on the stack, `ret 4`): `for (; first != last; ++first) _Destroy(&*first);` with the
// element's implicit destructor inlined.
// Label correction: the catalog's `_Uninit_copy<_Vector_const_iterator<_Vector_val<unsigned int,allocator<unsigned int> > >,
// unsigned int*,allocator<unsigned int> >` is a Dinkumware name propagated over all 40 rows of this chunk; retail's STL
// is STLport 4.x and the `ret 4` (no destination), the element stride and the inline destructor calls name this body.
extern "C" void __cdecl free(void* p);                  // 0x00BFEA14
struct __false_type { __false_type() { } };
namespace FableDestroy_006b6f50 {
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
struct SRecord30 {                                      // 48 bytes
    long                        m_word0;                // +0x00
    long                        m_word4;                // +0x04
    CRetailVector<unsigned long> m_values;              // +0x08
    long                        m_rest[7];              // +0x14 .. +0x30
};
typedef CRetailVector<SRecord30> SRecord30Vector;
}
namespace std {
template <class T> inline void _Destroy(T* p) { p->~T(); }
void __fastcall __destroy_aux(FableDestroy_006b6f50::SRecord30Vector* first, FableDestroy_006b6f50::SRecord30Vector* last, const __false_type&)
{
    for (; first != last; ++first)
        _Destroy(&*first);
}
}