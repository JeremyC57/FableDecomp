// std::__insertion_sort(_RandomAccessIter first, _RandomAccessIter last, _Compare comp) -- (pair<unsigned long, CCountedPointer<CFaction> > under CKeyPairCompareLess) insert each entry after the first into the sorted run before it
// Leaf: ??$__insertion_sort@PAU?$pair@KV?$CCountedPointer@VCFaction@@@@@std@@V?$CKeyPairCompareLess@KV?$CCountedPointer@VCFaction@@@@@@@std@@YIXPAU?$pair@KV?$CCountedPointer@VCFaction@@@@@0@0V?$CKeyPairCompareLess@KV?$CCountedPointer@VCFaction@@@@@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006BAD60, 74 bytes, /Gr `__fastcall` (first in ecx, last in edx, the empty
//   comparator on the stack), `ret 4`.
// Identity (proven): std::__insertion_sort<pair<unsigned long, CCountedPointer<CFaction> >*, CKeyPairCompareLess<unsigned long, CCountedPointer<CFaction> > >; evidence: reached from the sort 0x006BB140 whose element H047 proves (CFactionManager::Factions (+0, CFactionManager::CFactionManager(const CWorld&) 0x006B97E0); swap 0x006B8E70) by direct calls 0x006BB140 -> 0x006BAF40 -> 0x006BAD60 (sort -> __final_insertion_sort -> this); its callers 0x006BAF69, 0x006BAF87 are the __final_insertion_sort (and inlined sorts) over the same element.
// Label: the manifest's "Heap_BuildHeapPair" is propagated (family H050: six byte-identical bodies, each its own COMDAT; retail has
//   no identical-code folding, so each address is one element type).
// Callee: __linear_insert 0x006BA7B0 (out of line here: __fastcall first ecx, i edx, then the 12-byte value
//   copy-constructed in its argument slot (`inc [Info]`) and the comparator).
// Shape: STLport 4.6 _algo.c __insertion_sort (landed 0x00ADBE70 model) with __linear_insert left out of line:
//   `if (first == last) return; for (i = first + 1; i != last; ++i) __linear_insert(first, i, *i, comp);`.
class CCPPointerInfo { public:                                    // CCountedPointer's shared block
    long  RefCount;                                        // +0x0
    void  (__fastcall* DeleteFunc)(void* data);            // +0x4
    void* Data;                                            // +0x8
};
template <class T> class CCountedPointer {
public:
    T*              Data;                                  // +0x0
    CCPPointerInfo* Info;                                  // +0x4
    CCountedPointer(const CCountedPointer& o) : Data(o.Data), Info(o.Info) { if (Info) ++Info->RefCount; }
    ~CCountedPointer()
    {
        if (Info) {
            if (--Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
    }
    CCountedPointer& operator=(const CCountedPointer& o)
    {
        T* const              data = o.Data;
        CCPPointerInfo* const info = o.Info;
        if (Info != info) {
            if (Info) {
                --Info->RefCount;
                if (Info->RefCount == 0) {
                    Info->DeleteFunc(Info->Data);
                    delete Info;
                }
            }
            Data = data;
            Info = info;
            if (info)
                ++info->RefCount;
        }
        return *this;
    }
};
class CFaction;

namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;                                             // +0x0
    _T2 second;                                            // +0x4
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) { }   // STLport 4.6 _pair.h
};
}
template <class K, class V> class CKeyPairCompareLess {    // Ego_r.pdb: orders vector-map entries by key
public:
    bool operator()(const std::pair<K, V>& a, const std::pair<K, V>& b) const { return a.first < b.first; }
};
namespace std {
template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __linear_insert(_RandomAccessIter __first, _RandomAccessIter __last, _Tp __val,
                                _Compare __comp);          // 0x006BA7B0
template <class _RandomAccessIter, class _Compare>
void __fastcall __insertion_sort(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp)
{
    if (__first == __last)
        return;
    for (_RandomAccessIter __i = __first + 1; __i != __last; ++__i)
        __linear_insert(__first, __i, *__i, __comp);
}
typedef pair<unsigned long, CCountedPointer<CFaction> > _Elem;
typedef CKeyPairCompareLess<unsigned long, CCountedPointer<CFaction> > _Cmp;
template void __fastcall __insertion_sort<_Elem*, _Cmp>(_Elem*, _Elem*, _Cmp);
} // namespace std