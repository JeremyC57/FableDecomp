// std::__median(const std::pair<CCharString, NParticleEngine::CRegisteredPSCInfo >& a, const std::pair<CCharString, NParticleEngine::CRegisteredPSCInfo >& b, const std::pair<CCharString, NParticleEngine::CRegisteredPSCInfo >& c, CKeyPairCompareLess<CCharString, NParticleEngine::CRegisteredPSCInfo > comp) -- STLport median-of-three (introsort pivot choice) over CPSCManager's registered-PSC entries
// Leaf: ??$__median@U?$pair@VCCharString@@VCRegisteredPSCInfo@NParticleEngine@@@std@@V?$CKeyPairCompareLess@VCCharString@@VCRegisteredPSCInfo@NParticleEngine@@@@@std@@YIABU?$pair@VCCharString@@VCRegisteredPSCInfo@NParticleEngine@@@0@ABU10@00V?$CKeyPairCompareLess@VCCharString@@VCRegisteredPSCInfo@NParticleEngine@@@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AF30F0, 194 bytes (+int3 pad to 208), `__fastcall` `ret 8` (a in ecx, b in edx;
//   c and the empty comparator slot on the stack); returns the chosen reference in eax.
// Callee: CBasicString<char>::operator< 0x00429950 (the string-data compare behind the inline CCharString::operator<).
// Identity (proven): _Tp = std::pair<CCharString, NParticleEngine::CRegisteredPSCInfo >, _Compare = CKeyPairCompareLess<CCharString, NParticleEngine::CRegisteredPSCInfo >. Its only caller is the landed __introsort_loop 0x00AF3AA0 over CPSCManager's 16-byte RegisteredPSCs entries pair<CCharString, NParticleEngine::CRegisteredPSCInfo> (CVectorMap with CKeyPairCompareLess<CCharString, CRegisteredPSCInfo>, Ego_r).
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

namespace NParticleEngine {
class CRegisteredPSCInfo {                           // AllocFunc, ComponentType, GUIName (12 bytes)
public:
    void* AllocFunc;
    long ComponentType;
    CCharString GUIName;
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

template const std::pair<CCharString, NParticleEngine::CRegisteredPSCInfo >& __fastcall std::__median<std::pair<CCharString, NParticleEngine::CRegisteredPSCInfo >, CKeyPairCompareLess<CCharString, NParticleEngine::CRegisteredPSCInfo > >(
    const std::pair<CCharString, NParticleEngine::CRegisteredPSCInfo >&, const std::pair<CCharString, NParticleEngine::CRegisteredPSCInfo >&, const std::pair<CCharString, NParticleEngine::CRegisteredPSCInfo >&, CKeyPairCompareLess<CCharString, NParticleEngine::CRegisteredPSCInfo >);