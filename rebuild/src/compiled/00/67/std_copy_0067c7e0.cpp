// std::__copy(CBlurredAttitudesDef* first, CBlurredAttitudesDef* last, CBlurredAttitudesDef* result, const random_access_iterator_tag&, int*) -- STLport counted forward copy of blurred-attitudes defs through the def's implicit assignment, returns the end of the output
// Leaf: ??$__copy@PAVCBlurredAttitudesDef@@
//
// Retail 0x0067C7E0, 194 bytes (+ int3 padding to 208), `ret 0Ch` (/Gr: first in ecx, last in edx; result, the tag
//   reference and the _Distance* tag on the stack).
// Callers: vector<CBlurredAttitudesDef>::erase(first, last) 0x0067CAD0 (call 0x0067CAE9: copy the tail down, then the
//   virtual dtor over the leftovers) and the copy() front end 0x0067C9D0 -- the element of the landed
//   CBlurredAttitudesDefVector::_M_fill_insert 0x0067CC50.  The catalog's "_Copy_impl<..>" is the Dinkumware spelling.
// Layout (Ego_r.pdb names, retail offsets; the hierarchy of the landed std::fill 0x0067C720 over the same element):
//   CResource root (vptr; six words +4..+0x18); CDefClassBase, pack(1), 0x25 bytes: PDefManager +0x1C, GlobalIndex
//   +0x20, the Setup / Template / DefaultValsApplied bits at +0x24; CParentDefClassBase, pack(1): InstantiationName
//   +0x25 (unaligned), SubDefInfoMap CVectorMap +0x29 (vector, empty comparator +0x35, Dirty +0x36); CBlurredAttitudesDef
//   (0x48, vtable 0x0125B8AC) adds Attitudes vector<EOpinionAttitudeType> at +0x3C.  The generated engine headers are
//   flattened non-polymorphic PDB layouts (the vptr a plain word the assignment would copy), so the model is local.
// Callees: the SubDefInfoMap vector's operator= @ 0x00431F90, the Attitudes vector's operator= @ 0x00471E44.
// Shape: STLport _algobase.h `for (_Distance n = last - first; n > 0; --n) { *result = *first; ++first; ++result; }
//   return result;` (n = byte span / 0x48 via the 0x38E38E39 multiply) with the implicit def assignment inline: the
//   vptr is not copied, the root words, PDefManager, GlobalIndex, each flag bit by read-modify-write, the unaligned
//   InstantiationName, the map's vector call, the Dirty byte, then the Attitudes call.  /Gr TU written as __fastcall.
namespace std {
template <class T>
class vector {   // STLport vector: 12 bytes
public:
    T* _M_start;                                // +0x00
    T* _M_finish;                               // +0x04
    T* _M_end_of_storage;                       // +0x08
    vector<T>& operator=(const vector<T>& x);   // out of line: 0x00431F90 (SubDefInfoMap), 0x00471E44 (Attitudes)
};
template <class T1, class T2>
class pair {
public:
    T1 first;
    T2 second;
};
}
template <class K, class V>
class CKeyPairCompareLess {
public:
    bool operator()(const std::pair<K, V>& a, const std::pair<K, V>& b) const { return a.first < b.first; }
};
template <class K, class V, class Compare>
class CVectorMap {   // retail 16 bytes
public:
    std::vector<std::pair<K, V> > Data;         // +0x00
    Compare                       Comparator;   // +0x0C (empty)
    bool                          Dirty;        // +0x0D
};

class CResourceList;
class CResource {   // 28 bytes
public:
    virtual void v00();
    long           m_refCount;                  // +0x04
    CResourceList* ResourceList;                // +0x08
    CResource*     PrevResource;                // +0x0C
    CResource*     NextResource;                // +0x10
    unsigned long  ResourceSize;                // +0x14
    unsigned long  LastUsedFrame;               // +0x18
};
class CDefinitionManager;
class CDefString {   // one word (Ego_r.pdb: TablePos)
public:
    long TablePos;                              // +0x00
};
#pragma pack(push, 1)
class CDefPointeeBase : public CResource {
};
class CDefClassBase : public CDefPointeeBase {   // 0x25 bytes
public:
    class CSubDefInfo { public: long m_0; long m_4; };
    CDefinitionManager* PDefManager;            // +0x1C
    unsigned long       GlobalIndex;            // +0x20
    unsigned char       Setup : 1;              // +0x24 bit 0
    unsigned char       Template : 1;           // +0x24 bit 1
    unsigned char       DefaultValsApplied : 1; // +0x24 bit 2
};
class CParentDefClassBase : public CDefClassBase {   // 0x39 bytes in retail
public:
    CDefString InstantiationName;               // +0x25
    CVectorMap<unsigned long, CDefClassBase::CSubDefInfo,
               CKeyPairCompareLess<unsigned long, CDefClassBase::CSubDefInfo> > SubDefInfoMap;   // +0x29
};
#pragma pack(pop)
enum EOpinionAttitudeType { OPINION_ATTITUDE_NONE };
class CBlurredAttitudesDef : public CParentDefClassBase {   // 0x48 bytes
public:
    std::vector<EOpinionAttitudeType> Attitudes; // +0x3C
};

namespace std {
struct random_access_iterator_tag { };

template <class _RandomAccessIter, class _OutputIter, class _Distance>
_OutputIter __fastcall __copy(_RandomAccessIter __first, _RandomAccessIter __last, _OutputIter __result,
                              const random_access_iterator_tag&, _Distance*)
{
    for (_Distance __n = __last - __first; __n > 0; --__n) {
        *__result = *__first;
        ++__first;
        ++__result;
    }
    return __result;
}
template CBlurredAttitudesDef* __fastcall __copy(CBlurredAttitudesDef*, CBlurredAttitudesDef*, CBlurredAttitudesDef*,
                                                 const random_access_iterator_tag&, int*);
}