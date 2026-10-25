// std::vector<CShopItemDef>::_M_insert_overflow(CShopItemDef* position, const CShopItemDef& x, const __false_type&, size_t fill_len, bool atend) -- STLport regrowth of the 72-byte shop item def vector: allocate size + max(size, n), copy the head, construct the n new elements, copy the tail unless appending, release the old block
// Leaf: _M_insert_overflow
// Retail: Fable.exe (TLC, VC7.1) @ 0x004ED353, 228 bytes, `ret 14h` (thiscall).
// Caller: 0x004ED2E6 (the vector's push_back / insert overflow path).
// Label correction: the catalog's "Vector_ReallocateInsert_0x48" is descriptive; the element is CShopItemDef (0x48) by the
//   callees: _Construct<CShopItemDef> @ 0x004E9C39 (landed) and the vector's _M_clear @ 0x004E9BB7 (shared with operator=
//   0x004E9AD3, this chunk).
// Callees: CRT malloc @ 0x00BFEA0E, _Construct(CShopItemDef*, const CShopItemDef&) @ 0x004E9C39 (/Gr __fastcall: slot in ecx,
//   source in edx), _M_clear() @ 0x004E9BB7 (catalogued CActiveFile::OnReadFinished, propagated).
// Shape: STLport 4.x `_M_insert_overflow` (non-POD) verbatim with every helper inlined but _Construct: `(max)(old_size,
//   fill_len)` through const references (`lea ecx` of the winner, old_size in a frame word), `allocate(len)` = `len ? malloc(len
//   * 72) : 0` (the result shares old_size's word), the two `__uninitialized_copy(..., __false_type())` loops and the
//   `__uninitialized_fill_n` loop (unsigned count, `jbe`) as _Construct calls, the single-element case one _Construct, then
//   `_M_clear(); _M_set(new_start, new_finish, new_start + len)`. In the size TU the helpers must be `__forceinline`.
//   Frame-pointer size TU: `optimize("y", off)` + `("s", on)`.
// Model: the element is engine/CShopItemDef.h (only its 0x48 size matters: every element operation is out of line).
#pragma optimize("y", off)
#pragma optimize("s", on)
#include "engine/CShopItemDef.h"

extern "C" void* __cdecl malloc(size_t size);   // 0x00BFEA0E

namespace std {
struct __false_type { __false_type() { } };
template <class T> void __fastcall _Construct(T* p, const T& value);   // 0x004E9C39
template <class T> __forceinline const T& (max)(const T& a, const T& b) { return a < b ? b : a; }
template <class InputIter, class ForwardIter>
__forceinline ForwardIter __uninitialized_copy(InputIter first, InputIter last, ForwardIter result, const __false_type&)
{
    ForwardIter cur = result;
    for (; first != last; ++first, ++cur)
        _Construct(&*cur, *first);
    return cur;
}
template <class ForwardIter, class Size, class T>
__forceinline ForwardIter __uninitialized_fill_n(ForwardIter first, Size n, const T& x, const __false_type&)
{
    ForwardIter cur = first;
    for (; n > 0; --n, ++cur)
        _Construct(&*cur, x);
    return cur;
}
template <class T> struct allocator {           // STLport allocator<T>: empty
    allocator() { }
    __forceinline T* allocate(size_t n) { return n != 0 ? static_cast<T*>(malloc(n * sizeof(T))) : 0; }
};
template <class T> struct _STLP_alloc_proxy : allocator<T> {
    T* _M_data;
};
template <class T> class vector {
public:
    T*                   _M_start;              // +0
    T*                   _M_finish;             // +4
    _STLP_alloc_proxy<T> _M_end_of_storage;     // +8
    size_t size() const { return size_t(_M_finish - _M_start); }
    void _M_clear();                            // 0x004E9BB7
    __forceinline void _M_set(T* s, T* f, T* e) { _M_start = s; _M_finish = f; _M_end_of_storage._M_data = e; }
    void _M_insert_overflow(T* position, const T& x, const __false_type&, size_t fill_len, bool atend);
};
template <class T>
void vector<T>::_M_insert_overflow(T* position, const T& x, const __false_type&, size_t fill_len, bool atend)
{
    const size_t old_size = size();
    const size_t len = old_size + (max)(old_size, fill_len);
    T* new_start = _M_end_of_storage.allocate(len);
    T* new_finish = new_start;
    new_finish = __uninitialized_copy(_M_start, position, new_start, __false_type());
    if (fill_len == 1) {
        _Construct(new_finish, x);
        ++new_finish;
    }
    else
        new_finish = __uninitialized_fill_n(new_finish, fill_len, x, __false_type());
    if (!atend)
        new_finish = __uninitialized_copy(position, _M_finish, new_finish, __false_type());
    _M_clear();
    _M_set(new_start, new_finish, new_start + len);
}
template void vector<CShopItemDef>::_M_insert_overflow(CShopItemDef*, const CShopItemDef&, const __false_type&, size_t, bool);
}