// Behaviour test for 00b8ff00: body copied from src/00b8ff00.cpp (CEngineInternalPrimitiveBase::CEngineInternalPrimitiveBase), the render-settings global stubbed.
#include <cstdio>
#include <cstring>
#include <new>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
struct SRenderSettings {
    unsigned char _pad[0x64];
    unsigned char DefaultAlphaRef;
};
static SRenderSettings g_settings; SRenderSettings* g_pRenderSettings = &g_settings;
class CEngineSceneGridCell;
class CEngineSceneGridPos { public:
    signed char   Level;
    unsigned char X;
    unsigned char Y;
    CEngineSceneGridPos() : Level(-1) { }
};
class CEngineFadeDistance { public:
    float FadeBeginDistance;
    float MaxDrawDistance;
    CEngineFadeDistance() { }
};
class CEnginePersistentPrimitiveBase {
public:
    virtual void v000() {}
    long RefCount;
    long Type;
    CEnginePersistentPrimitiveBase(long type) : RefCount(1), Type(type) { }
};
class CEngineInternalPrimitiveBase : public CEnginePersistentPrimitiveBase {
public:
    unsigned long        EnginePrimitiveFlags;
    float                AlphaMipBias;
    CEngineFadeDistance  FadeDistance;
    unsigned long        OcclusionLayerBitMask;
    CEngineSceneGridCell* GridCell;
    CEngineSceneGridPos  GridPos;
    unsigned char        Alpha;
    unsigned char        AlphaRef;
    unsigned char        PreloadStat;
    long                 InterpolationSet;
    unsigned long        RenderLayerMask;
    unsigned char        Enable : 1;
    unsigned char        NonPersistent : 1;
    unsigned char        OverrideFadeDistance : 1;
    unsigned char        PostRenderEnable : 1;
    unsigned short       LastFrameRendered;
    CEngineInternalPrimitiveBase*  NextPrimitive;
    CEngineInternalPrimitiveBase** RefPrimitive;
    CEngineInternalPrimitiveBase*  NextLayerMask;
    CEngineInternalPrimitiveBase** RefLayerMask;
    virtual void v001() {}
    CEngineInternalPrimitiveBase(long type, unsigned long renderLayerMask);
};
CEngineInternalPrimitiveBase::CEngineInternalPrimitiveBase(long type, unsigned long renderLayerMask)
    : CEnginePersistentPrimitiveBase(type), EnginePrimitiveFlags(0), AlphaMipBias(0.0f), OcclusionLayerBitMask(0xffffffff), GridCell(0)
{
    RenderLayerMask = renderLayerMask;
    Enable = 1;
    Alpha = 0xff;
    PreloadStat = 0;
    InterpolationSet = 0;
    NonPersistent = 0;
    OverrideFadeDistance = 0;
    PostRenderEnable = 0;
    LastFrameRendered = 0;
    NextPrimitive = 0;
    RefPrimitive = 0;
    NextLayerMask = 0;
    RefLayerMask = 0;
    AlphaRef = g_pRenderSettings->DefaultAlphaRef;
}
int main()
{
    g_settings.DefaultAlphaRef = 0x7b;
    unsigned char raw[sizeof(CEngineInternalPrimitiveBase)]; memset(raw, 0xAA, sizeof(raw));
    CEngineInternalPrimitiveBase* p = new (raw) CEngineInternalPrimitiveBase(5, 0x12345678);
    CHECK(p->RefCount == 1); CHECK(p->Type == 5);
    CHECK(p->EnginePrimitiveFlags == 0); CHECK(p->AlphaMipBias == 0.0f); CHECK(p->OcclusionLayerBitMask == 0xffffffff); CHECK(p->GridCell == 0);
    CHECK(p->GridPos.Level == -1);
    CHECK(p->RenderLayerMask == 0x12345678);
    CHECK(p->Enable == 1); CHECK(p->NonPersistent == 0); CHECK(p->OverrideFadeDistance == 0); CHECK(p->PostRenderEnable == 0);
    CHECK(p->Alpha == 0xff); CHECK(p->PreloadStat == 0); CHECK(p->InterpolationSet == 0); CHECK(p->LastFrameRendered == 0);
    CHECK(p->NextPrimitive == 0); CHECK(p->RefPrimitive == 0); CHECK(p->NextLayerMask == 0); CHECK(p->RefLayerMask == 0);
    CHECK(p->AlphaRef == 0x7b);
    // the untouched bit-field upper nibble and the fade distance keep their previous bytes
    CHECK((raw[0x34] & 0xf0) == 0xa0);
    if (fails == 0) printf("00b8ff00_TEST PASS\n"); else printf("00b8ff00_TEST FAIL\n");
    return fails;
}