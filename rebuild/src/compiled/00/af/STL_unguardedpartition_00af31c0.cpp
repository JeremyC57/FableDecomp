// _STL::__unguarded_partition(CPSCPair* first, CPSCPair* last, CPSCPair pivot, CPSCLess comp) -- STLport quicksort partition step over the 16-byte registered-PSC entries pair<CCharString, NParticleEngine::CRegisteredPSCInfo>: walk first up past names below the pivot's, last down past names above it, swap, repeat until the cursors cross
// Leaf: ??$__unguarded_partition@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AF31C0, 119 bytes (+9 int3 padding), `ret 14h`. /Gr algorithm TU written as explicit __fastcall: first in
//   ecx, last in edx, the 16-byte pivot by value and the 4-byte empty comparator slot on the stack; the crossing cursor
//   comes back in eax after the pivot is destroyed (GUIName, then the key).
// Callers: __introsort_loop 0x00AF3AA0. Callees: CBasicString<char>::operator< 0x00429950 (behind the inline
//   CCharString::operator<), swap<CPSCPair> 0x00AF2CD0 (iter_swap's out-of-line worker: `swap(*a, *b)`, __fastcall, two
//   references; 0x00AF2E60 is the out-of-line iter_swap that tail-jumps to it), ~CCharString 0x0099EAE0.
// Element: CPSCManager's RegisteredPSCs entry: key CCharString +0, CRegisteredPSCInfo {AllocFunc, ComponentType, GUIName} +4.
// Label: catalog "QuickSort_Partition" is descriptive.
// Shape: STLport 4.6 _algo.c verbatim (`for (;;) { while (comp(*first, pivot)) ++first; --last; while (comp(pivot, *last))
//   --last; if (!(first < last)) return first; iter_swap(first, last); ++first; }`). The block compare is a same-TU
//   noinline body (no memory writes), the swap a declaration: the pivot's key pointer stays in esi across the compares
//   and is reloaded only after the swap (`jmp` back to the `mov esi,[esp+10h]`).
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
template <class _Tp>
void __fastcall swap(_Tp& __a, _Tp& __b);   // 0x00AF2CD0
template <class _ForwardIter1, class _ForwardIter2>
inline void iter_swap(_ForwardIter1 __i1, _ForwardIter2 __i2)
{
    swap(*__i1, *__i2);
}

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

template CPSCPair* __fastcall _STL::__unguarded_partition<CPSCPair*, CPSCPair, CPSCLess>(CPSCPair*, CPSCPair*, CPSCPair, CPSCLess);