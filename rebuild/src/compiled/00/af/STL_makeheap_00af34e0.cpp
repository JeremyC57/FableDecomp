// _STL::__make_heap(CPSCPair* first, CPSCPair* last, CPSCLess comp, CPSCPair*, int*) -- STLport make_heap worker over the 16-byte registered-PSC entries pair<CCharString, NParticleEngine::CRegisteredPSCInfo>: sift every parent from (len - 2) / 2 down to the root through __adjust_heap
// Leaf: ??$__make_heap@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AF34E0, 112 bytes, `ret 0Ch`. /Gr algorithm TU written as explicit __fastcall: first in
//   ecx, last in edx, the 4-byte empty comparator, the value-type and distance-type pointer tags on the stack.
// Callers: __partial_sort 0x00AF3920 (make_heap inline). Callees: __adjust_heap 0x00AF3290 (manifest
//   `_Adjust_heap<pair<CCharString,NParticleEngine::CRegisteredPSCInfo>*,...>`), CCharString copy ctor 0x0099EC30 (x2).
// Element: CPSCManager's RegisteredPSCs entry: key CCharString +0, CRegisteredPSCInfo {AllocFunc, ComponentType, GUIName} +4.
// Label: catalog "Heap_BuildHeap" is descriptive.
// Shape: STLport 4.6 _heap.c verbatim (`if (last - first < 2) return; len = last - first; parent = (len - 2) / 2; for (;;) {
//   __adjust_heap(first, parent, len, T(*(first + parent)), comp); if (parent == 0) return; parent--; }`); the by-value
//   copy is the pair's explicit copy constructor built in the argument area (key, two dwords, GUIName off the
//   strength-reduced `lea edi,[eax+ecx+0Ch]` pointer); `first` lives in the pushed-ecx frame slot.
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
template <class _RandomAccessIterator, class _Distance, class _Tp, class _Compare>
void __fastcall __adjust_heap(_RandomAccessIterator __first, _Distance __holeIndex, _Distance __len, _Tp __value, _Compare __comp);   // 0x00AF3290

template <class _RandomAccessIterator, class _Compare, class _Tp, class _Distance>
void __fastcall __make_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp, _Tp*, _Distance*)
{
    if (__last - __first < 2)
        return;
    _Distance __len = __last - __first;
    _Distance __parent = (__len - 2) / 2;

    for (;;) {
        __adjust_heap(__first, __parent, __len, _Tp(*(__first + __parent)), __comp);
        if (__parent == 0)
            return;
        __parent--;
    }
}
}

template void __fastcall _STL::__make_heap<CPSCPair*, CPSCLess, CPSCPair, int>(CPSCPair*, CPSCPair*, CPSCLess, CPSCPair*, int*);