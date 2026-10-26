// std::__unguarded_linear_insert(CPSCPair* last, CPSCPair val, CPSCLess comp) -- STLport insertion-sort inner step over the 16-byte registered-PSC entries pair<CCharString, NParticleEngine::CRegisteredPSCInfo>: shift every entry whose name sorts after val's up one slot, then assign val into the hole
// Leaf: ??$__unguarded_linear_insert@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AF2E70, 146 bytes (+14 int3 padding), `ret 14h`. /Gr algorithm TU written as explicit __fastcall: last in
//   ecx, the 16-byte val by value and the 4-byte empty comparator slot on the stack; the callee destroys val (GUIName, key).
// Callers: __linear_insert 0x00AF35A0 (and the unguarded insertion sort 0x00AF3240).
// Callees: CBasicString<char>::operator< @ 0x00429950 (behind the inline CCharString::operator<), CCharString::operator=
//   @ 0x0099EFB0, ~CCharString @ 0x0099EAE0.
// Element: CPSCManager's RegisteredPSCs entry (the landed __pop_heap 0x00AF3430 / LowerBound 0x00AF3000): key CCharString
//   +0, CRegisteredPSCInfo {AllocFunc, ComponentType, CCharString GUIName} +4. The implicit pair assignment is the key
//   operator=, two dwords and the GUIName operator= (addressed off a second induction pointer `lea ebx,[edi+0Ch]`).
// Label: catalog "TreeNode_InsertPosition" is descriptive.
// Shape: STLport 4.6 _algo.c verbatim (`next = last; --next; while (comp(val, *next)) { *last = *next; last = next; --next; }
//   *last = val;`); the comparator orders on the key only (CKeyPairCompareLess), the block compare a same-TU
//   noinline body.
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
class CRegisteredPSCInfo { public:
    CPSCBase* (__cdecl* AllocFunc)();                       // +0x0
    EPSCType ComponentType;                                 // +0x4
    CCharString GUIName;                                    // +0x8
};
}

namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) { }
};
}

template <class K, class V> class CKeyPairCompareLess {
public:
    bool operator()(const std::pair<K, V>& a, const std::pair<K, V>& b) const { return a.first < b.first; }
};

typedef std::pair<CCharString, NParticleEngine::CRegisteredPSCInfo> CPSCPair;
typedef CKeyPairCompareLess<CCharString, NParticleEngine::CRegisteredPSCInfo> CPSCLess;

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
}

template void __fastcall std::__unguarded_linear_insert<CPSCPair*, CPSCPair, CPSCLess>(CPSCPair*, CPSCPair, CPSCLess);