// CRandomAppearanceMorph::AddTextureMorphToBodyPart(unsigned long bodyPart, unsigned long originalTexture, unsigned long newTexture, unsigned long groupID) -- add a texture morph (new texture + group) for one texture of a body part's morph map, and raise the max texture group id
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0086BCC0, length 82 bytes (`ret 10h`, an 8-byte stack local).
// Callees: the member that inserts a morph into a texture-morph map @ 0x0086BC10 (manifest "TextureMorph_AddToList", a
// description: thiscall on `this` with the map, the key and the morph by reference).
// Layout: retail container sizes: BodyPartTextureMorphs (CArray of 16-byte CVectorMap<unsigned long, CTextureMorph>) at
// +0x0c, MaxTextureGroupID +0x38; CTextureMorph {NewTexture, GroupID} (Ego_r.pdb).
// Shape: the morph is a named local built from the last two arguments, the map is `BodyPartTextureMorphs[bodyPart]`
// (`shl eax,4 ; add eax,[esi+0Ch]`), and the max is an inline value ternary with the member on the left
// (`MaxTextureGroupID > groupID ? MaxTextureGroupID : groupID`: `cmp eax,edi ; jbe`, the self-store kept, the two exits
// tail-duplicated). A const-reference max template re-reads the argument through its address, and `groupID < Max` swaps
// the compare operands (`cmp edi,eax ; jae`).
// Leaf: AddTextureMorphToBodyPart
template<class T> class CArray { public:
    T* m_first; T* m_last; T* m_end;
    T& operator[](unsigned long index) { return m_first[index]; }
};
struct CKeyPairCompareLess {};
template<class K, class V> class CVectorMap { public:
    CArray<V> m_vector; CKeyPairCompareLess m_compare; bool m_sorted;
};

struct CBodyPartMesh;
struct CSkeletalMorph;

class CRandomAppearanceMorph { public:
    class CTextureMorph { public:
        unsigned long NewTexture;                       // +0x00
        unsigned long GroupID;                          // +0x04
    };
    typedef CVectorMap<unsigned long, CTextureMorph> CTextureMorphMap;

    CArray<CArray<CBodyPartMesh> >  BodyParts;          // +0x00
    CArray<CTextureMorphMap>        BodyPartTextureMorphs;   // +0x0c
    CVectorMap<long, CSkeletalMorph> SkeletalMorphs;    // +0x18
    CTextureMorphMap                GeneralTextureMorphs;    // +0x28
    unsigned long                   MaxTextureGroupID;  // +0x38

    void AddTextureMorph(CTextureMorphMap& morphs, unsigned long originalTexture, const CTextureMorph& morph);   // 0x0086BC10
    void AddTextureMorphToBodyPart(unsigned long bodyPart, unsigned long originalTexture, unsigned long newTexture, unsigned long groupID);
};

void CRandomAppearanceMorph::AddTextureMorphToBodyPart(unsigned long bodyPart, unsigned long originalTexture, unsigned long newTexture, unsigned long groupID)
{
    CTextureMorph morph;
    morph.NewTexture = newTexture;
    morph.GroupID = groupID;
    AddTextureMorph(BodyPartTextureMorphs[bodyPart], originalTexture, morph);
    MaxTextureGroupID = MaxTextureGroupID > groupID ? MaxTextureGroupID : groupID;
}