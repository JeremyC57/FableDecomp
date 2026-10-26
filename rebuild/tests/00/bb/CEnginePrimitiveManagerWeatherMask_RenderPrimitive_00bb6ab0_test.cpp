// Behaviour test for 00bb6ab0: body copied from src/00bb6ab0.cpp (CEnginePrimitiveManagerWeatherMask::RenderPrimitive), operator new / AddSubPrimitive / globals stubbed.
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <math.h>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
class C3DVector { public: float X, Y, Z; };
class CBase {
public:
    virtual ~CBase() {}
    CBase() {}
};
class CEngineInternalPrimitiveBase;
class CEnginePrimitiveManager;
static int g_newCalls; static unsigned int g_newSize; static unsigned char g_pool[64];
class CEngineSubPrimitiveBase : public CBase {
public:
    unsigned long                 RenderFlags;
    unsigned char                 Alpha;
    CEngineInternalPrimitiveBase* Parent;
    CEngineSubPrimitiveBase*      NextSubPrimitive;
    CEnginePrimitiveManager*      Manager;
    CEngineSubPrimitiveBase(CEngineInternalPrimitiveBase& parent) : Parent(&parent), NextSubPrimitive(0) { }
    static void* operator new(unsigned int size) { ++g_newCalls; g_newSize = size; return g_pool; }
    static void operator delete(void*) {}
};
class CEngineCamera { public:
    unsigned char _pad00[0x54];
    C3DVector     WorldPosition;
};
CEngineCamera* g_pEngineCamera;
class C3DBoundingSphere { public: C3DVector Center; float Radius; };
class CEngineInternalPrimitiveWeatherMask {
public:
    unsigned char     _base[0x48];
    C3DBoundingSphere BSphere;
};
class CEngineSubPrimitiveWeatherMask : public CEngineSubPrimitiveBase {
public:
    CEngineSubPrimitiveWeatherMask(CEngineInternalPrimitiveBase& parent) : CEngineSubPrimitiveBase(parent) { }
    virtual ~CEngineSubPrimitiveWeatherMask() {}
};
static int g_addCalls; static CEngineSubPrimitiveBase* g_addSub; static float g_addDist; static unsigned char g_addAlpha;
static unsigned long g_addFlags; static CEnginePrimitiveManager* g_addMgr;
class CEngineSubPrimitiveRenderer {
public:
    void AddSubPrimitive(CEngineSubPrimitiveBase* subPrimitive, float distance, unsigned char alpha,
                         unsigned long renderFlags, CEnginePrimitiveManager* manager)
    { ++g_addCalls; g_addSub = subPrimitive; g_addDist = distance; g_addAlpha = alpha; g_addFlags = renderFlags; g_addMgr = manager; }
};
CEngineSubPrimitiveRenderer* g_pEngineSubPrimitiveRenderer;
enum ERendered { RENDERED_NOTHING = 0, RENDERED_SUB_PRIMITIVES = 2 };
enum EEngineRenderLayer { ENGINE_RENDER_LAYER_DEFAULT = 0 };
class CEngineSubPrimitiveChain;
class CEnginePrimitiveManagerWeatherMask {
public:
    void*         SceneList;
    bool          Enable;
    virtual ERendered RenderPrimitive(CEngineInternalPrimitiveBase& primitive, EEngineRenderLayer layer,
                                      CEngineSubPrimitiveChain& chain);
};
ERendered CEnginePrimitiveManagerWeatherMask::RenderPrimitive(CEngineInternalPrimitiveBase& primitive,
                                                              EEngineRenderLayer layer, CEngineSubPrimitiveChain& chain)
{
    if (!Enable)
        return RENDERED_NOTHING;
    const CEngineInternalPrimitiveWeatherMask& mask = (const CEngineInternalPrimitiveWeatherMask&)primitive;
    float dx = mask.BSphere.Center.X - g_pEngineCamera->WorldPosition.X;
    float dy = mask.BSphere.Center.Y - g_pEngineCamera->WorldPosition.Y;
    float dz = mask.BSphere.Center.Z - g_pEngineCamera->WorldPosition.Z;
    float d2 = dy * dy;
    d2 += dz * dz;
    d2 += dx * dx;
    float distance = (float)sqrt(d2);
    CEngineSubPrimitiveWeatherMask* subPrimitive = new CEngineSubPrimitiveWeatherMask(primitive);
    g_pEngineSubPrimitiveRenderer->AddSubPrimitive(subPrimitive, distance, 0xff, 1, (CEnginePrimitiveManager*)this);
    return RENDERED_SUB_PRIMITIVES;
}
int main()
{
    CEngineCamera cam; memset(&cam, 0, sizeof(cam)); g_pEngineCamera = &cam;
    CEngineSubPrimitiveRenderer renderer; g_pEngineSubPrimitiveRenderer = &renderer;
    CEngineInternalPrimitiveWeatherMask mask; memset(&mask, 0, sizeof(mask));
    CEnginePrimitiveManagerWeatherMask mgr; mgr.SceneList = 0; mgr.Enable = false;
    CEngineInternalPrimitiveBase& prim = (CEngineInternalPrimitiveBase&)mask;
    // 1. disabled: nothing rendered, no allocation
    g_newCalls = 0; g_addCalls = 0;
    CHECK(mgr.RenderPrimitive(prim, ENGINE_RENDER_LAYER_DEFAULT, *(CEngineSubPrimitiveChain*)0) == RENDERED_NOTHING);
    CHECK(g_newCalls == 0); CHECK(g_addCalls == 0);
    // 2. enabled: distance 3-4-12 -> 13, sub-primitive queued with alpha 0xff, flags 1, this manager
    mgr.Enable = true;
    cam.WorldPosition.X = 1.0f; cam.WorldPosition.Y = 2.0f; cam.WorldPosition.Z = 3.0f;
    mask.BSphere.Center.X = 4.0f; mask.BSphere.Center.Y = 6.0f; mask.BSphere.Center.Z = 15.0f;
    CHECK(mgr.RenderPrimitive(prim, ENGINE_RENDER_LAYER_DEFAULT, *(CEngineSubPrimitiveChain*)0) == RENDERED_SUB_PRIMITIVES);
    CHECK(g_newCalls == 1); CHECK(g_newSize == 0x18);
    CHECK(g_addCalls == 1); CHECK(g_addSub == (CEngineSubPrimitiveBase*)g_pool);
    CHECK(fabs(g_addDist - 13.0f) < 1e-5f); CHECK(g_addAlpha == 0xff); CHECK(g_addFlags == 1);
    CHECK(g_addMgr == (CEnginePrimitiveManager*)&mgr);
    CHECK(g_addSub->Parent == &prim); CHECK(g_addSub->NextSubPrimitive == 0);
    if (fails == 0) printf("00bb6ab0_TEST PASS\n");
    else printf("00bb6ab0_TEST FAIL (%d)\n", fails);
    return fails;
}