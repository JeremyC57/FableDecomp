// std::__unguarded_partition(pair<CCharString, CTCBase::CRegisteredTCInfo>* first, pair* last, pair pivot, CKeyPairCompareLess comp) -- STLport quicksort partition step over the thing-component registry (ordered by name): advance first past smaller names, retreat last past larger ones, swap the out-of-place pair, return the split point; the by-value pivot is destroyed here
// Leaf: ??$__unguarded_partition@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004DFCEA, 113 bytes, `ret 14h` (size-optimised TU: unaligned entry).
// Caller: 0x004E828F (the introsort loop of CVectorMap<CCharString, CTCBase::CRegisteredTCInfo, ..>::Sort, Ego_r.pdb).
// Callees: CBasicString<char>::operator< 0x00429950 (behind the inlined CCharString::operator< block-pointer tests),
//   iter_swap 0x004DA6FC (`__fastcall`, out of line in this /Os TU), ~CCharString 0x0099EAE0 (x2: the pivot's
//   second.Name at +0xc, then its key).
// /Gr algorithm TU (link-time __fastcall): first in ecx, last in edx, the 16-byte pivot and the empty comparator on the
//   stack (16 + 4 = 0x14). CTCBase::CRegisteredTCInfo (Ego_r.pdb, 12 bytes): AllocFunc +0, DriverType +4, Name +8.
// Recipe (agent902): the real CBasicString<char>::operator< body (landed 0x00429950, `other` read first) as a same-TU
//   `auto_inline(off)` + noinline stand-in. VC7.1 then knows the compare writes no memory, keeps pivot.first's block
//   pointer in esi across the scans (re-read only after iter_swap) and drops the ebp frame the opaque call forced
//   (parked 110v113 -> match). __forceinline comparator + CCharString::operator< as in the size TU.
#pragma optimize("s", on)
template <class T> class CBasicString {
public:
    T* m_pData;
    bool operator<(const CBasicString& other) const;
};
#pragma auto_inline(off)
template <class T> __declspec(noinline) bool CBasicString<T>::operator<(const CBasicString& other) const
{
    const T* b = other.m_pData;
    const T* a = m_pData;
    for (;;) {
        if (*a == 0 && *b == 0)
            return false;
        if (*a < *b)
            return true;
        if (*a > *b)
            return false;
        ++a;
        ++b;
    }
}
#pragma auto_inline(on)

class CCharString {
public:
    CBasicString<char>* PStringData;
    CCharString(const CCharString& other);                 // 0x0099EC30
    ~CCharString();                                        // 0x0099EAE0
    CCharString& operator=(const CCharString& other);      // 0x0099EFB0
    __forceinline bool operator<(const CCharString& other) const
    {
        if (other.PStringData == PStringData)
            return false;
        if (!PStringData)
            return true;
        if (!other.PStringData)
            return false;
        return *PStringData < *other.PStringData;
    }
};

class CThing;
class CTCBase {
public:
    class CRegisteredTCInfo {
    public:
        CTCBase* (__cdecl* AllocFunc)(CThing*);
        unsigned long DriverType;
        CCharString   Name;
    };
};

namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }
    pair(const pair& __p) : first(__p.first), second(__p.second) { }
};
template <class _ForwardIter1, class _ForwardIter2> void __fastcall iter_swap(_ForwardIter1 __a, _ForwardIter2 __b);
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
}

template <class K, class V> class CKeyPairCompareLess {
public:
    __forceinline bool operator()(const std::pair<K, V>& a, const std::pair<K, V>& b) const { return a.first < b.first; }
};
typedef std::pair<CCharString, CTCBase::CRegisteredTCInfo> TEntry;
template TEntry* __fastcall std::__unguarded_partition(TEntry*, TEntry*, TEntry, CKeyPairCompareLess<CCharString, CTCBase::CRegisteredTCInfo>);