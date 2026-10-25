// std::fill(CVectorMap<unsigned long, CRandomAppearanceMorph::CTextureMorph, CKeyPairCompareLess<unsigned long, CRandomAppearanceMorph::CTextureMorph> >* first, ...* last, const CVectorMap<...>& value) -- assign one texture-morph vector-map to every element of a range
// Leaf: ??$fill@PAV?$CVectorMap@KVCTextureMorph@CRandomAppearanceMorph@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0086D980, 43 bytes, `__fastcall` (first ecx, last edx, value
//   pointer on the stack), `ret 4`.
// Identity: 16-byte stride; each element is assigned through 0x006741E0 (the vector operator= over 12-byte
//   CKeyValuePair<unsigned long, CTextureMorph> elements, also called by the CBodyPartMesh fill 0x0086D9D0) and its
//   +0xd byte (Dirty) copied: the retail 16-byte CVectorMap<unsigned long, CRandomAppearanceMorph::CTextureMorph,
//   CKeyPairCompareLess<...> >, the element of CRandomAppearanceMorph::BodyPartTextureMorphs (Ego_r.pdb
//   CArray<CVectorMap<...> >). The manifest's "pair<CCharString, long>" is propagated. One caller, 0x0086E57A.
// Callees: vector<CKeyValuePair<unsigned long, CTextureMorph> >::operator= @ 0x006741E0.
// Shape: STLport fill; CVectorMap's implicit operator= inline (Data out of line, Dirty copied).

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
};
typedef CVectorMap<unsigned long, CRandomAppearanceMorph::CTextureMorph,
                   CKeyPairCompareLess<unsigned long, CRandomAppearanceMorph::CTextureMorph> > CTextureMorphMap;

namespace std {
// STLport 4.6 fill; the algorithm templates are __fastcall here (first/last in ecx/edx, callee pops the value pointer).
template <class _ForwardIter, class _Tp>
void __fastcall fill(_ForwardIter __first, _ForwardIter __last, const _Tp& __val)
{
    for ( ; __first != __last; ++__first)
        *__first = __val;
}
}

template void __fastcall std::fill<CTextureMorphMap*, CTextureMorphMap>(CTextureMorphMap*, CTextureMorphMap*, const CTextureMorphMap&);