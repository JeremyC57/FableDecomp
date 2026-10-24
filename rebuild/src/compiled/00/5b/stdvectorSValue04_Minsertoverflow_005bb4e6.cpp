// std::vector<SValue04>::_M_insert_overflow(SValue04* position, const SValue04& x, const __false_type&, size_t fill_len, bool atend) -- STLport vector growth for a 4-byte class-type value: allocate old + max(old, fill_len) slots, copy the head, construct fill_len copies of x, copy the tail unless appending, free the old block and adopt the new one
// Leaf: ?_M_insert_overflow@
// Retail: Fable.exe @ 0x005BB4E6, 180 bytes, `ret 14h` (thiscall; position, x, the __false_type reference, fill_len, atend).
// Only caller: the vector's push_back @ 0x005BBA94 (`_M_insert_overflow(_M_finish, x, __false_type(), 1, true)`).
// Callees: malloc @ 0x00BFEA0E, free @ 0x00BFEA14 (both `__cdecl`), STLport __uninitialized_copy(first, last, result,
// const __false_type&) @ 0x005BA146 (landed; `__fastcall`, called twice).
// Body: STLport 4.x _vector.c _M_insert_overflow (the __false_type overload):
//     old = size(); len = old + (max)(old, fill_len); new_start = allocate(len);
//     new_finish = __uninitialized_copy(_M_start, pos, new_start, __false_type());
//     if (fill_len == 1) { _Construct(new_finish, x); ++new_finish; }
//     else new_finish = __uninitialized_fill_n(new_finish, fill_len, x, __false_type());
//     if (!atend) new_finish = __uninitialized_copy(pos, _M_finish, new_finish, __false_type());
//     _M_clear(); _M_set(new_start, new_finish, new_start + len);
// Shape: frame-pointer size TU (`optimize("y",off)` + `("s",on)`, `leave`); `(max)` through references (`lea ecx,[ebp+14h] ;
// jb ; lea ecx,[ebp-4]`) so old size lives in the frame word that allocate's result then reuses (`and [ebp-4],0` for zero);
// the __false_type temporaries sit in the high byte of the atend slot (`lea eax,[ebp+1Bh]`); fill_n, _Construct, _M_clear
// (trivial _Destroy + `if (p) free(p)`) and _M_set are inline. Model follows the landed vector<CCategory> twin 0x005C2C65.
// Element: a 4-byte class-type value (STLport's traits send class types down the __false_type path); type not established.
#pragma optimize("y", off)
#pragma optimize("s", on)
extern "C" void* __cdecl malloc(unsigned int size);       // 0x00BFEA0E
extern "C" void __cdecl free(void* p);                    // 0x00BFEA14
inline void* operator new(unsigned int, void* where) { return where; }
struct __false_type { __false_type() { } };               // user ctor: the tag temporaries take no stores
struct SValue04 {                                         // 4-byte class-type value
    unsigned long m_value;
};
// The callee's real body (landed 0x005BA146), kept out of line as in retail: with only a declaration VC7.1 must assume the
// tag reference escapes and gives the first __false_type temporary its own frame byte (`lea eax,[ebp-1]`, 181 v 180); seeing
// the body it homes the temporary in the dead high byte of the atend slot like retail.
__declspec(noinline) SValue04* __fastcall __uninitialized_copy(const SValue04* first, const SValue04* last, SValue04* result,
                                                               const __false_type&)               // 0x005BA146
{
    SValue04* cur = result;
    for (; first != last; ++first, ++cur)
        new ((void*)cur) SValue04(*first);
    return cur;
}
template <class T> __forceinline const T& Max(const T& a, const T& b) { return a < b ? b : a; }   // STLport (max)
template <class T> __forceinline T* __uninitialized_fill_n(T* first, unsigned long n, const T& x, const __false_type&)
{
    T* cur = first;
    for (; n > 0; --n, ++cur)
        new (cur) T(x);
    return cur;
}
struct SValue04Vector {                                   // STLport vector<SValue04>: start, finish, end of storage
    SValue04* _M_start;
    SValue04* _M_finish;
    SValue04* _M_end_of_storage;
    __forceinline unsigned long size() const { return _M_finish - _M_start; }
    static __forceinline SValue04* allocate(unsigned long n) { return n != 0 ? (SValue04*)malloc(n * sizeof(SValue04)) : 0; }
    __forceinline void _M_clear()
    {
        if (_M_start)
            free(_M_start);
    }
    __forceinline void _M_set(SValue04* start, SValue04* finish, SValue04* endOfStorage)
    {
        _M_start = start;
        _M_finish = finish;
        _M_end_of_storage = endOfStorage;
    }
    void _M_insert_overflow(SValue04* position, const SValue04& x, const __false_type&, unsigned long fill_len, bool atend);
};
void SValue04Vector::_M_insert_overflow(SValue04* position, const SValue04& x, const __false_type&, unsigned long fill_len, bool atend)
{
    const unsigned long old_size = size();
    const unsigned long len = old_size + Max(old_size, fill_len);
    SValue04* new_start = allocate(len);
    SValue04* new_finish = __uninitialized_copy(_M_start, position, new_start, __false_type());
    if (fill_len == 1) {
        new (new_finish) SValue04(x);
        ++new_finish;
    } else {
        new_finish = __uninitialized_fill_n(new_finish, fill_len, x, __false_type());
    }
    if (!atend)
        new_finish = __uninitialized_copy(position, _M_finish, new_finish, __false_type());
    _M_clear();
    _M_set(new_start, new_finish, new_start + len);
}