// std::sort(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp) -- (pair<CCharString, CTCBase::CRegisteredTCInfo >*, CKeyPairCompareLess<CCharString, CTCBase::CRegisteredTCInfo>) STLport introsort: for a non-empty range run __introsort_loop to depth 2*lg(n), then __final_insertion_sort
// Flags: /Gr
// Leaf: ??$sort@PAU?$pair@VCCharString@@VCRegisteredTCInfo@CTCBase@@@std@@V?$CKeyPairCompareLess@VCCharString@@VCRegisteredTCInfo@CTCBase@@@@@std@@YIXPAU?$pair@VCCharString@@VCRegisteredTCInfo@CTCBase@@@0@0V?$CKeyPairCompareLess@VCCharString@@VCRegisteredTCInfo@CTCBase@@@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004EB9A6, 66 bytes, __fastcall (/Gr TU: first ecx, last edx, the comparator by value on the stack), `ret 4`.
// Identity (proven): std::sort<std::pair<CCharString, CTCBase::CRegisteredTCInfo >*, CKeyPairCompareLess<CCharString, CTCBase::CRegisteredTCInfo> >; evidence: its only caller is CVectorMap<CCharString, CTCBase::CRegisteredTCInfo, CKeyPairCompareLess<CCharString, CTCBase::CRegisteredTCInfo> >::Sort 0x004EBACE, called at 0x004F9139 on the global map 0x013BA89C (g_RegisteredTCs in the bindings) after the TC registration run whose every entry builds a CRegisteredTCInfo through 0x004D2EF0 (alloc function +0, driver type +4, CCharString copy 0x0099EC30 at +8: the PDB ctor CRegisteredTCInfo(CTCBase* (*)(CThing&), unsigned long, const CCharString&)), pairs it with a CCharString key in the pair ctor 0x004D9D2F (CCharString copy at +0, info copy at +4), pushes it into that map and sets its Dirty byte 0x013BA8A9; the 16-byte stride (`sar 4`) is the pair's size
// Label: the manifest's "GUI_HeapSort_BestForTargeting" is a behavioural / propagated label (family J067: five byte-identical sort bodies, each its own element and comparator).
// Callees: __introsort_loop(first, last, (value_type*)0, int depth_limit, comp) 0x004E823B, __final_insertion_sort(first, last, comp) 0x004E415D
//   (both out of line; the bindings' ??$__introsort_loop spellings for this element where present).
// Shape: STLport 4.6 _algo.c `sort` verbatim in a size-optimised TU (the __lg loop enters at its test, the comparator is re-pushed
//   from its stack slot); __lg is inlined (`for (k = 0; n != 1; n >>= 1) ++k;`), (last - first) is the exact
//   pointer difference (`sar 4` for the 0x10-byte pair); the empty comparator travels as one pushed dword.
// Element: std::pair<CCharString, CTCBase::CRegisteredTCInfo> (16 bytes: CCharString 4 + CRegisteredTCInfo 12, Ego_r.pdb)
#pragma optimize("s", on)
namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};
template <class _Tp> struct iterator_traits;
template <class _Tp> struct iterator_traits<_Tp*> {
    typedef _Tp value_type;
    typedef int difference_type;
};
template <class _Size> inline _Size __lg(_Size __n)
{
    _Size __k;
    for (__k = 0; __n != 1; __n >>= 1) ++__k;
    return __k;
}
template <class _RandomAccessIter, class _Tp, class _Size, class _Compare>
void __introsort_loop(_RandomAccessIter __first, _RandomAccessIter __last, _Tp*, _Size __depth_limit, _Compare __comp);   // 0x004E823B
template <class _RandomAccessIter, class _Compare>
void __final_insertion_sort(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp);   // 0x004E415D
template <class _RandomAccessIter, class _Compare>
void sort(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp)
{
    if (__first != __last) {
        __introsort_loop(__first, __last, (typename iterator_traits<_RandomAccessIter>::value_type*)0,
                         __lg(__last - __first) * 2, __comp);
        __final_insertion_sort(__first, __last, __comp);
    }
}
}   // namespace std
class CCharStringData;
class CCharString {                                      // Ego_r.pdb, 4 bytes
public:
    CCharStringData* PStringData;
};
class CThing;
class CTCBase {                                         // only its nested CRegisteredTCInfo is needed here
public:
    class CRegisteredTCInfo;
};
class CTCBase::CRegisteredTCInfo {                       // Ego_r.pdb 12 bytes; ctor 0x004D2EF0 (FableWin/Ego_r ??0CRegisteredTCInfo@CTCBase@@QAE@P6IPAV1@AAVCThing@@@ZKABVCCharString@@@Z)
public:
    CTCBase* (__fastcall* AllocFunc)(CThing&);           // +0x00
    unsigned long DriverType;                            // +0x04
    CCharString   Name;                                  // +0x08
};
template <class K, class V> class CKeyPairCompareLess { };   // Fable's vector-map key order (1 byte; its operator() lives in the out-of-line callees)
namespace std {
template void sort<pair<CCharString, CTCBase::CRegisteredTCInfo >*, CKeyPairCompareLess<CCharString, CTCBase::CRegisteredTCInfo> >(pair<CCharString, CTCBase::CRegisteredTCInfo >*, pair<CCharString, CTCBase::CRegisteredTCInfo >*, CKeyPairCompareLess<CCharString, CTCBase::CRegisteredTCInfo>);
}