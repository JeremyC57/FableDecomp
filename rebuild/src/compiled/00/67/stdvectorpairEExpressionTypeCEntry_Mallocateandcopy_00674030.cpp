// std::vector<pair<EExpressionType, CEntry> >::_M_allocate_and_copy(unsigned int n, const value_type* first, const value_type* last) -- STLport: malloc n 12-byte expression entries (none for zero), copy-construct [first, last) into them, return the new block
// Leaf: ??$_M_allocate_and_copy@
//
// Retail 0x00674030, 86 bytes (+ int3 padding to 96), thiscall `ret 0Ch` (n, first, last; `this` unused).
// Callee: malloc @ 0x00BFEA0E (STLport's __malloc_alloc behind allocator<T>::allocate).
// Caller: 0x00674452 in vector<pair<EExpressionType, CExpressionSet::CEntry> >::operator= 0x00674400, the entry vector of
//   CExpressionSet::Expressions (landed CCreatureDef::operator= 0x00675C00 names 0x00674400 as that vector's operator=)
//   -- CVectorMap<EExpressionType, CExpressionSet::CEntry> keeps its entries as pair<key, value>.
// Layout: Ego_r.pdb CExpressionSet::CEntry = { EExpressionType Type; long AnimIndex; } (8 bytes), so the pair is 12 bytes
//   (`lea eax,[eax+eax*2] ; shl eax,2`).
// Shape: byte-identical to the texture-morph instance 0x00673F90: STLport 4.x _vector.h member template
//   `_M_allocate_and_copy(n, first, last)`: `result = allocate(n)` (n ? malloc(n * 12) : 0) then
//   `__uninitialized_copy(first, last, result, __false_type())` with _Construct's placement null test and the pair's copy
//   inline (three dword moves); the copy helper needs __forceinline (plain inline stays a call); speed TU.
// Label: catalog "Vector_AllocateTriplets" (descriptive).
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E
inline void* operator new(unsigned int, void* where) { return where; }

enum EExpressionType { EXPRESSION_TYPE_NONE = -1, EXPRESSION_TYPE_0 = 0 };

class CExpressionSet {
public:
    class CEntry {                                          // 8 bytes
    public:
        EExpressionType Type;                               // +0x00
        long AnimIndex;                                     // +0x04
    };
};

namespace std {
struct __false_type { };

template <class _T1, class _T2> class pair {
public:
    _T1 first;
    _T2 second;
    pair(const pair& p) : first(p.first), second(p.second) {}
};

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _InputIter, class _ForwardIter>
__forceinline _ForwardIter __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __false_type&)
{
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}

template <class _Tp> class allocator {
public:
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
};

template <class _Tp, class _Alloc = allocator<_Tp> > class vector {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    struct : _Alloc { _Tp* _M_data; } _M_end_of_storage;
    template <class _ForwardIterator>
    _Tp* _M_allocate_and_copy(unsigned int __n, _ForwardIterator __first, _ForwardIterator __last)
    {
        _Tp* __result = _M_end_of_storage.allocate(__n);
        __uninitialized_copy(__first, __last, __result, __false_type());
        return __result;
    }
};
}

typedef std::pair<EExpressionType, CExpressionSet::CEntry> CExpressionEntryPair;
template CExpressionEntryPair* std::vector<CExpressionEntryPair>::_M_allocate_and_copy<const CExpressionEntryPair*>(
    unsigned int, const CExpressionEntryPair*, const CExpressionEntryPair*);