// CRandomAppearanceMorph::GetRandomBodyParts(unsigned long& seed, CMorphSet& morphSet, bool skipFirstPart) const -- pick a random texture-morph variant, then for each of the three body-part slots pick a random mesh, gather its texture morphs plus the slot's own, and add the mesh/texture set to the morph set (the first slot optionally skipped)
// Grade: DIFFER(294v294) -- dl=2: set zero stores sit after the je (set inside the if) vs retail before it; set before the if forms the element address first (dl=6)
// Leaf: ?GetRandomBodyParts@CRandomAppearanceMorph@@IBEXAAKAAVCMorphSet@1@_N@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x0086BE40, 294 bytes, `ret 0Ch`. Ego_r.pdb / FableWin.pdb
//   ?GetRandomBodyParts@CRandomAppearanceMorph@@IBEXAAKAAVCMorphSet@1@_N@Z (protected const).
// Callees: GFROR13 0x00497890 (?GFROR13@@YIJK@Z, inside the inline GFRandom), CRandomAppearanceMorph::GetRandomTextureMorphs
//   0x0086B810 (Ego_r.pdb ?GetRandomTextureMorphs@CRandomAppearanceMorph@@IBEXAAKKABV?$CVectorMap@...@@AAV?$CArray@...@@@Z, twice),
//   vector<CMeshTextureSet>::push_back 0x0086E7C0 (out of line in this TU), free 0x00BFEA14 (the set's CArray destructor).
// Layout: Ego_r.pdb with STLport's 12-byte vectors (CArray 12, CVectorMap 16): BodyParts +0x00, BodyPartTextureMorphs +0x0c,
//   SkeletalMorphs +0x18, GeneralTextureMorphs +0x28, MaxTextureGroupID +0x38; CBodyPartMesh (24): MeshIndex +0, Priority +4,
//   TextureMorphs +8; CMorphSet::CMeshTextureSet: MeshIndex +0, TextureMorphs +4.
// Shape: CArray::operator[] through begin() (FABLE_RETAIL_CARRAY): the slot's empty() test folds the element offset into its
//   two loads before the element address is formed.
#define FABLE_RETAIL_STLP_VECTOR_PUSHBACK_OUTOFLINE
#define FABLE_RETAIL_CARRAY
#include "engine/CArray.h"
#include "engine/CVectorMap.h"

long __fastcall GFROR13(unsigned long value);        // 0x00497890 ?GFROR13@@YIJK@Z
__forceinline unsigned long __fastcall GFRandom(unsigned long range, unsigned long* seed)   // ?GFRandom@@YIKKPAK@Z (inline in retail)
{
    *seed = *seed * 0x24A1 + 0x24DF;
    *seed = GFROR13(*seed);
    return range ? *seed % range : 0;
}

class CRandomAppearanceMorph {
public:
    class CTextureMorph { public: unsigned long NewTexture; unsigned long GroupID; };
    class CSkeletalMorph { public: long Data[2]; };
    class CBodyPartMesh {                            // 24 bytes
    public:
        long MeshIndex;                              // +0x00
        long Priority;                               // +0x04
        CVectorMap<unsigned long, CTextureMorph> TextureMorphs;   // +0x08
    };
    class CMorphSet {
    public:
        class CMorphSetTextureMorph { public: long OriginalTexture; long NewTexture; };
        class CMeshTextureSet {
        public:
            long MeshIndex;                          // +0x00
            CArray<CMorphSetTextureMorph> TextureMorphs;          // +0x04
        };
        CArray<CMeshTextureSet> BodyParts;           // +0x00
    };
protected:
    void GetRandomBodyParts(unsigned long& seed, CMorphSet& morphSet, bool skipFirstPart) const;
    void GetRandomTextureMorphs(unsigned long& seed, unsigned long variant, const CVectorMap<unsigned long, CTextureMorph>& morphs,
                                CArray<CMorphSet::CMorphSetTextureMorph>& out) const;      // 0x0086B810
    CArray<CArray<CBodyPartMesh> >                      BodyParts;               // +0x00
    CArray<CVectorMap<unsigned long, CTextureMorph> >   BodyPartTextureMorphs;   // +0x0c
    CVectorMap<long, CSkeletalMorph>                    SkeletalMorphs;          // +0x18
    CVectorMap<unsigned long, CTextureMorph>            GeneralTextureMorphs;    // +0x28
    unsigned long                                       MaxTextureGroupID;       // +0x38
};

void CRandomAppearanceMorph::GetRandomBodyParts(unsigned long& seed, CMorphSet& morphSet, bool skipFirstPart) const
{
    unsigned long variant = 0;
    if (MaxTextureGroupID)
        variant = GFRandom(MaxTextureGroupID, &seed) + 1;
    for (long part = 0; part != 3; ++part) {
        if (!BodyParts[part].empty()) {
            CMorphSet::CMeshTextureSet set;
            const CBodyPartMesh& mesh = BodyParts[part][GFRandom(BodyParts[part].size(), &seed)];
            set.MeshIndex = mesh.MeshIndex;
            GetRandomTextureMorphs(seed, variant, mesh.TextureMorphs, set.TextureMorphs);
            GetRandomTextureMorphs(seed, variant, BodyPartTextureMorphs[part], set.TextureMorphs);
            if (!skipFirstPart || part != 0)
                morphSet.BodyParts.push_back(set);
        }
    }
}
