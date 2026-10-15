// CRandomAppearanceMorph::GetRandomMorphSet(unsigned long& seed, CMorphSet& morphSet, bool flag) -- roll a complete random morph set: skeletal morphs, body parts, then the general texture morphs
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0086BF70, length 58 bytes (`ret 0Ch`). Called from 0x00728181
// (CTCRandomAppearanceMorph). Callees: GetRandomSkeletalMorph @ 0x0086B9D0, GetRandomBodyParts @ 0x0086BE40,
// GetRandomTextureMorphs @ 0x0086B810 (all CRandomAppearanceMorph members; prototypes from the manifest).
// Layout: retail container sizes (CArray = 12 bytes, CVectorMap = 16): the class's GeneralTextureMorphs map at +0x28,
// CMorphSet's GeneralTextureMorphs array at +0x18 (BodyParts +0x00, SkeletalMorphs +0x0c; the PDB's 16-byte CArrays put
// it at +0x20).
// Shape: three member calls forwarding the seed and the set; the last passes group 0, the class's general texture-morph map
// (`lea ecx,[esi+28h]`) and the set's array (`add edi,18h` on the set register, no longer needed).
// Leaf: GetRandomMorphSet
template<class T> struct CArray { T* m_first; T* m_last; T* m_end; };
struct CKeyPairCompareLess {};
template<class K, class V> struct CVectorMap {
    CArray<V> m_vector; CKeyPairCompareLess m_compare; bool m_sorted;
};

struct CBodyPartMesh;
struct CSkeletalMorph;
struct CTextureMorph;

struct CMorphSet {
    struct CMeshTextureSet;
    struct CMorphSetSkeletalMorph;
    struct CMorphSetTextureMorph;
    CArray<CMeshTextureSet>        BodyParts;           // +0x00
    CArray<CMorphSetSkeletalMorph> SkeletalMorphs;      // +0x0c
    CArray<CMorphSetTextureMorph>  GeneralTextureMorphs;// +0x18
};

struct CRandomAppearanceMorph {
    CArray<CArray<CBodyPartMesh> >                    BodyParts;               // +0x00
    CArray<CVectorMap<unsigned long, CTextureMorph> > BodyPartTextureMorphs;   // +0x0c
    CVectorMap<long, CSkeletalMorph>                  SkeletalMorphs;          // +0x18
    CVectorMap<unsigned long, CTextureMorph>          GeneralTextureMorphs;    // +0x28
    unsigned long                                     MaxTextureGroupID;       // +0x38

    void GetRandomSkeletalMorph(unsigned long& seed, CMorphSet& morphSet);                     // 0x0086B9D0
    void GetRandomBodyParts(unsigned long& seed, CMorphSet& morphSet, bool flag);              // 0x0086BE40
    void GetRandomTextureMorphs(unsigned long& seed, unsigned long groupID,
                                CVectorMap<unsigned long, CTextureMorph>& textureMorphs,
                                CArray<CMorphSet::CMorphSetTextureMorph>& result);             // 0x0086B810
    void GetRandomMorphSet(unsigned long& seed, CMorphSet& morphSet, bool flag);
};

void CRandomAppearanceMorph::GetRandomMorphSet(unsigned long& seed, CMorphSet& morphSet, bool flag)
{
    GetRandomSkeletalMorph(seed, morphSet);
    GetRandomBodyParts(seed, morphSet, flag);
    GetRandomTextureMorphs(seed, 0, GeneralTextureMorphs, morphSet.GeneralTextureMorphs);
}