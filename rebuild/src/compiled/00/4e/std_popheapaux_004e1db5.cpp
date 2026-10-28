// std::__pop_heap_aux(pair<CCharString,CTCBase::CRegisteredTCInfo>* first, pair<...>* last, pair<...>*, CKeyPairCompareLess comp) -- STLport pop_heap step for the registered-TC table: copy the back element and hand it to __pop_heap
// Leaf: ??$__pop_heap_aux@PAU?$pair@VCCharString@@VCRegisteredTCInfo@CTCBase@@@std@@U12@V?$CKeyPairCompareLess@VCCharString@@VCRegisteredTCInfo@CTCBase@@@@@std@@
// Retail: Fable.exe @ 0x004E1DB5, 39 bytes (/Os /Gr TU: packed address; __fastcall: first ecx, last edx, the null
// value-type pointer and the empty comparator on the stack, `ret 8`).
// Identity: callers 0x004E36D9 and 0x004E41FE (the table's pop_heap / sort_heap: `push comp ; push 0 ; call`); the callee is
// the landed __pop_heap over the same 16-byte pair 0x004E1D20 (`ret 1Ch`: result, the by-value pair, comp, the distance
// pointer). Element: pair<CCharString, CTCBase::CRegisteredTCInfo> (Ego_r.pdb CRegisteredTCInfo: AllocFunc, DriverType,
// Name; the comparator is CKeyPairCompareLess<CCharString, CTCBase::CRegisteredTCInfo>, as the PDB name sets spell the
// table's sort helpers). The catalog label CEngine::ClipBox is propagated.
// Callees: pair<CCharString, CTCBase::CRegisteredTCInfo>::pair(const pair&) 0x004D788D (out of line; builds `_Tp(*(last - 1))`
// straight into __pop_heap's argument area), std::__pop_heap 0x004E1D20.
// Shape: STLport `__pop_heap(__first, __last - 1, __last - 1, _Tp(*(__last - 1)), __comp, _STLP_DISTANCE_TYPE(...))`, the
// distance type a null `int*`; arguments right to left.
#pragma optimize("s", on)
class CCharString { public:
    void* PStringData;
    ~CCharString();                                      // 0x0099EAE0
};
class CThing;
class CTCBase { public:
    class CRegisteredTCInfo { public:
        CTCBase* (__cdecl* AllocFunc)(CThing*);          // +0x00
        unsigned long      DriverType;                   // +0x04
        CCharString        Name;                         // +0x08
    };
};
template <class K, class V> class CKeyPairCompareLess { public:
    CKeyPairCompareLess() {}
};
namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;                                           // +0x00
    _T2 second;                                          // +0x04
    pair(const pair& other);                             // 0x004D788D (pair<CCharString, CTCBase::CRegisteredTCInfo>)
};
template <class _RandomAccessIterator, class _Tp, class _Compare, class _Distance>
void __fastcall __pop_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _RandomAccessIterator __result,
                           _Tp __val, _Compare __comp, _Distance*);                  // 0x004E1D20
template <class _RandomAccessIterator, class _Tp, class _Compare>
void __fastcall __pop_heap_aux(_RandomAccessIterator __first, _RandomAccessIterator __last, _Tp*, _Compare __comp)
{
    __pop_heap(__first, __last - 1, __last - 1, _Tp(*(__last - 1)), __comp, (int*)0);
}
typedef pair<CCharString, CTCBase::CRegisteredTCInfo> CRegisteredTCPair;
template void __fastcall __pop_heap_aux<CRegisteredTCPair*, CRegisteredTCPair, CKeyPairCompareLess<CCharString, CTCBase::CRegisteredTCInfo> >(
    CRegisteredTCPair*, CRegisteredTCPair*, CRegisteredTCPair*, CKeyPairCompareLess<CCharString, CTCBase::CRegisteredTCInfo>);
}