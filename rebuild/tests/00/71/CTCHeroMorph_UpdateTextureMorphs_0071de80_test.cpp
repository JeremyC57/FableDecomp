// 0071de80 behaviour test: CTCHeroMorph::UpdateTextureMorphs (final.cpp body over standard containers)
#include <stdio.h>
#include <vector>
#include <map>
#include <algorithm>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { printf("FAIL line %d: %s\n", __LINE__, #c); g_fail = 1; } } while (0)
struct CTextureMorph { int NameIndex; short Morph; bool OverridesLower; float Strength; };
struct CTextureMorphLess { bool operator()(const CTextureMorph& a, const CTextureMorph& b) const { return a.Morph < b.Morph; } };
struct CCompositeTextureEntry { float Priority; int Key; int Data; };
struct CCompositeTextureLess { bool operator()(const CCompositeTextureEntry& a, const CCompositeTextureEntry& b) const { return a.Priority < b.Priority; } };
struct CCompositeTextureList { std::vector<CCompositeTextureEntry> Entries; bool NeedsHeap; CCompositeTextureList() : NeedsHeap(false) {} };
struct CTextureMapSet { std::map<int, int> Maps; int Clears; void Clear() { Maps.clear(); ++Clears; } void Set(int k, int d) { Maps[k] = d; } };
struct CTCTextureMorphs {
    std::vector<int> Names; std::vector<float> Strengths; int Removed;
    void RemoveMorphs(int) { ++Removed; Names.clear(); Strengths.clear(); }
    void AddMorph(int, int name, float s) { Names.push_back(name); Strengths.push_back(s); }
};
static const float g_fEpsilon = 1e-4f;
class CTCHeroMorph { public:
    CTextureMapSet Maps; CTCTextureMorphs* Textures; float Strengths[0x13]; int Particles; int Kills;
    float GetMorphStrength(int type) { return Strengths[type]; }
    void KillParticles() { ++Kills; }
    void AddParticles(int, float) { ++Particles; }
    void AddCompositeTexture(int type, float s, CCompositeTextureList* list)
    {
        CCompositeTextureEntry e = { (float)(0x13 - type), type, (int)(s * 100) };
        list->Entries.push_back(e); list->NeedsHeap = true;
    }
    void UpdateTattoos(CCompositeTextureList* list) { CCompositeTextureEntry e = { 50.0f, 99, 7 }; list->Entries.push_back(e); }
    void AddMorph(int type, float s, std::vector<CTextureMorph>* out)
    {
        CTextureMorph m = { 1000 + type, (short)(0x13 - type), type == 2, s };
        out->push_back(m);
    }
    void AdjustMorphsOfLowerPriority(unsigned index, std::vector<CTextureMorph>* morphs, int, float amount)
    {
        for (unsigned i = 0; i < index; ++i) (*morphs)[i].Strength *= amount;
    }
    void UpdateTextureMorphs();
};
void CTCHeroMorph::UpdateTextureMorphs()
{
    CTextureMapSet& maps = Maps;
    std::vector<CTextureMorph> morphs;
    maps.Clear();
    KillParticles();
    CCompositeTextureList composites;
    for (int type = 0; type < 0x13; ++type) {
        float strength = GetMorphStrength(type);
        if (strength > g_fEpsilon) {
            AddCompositeTexture(type, strength, &composites);
            AddMorph(type, strength, &morphs);
            AddParticles(type, strength);
        }
    }
    UpdateTattoos(&composites);
    if (composites.NeedsHeap)
        std::make_heap(composites.Entries.begin(), composites.Entries.end(), CCompositeTextureLess());
    for (unsigned i = 0; i < composites.Entries.size(); ++i)
        maps.Set(composites.Entries[i].Key, composites.Entries[i].Data);
    if (Textures) {
        Textures->RemoveMorphs('HERO');
        std::sort(morphs.begin(), morphs.end(), CTextureMorphLess());
        unsigned j = 1;
        unsigned n = morphs.size();
        for (; j < n; ++j) {
            CTextureMorph& m = morphs[j];
            if (m.OverridesLower)
                AdjustMorphsOfLowerPriority(j, &morphs, m.Morph, 1.0f - m.Strength);
        }
        for (unsigned k = 0; k < n; ++k)
            Textures->AddMorph('HERO', morphs[k].NameIndex, morphs[k].Strength);
    }
}
int main()
{
    CTCTextureMorphs tex; tex.Removed = 0;
    CTCHeroMorph h; h.Textures = &tex; h.Particles = 0; h.Kills = 0; h.Maps.Clears = 0;
    for (int i = 0; i < 0x13; ++i) h.Strengths[i] = 0.0f;
    h.Strengths[2] = 0.25f; h.Strengths[5] = 0.5f; h.Strengths[7] = 1.0f;
    h.UpdateTextureMorphs();
    CHECK(h.Kills == 1 && h.Particles == 3 && h.Maps.Clears == 1);
    CHECK(h.Maps.Maps.size() == 4 && h.Maps.Maps[5] == 50 && h.Maps.Maps[99] == 7);
    CHECK(tex.Removed == 1 && tex.Names.size() == 3);
    // sorted by Morph (0x13 - type): types 7, 5, 2; type 2 overrides lower (scales the first two by 0.75)
    CHECK(tex.Names[0] == 1007 && tex.Names[1] == 1005 && tex.Names[2] == 1002);
    CHECK(tex.Strengths[0] == 0.75f && tex.Strengths[1] == 0.375f && tex.Strengths[2] == 0.25f);
    h.Textures = 0;
    h.UpdateTextureMorphs();
    CHECK(h.Maps.Clears == 2 && tex.Removed == 1);
    if (!g_fail) printf("0071de80_TEST PASS\n");
    return g_fail;
}