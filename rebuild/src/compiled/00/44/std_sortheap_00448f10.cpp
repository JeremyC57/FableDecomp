// std::sort_heap(pair<EGameAction, CCountedPointer<const CInputTypeBase> >* first, pair* last, CKeyPairCompareLess comp) -- STLport sort_heap: pop the root to the back until one element remains
// Flags: /Gr
// Leaf: ??$sort_heap@PAU?$pair@W4EGameAction@@V?$CCountedPointer@$$CBVCInputTypeBase@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00448F10, 122 bytes, /Gr __fastcall (first ecx, last edx, the empty comparator on the
//   stack), `ret 4`.
// Identity (proven): reloc_bindings names this ??$sort_heap@PAU?$pair@W4EGameAction@@V?$CCountedPointer@$$CBVCInputTypeBase@@@@@std@@V?$CKeyPairCompareLess@...;
//   its caller is 0x00449270 (the partial_sort front end); its callee is the bound __pop_heap 0x00448B30 of the same element.
// Callees: __pop_heap(first, last, result, val, comp, int*) 0x00448B30 (__fastcall; the 12-byte value copy-constructed into its
//   argument area with `inc [Info]`).
// Shape (family K088, representative 0x00448F10): STLport 4.6 `while (__last - __first > 1) pop_heap(__first, __last--, __comp);`
//   with pop_heap / __pop_heap_aux inline and __pop_heap out of line (the landed 0x007590C0 text); the /12 distance is the
//   0x2AAAAAAB reciprocal.
void __cdecl operator delete(void* p);                  // 0x00BFE9BC
class CCPPointerInfo {                                   // CCountedPointer's shared block
public:
    unsigned long RefCount;                             // +0
    void (__fastcall* DeleteFunc)(void* data);          // +4, Data in ecx
    void* Data;                                         // +8
};
template <class T> class CCountedPointer {              // agent950's landed model (0x00A99110: ReleaseData / ShareData)
public:
    T*              Data;                               // +0
    CCPPointerInfo* Info;                               // +4
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info) { if (Info) ++Info->RefCount; }
    ~CCountedPointer() { ReleaseData(); Data = 0; Info = 0; }
    void ReleaseData()
    {
        if (Info) {
            if (--Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
    }
};
enum EGameAction { EGameAction_Force32 = 0x7fffffff };   // enumerators not needed
class CInputTypeBase;
namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;                                          // +0x0
    _T2 second;                                         // +0x4
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) { }   // STLport 4.6 _pair.h
};
}
template <class K, class V> class CKeyPairCompareLess {  // Ego_r.pdb: orders vector-map entries by key
public:
    bool operator()(const std::pair<K, V>& a, const std::pair<K, V>& b) const { return a.first < b.first; }
};
namespace std {
template <class _RandomAccessIterator, class _Tp, class _Compare, class _Distance>
void __fastcall __pop_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _RandomAccessIterator __result,
                           _Tp __val, _Compare __comp, _Distance*);       // 0x00448B30
template <class _RandomAccessIterator, class _Tp, class _Compare>
inline void __pop_heap_aux(_RandomAccessIterator __first, _RandomAccessIterator __last, _Tp*, _Compare __comp)
{
    __pop_heap(__first, __last - 1, __last - 1, _Tp(*(__last - 1)), __comp, (int*)0);
}
template <class _Tp> inline _Tp* __value_type(const _Tp*) { return (_Tp*)0; }
template <class _RandomAccessIterator, class _Compare>
inline void pop_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp)
{
    __pop_heap_aux(__first, __last, __value_type(__first), __comp);
}
template <class _RandomAccessIterator, class _Compare>
void __fastcall sort_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp)
{
    while (__last - __first > 1)
        pop_heap(__first, __last--, __comp);
}
typedef pair<EGameAction, CCountedPointer<const CInputTypeBase> > _Elem;
typedef CKeyPairCompareLess<EGameAction, CCountedPointer<const CInputTypeBase> > _Cmp;
template void __fastcall sort_heap<_Elem*, _Cmp>(_Elem*, _Elem*, _Cmp);
} // namespace std