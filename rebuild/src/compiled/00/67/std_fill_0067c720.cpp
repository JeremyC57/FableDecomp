// std::fill(CBlurredAttitudesDef* first, CBlurredAttitudesDef* last, const CBlurredAttitudesDef& val) -- assign val to every blurred-attitudes def of [first, last)
// Leaf: fill
// Retail: Fable.exe @ 0x0067C720, 192 bytes, `ret 4`: __fastcall (first ecx -> esi, last edx -- spilled to the `push ecx`
// frame word -- and the value reference on the stack -> edi). Only call site 0x0067CD45 in the landed
// CBlurredAttitudesDefVector::_M_fill_insert 0x0067CC50, which names this body as its `fill` callee.
// Identity: the catalog's pair<unsigned __int64, NScript::CRuntimeScriptedEntityPersist::CPersistInfo> is propagated;
// the element is CBlurredAttitudesDef (0x48 bytes, vtable 0x0125B8AC) as in the landed 0x0067CC50 model.
// Layout (Ego_r.pdb names, retail offsets): the CResource root (vptr; count word, ResourceList, PrevResource,
//   NextResource, ResourceSize, LastUsedFrame +4..+0x18); CDefClassBase, pack(1) and 0x25 bytes as in the PDB:
//   PDefManager +0x1C, GlobalIndex +0x20, the Setup / Template / DefaultValsApplied bits at +0x24; CParentDefClassBase,
//   pack(1): InstantiationName CDefString +0x25 (unaligned), SubDefInfoMap CVectorMap +0x29 (retail 16 bytes: the
//   12-byte vector, the empty comparator +0x35, Dirty +0x36, two pad bytes) -> sizeof 0x39; CBlurredAttitudesDef is
//   not packed, so Attitudes (vector<EOpinionAttitudeType>) lands on the next 4-byte boundary, +0x3C (PDB +0x40 with
//   its 16-byte vectors). The generated engine headers CResource.h / CDefClassBase.h / CParentDefClassBase.h are
//   flattened non-polymorphic PDB layouts (vptr as a plain word the assignment would copy; 20-byte CVectorMap), so the
//   hierarchy is modelled locally.
// Callees: the SubDefInfoMap vector's operator= @ 0x00431F90, the Attitudes vector's operator= @ 0x00471E44.
// Shape: the def's implicit copy assignment never copies the vptr: the six root words, PDefManager, GlobalIndex, each of
// the three flag bits by read-modify-write, the unaligned InstantiationName word, the map's vector assignment call
// (source address hoisted into ebx), nothing for the empty comparator, the Dirty byte, then the Attitudes assignment
// call (source address hoisted into ebp).
namespace std {
template <class T>
struct vector {                                 // STLport vector: 12 bytes
    T* _M_start;                                // +0x00
    T* _M_finish;                               // +0x04
    T* _M_end_of_storage;                       // +0x08
    vector<T>& operator=(const vector<T>& x);   // out of line
};
template <class T1, class T2>
struct pair {
    T1 first;
    T2 second;
};
}
template <class K, class V>
struct CKeyPairCompareLess {
    bool operator()(const std::pair<K, V>& a, const std::pair<K, V>& b) const { return a.first < b.first; }
};
template <class K, class V, class Compare>
struct CVectorMap {                             // retail 16 bytes
    std::vector<std::pair<K, V> > Data;         // +0x00
    Compare                       Comparator;   // +0x0C (empty)
    bool                          Dirty;        // +0x0D
};

struct CResourceList;
struct CResource {                              // 28 bytes
    virtual void v00();
    long           m_refCount;                  // +0x04
    CResourceList* ResourceList;                // +0x08
    CResource*     PrevResource;                // +0x0C
    CResource*     NextResource;                // +0x10
    unsigned long  ResourceSize;                // +0x14
    unsigned long  LastUsedFrame;               // +0x18
};
struct CDefinitionManager;
struct CDefString {                             // one word (Ego_r.pdb: TablePos)
    long TablePos;                              // +0x00
};
#pragma pack(push, 1)
struct CDefPointeeBase : CResource {
};
struct CDefClassBase : CDefPointeeBase {        // 0x25 bytes
    struct CSubDefInfo { long m_0; long m_4; };
    CDefinitionManager* PDefManager;            // +0x1C
    unsigned long       GlobalIndex;            // +0x20
    unsigned char       Setup : 1;              // +0x24 bit 0
    unsigned char       Template : 1;           // +0x24 bit 1
    unsigned char       DefaultValsApplied : 1; // +0x24 bit 2
};
struct CParentDefClassBase : CDefClassBase {    // 0x39 bytes in retail
    CDefString InstantiationName;               // +0x25
    CVectorMap<unsigned long, CDefClassBase::CSubDefInfo,
               CKeyPairCompareLess<unsigned long, CDefClassBase::CSubDefInfo> > SubDefInfoMap;   // +0x29
};
#pragma pack(pop)
enum EOpinionAttitudeType { OPINION_ATTITUDE_NONE };
struct CBlurredAttitudesDef : CParentDefClassBase {   // 0x48 bytes
    std::vector<EOpinionAttitudeType> Attitudes; // +0x3C
};

namespace std {
template <class ForwardIter, class T>
void __fastcall fill(ForwardIter first, ForwardIter last, const T& val)
{
    for (; first != last; ++first)
        *first = val;
}
template void __fastcall fill(CBlurredAttitudesDef*, CBlurredAttitudesDef*, const CBlurredAttitudesDef&);
}