// std::vector<CSoundBankEntry>::_M_insert_overflow(CSoundBankEntry* position, const CSoundBankEntry& x, const __false_type&, unsigned int fillLength, bool atEnd) -- STLport growth path of the sound-bank entry vector: allocate old+max(old,n), copy the head, construct n copies of x, copy the tail unless inserting at the end, free the old block
// Leaf: ?_M_insert_overflow@?$vector@VCSoundBankEntry@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x0046AECC, 250 bytes, thiscall `ret 14h`; size TU with a frame pointer.
// Identity: the only caller is vector<CSoundBankEntry>::_M_fill_insert 0x0046AD59 (this chunk). Catalog label
//   "Vector_Reallocate" is behavioural.
// Callees: malloc 0x00BFEA0E, free 0x00BFEA14.
// Shape: STLport 4.x `_M_insert_overflow(..., const __false_type&, ...)` verbatim (the landed 0x0046916B CRGBColour
//   template): `len = old + max(old, n)` with the const-reference `max` (`lea eax,[ebp+14h] ; jb ; lea eax,[ebp-8]`),
//   the signed `idiv` element count of the size TU, `allocate(len)` = `len ? malloc(len * 20) : 0`, the inlined
//   null-checked placement copies (`rep movsd` x5), the `n == 1` special case then the inlined __uninitialized_fill_n,
//   the tail copy unless atEnd (its end pointer homed in the dead atEnd slot), `_M_clear()` (trivial destructor: only
//   `if (_M_start) free(_M_start)`) and `_M_set`.
// Model: Ego_r.pdb CSoundBankEntry (20 bytes, implicit bitwise copy); the three STLport vector words.
#pragma optimize("y", off)
#pragma optimize("s", on)
extern "C" void* __cdecl malloc(unsigned int bytes);                                   // 0x00BFEA0E
extern "C" void __cdecl free(void* block);                                            // 0x00BFEA14

inline void* operator new(unsigned int, void* where) { return where; }

class CDefString {
public:
    long Index;
    CDefString() : Index(-1) { }
};

class CSoundBankEntry {
public:
    unsigned long ID;                                    // +0x00
    CDefString Filename;                                 // +0x04
    CDefString SymbolFilename;                           // +0x08
    CDefString LipSyncBankHandle;                        // +0x0C
    bool DVD;                                            // +0x10
    CSoundBankEntry() : ID(0) { }
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
template void vector<CSoundBankEntry>::_M_insert_overflow(CSoundBankEntry*, const CSoundBankEntry&, const __false_type&, unsigned int, bool);
}