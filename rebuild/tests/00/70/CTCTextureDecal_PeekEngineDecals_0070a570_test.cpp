// Behaviour test for 0x0070A570 CTCTextureDecal::PeekEngineDecals
// Model: when Dirty, the engine decal list is rebuilt from scratch: one engine decal per distinct decalled texture, in
// ascending texture order (map iteration), {OriginalTextureIndex = texture, DecalID = 0, OverlaidBlends = that
// texture's decal descs in list order}; Dirty is cleared. When clean the list is returned untouched.
#include <cstdio>
#include <list>
#include <map>
#include <vector>

namespace model {

struct Blend { unsigned int TextureIndex; int BlendType; unsigned int Diffuse; unsigned int Specular; };
struct EngineDecal {
    int OriginalTextureIndex;
    int DecalID;
    std::vector<Blend> OverlaidBlends;
    EngineDecal(int texture) : OriginalTextureIndex(texture), DecalID(0) {}
};
struct Entry { int EntryID; int TextureID; Blend DecalDesc; };

struct TextureDecal {
    std::list<Entry> Decals;
    std::vector<EngineDecal> EngineDecals;
    bool Dirty;
    const std::vector<EngineDecal>& PeekEngineDecals()
    {
        if (Dirty) {
            EngineDecals.clear();
            std::map<int, EngineDecal> decalMap;
            for (std::list<Entry>::iterator it = Decals.begin(); it != Decals.end(); ++it) {
                std::map<int, EngineDecal>::iterator found = decalMap.find(it->TextureID);
                if (found == decalMap.end()) {
                    EngineDecal decal(it->TextureID);
                    decal.OverlaidBlends.push_back(it->DecalDesc);
                    decalMap.insert(std::map<int, EngineDecal>::value_type(it->TextureID, decal));
                }
                else
                    found->second.OverlaidBlends.push_back(it->DecalDesc);
            }
            for (std::map<int, EngineDecal>::iterator it = decalMap.begin(); it != decalMap.end(); ++it)
                EngineDecals.push_back(it->second);
            Dirty = false;
        }
        return EngineDecals;
    }
};

Entry MakeEntry(int id, int texture, unsigned int blendTexture)
{
    Entry e;
    e.EntryID = id;
    e.TextureID = texture;
    e.DecalDesc.TextureIndex = blendTexture;
    e.DecalDesc.BlendType = 2;
    e.DecalDesc.Diffuse = 0xffffffffu;
    e.DecalDesc.Specular = 0;
    return e;
}

int run()
{
    TextureDecal t;
    t.Dirty = true;
    t.EngineDecals.push_back(EngineDecal(77));                 // stale entry, dropped by the rebuild
    t.Decals.push_back(MakeEntry(1, 30, 100));
    t.Decals.push_back(MakeEntry(2, 10, 101));
    t.Decals.push_back(MakeEntry(3, 30, 102));
    t.Decals.push_back(MakeEntry(4, 20, 103));
    t.Decals.push_back(MakeEntry(5, 10, 104));

    const std::vector<EngineDecal>& out = t.PeekEngineDecals();
    if (&out != &t.EngineDecals) return 1;
    if (t.Dirty) return 2;
    if (out.size() != 3) return 3;
    if (out[0].OriginalTextureIndex != 10 || out[1].OriginalTextureIndex != 20 || out[2].OriginalTextureIndex != 30) return 4;
    for (unsigned int i = 0; i < out.size(); ++i)
        if (out[i].DecalID != 0) return 5;
    if (out[0].OverlaidBlends.size() != 2 || out[0].OverlaidBlends[0].TextureIndex != 101 || out[0].OverlaidBlends[1].TextureIndex != 104) return 6;
    if (out[1].OverlaidBlends.size() != 1 || out[1].OverlaidBlends[0].TextureIndex != 103) return 7;
    if (out[2].OverlaidBlends.size() != 2 || out[2].OverlaidBlends[0].TextureIndex != 100 || out[2].OverlaidBlends[1].TextureIndex != 102) return 8;

    // Clean: a new decal is not picked up until the list is dirtied again.
    t.Decals.push_back(MakeEntry(6, 5, 105));
    if (t.PeekEngineDecals().size() != 3) return 9;
    t.Dirty = true;
    if (t.PeekEngineDecals().size() != 4 || t.EngineDecals[0].OriginalTextureIndex != 5) return 10;

    // Dirty with no decals: empty list.
    t.Decals.clear();
    t.Dirty = true;
    if (!t.PeekEngineDecals().empty() || t.Dirty) return 11;
    if ((unsigned int)sizeof(Blend) != 16u) return 12;
    return 0;
}

}

int main()
{
    int bad = model::run();
    if (bad) { std::printf("0070a570_TEST FAIL %d\n", bad); return 1; }
    std::printf("0070a570_TEST PASS\n");
    return 0;
}