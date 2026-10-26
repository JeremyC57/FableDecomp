// std::vector<CRGBColour>::_M_insert_overflow(CRGBColour* position, const CRGBColour& x, const __false_type&, unsigned int fillLength, bool atEnd) -- STLport non-POD growth path: allocate old+max(old,n) colours, copy-construct the head, construct n copies of x, copy-construct the tail unless inserting at the end, free the old block
// Leaf: _M_insert_overflow
// Retail: Fable.exe (TLC, VC7.1) @ 0x0046916B, 210 bytes, `ret 14h` (__fastcall: position, &x, the __false_type tag, n, atEnd).
// Callees: malloc @ 0x00BFEA0E, free @ 0x00BFEA14, the out-of-line `__fastcall _Construct<CRGBColour>(p, value)` @ 0x0045BB8F
//   (placement copy, slot in ecx, value in edx). Caller: vector<CRGBColour>::_M_fill_insert @ 0x00469012.
// Shape: STLport 4.x `_M_insert_overflow(..., const __false_type&, ...)`: `len = old + max(old, n)` (const-reference `max`,
//   `lea ecx,[ebp+14h] ; jb ; lea ecx,[ebp-4]`), `allocate(len)` = `len ? malloc(len * 4) : 0`, `__uninitialized_copy`
//   (`for (; first != last; ++first, ++cur) _Construct(cur, *first)`) for the head, the `n == 1` special case then
//   `__uninitialized_fill_n` for the insert, the tail copy unless atEnd, `_M_clear()` (CRGBColour's destructor is trivial, so
//   only `if (_M_start) free(_M_start)`) and `_M_set`. The tail copy's cursor lives in the dead atEnd slot ([ebp+18h]).
//   Size TU with a frame pointer: `optimize("y", off)` + `("s", on)`, every helper `__forceinline`; exceptions are off
//   (the `_STLP_TRY/_STLP_UNWIND` guards compile to nothing).
// Model: the three STLport vector words; CRGBColour is the 4-byte D3DCOLOR record (B, G, R, A) with a user copy constructor
//   (engine/CRGBColour.h is a data-only header; the copy itself lives in 0x0045BB8F, so only the size matters here).
#pragma optimize("y", off)
#pragma optimize("s", on)
extern "C" void* __cdecl malloc(unsigned int bytes);                                   // 0x00BFEA0E
extern "C" void __cdecl free(void* block);                                            // 0x00BFEA14

class CRGBColour { public:
    unsigned char B, G, R, A;
    CRGBColour(const CRGBColour& o) { R = o.R; G = o.G; B = o.B; A = o.A; }
};

namespace std {
struct __false_type { __false_type() {} };
template <class T> __forceinline const T& (max)(const T& a, const T& b) { return a < b ? b : a; }
void __fastcall _Construct(CRGBColour* p, const CRGBColour& value);                    // 0x0045BB8F
template <class T> __forceinline T* __uninitialized_copy(T* first, T* last, T* result, const __false_type&)
{
    T* cur = result;
    for (; first != last; ++first, ++cur)
        _Construct(cur, *first);
    return cur;
}
template <class T> __forceinline T* __uninitialized_fill_n(T* first, unsigned int n, const T& x, const __false_type&)
{
    T* cur = first;
    for (; n > 0; --n, ++cur)
        _Construct(cur, x);
    return cur;
}
template <class T> class vector {
public:
    T* _M_start;                    // +0x00
    T* _M_finish;                   // +0x04
    T* _M_end_of_storage;           // +0x08
    unsigned int size() const { return _M_finish - _M_start; }
    static __forceinline T* allocate(unsigned int n) { return n != 0 ? (T*)malloc(n * sizeof(T)) : 0; }
    __forceinline void _M_clear() { if (_M_start != 0) free(_M_start); }
    __forceinline void _M_set(T* s, T* f, T* e) { _M_start = s; _M_finish = f; _M_end_of_storage = e; }
    void _M_insert_overflow(T* position, const T& x, const __false_type&, unsigned int fillLength, bool atEnd);
};
template <class T> void vector<T>::_M_insert_overflow(T* position, const T& x, const __false_type&, unsigned int fillLength, bool atEnd)
{
    const unsigned int oldSize = size();
    const unsigned int length = oldSize + (max)(oldSize, fillLength);
    T* newStart = allocate(length);
    T* newFinish = __uninitialized_copy(_M_start, position, newStart, __false_type());
    if (fillLength == 1) {
        _Construct(newFinish, x);
        ++newFinish;
    }
    else
        newFinish = __uninitialized_fill_n(newFinish, fillLength, x, __false_type());
    if (!atEnd)
        newFinish = __uninitialized_copy(position, _M_finish, newFinish, __false_type());
    _M_clear();
    _M_set(newStart, newFinish, newStart + length);
}
template void vector<CRGBColour>::_M_insert_overflow(CRGBColour*, const CRGBColour&, const __false_type&, unsigned int, bool);
}