#include <cstdio>
// behaviour test: AddCompositeTexture does nothing below strength 1e-4 or without a morph def / matching entries; otherwise every
// entry of the type becomes a (layer -> {replacing texture, blend(bank, white diffuse with alpha = strength*255, black specular)})
// item of the map, which is flagged dirty
struct CRGBColour { unsigned char B, G, R, A; };
struct CCompositeBlendDesc { unsigned long TextureIndex; long BlendType; CRGBColour DiffuseColour, SpecularColour; };
struct CTextureMorphEntry { int MorphType; long TextureLayer; long ReplacingTextureIndex; unsigned long BankIndex; long Blend; };
struct CTextureLayer { long TextureID; CCompositeBlendDesc Blend; };
struct CItem { long first; CTextureLayer second; };
struct CMap { CItem items[8]; int n; bool Dirty; };
struct CHeroMorphDef { CTextureMorphEntry entries[4]; int count; };
static CHeroMorphDef* g_def = 0;
static void AddCompositeTexture(int type, float strength, CMap& layers)
{
    if (!(strength > 1e-4f) || !g_def)
        return;
    CTextureMorphEntry found[4]; int n = 0;
    for (int i = 0; i < g_def->count; ++i)
        if (g_def->entries[i].MorphType == type) found[n++] = g_def->entries[i];
    if (n == 0)
        return;
    CRGBColour diffuse = { 0xff, 0xff, 0xff, (unsigned char)(strength * 255.0f) };
    CRGBColour specular = { 0, 0, 0, 0 };
    for (int i = 0; i < n; ++i) {
        CItem& it = layers.items[layers.n++];
        it.first = found[i].TextureLayer;
        it.second.TextureID = found[i].ReplacingTextureIndex;
        it.second.Blend.TextureIndex = found[i].BankIndex;
        it.second.Blend.BlendType = found[i].Blend;
        it.second.Blend.DiffuseColour = diffuse;
        it.second.Blend.SpecularColour = specular;
        layers.Dirty = true;
    }
}
int main()
{
    CHeroMorphDef def = { { { 1, 10, 100, 7, 2 }, { 2, 11, 101, 8, 3 }, { 1, 12, 102, 9, 4 } }, 3 };
    CMap m; m.n = 0; m.Dirty = false;
    AddCompositeTexture(1, 0.5f, m);                     // no def
    bool ok = m.n == 0 && !m.Dirty;
    g_def = &def;
    AddCompositeTexture(1, 0.00005f, m);                 // too weak
    ok = ok && m.n == 0;
    AddCompositeTexture(3, 0.5f, m);                     // no entries of the type
    ok = ok && m.n == 0 && !m.Dirty;
    AddCompositeTexture(1, 0.5f, m);
    ok = ok && m.n == 2 && m.Dirty && m.items[0].first == 10 && m.items[1].first == 12 &&
         m.items[0].second.TextureID == 100 && m.items[1].second.Blend.TextureIndex == 9 && m.items[1].second.Blend.BlendType == 4 &&
         m.items[0].second.Blend.DiffuseColour.R == 0xff && m.items[0].second.Blend.DiffuseColour.A == 127 &&
         m.items[0].second.Blend.SpecularColour.A == 0;
    if (!ok) { std::printf("0071dbd0_TEST FAIL\n"); return 1; }
    std::printf("0071dbd0_TEST PASS\n");
    return 0;
}