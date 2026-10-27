// std::vector<CObjectFamilyEntry>::_M_insert_overflow(CObjectFamilyEntry* position, const CObjectFamilyEntry& x, const __false_type&, unsigned int fillLength, bool atEnd) -- STLport growth path: allocate old+max(old,n), copy the head, construct n copies of x, copy the tail unless inserting at the end, free the old block
// Leaf: ?_M_insert_overflow@?$vector@VCObjectFamilyEntry@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x0046C5F8, 232 bytes, thiscall `ret 14h`; size TU with a frame pointer.
// Identity (proven): reloc_bindings names it ?_M_insert_overflow@?$vector@VCObjectFamilyEntry@@...; its only caller is that vector's
//   _M_fill_insert 0x0046C4A4 (landed).
// Callees: malloc 0x00BFEA0E, free 0x00BFEA14.
// Shape (family K100, representative 0x00469FC8): the landed vector<CSoundBankEntry>::_M_insert_overflow 0x0046AECC
//   verbatim with an 8-byte element (two dword moves per null-checked placement copy).
#pragma optimize("y", off)
#pragma optimize("s", on)
extern "C" void* __cdecl malloc(unsigned int bytes);                                   // 0x00BFEA0E
extern "C" void __cdecl free(void* block);                                            // 0x00BFEA14

inline void* operator new(unsigned int, void* where) { return where; }

class CObjectFamilyEntry {                               // Ego_r.pdb 8 bytes
public:
    long Object;
    long Probability;
};

namespace std {
struct __false_type { };
template <class T> __forceinline const T& (max)(const T& a, const T& b) { return a < b ? b : a; }
template <class _T1, class _T2> __forceinline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
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
template void vector<CObjectFamilyEntry >::_M_insert_overflow(CObjectFamilyEntry*, const CObjectFamilyEntry&, const __false_type&, unsigned int, bool);
}