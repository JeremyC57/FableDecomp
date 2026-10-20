// CRandomAppearanceMorph::AddTextureMorphToMesh(unsigned long, unsigned long, unsigned long, unsigned long) -- add a texture morph (new texture + group) for one texture of every body-part mesh with the given mesh ID, and raise the max texture group id
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0086BD20, length 185 bytes, `ret 10h` (an 8-byte stack morph).
// Leaf: AddTextureMorphToMesh
// The per-mesh sibling of the landed AddTextureMorphToBodyPart (0x0086BCC0): same morph local, same callee
// (0x0086BC10, the protected insert into a texture-morph map; PDB AddTextureToTextureMorph, the landed sibling's
// AddTextureMorph name kept), same tail-duplicated max.
// Layout: retail container sizes (12-byte CArray, 16-byte CVectorMap): BodyParts (three CArray<CBodyPartMesh>, one per
// body part) +0, BodyPartTextureMorphs +0x0c, SkeletalMorphs +0x18, GeneralTextureMorphs +0x28, MaxTextureGroupID
// +0x38; CBodyPartMesh is 24 bytes: the mesh ID +0 and its texture-morph map +8 (`imul 2AAAAAABh ; sar 2` = /24).
// Shape: the body parts are walked by a counted `for` over the three slots (`cmp edi,24h`), the meshes by an unsigned
// index against a size() recomputed every pass, both through `operator[]` on a part reference; the morph is built
// inside the hit block.
template<class T> struct CArray {
    T* m_first; T* m_last; T* m_end;
    unsigned long size() const { return m_last - m_first; }
    T& operator[](unsigned long index) { return m_first[index]; }
};
struct CKeyPairCompareLess {};
template<class K, class V> struct CVectorMap {
    CArray<V> m_vector; CKeyPairCompareLess m_compare; bool m_sorted;
};
struct CSkeletalMorph;

struct CRandomAppearanceMorph {
    struct CTextureMorph {
        unsigned long NewTexture;                       // +0x00
        unsigned long GroupID;                          // +0x04
    };
    typedef CVectorMap<unsigned long, CTextureMorph> CTextureMorphMap;
    struct CBodyPartMesh {                              // 24 bytes
        unsigned long    MeshID;                        // +0x00
        unsigned long    _04;                           // +0x04
        CTextureMorphMap TextureMorphs;                 // +0x08
    };

    CArray<CArray<CBodyPartMesh> >  BodyParts;          // +0x00
    CArray<CTextureMorphMap>        BodyPartTextureMorphs;   // +0x0c
    CVectorMap<long, CSkeletalMorph> SkeletalMorphs;    // +0x18
    CTextureMorphMap                GeneralTextureMorphs;    // +0x28
    unsigned long                   MaxTextureGroupID;  // +0x38

    void AddTextureMorph(CTextureMorphMap& morphs, unsigned long originalTexture, const CTextureMorph& morph);   // 0x0086BC10
    void AddTextureMorphToMesh(unsigned long meshID, unsigned long originalTexture, unsigned long newTexture, unsigned long groupID);
};

void CRandomAppearanceMorph::AddTextureMorphToMesh(unsigned long meshID, unsigned long originalTexture, unsigned long newTexture, unsigned long groupID)
{
    for (long part = 0; part < 3; ++part) {
        CArray<CBodyPartMesh>& meshes = BodyParts[part];
        for (unsigned long i = 0; i < meshes.size(); ++i) {
            if (meshes[i].MeshID == meshID) {
                CTextureMorph morph;
                morph.NewTexture = newTexture;
                morph.GroupID = groupID;
                AddTextureMorph(meshes[i].TextureMorphs, originalTexture, morph);
            }
        }
    }
    MaxTextureGroupID = MaxTextureGroupID > groupID ? MaxTextureGroupID : groupID;
}
