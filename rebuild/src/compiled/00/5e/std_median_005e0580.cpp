// std::__median(const CCharString& a, const CCharString& b, const CCharString& c, std::less<CCharString> comp) -- STLport median-of-three (introsort pivot choice) over a CCharString range
// Leaf: ??$__median@VCCharString@@U?$less@VCCharString@@@std@@@std@@YIABVCCharString@@ABV1@00U?$less@VCCharString@@@0@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005E0580, 194 bytes (+int3 pad to 208), `__fastcall` `ret 8` (a in ecx, b in edx;
//   c and the empty comparator slot on the stack); returns the chosen reference in eax.
// Callee: CBasicString<char>::operator< 0x00429950 (the string-data compare behind the inline CCharString::operator<).
// Identity (proven): _Tp = CCharString, _Compare = std::less<CCharString>. Its only caller is the landed __introsort_loop<CCharString*, CCharString, less<CCharString> > 0x005E3740 (4-byte elements, `and eax,-4 ; cmp eax,40h`; sort<CCharString*> 0x005E3A50); the comparator is STLport's default less<CCharString>.
//   Family H002 (7 byte-identical bodies; retail has no identical-code folding, so each address is exactly one instantiation);
//   the manifest label "_Med3<pair<CCharString,CCharString>*, CKeyPairCompareLess<...> >" is a propagated Dinkumware spelling.
// Shape: STLport 4.6 _algo.c __median verbatim (nested if/else returning a, b or c); CKeyPairCompareLess::operator() and
//   CCharString::operator< inline (equal data -> false, null left -> true, null right -> false, else the data compare).
class CCharString;
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
    CBasicString<char>* PStringData;
    bool operator<(const CCharString& other) const
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

namespace std {
template <class _Tp> struct less {
    bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; }
};
}
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

namespace std {
template <class _Tp, class _Compare>
const _Tp& __fastcall __median(const _Tp& __a, const _Tp& __b, const _Tp& __c, _Compare __comp)
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
}

template const CCharString& __fastcall std::__median<CCharString, std::less<CCharString> >(
    const CCharString&, const CCharString&, const CCharString&, std::less<CCharString>);