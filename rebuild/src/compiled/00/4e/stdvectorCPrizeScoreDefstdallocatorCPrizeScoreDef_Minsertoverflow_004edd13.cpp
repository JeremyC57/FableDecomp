// std::vector<CPrizeScoreDef,std::allocator<CPrizeScoreDef> >::_M_insert_overflow(CPrizeScoreDef* position, const CPrizeScoreDef& x, const __false_type&, unsigned int fillLength, bool atEnd) -- STLport growth path for the prize-score defs: allocate size + max(size, n) slots, copy-construct the prefix, the n copies of x and (unless at the end) the suffix, then release the old block
// Leaf: _M_insert_overflow
// Retail: Fable.exe (TLC, VC7.1) @ 0x004EDD13, 230 bytes, `ret 14h` (thiscall: position, x, the tag, fillLength, atEnd).
// Identity: single caller 0x004EDC8F, the overflow branch of vector<CPrizeScoreDef>::_M_fill_insert (it pushes atEnd 0,
//   the count, a stosb-zeroed __false_type and x/position). Stride 0x30 = CPrizeScoreDef (Ego_r.pdb: CDefClassBase
//   0x25 bytes padded to 0x28, float Score +0x28, long Mult +0x2C; vtable 0x012437D4). Catalog label
//   "Vector_ReallocateInsert_0x30" is descriptive.
// Callees: malloc @ 0x00BFEA0E; STLport `_Construct<CPrizeScoreDef, CPrizeScoreDef>` @ 0x004EAC30 (`__fastcall`, the
//   placement null test plus the inlined CPrizeScoreDef copy constructor; manifest "_Cons_val<..CShoveHaPennyRegionDef..>"
//   is propagated); vector::_M_clear @ 0x004EABC2 (manifest "CActiveFile::OnReadFinished" is propagated: __destroy_aux
//   of [start, finish) then free).
// Model: STLport 4.x `_M_insert_overflow(.., const __false_type&, ..)` as in the landed 0x00472D79, but this TU keeps
//   `_Construct` out of line (a /Gr algorithm instance) while __uninitialized_copy / __uninitialized_fill_n, allocate
//   and max inline. sizeof 0x30 is not a power of two, so size() is an `idiv` in the size TU.
// Shape: size TU with a frame pointer (`optimize("y", off)` + `("s", on)`; `and dword ptr [ebp-8],0` for the null block).
#pragma optimize("y", off)
#pragma optimize("s", on)

#include "engine/CDefClassBase.h"

extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E

class CPrizeScoreDef : public CDefClassBase {      // Ego_r.pdb, retail sizeof 0x30
public:
    unsigned char _pad_0x25[3];                     // +0x25 (alignment of the derived members)
    float Score;                                    // +0x28
    long Mult;                                      // +0x2C
};

namespace std {
struct __false_type { __false_type() { } };

template <class T> __forceinline const T& (max)(const T& a, const T& b) { return a < b ? b : a; }

template <class T1, class T2> void __fastcall _Construct(T1* p, const T2& value);   // 0x004EAC30

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

template <class T> class vector {
public:
    T* _M_start;                                    // +0x00
    T* _M_finish;                                   // +0x04
    T* _M_end_of_storage;                           // +0x08
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    static __forceinline T* allocate(unsigned int n) { return n != 0 ? static_cast<T*>(malloc(n * sizeof(T))) : 0; }
    void _M_clear();                                // 0x004EABC2
    __forceinline void _M_set(T* s, T* f, T* e) { _M_start = s; _M_finish = f; _M_end_of_storage = e; }
    void _M_insert_overflow(T* position, const T& x, const __false_type&, unsigned int fillLength, bool atEnd);
};

template <class T>
void vector<T>::_M_insert_overflow(T* position, const T& x, const __false_type&, unsigned int fillLength, bool atEnd)
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

template void vector<CPrizeScoreDef>::_M_insert_overflow(CPrizeScoreDef* position, const CPrizeScoreDef& x,
    const __false_type&, unsigned int fillLength, bool atEnd);
}