// CRandomAppearanceMorph::CRandomAppearanceMorph() -- empty morph tables, then three body-part slots in both per-body-part arrays
// Grade: RELOCATION_MATCH (retry 10, RTM 3077) -- CArray modelled with the STLport alloc-proxy end-of-storage member (the `lea edi,[esi+0Ch]` lands between the +0x1c and +0x20 zero stores as retail)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0086C4D0, length 106 bytes.
// Callees: CArray<CArray<CBodyPartMesh> >::resize(n, const T&) @ 0x0086EDE0, ~CArray<CBodyPartMesh> @ 0x006755A0
// (on the default-value temporary), CArray<CVectorMap<ulong,CTextureMorph> >::resize(n) @ 0x0086EA80 (the catalog
// labels of all three are propagated from other vector instantiations).
// Layout (Ego_r.pdb names, retail offsets: retail CArray<T> is 12 bytes {first, last, end} where the PDB's is 16):
// BodyParts +0x00, BodyPartTextureMorphs +0x0c, CVectorMap SkeletalMorphs +0x18 {vector +0x18, Compare +0x24,
// bool Dirty +0x25}, CVectorMap GeneralTextureMorphs +0x28 {vector, Compare +0x34, Dirty +0x35},
// MaxTextureGroupID +0x38.
// Shape: every member is zero-initialised in declaration order with one zero register (the empty comparator has a
// user constructor, so +0x24/+0x34 are not stored); the first resize takes an explicit default-constructed
// temporary (built in the frame, destroyed out of line), the second is the one-argument resize.
// Leaf: ??0CRandomAppearanceMorph
template <class T> struct CArrayAllocProxy {   // STLport _STLP_alloc_proxy: the end-of-storage pointer with its own constructor
    T* p;
    CArrayAllocProxy(T* v) : p(v) { }
};
template <class T> struct CArray {
    T* m_first;                                         // +0
    T* m_last;                                          // +4
    CArrayAllocProxy<T> m_end;                          // +8
    CArray() : m_first(0), m_last(0), m_end(0) {}
    ~CArray();
    void resize(unsigned long n, const T& value);
    void resize(unsigned long n);
};

template <class K, class V> struct CKeyPairCompareLess {
    CKeyPairCompareLess() {}
};

template <class K, class V, class C> struct CVectorMap {
    CArray<V> Values;                                   // +0x00 (the key/value pairs; modelled by value type)
    C         Compare;                                  // +0x0c
    bool      Dirty;                                    // +0x0d
    CVectorMap() : Dirty(false) {}
};

struct CRandomAppearanceMorph {
    struct CBodyPartMesh;
    struct CTextureMorph;
    struct CSkeletalMorph;
    typedef CVectorMap<unsigned long, CTextureMorph, CKeyPairCompareLess<unsigned long, CTextureMorph> > CTextureMorphMap;
    CArray<CArray<CBodyPartMesh> >                                                      BodyParts;              // +0x00
    CArray<CTextureMorphMap>                                                            BodyPartTextureMorphs;  // +0x0c
    CVectorMap<long, CSkeletalMorph, CKeyPairCompareLess<long, CSkeletalMorph> >        SkeletalMorphs;         // +0x18
    CTextureMorphMap                                                                    GeneralTextureMorphs;   // +0x28
    unsigned long                                                                       MaxTextureGroupID;      // +0x38
    CRandomAppearanceMorph();
};

CRandomAppearanceMorph::CRandomAppearanceMorph()
    : MaxTextureGroupID(0)
{
    BodyParts.resize(3, CArray<CBodyPartMesh>());
    BodyPartTextureMorphs.resize(3);
}