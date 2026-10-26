// Behaviour test for 007275c0: body copied from src/007275c0.cpp (CTCSimpleAppearanceMorph::OnCreate), callees stubbed.
#include <cstdio>
#include <cstring>
#include <cstdlib>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
class CRGBColour { public:
    unsigned char B, G, R, A;
    CRGBColour(const CRGBColour& o) : B(o.B), G(o.G), R(o.R), A(o.A) {}
    CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
};
class CCompositeBlendDesc { public:
    unsigned long TextureIndex;
    long          BlendType;
    CRGBColour    DiffuseColour;
    CRGBColour    SpecularColour;
    CCompositeBlendDesc(unsigned long texture, long type, unsigned char r, unsigned char g, unsigned char b, unsigned char a)
        : TextureIndex(texture), BlendType(type), DiffuseColour(r, g, b, a), SpecularColour(0, 0, 0, 0) {}
};
class CEngineTextureMorph;
static int g_blends = 0; static unsigned long g_blendTex[4], g_blendRep[4]; static long g_blendType[4]; static unsigned char g_blendA[4], g_blendR[4]; static void* g_blendOwner = 0;
class CTextureMorphs { public:
    CEngineTextureMorph* m_first;
    CEngineTextureMorph* m_last;
    CEngineTextureMorph* m_end;
    CTextureMorphs() : m_first(0), m_last(0), m_end(0) { }
    void AddBlend(unsigned long textureIndex, const CCompositeBlendDesc& blend)
    {
        g_blendOwner = this;
        if (g_blends < 4) { g_blendTex[g_blends] = textureIndex; g_blendRep[g_blends] = blend.TextureIndex; g_blendType[g_blends] = blend.BlendType; g_blendA[g_blends] = blend.DiffuseColour.A; g_blendR[g_blends] = blend.DiffuseColour.R | blend.SpecularColour.A | blend.SpecularColour.R; }
        ++g_blends;
    }
};
struct CMeshMorphSetParts;
struct CMeshMorphSet {
    struct CPartArray {
        CMeshMorphSetParts* m_first; CMeshMorphSetParts* m_last; CMeshMorphSetParts* m_end;
        CPartArray() : m_first(0), m_last(0), m_end(0) { }
    } MeshParts;
    CTextureMorphs OriginalMeshTextureMorphs;
};
class CCPPointerInfo;
static int g_resets = 0;
template <class T> class CCountedPointer { public:
    T* Data;
    CCPPointerInfo* Info;
    void Reset(T* object) { ++g_resets; Data = object; }
    T* operator->() const { return Data; }
};
static int g_destroyed = 0;
struct CDefBase {
    virtual void v00() {}
    virtual void Destroy() { ++g_destroyed; }
    long RefCount;
    void AddRef() { ++RefCount; }
    void Release() { if (--RefCount == 0) Destroy(); }
};
template <class T> class CDefPointer { public:
    T* Object;
    CDefPointer() : Object(0) { }
    CDefPointer(const CDefPointer& other) : Object(other.Object) { if (Object) Object->AddRef(); }
    ~CDefPointer() { if (Object) Object->Release(); }
    T* operator->() const { return Object; }
};
class CTextureReplacementDef : public CDefBase { public:
    class CEntry { public: unsigned long OriginalTexture; unsigned long ReplacementTexture; };
    unsigned char _pad08[0x28 - 0x08];
    struct CEntryArray {
        CEntry* m_first; CEntry* m_last; CEntry* m_end;
        long size() const { return m_last - m_first; }
        const CEntry& operator[](long i) const { return *(m_first + i); }
    } Entries;
};
static CTextureReplacementDef* g_repDef = 0; static int g_getSubDef = 0; static long g_countAtGet = -1;
class CDefClassBase : public CDefBase { public:
    bool GetSubDef_007086f0(CDefPointer<CTextureReplacementDef>& out)
    {
        ++g_getSubDef; g_countAtGet = RefCount;
        if (!g_repDef) return false;
        g_repDef->AddRef(); out.Object = g_repDef; return true;
    }
};
class CThing { public:
    unsigned char _pad00[0x70];
    CDefPointer<CDefClassBase> PDef;
    CDefPointer<CDefClassBase> GetPDef() const { return PDef; }
};
struct CTCSimpleAppearanceMorph {
    virtual void v00() {}
    CThing* ParentThing;
    unsigned char _pad08[0x0c - 0x08];
    CCountedPointer<CMeshMorphSet> PMeshMorphSet;
    void OnCreate();
};

void CTCSimpleAppearanceMorph::OnCreate()
{
    PMeshMorphSet.Reset(new CMeshMorphSet);
    CDefClassBase* thingDef = ParentThing->GetPDef().Object;
    CDefPointer<CTextureReplacementDef> replacements;
    thingDef->GetSubDef_007086f0(replacements);
    for (long i = 0; i < replacements->Entries.size(); ++i) {
        const CTextureReplacementDef::CEntry& entry = replacements->Entries[i];
        PMeshMorphSet->OriginalMeshTextureMorphs.AddBlend(entry.OriginalTexture,
            CCompositeBlendDesc(entry.ReplacementTexture, 3, 0, 0, 0, 255));
    }
}
int main()
{
    static CThing thing; static CDefClassBase classDef; static CTextureReplacementDef rep;
    thing.PDef.Object = &classDef; classDef.RefCount = 1; rep.RefCount = 1; g_repDef = &rep;
    CTextureReplacementDef::CEntry entries[2] = { { 10, 20 }, { 30, 40 } };
    rep.Entries.m_first = entries; rep.Entries.m_last = entries + 2; rep.Entries.m_end = entries + 2;
    CTCSimpleAppearanceMorph m; m.ParentThing = &thing; m.PMeshMorphSet.Data = 0; m.PMeshMorphSet.Info = 0;
    m.OnCreate();
    CHECK(g_resets == 1 && m.PMeshMorphSet.Data != 0); CHECK(m.PMeshMorphSet.Data->MeshParts.m_first == 0 && m.PMeshMorphSet.Data->OriginalMeshTextureMorphs.m_last == 0);
    CHECK(g_getSubDef == 1 && g_countAtGet == 1 && classDef.RefCount == 1);   // the by-value getter temporary is released before the lookup
    CHECK(g_blends == 2 && g_blendOwner == &m.PMeshMorphSet.Data->OriginalMeshTextureMorphs);
    CHECK(g_blendTex[0] == 10 && g_blendRep[0] == 20 && g_blendType[0] == 3 && g_blendA[0] == 255 && g_blendR[0] == 0);
    CHECK(g_blendTex[1] == 30 && g_blendRep[1] == 40 && g_blendType[1] == 3);
    CHECK(rep.RefCount == 1 && g_destroyed == 0);
    // no replacement entries: a fresh morph set, no blends; defs held only by the lookup are destroyed
    rep.Entries.m_last = entries; classDef.RefCount = 0; rep.RefCount = 0; m.OnCreate();
    CHECK(g_resets == 2 && g_blends == 2 && g_destroyed == 2);
    if (fails == 0) printf("007275c0_TEST PASS\n"); else printf("007275c0_TEST FAIL\n");
    return fails;
}