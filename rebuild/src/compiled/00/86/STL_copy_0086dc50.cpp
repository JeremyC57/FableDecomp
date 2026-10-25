// std::__copy(CRandomAppearanceMorph::CBodyPartMesh* first, CRandomAppearanceMorph::CBodyPartMesh* last, CRandomAppearanceMorph::CBodyPartMesh* result, const random_access_iterator_tag&, int*) -- STLport random-access copy of body-part meshes by assignment; returns the end of the destination run
// Leaf: ??$__copy@PAVCBodyPartMesh@CRandomAppearanceMorph@@
// Retail 0x0086DC50, 0x57 bytes (+ int3 padding to 0x60), `ret 0Ch` (/Gr: first in ecx, last in edx; result, the
// iterator-category tag and the null distance pointer on the stack). Callers 0x0086DE2C, 0x0086E169 (the
// vector<CBodyPartMesh> insert/erase paths of the CRandomAppearanceMorph container run).
// Callee: vector<pair<unsigned long, CTextureMorph> >::operator=(const vector&) @ 0x006741E0 (the TextureMorphs map's
// vector; catalog "Vector_Assign").
// Element: CRandomAppearanceMorph::CBodyPartMesh (Ego_r: long MeshIndex +0, long Priority +4, CVectorMap<unsigned long,
// CTextureMorph> TextureMorphs +8; retail 0x18 = 8 + the 16-byte retail CVectorMap: vector +8, empty Compare +0x14,
// bool Dirty +0x15). Its implicit operator= is memberwise: two dwords, the vector's operator= on +8, the Dirty byte.
// Shape: STLport `for (_Distance n = last - first; n > 0; --n) { *result = *first; ++first; ++result; } return result;`
// (the count is a signed divide by 24: `imul 2AAAAAABh ; sar edx,2`).
// Label: the manifest's "_Move<CEngineTextureMorph*,CEngineTextureMorph*>" is a propagated label.
namespace std {
struct random_access_iterator_tag { random_access_iterator_tag() { } };

template <class _T1, class _T2> struct pair
{
    _T1 first;
    _T2 second;
};

template <class _Tp> class vector                    // STLport vector<_Tp, allocator<_Tp> >, 12 bytes
{
public:
    _Tp* _M_start;                                   // +0x00
    _Tp* _M_finish;                                  // +0x04
    _Tp* _M_end_of_storage;                          // +0x08
    vector<_Tp>& operator=(const vector<_Tp>& __x);  // 0x006741E0 for pair<unsigned long, CTextureMorph> (out of line)
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

template <class K, class V, class C> class CVectorMap
{
public:
    std::vector<std::pair<K, V> > Vector;          // +0x00
    C                               Compare;         // +0x0C
    bool                            Dirty;           // +0x0D
};

class CRandomAppearanceMorph
{
public:
    class CTextureMorph
    {
    public:
        unsigned long NewTexture;                    // +0x00
        unsigned long GroupID;                       // +0x04
    };
    typedef CVectorMap<unsigned long, CTextureMorph, CKeyPairCompareLess<unsigned long, CTextureMorph> > TTextureMorphMap;

    class CBodyPartMesh
    {
    public:
        long             MeshIndex;                  // +0x00
        long             Priority;                   // +0x04
        TTextureMorphMap TextureMorphs;              // +0x08
    };
};

typedef CRandomAppearanceMorph::CBodyPartMesh TBodyPartMesh;

template TBodyPartMesh* __fastcall std::__copy<TBodyPartMesh*, TBodyPartMesh*, int>(TBodyPartMesh*, TBodyPartMesh*,
    TBodyPartMesh*, const std::random_access_iterator_tag&, int*);