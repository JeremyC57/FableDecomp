// Behaviour test for CRandomAppearanceMorph::AddTextureMorphToMesh (0x0086BD20): every mesh with the ID in all three
// body parts gets the morph {newTexture, groupID} under the original texture; other meshes are untouched; the max
// group id only grows.
#include <cstdio>
template<class T> struct CArray {
    T* m_first; T* m_last; T* m_end;
    unsigned long size() const { return m_last - m_first; }
    T& operator[](unsigned long index) { return m_first[index]; }
};
struct CTextureMorph { unsigned long NewTexture, GroupID; };
struct CTextureMorphMap { int Count; unsigned long LastKey; CTextureMorph Last; };
struct CBodyPartMesh { unsigned long MeshID; unsigned long _04; CTextureMorphMap TextureMorphs; };
struct CRandomAppearanceMorph {
    CArray<CArray<CBodyPartMesh> > BodyParts;
    unsigned long MaxTextureGroupID;
    void AddTextureMorph(CTextureMorphMap& morphs, unsigned long originalTexture, const CTextureMorph& morph)
    { ++morphs.Count; morphs.LastKey = originalTexture; morphs.Last = morph; }
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
int main()
{
    CBodyPartMesh head[2] = { { 7, 0, { 0, 0, { 0, 0 } } }, { 8, 0, { 0, 0, { 0, 0 } } } };
    CBodyPartMesh body[1] = { { 7, 0, { 0, 0, { 0, 0 } } } };
    CArray<CBodyPartMesh> parts[3] = { { head, head + 2, head + 2 }, { 0, 0, 0 }, { body, body + 1, body + 1 } };
    CRandomAppearanceMorph m; m.BodyParts.m_first = parts; m.BodyParts.m_last = parts + 3; m.MaxTextureGroupID = 5;
    m.AddTextureMorphToMesh(7, 100, 200, 3);
    int bad = 0;
    if (head[0].TextureMorphs.Count != 1 || body[0].TextureMorphs.Count != 1 || head[1].TextureMorphs.Count != 0) bad |= 1;
    if (head[0].TextureMorphs.LastKey != 100 || body[0].TextureMorphs.Last.NewTexture != 200 || body[0].TextureMorphs.Last.GroupID != 3) bad |= 2;
    if (m.MaxTextureGroupID != 5) bad |= 4;
    m.AddTextureMorphToMesh(8, 101, 201, 9);
    if (head[1].TextureMorphs.Count != 1 || head[0].TextureMorphs.Count != 1 || m.MaxTextureGroupID != 9) bad |= 8;
    m.AddTextureMorphToMesh(99, 1, 2, 4);
    if (m.MaxTextureGroupID != 9 || head[0].TextureMorphs.Count + head[1].TextureMorphs.Count + body[0].TextureMorphs.Count != 3) bad |= 16;
    if (bad) { std::printf("0086bd20_TEST FAIL\n"); return 1; }
    std::printf("0086bd20_TEST PASS\n");
    return 0;
}