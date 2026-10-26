// Behaviour test for 00bb6b50: body copied from src/00bb6b50.cpp (CEnginePrimitiveManagerWeatherMask::RenderSubPrimitive), callees stubbed.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
class C3DVector { public: float X, Y, Z; C3DVector() { } C3DVector(const C3DVector& other) : X(other.X), Y(other.Y), Z(other.Z) { } };
class C3DBoundingSphere { public: C3DVector Center; float Radius; };
class C3DBoundingBox { public: C3DVector Min; C3DVector Max; };
class CMatrix3x4 { public: float m[12]; };
class CWeatherMask { public:
    C3DBoundingSphere BSphere; C3DBoundingBox BBox; CMatrix3x4 InvTransform; float Scale;
    CWeatherMask(const C3DBoundingSphere& bSphere, const C3DBoundingBox& bBox, const CMatrix3x4& invTransform, const float& scale)
        : BSphere(bSphere), BBox(bBox), InvTransform(invTransform), Scale(scale) { }
};
class CEngineInternalPrimitiveWeatherMask { public:
    unsigned char _base[0x48]; C3DBoundingSphere BSphere; C3DBoundingBox BBox; CMatrix3x4 InvTransform; float Scale; unsigned long Flags;
};
class CEngineSubPrimitiveBase { public: void* vptr; unsigned long RenderFlags; unsigned char Alpha; CEngineInternalPrimitiveWeatherMask* Parent; CEngineSubPrimitiveBase* NextSubPrimitive; void* Manager; };
static int g_add = 0; static unsigned long g_addFlags = 0; static float g_scale = 0, g_radius = 0, g_maxZ = 0, g_m11 = 0;
class CEngineWeatherRenderer { public:
    void AddWeatherMask(const CWeatherMask& mask, unsigned long flags)
    { ++g_add; g_addFlags = flags; g_scale = mask.Scale; g_radius = mask.BSphere.Radius; g_maxZ = mask.BBox.Max.Z; g_m11 = mask.InvTransform.m[11]; }
};
static CEngineWeatherRenderer g_renderer;
CEngineWeatherRenderer* g_pEngineWeatherRenderer = &g_renderer;
class CEnginePrimitiveManagerWeatherMask { public:
    virtual void RenderSubPrimitive(CEngineSubPrimitiveBase& subPrimitive, long passType);
};
void CEnginePrimitiveManagerWeatherMask::RenderSubPrimitive(CEngineSubPrimitiveBase& subPrimitive, long passType)
{
    CEngineInternalPrimitiveWeatherMask* primitive = subPrimitive.Parent;
    CWeatherMask mask(primitive->BSphere, primitive->BBox, primitive->InvTransform, primitive->Scale);
    g_pEngineWeatherRenderer->AddWeatherMask(mask, primitive->Flags);
}
int main()
{
    CEngineInternalPrimitiveWeatherMask prim; memset(&prim, 0, sizeof prim);
    prim.BSphere.Radius = 2.5f; prim.BBox.Max.Z = 7.0f; for (int i = 0; i < 12; ++i) prim.InvTransform.m[i] = (float)i; prim.Scale = 0.5f; prim.Flags = 0x31;
    CEngineSubPrimitiveBase sub; memset(&sub, 0, sizeof sub); sub.Parent = &prim;
    CEnginePrimitiveManagerWeatherMask mgr;
    mgr.RenderSubPrimitive(sub, 3);
    CHECK(g_add == 1 && g_addFlags == 0x31 && g_scale == 0.5f && g_radius == 2.5f && g_maxZ == 7.0f && g_m11 == 11.0f);
    prim.Flags = 0; prim.Scale = 2.0f; mgr.RenderSubPrimitive(sub, 0);
    CHECK(g_add == 2 && g_addFlags == 0 && g_scale == 2.0f);
    if (fails == 0) printf("00bb6b50_TEST PASS\n"); else printf("00bb6b50_TEST FAIL (%d)\n", fails);
    return fails;
}