// _STL::fill(CRandomAppearanceMorph::CBodyPartMesh* first, CRandomAppearanceMorph::CBodyPartMesh* last, const CRandomAppearanceMorph::CBodyPartMesh& value) -- assign one body-part mesh (index, priority, texture-morph map) to every element of a range
// Leaf: ??$fill@PAVCBodyPartMesh@CRandomAppearanceMorph@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0086D9D0, 58 bytes, `__fastcall` (first ecx, last edx, value
//   pointer on the stack), `ret 4`.
// Identity: 24-byte stride = {long, long, 16-byte CVectorMap}: the member at +8 is assigned through 0x006741E0,
//   a vector operator= over 12-byte elements (divide by 12), and its +0xd byte (CVectorMap::Dirty) is copied
//   after it. That is Ego_r.pdb CRandomAppearanceMorph::CBodyPartMesh {long MeshIndex; long Priority;
//   CVectorMap<unsigned long, CTextureMorph, CKeyPairCompareLess<...> > TextureMorphs} with the retail 16-byte
//   CVectorMap (CKeyValuePair<unsigned long, CTextureMorph{NewTexture, GroupID}> = 12 bytes); the neighbouring
//   GFReadVectorMapMemDump<CRandomAppearanceMorph::CTextureMorph, ...> 0x0086DEB0 and the CVectorMap fill
//   0x0086D980 belong to the same family. The manifest's "CSharerParserOutputToDebugContextMapper::CEntry" is
//   propagated (identical-shape body). One caller, 0x0086E6CC.
// Callees: vector<CKeyValuePair<unsigned long, CTextureMorph> >::operator= @ 0x006741E0.
// Shape: STLport fill with the implicit operator= inline: the two longs by register, then CVectorMap's implicit
//   operator= (Data assigned out of line, the empty comparator skipped, Dirty copied); &val.TextureMorphs hoisted.

template <class T> class CVector {                       // STLport vector, 12 bytes
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    CVector& operator=(const CVector& other);            // out of line
};
template <class K, class V> struct CKeyValuePair { K first; V second; };
template <class K, class V> class CKeyPairCompareLess { };
template <class K, class V, class C> class CVectorMap {  // retail 16 bytes
public:
    CVector<CKeyValuePair<K, V> > Data;                  // +0x00
    C                             Compare;               // +0x0c (empty)
    bool                          Dirty;                 // +0x0d
};

class CRandomAppearanceMorph {
public:
    class CTextureMorph {                                // 8 bytes
    public:
        unsigned long NewTexture;
        unsigned long GroupID;
    };
    class CBodyPartMesh {                                // 24 bytes
    public:
        long MeshIndex;                                  // +0x00
        long Priority;                                   // +0x04
        CVectorMap<unsigned long, CTextureMorph, CKeyPairCompareLess<unsigned long, CTextureMorph> > TextureMorphs;   // +0x08
    };
};

namespace _STL {
// STLport 4.6 fill; the algorithm templates are __fastcall here (first/last in ecx/edx, callee pops the value pointer).
template <class _ForwardIter, class _Tp>
void __fastcall fill(_ForwardIter __first, _ForwardIter __last, const _Tp& __val)
{
    for ( ; __first != __last; ++__first)
        *__first = __val;
}
}

template void __fastcall _STL::fill<CRandomAppearanceMorph::CBodyPartMesh*, CRandomAppearanceMorph::CBodyPartMesh>(
    CRandomAppearanceMorph::CBodyPartMesh*, CRandomAppearanceMorph::CBodyPartMesh*, const CRandomAppearanceMorph::CBodyPartMesh&);