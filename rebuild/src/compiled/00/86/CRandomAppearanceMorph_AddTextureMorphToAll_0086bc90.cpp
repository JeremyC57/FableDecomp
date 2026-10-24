// CRandomAppearanceMorph::AddTextureMorphToAll(unsigned long, unsigned long) -- add a texture morph (original texture -> new texture, no group) to the general texture morphs applied to every body part
// Leaf: ?AddTextureMorphToAll@CRandomAppearanceMorph@@QAEXKK@Z
// Retail 0x0086BC90, 0x2B bytes (+ int3 padding to 0x30), thiscall `ret 8`. FableWin/Ego_r name:
// ?AddTextureMorphToAll@CRandomAppearanceMorph@@QAEXKK@Z.
// Callee: CRandomAppearanceMorph::AddTextureToTextureMorph(CVectorMap<unsigned long, CTextureMorph>&, long, const
// CTextureMorph&) @ 0x0086BC10 (protected, out of line; `this` passed through in ecx).
// Shape: `AddTextureToTextureMorph(GeneralTextureMorphs, originalTexture, CTextureMorph(newTexture, 0))` -- the morph
// temporary is built in the frame (NewTexture stored between the pushes, GroupID = 0 last), GeneralTextureMorphs is
// `lea edx,[ecx+28h]`.
// Layout (retail): CRandomAppearanceMorph GeneralTextureMorphs +0x28 (Ego_r +0x34: BodyParts/BodyPartTextureMorphs are
// 12-byte vectors and SkeletalMorphs a 16-byte CVectorMap in retail); CTextureMorph {unsigned long NewTexture +0,
// unsigned long GroupID +4}.
// Manifest label "CRandomAppearanceMorph_AddTextureMorph" names the class but not the member.
template <class T> class CRetailVector               // STLport vector<T>, 12 bytes
{
public:
    T* _M_start;                                     // +0x00
    T* _M_finish;                                    // +0x04
    T* _M_end_of_storage;                            // +0x08
};

template <class K, class V> class CKeyPairCompareLess { };

template <class K, class V> struct SVectorMapPair    // std::pair<K, V>
{
    K first;
    V second;
};

template <class K, class V, class C> class CVectorMap
{
public:
    CRetailVector<SVectorMapPair<K, V> > Vector;     // +0x00
    C                                    Compare;    // +0x0C
    bool                                 Dirty;      // +0x0D
};

class CRandomAppearanceMorph
{
public:
    class CBodyPartMesh;
    class CSkeletalMorph;
    class CTextureMorph
    {
    public:
        unsigned long NewTexture;                    // +0x00
        unsigned long GroupID;                       // +0x04
        CTextureMorph(unsigned long newTexture, unsigned long groupID) : NewTexture(newTexture), GroupID(groupID) { }
    };
    typedef CVectorMap<long, CSkeletalMorph, CKeyPairCompareLess<long, CSkeletalMorph> > TSkeletalMorphMap;
    typedef CVectorMap<unsigned long, CTextureMorph, CKeyPairCompareLess<unsigned long, CTextureMorph> > TTextureMorphMap;

    CRetailVector<CRetailVector<CBodyPartMesh> > BodyParts;               // +0x00
    CRetailVector<TTextureMorphMap>              BodyPartTextureMorphs;   // +0x0C
    TSkeletalMorphMap                            SkeletalMorphs;          // +0x18
    TTextureMorphMap                             GeneralTextureMorphs;    // +0x28
    unsigned long                                MaxTextureGroupID;       // +0x38

    void AddTextureMorphToAll(unsigned long originalTexture, unsigned long newTexture);

protected:
    void AddTextureToTextureMorph(TTextureMorphMap& map, long key, const CTextureMorph& morph);   // 0x0086BC10
};

void CRandomAppearanceMorph::AddTextureMorphToAll(unsigned long originalTexture, unsigned long newTexture)
{
    AddTextureToTextureMorph(GeneralTextureMorphs, originalTexture, CTextureMorph(newTexture, 0));
}