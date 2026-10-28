// std::__unguarded_linear_insert(pair<CCharString, CTCBase::CRegisteredTCInfo>* last, pair<CCharString, CTCBase::CRegisteredTCInfo> val, CKeyPairCompareLess comp) -- STLport insertion-sort inner loop over the thing-component registry (component name -> registration): shift every greater-named entry up one slot, then store the value in the hole
// Leaf: ??$__unguarded_linear_insert@
// Retail: Fable.exe (TLC, VC7.1) @ 0x004DD80B (unaligned: a size-optimised TU), 85 bytes, `ret 14h` (/Gr algorithm TU,
//   explicit __fastcall here: last in ecx, the 16-byte by-value pair and the empty comparator slot on the stack; the
//   callee destroys both strings of the by-value pair).
// Callers: call sites 0x004DFD79 and 0x004E1CE1 (the same size-optimised TU; by shape the insertion-sort callers).
// Callees: CBasicString<char>::operator< 0x00429950 (landed; the real body sits in this TU under auto_inline(off)),
//   pair<CCharString, CTCBase::CRegisteredTCInfo>::operator= 0x004DA56A (implicit, out of line: it calls
//   CCharString::operator= and CRegisteredTCInfo::operator= 0x004D78C5), CCharString::~CCharString 0x0099EAE0 (twice).
// Identity: CTCBase's CVectorMap<CCharString, CTCBase::CRegisteredTCInfo, CKeyPairCompareLess<...> > (Ego_r.pdb names the
//   map; CRegisteredTCInfo is {AllocFunc, DriverType, Name}, 12 bytes). The catalog's "std::set_Insert_Sorted" is
//   descriptive; the manifest labels on 0x004DA56A / 0x004D78C5 are propagated from other instantiations.
// Shape: `#pragma optimize("s", on)` (the TU's functions are packed without 16-byte alignment). Retail's compare is the
//   inline CCharString::operator< (same block -> false, null this -> true, null other -> false, else the out-of-line
//   CBasicString compare) with no frame, which under /Os needs `__forceinline` on both CCharString::operator< and the
//   comparator's operator(); without it VC7.1 emits an ebp frame and calls the comparator (DIFFER 69v85).
#pragma optimize("s", on)
template <class T> class CBasicString {
public:
    T* m_pData;
#pragma auto_inline(off)
    // 0x00429950 (landed): the real body as a same-TU noinline function, as the STLport-over-CCharString rule asks.
    __declspec(noinline) bool operator<(const CBasicString& other) const
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
};

class CCharString {
public:
    CBasicString<char>* PStringData;                    // the shared block
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
    const CCharString& operator=(const CCharString& other);   // 0x0099EFB0
    __forceinline bool operator<(const CCharString& other) const
    {
        if (other.PStringData == PStringData)
            return false;
        if (PStringData == 0)
            return true;
        if (other.PStringData == 0)
            return false;
        return *PStringData < *other.PStringData;
    }
};

class CThing;

class CTCBase {
public:
    class CRegisteredTCInfo {                           // Ego_r.pdb, 12 bytes
    public:
        CTCBase* (__cdecl* AllocFunc)(CThing*);         // +0x00
        unsigned long DriverType;                       // +0x04
        CCharString   Name;                             // +0x08
    };
};

namespace std {
template <class _T1, class _T2> struct pair {          // STLport pair: user constructors, implicit operator=
    _T1 first;                                          // +0x00
    _T2 second;                                         // +0x04
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) { }
};
}

template <class K, class V> class CKeyPairCompareLess { // Ego_r.pdb: empty, passed by value (4-byte slot)
public:
    __forceinline bool operator()(const std::pair<K, V>& a, const std::pair<K, V>& b) const { return a.first < b.first; }
};

typedef std::pair<CCharString, CTCBase::CRegisteredTCInfo> TTCInfoPair;            // 16 bytes
typedef CKeyPairCompareLess<CCharString, CTCBase::CRegisteredTCInfo> TTCInfoLess;

namespace std {
template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __unguarded_linear_insert(_RandomAccessIter __last, _Tp __val, _Compare __comp)
{
    _RandomAccessIter __next = __last;
    --__next;
    while (__comp(__val, *__next)) {
        *__last = *__next;
        __last = __next;
        --__next;
    }
    *__last = __val;
}
template void __fastcall __unguarded_linear_insert<TTCInfoPair*, TTCInfoPair, TTCInfoLess>(TTCInfoPair*, TTCInfoPair, TTCInfoLess);
}