// std::__partial_sort(pair* first, pair* middle, pair* last, pair*, CKeyPairCompareLess comp) -- (pair<EGameAction, CCountedPointer<const CInputTypeBase> >) heap-select the smallest keys into [first, middle), then sort that heap
// Leaf: ??$__partial_sort@PAU?$pair@W4EGameAction@@V?$CCountedPointer@$$CBVCInputTypeBase@@@@@std@@U12@V?$CKeyPairCompareLess@W4EGameAction@@V?$CCountedPointer@$$CBVCInputTypeBase@@@@@@@std@@YIXPAU?$pair@W4EGameAction@@V?$CCountedPointer@$$CBVCInputTypeBase@@@@@0@000V?$CKeyPairCompareLess@W4EGameAction@@V?$CCountedPointer@$$CBVCInputTypeBase@@@@@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00449270, 104 bytes, `__fastcall` (first in ecx, middle in edx; last, the null value-type pointer and the empty comparator on the stack, `ret 0Ch`).
// Identity (proven): std::__partial_sort<pair<EGameAction, CCountedPointer<const CInputTypeBase> >*, pair<...>, CKeyPairCompareLess<...> > (STLport 4.6 _algo.c; the PDB builds are Dinkumware); evidence: its callers (sites 0x0044940C, 0x004495BE) are its type's introsort loop 0x004494D0 (J055: the depth-limit fallback `partial_sort(first, last, last)`) and an unreferenced out-of-line partial_sort(first, middle, last, comp) forwarder (`push 0` for the value type, then the call); the loop is called by sort 0x004495D0, the sort of CGamePlayerInterface::Controls (sorted at this+0x7A8 by CGamePlayerInterface::SetControlScheme 0x00446EF0): CVectorMap<EGameAction, CCountedPointer<const CInputTypeBase> > (J050 proves the element type from that sort's owner)
// Label: the manifest's "Heap_OrderAndSort" is invented (family J053: five byte-identical bodies, one per element type).
// Callees: std::__make_heap 0x00448BE0 (via the inline make_heap), std::__pop_heap 0x00448B30 (with a counted copy of *i), std::sort_heap 0x00448F10; all `__fastcall`, out of line.
// Body: `make_heap(__first, __middle, __comp); for (_RandomAccessIter __i = __middle; __i < __last; ++__i) if (__comp(*__i, *__first))
//   __pop_heap(__first, __middle, __i, _Tp(*__i), __comp, _STLP_DISTANCE_TYPE(__first, _RandomAccessIter)); sort_heap(__first, __middle, __comp);`
// Layout: pair<K, CCountedPointer<V> > is 12 bytes {first +0, second.Data +4, second.Info +8}; CCPPointerInfo {RefCount +0, DeleteFunc +4, Data +8}.
// Model: J050 (agent950's landed CCountedPointer, key-only CKeyPairCompareLess).
void __cdecl operator delete(void* p);                      // 0x00BFE9BC
class CCPPointerInfo {
public:
    unsigned long RefCount;                                 // +0
    void (__fastcall* DeleteFunc)(void* data);              // +4, Data in ecx
    void* Data;                                             // +8
};
template <class T> class CCountedPointer {
public:
    T*              Data;                                   // +0
    CCPPointerInfo* Info;                                   // +4
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info) { if (Info) ++Info->RefCount; }
    ~CCountedPointer() { ReleaseData(); Data = 0; Info = 0; }
    CCountedPointer& operator=(const CCountedPointer& other) { ShareData(other.Data, other.Info); return *this; }
    void ReleaseData()
    {
        if (Info) {
            if (--Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
    }
    void ShareData(T* data, CCPPointerInfo* info)
    {
        if (Info != info) {
            ReleaseData();
            Data = data;
            Info = info;
            if (info)
                ++info->RefCount;
        }
    }
};
enum EGameAction { };                                      // Ego_r.pdb (enumerators not needed)
class CInputTypeBase;
namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};
}   // namespace std
template <class K, class V> class CKeyPairCompareLess {
public:
    bool operator()(const std::pair<K, V>& a, const std::pair<K, V>& b) const { return a.first < b.first; }
    bool operator()(const std::pair<K, V>& a, const K& key) const { return a.first < key; }
};
namespace std {
template <class _Iterator> struct iterator_traits { };
template <class _Tp> struct iterator_traits<_Tp*> {
    typedef _Tp value_type;
    typedef int difference_type;
};
template <class _RandomAccessIterator, class _Compare, class _Tp, class _Distance>
void __fastcall __make_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp, _Tp*, _Distance*);   // 0x00448BE0
template <class _RandomAccessIterator, class _Compare>
inline void make_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp)
{
    __make_heap(__first, __last, __comp, (typename iterator_traits<_RandomAccessIterator>::value_type*)0,
                (typename iterator_traits<_RandomAccessIterator>::difference_type*)0);
}
template <class _RandomAccessIterator, class _Tp, class _Compare, class _Distance>
void __fastcall __pop_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _RandomAccessIterator __result,
                           _Tp __val, _Compare __comp, _Distance*);   // 0x00448B30
template <class _RandomAccessIterator, class _Compare>
void __fastcall sort_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp);   // 0x00448F10
template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __partial_sort(_RandomAccessIter __first, _RandomAccessIter __middle, _RandomAccessIter __last, _Tp*, _Compare __comp)
{
    make_heap(__first, __middle, __comp);
    for (_RandomAccessIter __i = __middle; __i < __last; ++__i)
        if (__comp(*__i, *__first))
            __pop_heap(__first, __middle, __i, _Tp(*__i), __comp, (typename iterator_traits<_RandomAccessIter>::difference_type*)0);
    sort_heap(__first, __middle, __comp);
}
typedef pair<EGameAction, CCountedPointer<const CInputTypeBase> > _Entry;
template void __fastcall __partial_sort<_Entry*, _Entry, CKeyPairCompareLess<EGameAction, CCountedPointer<const CInputTypeBase> > >(_Entry*, _Entry*, _Entry*, _Entry*, CKeyPairCompareLess<EGameAction, CCountedPointer<const CInputTypeBase> >);
}   // namespace std