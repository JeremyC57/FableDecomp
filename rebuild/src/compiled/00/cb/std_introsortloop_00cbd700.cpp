// std::__introsort_loop(CActiveScriptPair* first, CActiveScriptPair* last, CActiveScriptPair*, int depth_limit, CActiveScriptLess comp) -- STLport 4.6 introsort loop over CScriptBase::EntityScripts' (UID, active entity script) pairs: median-of-three quicksort partitions until the range is at most 16 elements, falling back to partial_sort (heap sort) when the depth limit runs out
// Leaf: ??$__introsort_loop@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CBD700, 969 bytes, __fastcall (first ecx, last edx; the value-type tag, depth
//   limit and the empty 4-byte comparator on the stack, `ret 0Ch`).
// Identity: STLport 4.6 _algo.c `__introsort_loop` (the pair element is 16 bytes: unsigned __int64 key compared ja/jb
//   high word first, then a CCountedPointer whose count block is AddRef'd on copy). Its callers are itself
//   (0x00CBD9DB) and the sort<> at 0x00CBDEF0 (`__lg(n) * 2` depth, then the threshold-16 final insertion sort), called
//   from CScriptBase::OnScriptedEntityActivated 0x00CB8960 on the Ego_r.pdb member CScriptBase::EntityScripts =
//   CVectorMap<unsigned __int64, CCountedPointer<CActiveEntityScriptBase>, CKeyPairCompareLess<...> > (sorted after an
//   insert). The manifest label CItem_QuickSort is propagated and wrong.
// Callees: __unguarded_partition 0x00CBB710 (pivot by value, built in its argument slot), __median 0x00CB9DA0 (landed),
//   the pair copy constructor 0x00CB9C20, __partial_sort 0x00CBCFC0 (through the inline partial_sort), itself.
// Shape: the STLport text as written under `#pragma inline_recursion(on)`: VC7.1 expands the recursive call five levels
//   deep inside the out-of-line body (the sixth calls 0x00CBD700). Only the outermost level inlines __median and the
//   pivot copy; the nested levels call them (the inline budget), exactly as retail.
#pragma inline_recursion(on)
void __cdecl operator delete(void* block);                 // 0x00BFE9BC
class CCPPointerInfo { public:
    long  RefCount;
    void  (__fastcall* DeleteFunc)(void* data);
    void* Data;
};
template <class T> class CCountedPointer {
public:
    T*              Data;
    CCPPointerInfo* Info;
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info)
    {
        if (Info)
            ++Info->RefCount;
    }
    ~CCountedPointer()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                operator delete(Info);
            }
        }
        Data = 0;
        Info = 0;
    }
};
class CActiveEntityScriptBase;
namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};
}
template <class K, class V> class CKeyPairCompareLess {
public:
    bool operator()(const std::pair<K, V>& a, const std::pair<K, V>& b) const { return a.first < b.first; }
};
typedef std::pair<unsigned __int64, CCountedPointer<CActiveEntityScriptBase> > CActiveScriptPair;
typedef CKeyPairCompareLess<unsigned __int64, CCountedPointer<CActiveEntityScriptBase> > CActiveScriptLess;
namespace std {
template <class _Iterator> struct iterator_traits;
template <class _Tp> struct iterator_traits<_Tp*> {
    typedef _Tp value_type;
};
const int __stl_threshold = 16;
template <class _Tp, class _Compare>
inline const _Tp& __fastcall __median(const _Tp& __a, const _Tp& __b, const _Tp& __c, _Compare __comp)
{
    if (__comp(__a, __b))
        if (__comp(__b, __c))
            return __b;
        else if (__comp(__a, __c))
            return __c;
        else
            return __a;
    else if (__comp(__a, __c))
        return __a;
    else if (__comp(__b, __c))
        return __c;
    else
        return __b;
}
template <class _RandomAccessIter, class _Tp, class _Compare>
_RandomAccessIter __fastcall __unguarded_partition(_RandomAccessIter __first, _RandomAccessIter __last, _Tp __pivot, _Compare __comp);
template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __partial_sort(_RandomAccessIter __first, _RandomAccessIter __middle, _RandomAccessIter __last, _Tp*, _Compare __comp);
template <class _RandomAccessIter, class _Compare>
inline void __fastcall partial_sort(_RandomAccessIter __first, _RandomAccessIter __middle, _RandomAccessIter __last, _Compare __comp)
{
    __partial_sort(__first, __middle, __last, (typename iterator_traits<_RandomAccessIter>::value_type*)0, __comp);   // _STLP_VALUE_TYPE
}
template <class _RandomAccessIter, class _Tp, class _Size, class _Compare>
void __fastcall __introsort_loop(_RandomAccessIter __first, _RandomAccessIter __last, _Tp*, _Size __depth_limit, _Compare __comp)
{
    while (__last - __first > __stl_threshold) {
        if (__depth_limit == 0) {
            partial_sort(__first, __last, __last, __comp);
            return;
        }
        --__depth_limit;
        _RandomAccessIter __cut =
            __unguarded_partition(__first, __last,
                                  _Tp(__median(*__first,
                                               *(__first + (__last - __first) / 2),
                                               *(__last - 1), __comp)),
                                  __comp);
        __introsort_loop(__cut, __last, (_Tp*)0, __depth_limit, __comp);
        __last = __cut;
    }
}
template void __fastcall __introsort_loop<CActiveScriptPair*, CActiveScriptPair, int, CActiveScriptLess>(CActiveScriptPair*, CActiveScriptPair*, CActiveScriptPair*, int, CActiveScriptLess);
}