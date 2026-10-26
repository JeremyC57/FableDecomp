// std::sort_heap(CPSCPair* first, CPSCPair* last, CPSCLess comp) -- SGI sort_heap over the registered particle-component table (CCharString, NParticleEngine::CRegisteredPSCInfo): pop the root to the back until one element remains
// Leaf: sort_heap
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AF3770, length 107 bytes, `ret 4`.
// Identity: the catalog label `_Sort_heap<pair<CCharString,CCharString>*,...>` is propagated over the byte-identical
// skeleton. The callee is the landed SGI `__pop_heap` @ 0x00AF3430 over pair<CCharString,
// NParticleEngine::CRegisteredPSCInfo> (its caller list names 0x00AF37C1, inside this body); element model reused.
// Callees: __pop_heap @ 0x00AF3430 (out of line, __fastcall, `ret 1Ch`), CCharString copy constructor @ 0x0099EC30 (x2).
// Shape: /Gr SGI algorithm TU -- `while (last - first > 1) pop_heap(first, last--, comp)` with pop_heap and
// __pop_heap_aux inline (16-byte stride: `and -16 ; cmp 16`). The value copy in the argument area is the implicit
// pair copy: the key string, AllocFunc and ComponentType dwords, then the GUIName string (addressed off
// `lea eax,[esi+0Ch]`).
class CPSCBase;

class CCharString { public:
    void* PStringData;
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
    CCharString& operator=(const CCharString& other);   // 0x0099EFB0
};

enum EPSCType { PSC_TYPE_NONE = 0 };

namespace NParticleEngine {
class CRegisteredPSCInfo { public:
    CPSCBase* (__cdecl* AllocFunc)();           // +0x0
    EPSCType ComponentType;                     // +0x4
    CCharString GUIName;                        // +0x8
};
}

namespace std {
template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
};
}
template <class K, class V> class CKeyPairCompareLess { public:
    bool operator()(const std::pair<K, V>& a, const std::pair<K, V>& b) const;
};
typedef std::pair<CCharString, NParticleEngine::CRegisteredPSCInfo> CPSCPair;
typedef CKeyPairCompareLess<CCharString, NParticleEngine::CRegisteredPSCInfo> CPSCLess;

void __fastcall __pop_heap(CPSCPair* first, CPSCPair* last, CPSCPair* result, CPSCPair value, CPSCLess comp, int*);   // 0x00AF3430

inline void __pop_heap_aux(CPSCPair* first, CPSCPair* last, CPSCPair*, CPSCLess comp)
{
    __pop_heap(first, last - 1, last - 1, CPSCPair(*(last - 1)), comp, (int*)0);
}

inline void pop_heap(CPSCPair* first, CPSCPair* last, CPSCLess comp)
{
    __pop_heap_aux(first, last, (CPSCPair*)0, comp);
}

void __fastcall sort_heap(CPSCPair* first, CPSCPair* last, CPSCLess comp)
{
    while (last - first > 1)
        pop_heap(first, last--, comp);
}