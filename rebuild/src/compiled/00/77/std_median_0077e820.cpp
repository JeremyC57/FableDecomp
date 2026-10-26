// std::__median(const std::pair<CCharString, C3DVector >& a, const std::pair<CCharString, C3DVector >& b, const std::pair<CCharString, C3DVector >& c, CKeyPairCompareLess<CCharString, C3DVector > comp) -- STLport median-of-three (introsort pivot choice) over the 16-byte (CCharString, C3DVector) named positions
// Leaf: ??$__median@U?$pair@VCCharString@@VC3DVector@@@std@@V?$CKeyPairCompareLess@VCCharString@@VC3DVector@@@@@std@@YIABU?$pair@VCCharString@@VC3DVector@@@0@ABU10@00V?$CKeyPairCompareLess@VCCharString@@VC3DVector@@@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0077E820, 194 bytes (+int3 pad to 208), `__fastcall` `ret 8` (a in ecx, b in edx;
//   c and the empty comparator slot on the stack); returns the chosen reference in eax.
// Callee: CBasicString<char>::operator< 0x00429950 (the string-data compare behind the inline CCharString::operator<).
// Identity (proven): _Tp = std::pair<CCharString, C3DVector >, _Compare = CKeyPairCompareLess<CCharString, C3DVector >. Its only caller is the landed __introsort_loop 0x0077F370 over 16-byte pair<CCharString, C3DVector> elements sorted by CKeyPairCompareLess<CCharString, C3DVector> (both in the Ego_r/FableWin name sets).
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

class C3DVector {
public:
    float X;
    float Y;
    float Z;
};
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

template const std::pair<CCharString, C3DVector >& __fastcall std::__median<std::pair<CCharString, C3DVector >, CKeyPairCompareLess<CCharString, C3DVector > >(
    const std::pair<CCharString, C3DVector >&, const std::pair<CCharString, C3DVector >&, const std::pair<CCharString, C3DVector >&, CKeyPairCompareLess<CCharString, C3DVector >);