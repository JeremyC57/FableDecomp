// _STL::__introsort_loop(CPSCPair* first, CPSCPair* last, CPSCPair*, int depth_limit, CPSCLess comp) -- STLport introsort recursion over the 16-byte registered-PSC entries pair<CCharString, NParticleEngine::CRegisteredPSCInfo>: median-of-three partition and recurse on the right part until ranges reach 16 entries; at depth zero fall back to partial_sort
// Leaf: ??$__introsort_loop@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AF3AA0, 205 bytes (+3 int3 padding), `ret 0Ch`. /Gr algorithm TU written as explicit __fastcall: first in
//   ecx, last in edx, the value-type pointer tag, the depth limit and the 4-byte empty comparator slot on the stack.
// Callers: sort 0x00AF3B70 (landed; depth 2*lg(n)) and itself. Callees: __median 0x00AF30F0 (manifest `_Med3<...>`,
//   __fastcall, returns a const reference), CCharString copy ctor 0x0099EC30 (x2: the pivot T(__median(...)) built in the
//   argument area), __unguarded_partition 0x00AF31C0, __partial_sort 0x00AF3920 (behind the inline partial_sort).
// Element: CPSCManager's RegisteredPSCs entry: key CCharString +0, CRegisteredPSCInfo {AllocFunc, ComponentType, GUIName} +4.
// Label: catalog "QuickSort_IntroSort_16Byte" is descriptive.
// Shape: STLport 4.6 _algo.c verbatim (`while (last - first > __stl_threshold) { if (depth_limit == 0) { partial_sort(first,
//   last, last, comp); return; } --depth_limit; cut = __unguarded_partition(first, last, T(__median(*first, *(first +
//   (last - first)/2), *(last - 1), comp)), comp); __introsort_loop(cut, last, (T*)0, depth_limit, comp); last = cut; }`);
//   the threshold test is the byte form `and eax,-10h ; cmp eax,100h`, the comparator pushed once ahead of both paths.
class CPSCBase;

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
    CBasicString<char>* PStringData;                        // the shared block
    CCharString(const CCharString& other);                  // 0x0099EC30
    ~CCharString();                                         // 0x0099EAE0
    CCharString& operator=(const CCharString& other);       // 0x0099EFB0
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

enum EPSCType { PSC_TYPE_NONE = 0 };

namespace NParticleEngine {
struct CRegisteredPSCInfo {
    CPSCBase* (__cdecl* AllocFunc)();                       // +0x0
    EPSCType ComponentType;                                 // +0x4
    CCharString GUIName;                                    // +0x8
};
}

namespace _STL {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) { }
};
}

template <class K, class V> class CKeyPairCompareLess {
public:
    bool operator()(const _STL::pair<K, V>& a, const _STL::pair<K, V>& b) const { return a.first < b.first; }
};

typedef _STL::pair<CCharString, NParticleEngine::CRegisteredPSCInfo> CPSCPair;
typedef CKeyPairCompareLess<CCharString, NParticleEngine::CRegisteredPSCInfo> CPSCLess;

namespace _STL {
template <class _Tp, class _Compare>
const _Tp& __fastcall __median(const _Tp& __a, const _Tp& __b, const _Tp& __c, _Compare __comp);   // 0x00AF30F0
template <class _RandomAccessIter, class _Tp, class _Compare>
_RandomAccessIter __fastcall __unguarded_partition(_RandomAccessIter __first, _RandomAccessIter __last, _Tp __pivot, _Compare __comp);   // 0x00AF31C0
template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __partial_sort(_RandomAccessIter __first, _RandomAccessIter __middle, _RandomAccessIter __last, _Tp*, _Compare __comp);   // 0x00AF3920

template <class _Tp>
inline _Tp* __value_type(const _Tp*) { return (_Tp*)0; }

template <class _RandomAccessIter, class _Compare>
inline void partial_sort(_RandomAccessIter __first, _RandomAccessIter __middle, _RandomAccessIter __last, _Compare __comp)
{
    __partial_sort(__first, __middle, __last, __value_type(__first), __comp);
}

template <class _RandomAccessIter, class _Tp, class _Size, class _Compare>
void __fastcall __introsort_loop(_RandomAccessIter __first, _RandomAccessIter __last, _Tp*, _Size __depth_limit, _Compare __comp)
{
    while (__last - __first > 16) {
        if (__depth_limit == 0) {
            partial_sort(__first, __last, __last, __comp);
            return;
        }
        --__depth_limit;
        _RandomAccessIter __cut =
            __unguarded_partition(__first, __last,
                                  _Tp(__median(*__first, *(__first + (__last - __first) / 2), *(__last - 1), __comp)),
                                  __comp);
        __introsort_loop(__cut, __last, (_Tp*)0, __depth_limit, __comp);
        __last = __cut;
    }
}
}

template void __fastcall _STL::__introsort_loop<CPSCPair*, CPSCPair, int, CPSCLess>(CPSCPair*, CPSCPair*, CPSCPair*, int, CPSCLess);