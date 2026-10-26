// std::__median(const std::pair<CCharString, CSoundBankLH* >& a, const std::pair<CCharString, CSoundBankLH* >& b, const std::pair<CCharString, CSoundBankLH* >& c, CKeyPairCompareLess<CCharString, CSoundBankLH* > comp) -- STLport median-of-three (introsort pivot choice) over CSoundManagerLH's name-map entries
// Leaf: ??$__median@U?$pair@VCCharString@@PAVCSoundBankLH@@@std@@V?$CKeyPairCompareLess@VCCharString@@PAVCSoundBankLH@@@@@std@@YIABU?$pair@VCCharString@@PAVCSoundBankLH@@@0@ABU10@00V?$CKeyPairCompareLess@VCCharString@@PAVCSoundBankLH@@@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A40590, 194 bytes (+int3 pad to 208), `__fastcall` `ret 8` (a in ecx, b in edx;
//   c and the empty comparator slot on the stack); returns the chosen reference in eax.
// Callee: CBasicString<char>::operator< 0x00429950 (the string-data compare behind the inline CCharString::operator<).
// Identity (descriptive): _Tp = std::pair<CCharString, CSoundBankLH* >, _Compare = CKeyPairCompareLess<CCharString, CSoundBankLH* >. Its only caller is the landed __introsort_loop 0x00A41D50 over 8-byte pair<CCharString, value> entries, whose sort 0x00A41E20 is called by CSoundManagerLH vtable 0x0129D2C4 slots 37 (0x00A3F140) and 41 (0x00A3F340) on the name map at +0x64 (the value is the object pointer slot 37 obtains; the landed 0x00A3ED90 / 0x00A40D80 call it CSoundBankLH, descriptive: CSoundManagerLH has no PDB entry).
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

class CSoundBankLH;                                     // descriptive: the mapped object of CSoundManagerLH's name map
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

template const std::pair<CCharString, CSoundBankLH* >& __fastcall std::__median<std::pair<CCharString, CSoundBankLH* >, CKeyPairCompareLess<CCharString, CSoundBankLH* > >(
    const std::pair<CCharString, CSoundBankLH* >&, const std::pair<CCharString, CSoundBankLH* >&, const std::pair<CCharString, CSoundBankLH* >&, CKeyPairCompareLess<CCharString, CSoundBankLH* >);