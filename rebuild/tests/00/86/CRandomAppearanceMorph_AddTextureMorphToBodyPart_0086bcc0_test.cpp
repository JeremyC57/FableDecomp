// Behaviour test for CRandomAppearanceMorph::AddTextureMorphToBodyPart @ 0x0086BCC0: the morph goes to the indexed map, the max id rises.
#include <cstdio>

template<class T> struct CArray {
    T* m_first; T* m_last; T* m_end;
    T& operator[](unsigned long index) { return m_first[index]; }
};
struct CTextureMorph { unsigned long NewTexture, GroupID; };
struct CTextureMorphMap { unsigned long keys[8]; CTextureMorph morphs[8]; int count; };

struct CRandomAppearanceMorph {
    CArray<CTextureMorphMap> BodyPartTextureMorphs;
    unsigned long MaxTextureGroupID;
    void AddTextureMorph(CTextureMorphMap& morphs, unsigned long originalTexture, const CTextureMorph& morph)
    { morphs.keys[morphs.count] = originalTexture; morphs.morphs[morphs.count++] = morph; }
    void AddTextureMorphToBodyPart(unsigned long bodyPart, unsigned long originalTexture, unsigned long newTexture, unsigned long groupID)
    {
        CTextureMorph morph;
        morph.NewTexture = newTexture;
        morph.GroupID = groupID;
        AddTextureMorph(BodyPartTextureMorphs[bodyPart], originalTexture, morph);
        MaxTextureGroupID = MaxTextureGroupID > groupID ? MaxTextureGroupID : groupID;
    }
};

int main()
{
    static CTextureMorphMap maps[3];
    for (int i = 0; i < 3; ++i) maps[i].count = 0;
    CRandomAppearanceMorph m; m.BodyPartTextureMorphs.m_first = maps; m.BodyPartTextureMorphs.m_last = maps + 3; m.BodyPartTextureMorphs.m_end = maps + 3;
    m.MaxTextureGroupID = 4;
    m.AddTextureMorphToBodyPart(2, 100, 200, 2);     // lower group: max stays
    m.AddTextureMorphToBodyPart(1, 101, 201, 9);     // higher group: max rises
    m.AddTextureMorphToBodyPart(2, 102, 202, 9);     // equal: unchanged
    if (maps[2].count != 2 || maps[1].count != 1 || maps[0].count != 0 || maps[2].keys[0] != 100 || maps[2].morphs[0].NewTexture != 200 ||
        maps[2].morphs[0].GroupID != 2 || maps[1].morphs[0].GroupID != 9 || m.MaxTextureGroupID != 9) {
        std::printf("0086bcc0_TEST FAIL\n"); return 1;
    }
    std::printf("0086bcc0_TEST PASS\n");
    return 0;
}