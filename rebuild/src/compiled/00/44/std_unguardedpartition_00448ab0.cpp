// std::__unguarded_partition(pair* first, pair* last, pair pivot, CKeyPairCompareLess comp) -- (pair<EGameAction, CCountedPointer<const CInputTypeBase> >) Hoare partition around the pivot's key: swap out-of-place pairs inward and return the split point
// Leaf: ??$__unguarded_partition@PAU?$pair@W4EGameAction@@V?$CCountedPointer@$$CBVCInputTypeBase@@@@@std@@U12@V?$CKeyPairCompareLess@W4EGameAction@@V?$CCountedPointer@$$CBVCInputTypeBase@@@@@@@std@@YIPAU?$pair@W4EGameAction@@V?$CCountedPointer@$$CBVCInputTypeBase@@@@@0@PAU10@0U10@V?$CKeyPairCompareLess@W4EGameAction@@V?$CCountedPointer@$$CBVCInputTypeBase@@@@@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00448AB0, 103 bytes, `__fastcall` (first in ecx, last in edx; the 12-byte pivot and the empty comparator on the stack, `ret 10h`), returns first in eax.
// Identity (proven): std::__unguarded_partition<pair<EGameAction, CCountedPointer<const CInputTypeBase> >*, pair<...>, CKeyPairCompareLess<...> > (STLport 4.6 _algo.c; the PDB builds are Dinkumware); evidence: its only caller is the introsort loop 0x004494D0 (J055), called by sort 0x004495D0, the sort of CGamePlayerInterface::Controls (sorted at this+0x7A8 by CGamePlayerInterface::SetControlScheme 0x00446EF0): CVectorMap<EGameAction, CCountedPointer<const CInputTypeBase> > (J050 proves the element type from that sort's owner); it swaps through iter_swap 0x00448450 and drops its by-value pivot's counted pointer at the end
// Label: the manifest's "Heap_SiftDown_ContainmentVolume" is invented (family J052: five byte-identical bodies, one per element type; retail has no identical-code folding).
// Callees: std::iter_swap<pair*, pair*>(a, b) 0x00448450 (__fastcall, out of line), the pivot's count-block DeleteFunc and operator delete 0x00BFE9BC (~pivot).
// Body: `for (;;) { while (__comp(*__first, __pivot)) ++__first; --__last; while (__comp(__pivot, *__last)) --__last;
//   if (!(__first < __last)) return __first; iter_swap(__first, __last); ++__first; }`, the signed key compare only, then ~pivot.
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
template <class _ForwardIter1, class _ForwardIter2>
void __fastcall iter_swap(_ForwardIter1 __i1, _ForwardIter2 __i2);   // 0x00448450
template <class _RandomAccessIter, class _Tp, class _Compare>
_RandomAccessIter __fastcall __unguarded_partition(_RandomAccessIter __first, _RandomAccessIter __last, _Tp __pivot, _Compare __comp)
{
    for (;;) {
        while (__comp(*__first, __pivot))
            ++__first;
        --__last;
        while (__comp(__pivot, *__last))
            --__last;
        if (!(__first < __last))
            return __first;
        iter_swap(__first, __last);
        ++__first;
    }
}
typedef pair<EGameAction, CCountedPointer<const CInputTypeBase> > _Entry;
template _Entry* __fastcall __unguarded_partition<_Entry*, _Entry, CKeyPairCompareLess<EGameAction, CCountedPointer<const CInputTypeBase> > >(_Entry*, _Entry*, _Entry, CKeyPairCompareLess<EGameAction, CCountedPointer<const CInputTypeBase> >);
}   // namespace std