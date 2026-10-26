// Behaviour test for 00757440: src/00757440.cpp copied verbatim; the decal component stubbed so the delete (weak
// wound with an entry), add (no entry) and change (entry) paths and the filled blend desc are observable.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CWound::UpdateDecal(CTCTextureDecal&, long&, long, long, float) -- keep one wound/scar decal in step with its strength: delete it once the strength falls to 0.05, else add it (first time) or change it with a white diffuse colour whose alpha is the strength
// Leaf: UpdateDecal
// Retry 10b (B-12 sweep): the alpha is a const REFERENCE to the converted byte (`const unsigned char& alpha = (unsigned char)(strength * 255.0f);`):
// _ftol2 runs before the TextureIndex/BlendType/colour stores (alpha in al, stores through ecx) as in retail; a value local converts after the stores.
// Retail: Fable.exe @ 0x00757440, 179 bytes, `ret 14h`. PDB ?UpdateDecal@CWound@CTCWound@@IAEXAAVCTCTextureDecal@@AAJJJM@Z
// (CTCWound::CWound, protected).
// Callees: CTCTextureDecal::DeleteDecal(long) 0x0070A430, CTCTextureDecal::AddDecal(long, CCompositeBlendDesc&)
// 0x0070A490 (manifest "CRenderTarget_EnqueueDecalUpdate"), CTCTextureDecal::ChangeDecal(long, CCompositeBlendDesc&)
// 0x0070A3E0, _ftol2 0x00BFEA70. Float pool 0x0125A15C = 0.05f, 0x01230014 = 255.0f.
// CCompositeBlendDesc (16 bytes, PDB): TextureIndex +0, BlendType +4, DiffuseColour +8, SpecularColour +0xC;
// CRGBColour (4 bytes, PDB): B +0, G +1, R +2, A +3.
// Shape: `this` is unused; the alpha is a const-reference temporary converted before the desc is filled; the blend desc is
// filled member by member and the colours through an inline Set(r, g, b, a)
// storing R, G, B, A (the retail +2, +1, +0, +3 byte order); the
// entry id is read once and kept in esi for both tests.
class CRGBColour { public:
    unsigned char B, G, R, A;
    CRGBColour() { }
    void Set(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
};
enum ECompositeBlendType { COMPOSITE_BLEND_TYPE_2 = 2 };
class CCompositeBlendDesc { public:
    unsigned long       TextureIndex;                        // +0x00
    ECompositeBlendType BlendType;                           // +0x04
    CRGBColour          DiffuseColour;                       // +0x08
    CRGBColour          SpecularColour;                      // +0x0C
};
class CTCTextureDecal { public:
    void DeleteDecal(long id);                                           // 0x0070A430
    long AddDecal(long decal, CCompositeBlendDesc& desc);                // 0x0070A490
    void ChangeDecal(long id, CCompositeBlendDesc& desc);                // 0x0070A3E0
};

class CTCWound { public:
    class CWound { public:
        void UpdateDecal(CTCTextureDecal& decals, long& entryID, long decal, long textureIndex, float strength);
    };
};

void CTCWound::CWound::UpdateDecal(CTCTextureDecal& decals, long& entryID, long decal, long textureIndex, float strength)
{
    long id = entryID;
    if (id && strength <= 0.05f)
    {
        decals.DeleteDecal(id);
        entryID = 0;
        return;
    }
    const unsigned char& alpha = (unsigned char)(strength * 255.0f);
    CCompositeBlendDesc desc;
    desc.TextureIndex = textureIndex;
    desc.BlendType = COMPOSITE_BLEND_TYPE_2;
    desc.DiffuseColour.Set(255, 255, 255, alpha);
    desc.SpecularColour.Set(0, 0, 0, 0);
    if (!id)
        entryID = decals.AddDecal(decal, desc);
    else
        decals.ChangeDecal(id, desc);
}
// ---- end source ----
static int g_deletes = 0, g_adds = 0, g_changes = 0; static long g_lastId = 0, g_lastDecal = 0; static CCompositeBlendDesc g_desc;
void CTCTextureDecal::DeleteDecal(long id) { ++g_deletes; g_lastId = id; }
long CTCTextureDecal::AddDecal(long decal, CCompositeBlendDesc& desc) { ++g_adds; g_lastDecal = decal; g_desc = desc; return 77; }
void CTCTextureDecal::ChangeDecal(long id, CCompositeBlendDesc& desc) { ++g_changes; g_lastId = id; g_desc = desc; }
int main()
{
    CTCTextureDecal decals; CTCWound::CWound wound;
    long entry = 0;
    wound.UpdateDecal(decals, entry, 5, 9, 1.0f);                 // add
    CHECK(g_adds == 1 && entry == 77 && g_lastDecal == 5 && g_desc.TextureIndex == 9 && g_desc.BlendType == 2);
    CHECK(g_desc.DiffuseColour.R == 255 && g_desc.DiffuseColour.G == 255 && g_desc.DiffuseColour.B == 255 && g_desc.DiffuseColour.A == 255);
    CHECK(g_desc.SpecularColour.R == 0 && g_desc.SpecularColour.A == 0);
    wound.UpdateDecal(decals, entry, 5, 9, 0.5f);                 // change, alpha 127
    CHECK(g_changes == 1 && g_lastId == 77 && g_desc.DiffuseColour.A == 127 && entry == 77);
    wound.UpdateDecal(decals, entry, 5, 9, 0.05f);                // delete
    CHECK(g_deletes == 1 && g_lastId == 77 && entry == 0);
    wound.UpdateDecal(decals, entry, 5, 9, 0.01f);                // no entry: weak strength still adds
    CHECK(g_adds == 2 && entry == 77 && g_desc.DiffuseColour.A == 2);
    if (fails == 0) printf("00757440_TEST PASS\n"); else printf("00757440_TEST FAIL\n");
    return fails;
}