// std::__copy(CBodyPartMesh* first, CBodyPartMesh* last, CBodyPartMesh* result, const random_access_iterator_tag&, int*) -- STLport counted forward copy of body-part meshes (memberwise assignment, the texture-morph map through its vector's operator=), returns the end of the output
// Leaf: ??$__copy@PAVCBodyPartMesh@
//
// Retail 0x00674D80, 87 bytes (+ int3 padding to 96), `ret 0Ch` (/Gr: first in ecx, last in edx; result, the tag
//   reference and the _Distance* tag on the stack).
// Callee: vector<pair<unsigned long, CRandomAppearanceMorph::CTextureMorph> >::operator= @ 0x006741E0 (the entry vector of
//   the CVectorMap at +8; landed CRandomAppearanceMorph::operator= 0x00675B60 calls it for GeneralTextureMorphs).
// Identity: element = Ego_r.pdb CRandomAppearanceMorph::CBodyPartMesh { long MeshIndex; long Priority;
//   CVectorMap<unsigned long, CRandomAppearanceMorph::CTextureMorph> TextureMorphs; } -- PDB 28 bytes, retail 24 (STLport
//   vector 12 + empty compare +0x14 + Dirty +0x15), the only type holding that map besides CRandomAppearanceMorph.
//   Callers 0x0067503C / 0x006756A4 / 0x006756CD are the CArray<CBodyPartMesh> copy paths.  The catalog's
//   "_Move<CEngineTextureMorph*,CEngineTextureMorph*>" is a propagated Dinkumware label.
// Shape: STLport _algobase.h `for (_Distance n = last - first; n > 0; --n) { *result = *first; ++first; ++result; }
//   return result;` (n = byte span / 24 via the 0x2AAAAAAB multiply); CBodyPartMesh's implicit operator= is inline: the two
//   longs, the map's vector call, nothing for the empty comparator, the Dirty byte.  /Gr TU written as __fastcall.

class CRandomAppearanceMorph {
public:
    class CTextureMorph {                                   // 8 bytes
    public:
        unsigned long NewTexture;                           // +0x00
        unsigned long GroupID;                              // +0x04
    };
    class CBodyPartMesh;
};

namespace std {
struct random_access_iterator_tag { };

template <class _T1, class _T2> class pair {
public:
    _T1 first;
    _T2 second;
};

template <class _Tp> class vector {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
    vector<_Tp>& operator=(const vector<_Tp>& __x);         // 0x006741E0 for pair<unsigned long, CTextureMorph>
};

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
}

template <class K, class V> class CKeyPairCompareLess { };

template <class K, class V, class C> class CVectorMap {
public:
    std::vector<std::pair<K, V> > Entries;                   // +0x00
    C    Compare;                                            // +0x0c (empty)
    bool Dirty;                                              // +0x0d
};

class CRandomAppearanceMorph::CBodyPartMesh {               // retail 24 bytes
public:
    long MeshIndex;                                          // +0x00
    long Priority;                                           // +0x04
    CVectorMap<unsigned long, CTextureMorph, CKeyPairCompareLess<unsigned long, CTextureMorph> > TextureMorphs;   // +0x08
};

typedef CRandomAppearanceMorph::CBodyPartMesh CBodyPartMesh_t;
template CBodyPartMesh_t* __fastcall std::__copy<CBodyPartMesh_t*, CBodyPartMesh_t*, int>(
    CBodyPartMesh_t*, CBodyPartMesh_t*, CBodyPartMesh_t*, const std::random_access_iterator_tag&, int*);