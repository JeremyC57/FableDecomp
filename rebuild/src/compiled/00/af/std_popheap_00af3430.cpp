// std::__pop_heap(pair<CCharString,NParticleEngine::CRegisteredPSCInfo>* first, ..., pair value, CKeyPairCompareLess comp, int*) -- SGI pop_heap core for the registered particle-component table: *result = *first, then sift `value` down from the root
// Flags: /Gr
// Leaf: __pop_heap
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AF3430, 131 bytes.
// Label correction: the catalog name TeleportData::operator= is propagated; the callee 0x00AF3290
// carries the manifest name `_Adjust_heap<pair<CCharString,NParticleEngine::CRegisteredPSCInfo>*,...>`
// and the body is the SGI __pop_heap that feeds it (the family of the landed 0x00720610).  Callers:
// 0x00AF3589, 0x00AF36A9, 0x00AF37C1, 0x00AF3988 (the sort/pop loops of the same table, whose
// lower_bound is 0x00AF3000).
// Callees: CCharString::operator= @ 0x0099EFB0, CCharString copy ctor @ 0x0099EC30, ~CCharString
// @ 0x0099EAE0, __adjust_heap @ 0x00AF3290.  The SGI algorithm TUs are /Gr: first in ecx, last in
// edx, then result, the 16-byte value, the empty comparator and the distance-type pointer (`ret 0x1c`).
// Element (16 bytes): CCharString key +0, NParticleEngine::CRegisteredPSCInfo +4 (PDB: AllocFunc +0,
// EPSCType ComponentType +4, CCharString GUIName +8 -> element +0xc).
// Shape: `*result = *first` is the implicit pair assignment inline (string, two words, string); the
// by-value copy for __adjust_heap is the implicit pair copy ctor built in the argument area; the
// by-value `value` is destroyed here at the end (GUIName, then the key).
class CPSCBase;

class CCharString { public:
    void* PStringData;
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
    const CCharString& operator=(const CCharString& other);   // 0x0099EFB0
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

void __fastcall __adjust_heap(CPSCPair* first, int holeIndex, int len, CPSCPair value, CPSCLess comp);   // 0x00AF3290

void __fastcall __pop_heap(CPSCPair* first, CPSCPair* last, CPSCPair* result, CPSCPair value, CPSCLess comp, int*)
{
    *result = *first;
    __adjust_heap(first, 0, int(last - first), value, comp);
}