// std::vector<CAppearanceModifierGraphics::CEntry>::_M_insert_overflow(pointer __position, const T& __x, const __false_type&, size_type __fill_len, bool __atend) -- STLport 4.6 grow-on-insert for a class element: allocate size + max(size, fill_len) records, copy-construct the head, the fill_len copies of x (one plain construct when fill_len is 1) and, unless inserting at the end, the tail; free the old block and adopt the new one
// Leaf: ?_M_insert_overflow@?$vector@UCEntry@CAppearanceModifierGraphics@@V?$allocator@UCEntry@CAppearanceModifierGraphics@@@std@@@std@@IAEXPAUCEntry@CAppearanceModifierGraphics@@ABU34@ABU__false_type@2@I_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004701CE, 250 bytes, thiscall `ret 14h`, size-optimised TU (ebp frame, odd entry).
// Identity (proven): STLport 4.6 _vector.h _M_insert_overflow(..., const __false_type&, ...) verbatim for the 24-byte CAppearanceModifierGraphics::CEntry record (`push 6 ; pop ecx ; rep movsd`);
//   the only caller is 0x0047005B (vector<CAppearanceModifierGraphics::CEntry>::_M_fill_insert), whose overflow arm passes (position, x, tag, n) with atend
//   defaulted; `idiv` by sizeof(T) for size(), the `(max)` const-reference select (`lea eax,[ebp+14h]` / `lea eax,[ebp-8]`),
//   malloc 0x00BFEA0E of len*sizeof(T) behind the n != 0 test, the fill_len == 1 special case, the !atend tail copy, and
//   _M_clear (trivial destructor: just `if (start) free` 0x00BFEA14) then _M_set.
// Label: the catalog name is behavioural; STLport protects _M_insert_overflow (IAE) and the vector carries its allocator.
// Shape: every STLport helper is plain inline in the library; VC7.1 keeps the loops out of line unless forced, so the loop helpers
//   (__uninitialized_copy, __uninitialized_fill_n) and _Construct / (max) are __forceinline here.
#pragma optimize("y", off)
#pragma optimize("s", on)
#include <stddef.h>
extern "C" void* __cdecl malloc(size_t size);           // 0x00BFEA0E
extern "C" void __cdecl free(void* block);              // 0x00BFEA14
inline void* operator new(size_t, void* p) { return p; }
class CAppearanceModifierGraphics {                     // Ego_r.pdb, 16 bytes (CArray<CEntry> Graphics)
public:
    struct CEntry { unsigned long Word0, Word1, Word2, Word3, Word4, Word5; };   // 24 bytes (the stride); member names not in Ego_r
};

namespace std {
struct __false_type { };
template <class _Tp> __forceinline const _Tp& (max)(const _Tp& __a, const _Tp& __b) { return __a < __b ? __b : __a; }
template <class _T1, class _T2> __forceinline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _InputIter, class _ForwardIter>
__forceinline _ForwardIter __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __false_type&)
{
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}
template <class _ForwardIter, class _Size, class _Tp>
__forceinline _ForwardIter __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, const __false_type&)
{
    _ForwardIter __cur = __first;
    for (; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
}
template <class _Tp> class allocator {
public:
    __forceinline _Tp* allocate(size_t __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, size_t) { if (__p) free(__p); }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp* pointer;
    typedef size_t size_type;
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
protected:
    void _M_clear() { this->_M_end_of_storage.deallocate(this->_M_start, this->_M_end_of_storage._M_data - this->_M_start); }
    void _M_set(pointer __s, pointer __f, pointer __e)
    {
        this->_M_start = __s;
        this->_M_finish = __f;
        this->_M_end_of_storage._M_data = __e;
    }
    void _M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&, size_type __fill_len, bool __atend = false);
};
template <class _Tp, class _Alloc>
void vector<_Tp, _Alloc>::_M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&, size_type __fill_len, bool __atend)
{
    const size_type __old_size = size();
    const size_type __len = __old_size + (max)(__old_size, __fill_len);
    pointer __new_start = this->_M_end_of_storage.allocate(__len);
    pointer __new_finish = __new_start;
    __new_finish = __uninitialized_copy(this->_M_start, __position, __new_start, __false_type());
    if (__fill_len == 1) {
        _Construct(__new_finish, __x);
        ++__new_finish;
    }
    else
        __new_finish = __uninitialized_fill_n(__new_finish, __fill_len, __x, __false_type());
    if (!__atend)
        __new_finish = __uninitialized_copy(__position, this->_M_finish, __new_finish, __false_type());
    _M_clear();
    _M_set(__new_start, __new_finish, __new_start + __len);
}
typedef CAppearanceModifierGraphics::CEntry _Elem;
template void vector<_Elem>::_M_insert_overflow(_Elem*, const _Elem&, const __false_type&, size_t, bool);
}