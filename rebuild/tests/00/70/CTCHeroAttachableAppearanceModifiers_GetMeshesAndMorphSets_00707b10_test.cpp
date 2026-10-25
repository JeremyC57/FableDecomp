// Behaviour test for 0x00707B10 CTCHeroAttachableAppearanceModifiers::GetMeshesAndMorphSets
// Model: one (mesh info, morph set) pair appended per modifier graphic, in order; a graphic with a texture-replacement
// def gets one AddTextureMorph(original, {new texture, type 3, diffuse 0,0,0,255, specular 0}) per entry; a graphic
// without one gets an empty set; the mesh info is {BankIndex, RenderSizeX, PrimaryMesh = 1}.
#include <cstdio>
#include <vector>
#include <utility>

namespace model {

struct Colour { unsigned char B, G, R, A; };
struct Blend { unsigned int TextureIndex; int BlendType; Colour Diffuse; Colour Specular; };
struct Morph { int Original; Blend Layer; };
struct MorphSet {
    std::vector<Morph> Morphs;
    void AddTextureMorph(int original, const Blend& blend)   // recording stand-in for 0x006CA950
    {
        Morph m;
        m.Original = original;
        m.Layer = blend;
        Morphs.push_back(m);
    }
};
struct Entry { int OriginalTexture; int NewTexture; };
struct ReplacementDef { std::vector<Entry> Entries; };
struct Graphic { const ReplacementDef* PTextureReplacement; int BankIndex; float RenderSizeX; };
struct MeshInfo { int Index; float SizeX; unsigned char PrimaryMesh : 1; };
typedef std::pair<MeshInfo, MorphSet> MeshMorph;

void GetMeshesAndMorphSets(const std::vector<Graphic>& graphics, std::vector<MeshMorph>& out)
{
    for (unsigned int g = 0; g != graphics.size(); ++g) {
        const Graphic& it = graphics[g];
        MorphSet morphSet;
        if (it.PTextureReplacement) {
            for (int i = 0; i < (int)it.PTextureReplacement->Entries.size(); ++i) {
                const Entry& entry = it.PTextureReplacement->Entries[i];
                Blend b;
                b.TextureIndex = (unsigned int)entry.NewTexture;
                b.BlendType = 3;
                b.Diffuse.R = 0; b.Diffuse.G = 0; b.Diffuse.B = 0; b.Diffuse.A = 255;
                b.Specular.R = 0; b.Specular.G = 0; b.Specular.B = 0; b.Specular.A = 0;
                morphSet.AddTextureMorph(entry.OriginalTexture, b);
            }
        }
        MeshInfo meshInfo;
        meshInfo.Index = it.BankIndex;
        meshInfo.SizeX = it.RenderSizeX;
        meshInfo.PrimaryMesh = 1;
        out.push_back(std::make_pair(meshInfo, morphSet));
    }
}

int run()
{
    ReplacementDef two;
    Entry e1 = { 10, 110 };
    Entry e2 = { 20, 120 };
    two.Entries.push_back(e1);
    two.Entries.push_back(e2);
    ReplacementDef empty;
    std::vector<Graphic> graphics;
    Graphic g1 = { &two, 7, 1.5f };
    Graphic g2 = { 0, 8, 2.0f };
    Graphic g3 = { &empty, 9, 0.25f };
    graphics.push_back(g1);
    graphics.push_back(g2);
    graphics.push_back(g3);

    std::vector<MeshMorph> out;
    MeshInfo pre = { 99, 3.0f, 0 };
    out.push_back(std::make_pair(pre, MorphSet()));
    GetMeshesAndMorphSets(graphics, out);

    if (out.size() != 4) return 1;
    if (out[0].first.Index != 99 || !out[0].second.Morphs.empty()) return 2;          // existing entries kept
    if (out[1].first.Index != 7 || out[1].first.SizeX != 1.5f || out[1].first.PrimaryMesh != 1) return 3;
    if (out[2].first.Index != 8 || out[2].first.SizeX != 2.0f || !out[2].second.Morphs.empty()) return 4;
    if (out[3].first.Index != 9 || out[3].first.SizeX != 0.25f || !out[3].second.Morphs.empty()) return 5;
    const std::vector<Morph>& m = out[1].second.Morphs;
    if (m.size() != 2 || m[0].Original != 10 || m[1].Original != 20) return 6;
    if (m[0].Layer.TextureIndex != 110 || m[1].Layer.TextureIndex != 120) return 7;
    for (unsigned int i = 0; i < 2; ++i) {
        const Blend& b = m[i].Layer;
        if (b.BlendType != 3) return 8;
        if (b.Diffuse.R != 0 || b.Diffuse.G != 0 || b.Diffuse.B != 0 || b.Diffuse.A != 255) return 9;
        if (b.Specular.R != 0 || b.Specular.G != 0 || b.Specular.B != 0 || b.Specular.A != 0) return 10;
    }
    if ((unsigned int)sizeof(Blend) != 16u) return 11;

    std::vector<Graphic> none;
    GetMeshesAndMorphSets(none, out);                                                  // no graphics: unchanged
    if (out.size() != 4) return 12;
    return 0;
}

}

int main()
{
    int bad = model::run();
    if (bad) { std::printf("00707b10_TEST FAIL %d\n", bad); return 1; }
    std::printf("00707b10_TEST PASS\n");
    return 0;
}