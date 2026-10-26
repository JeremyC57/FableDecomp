// Behaviour test for 00bdda80: src/00bdda80.cpp copied verbatim (the function body and its same-TU stand-ins), the remaining
// declared callees stubbed below so calls and arguments are observable. CLocalDetailCacheMap::CQuadTreeElement::RenderUpdate(float lodDistance, CEngineCamera* camera, unsigned long clipFlags)
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CLocalDetailCacheMap::CQuadTreeElement::RenderUpdate(float lodDistance, CEngineCamera* camera, unsigned long clipFlags) -- if the element needs a render update and its bounding sphere is inside the (video-option adjusted) fade distance and not clipped, render-update its object-cache groups with the near/far sphere distances and recurse into the four children
// Leaf: RenderUpdate
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BDDA80, length 267 bytes, `ret 0xc`.
// Callees: CEngineLocalDetailGenerator::ModifyFadeDistanceForVideoOptions(unsigned long, float) 0x00B597E0 (global
// 0x01436E88), CEngineCamera::ClipSphere(const C3DVector&, float, unsigned long*) 0x00B30480,
// CObjectCacheGroupCollection::RenderUpdateGroups 0x00BDD930 (landed), itself for the children.
// Layout: Ego_r.pdb CQuadTreeElement (the landed 0x00BDE020 model): ObjectCacheGroupCollections +0x04, Children[4]
// +0x08, BoundingSphere +0x18 (radius +0x24), MaxFadeDistance +0x2c, PrimitiveTypeMask +0x30, RenderUpdateNeeded
// +0x48 bit 1; CEngineCamera position +0x54.
// Shape: the camera distance is the vector length taken into a DOUBLE local and narrowed (`double length =
// delta.GetLength(); float distance = (float)length;`): retail keeps the root on the FPU (`fstp st(2) ; fstp st(0) ;
// fst [slot]`); a float local stores and reloads it.
class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    float GetLength() const;
};
extern "C" double __cdecl sqrt(double);
#pragma intrinsic(sqrt)
inline float C3DVector::GetLength() const
{
    float lengthSq = Z * Z;
    lengthSq += Y * Y;
    lengthSq += X * X;
    return (float)sqrt(lengthSq);
}
class C3DBoundingSphere { public:
    C3DVector Center;                                  // +0x00
    float     Radius;                                  // +0x0c
};
class CEngineCamera { public:
    unsigned char _pad00[0x54];
    C3DVector     Position;                            // +0x54
    bool ClipSphere(const C3DVector& centre, float radius, unsigned long* flags);   // 0x00B30480
};
class CEngineLocalDetailGenerator { public:
    float ModifyFadeDistanceForVideoOptions(unsigned long primitiveTypeMask, float fadeDistance);   // 0x00B597E0
};
extern CEngineLocalDetailGenerator* g_pLocalDetailGenerator;   // 0x01436E88

class CLocalDetailCacheMap { public:
    class CObjectCacheGroupCollection { public:
        void* __vftable;
        CObjectCacheGroupCollection* Next;             // +0x04
        void RenderUpdateGroups(CEngineCamera* camera, float nearDistance, float farDistance, unsigned long clipFlags, float lodDistance);   // 0x00BDD930
    };
    class CQuadTreeElement { public:
        virtual ~CQuadTreeElement();
        CObjectCacheGroupCollection* ObjectCacheGroupCollections;   // +0x04
        CQuadTreeElement*  Children[4];                // +0x08
        C3DBoundingSphere  BoundingSphere;             // +0x18
        CLocalDetailCacheMap* Map;                     // +0x28
        float              MaxFadeDistance;            // +0x2c
        unsigned long      PrimitiveTypeMask;          // +0x30
        long               FileBlockPos;               // +0x34
        long               FileBlockSize;              // +0x38
        long               OffsetIntoFileBlock;        // +0x3c
        unsigned short     MapX, MapY, Width, Height;  // +0x40
        bool               HasDynamicAreasFlag : 1;    // +0x48 bit 0
        bool               RenderUpdateNeeded : 1;     // +0x48 bit 1
        bool               StaticContentsLoaded : 1;   // +0x48 bit 2
        bool               Loading : 1;                // +0x48 bit 3
        void RenderUpdate(float lodDistance, CEngineCamera* camera, unsigned long clipFlags);
    };
};

void CLocalDetailCacheMap::CQuadTreeElement::RenderUpdate(float lodDistance, CEngineCamera* camera, unsigned long clipFlags)
{
    if (!RenderUpdateNeeded)
        return;
    C3DVector cameraPosition = camera->Position;
    C3DVector delta = BoundingSphere.Center - cameraPosition;
    double length = delta.GetLength();
    float distance = (float)length;
    float nearDistance = distance - BoundingSphere.Radius;
    if (g_pLocalDetailGenerator->ModifyFadeDistanceForVideoOptions(PrimitiveTypeMask, MaxFadeDistance) < nearDistance)
        return;
    camera->ClipSphere(BoundingSphere.Center, BoundingSphere.Radius, &clipFlags);
    if (clipFlags & 0x40)
        return;
    nearDistance = distance - BoundingSphere.Radius;
    float farDistance = distance + BoundingSphere.Radius;
    for (CObjectCacheGroupCollection* group = ObjectCacheGroupCollections; group; group = group->Next)
        group->RenderUpdateGroups(camera, nearDistance, farDistance, clipFlags, lodDistance);
    for (int i = 0; i < 4; ++i)
        if (Children[i])
            Children[i]->RenderUpdate(lodDistance, camera, clipFlags);
}

// ---- stubs and driver ----

static char g_log[64]; static int g_logn = 0;
static void Log(char c) { if (g_logn < 63) { g_log[g_logn++] = c; g_log[g_logn] = 0; } }
CLocalDetailCacheMap::CQuadTreeElement::~CQuadTreeElement() { }
static float g_fade = 100.0f; static unsigned long g_mask; static float g_maxFade;
float CEngineLocalDetailGenerator::ModifyFadeDistanceForVideoOptions(unsigned long primitiveTypeMask, float fadeDistance)
{ g_mask = primitiveTypeMask; g_maxFade = fadeDistance; Log('M'); return g_fade; }
static CEngineLocalDetailGenerator g_gen;
CEngineLocalDetailGenerator* g_pLocalDetailGenerator = &g_gen;
static unsigned long g_clipOr = 0; static float g_clipRadius; static const C3DVector* g_clipCentre;
bool CEngineCamera::ClipSphere(const C3DVector& centre, float radius, unsigned long* flags)
{ g_clipCentre = &centre; g_clipRadius = radius; *flags |= g_clipOr; Log('C'); return true; }
static int g_groups = 0; static float g_near[4], g_far[4], g_lod[4]; static unsigned long g_flags[4]; static CEngineCamera* g_cam;
void CLocalDetailCacheMap::CObjectCacheGroupCollection::RenderUpdateGroups(CEngineCamera* camera, float nearDistance, float farDistance, unsigned long clipFlags, float lodDistance)
{ g_cam = camera; g_near[g_groups & 3] = nearDistance; g_far[g_groups & 3] = farDistance; g_flags[g_groups & 3] = clipFlags; g_lod[g_groups & 3] = lodDistance; ++g_groups; Log('G'); }
typedef CLocalDetailCacheMap::CQuadTreeElement Elem;
static void Init(Elem& e, float x, float y, float z, float r)
{
    e.ObjectCacheGroupCollections = 0; for (int i = 0; i < 4; ++i) e.Children[i] = 0;
    e.BoundingSphere.Center = C3DVector(x, y, z); e.BoundingSphere.Radius = r;
    e.MaxFadeDistance = 55.0f; e.PrimitiveTypeMask = 0x123; e.HasDynamicAreasFlag = 0; e.RenderUpdateNeeded = 1; e.StaticContentsLoaded = 0; e.Loading = 0;
}
int main()
{
    CEngineCamera cam; cam.Position = C3DVector(0.0f, 0.0f, 0.0f);
    Elem root, child; Init(root, 3.0f, 4.0f, 12.0f, 1.0f); Init(child, 0.0f, 6.0f, 8.0f, 2.0f);
    CLocalDetailCacheMap::CObjectCacheGroupCollection g1, g2; g1.Next = &g2; g2.Next = 0;
    root.ObjectCacheGroupCollections = &g1; root.Children[2] = &child;
    root.RenderUpdateNeeded = 0;
    root.RenderUpdate(7.0f, &cam, 0x5);
    CHECK(g_logn == 0);                                       // no update needed
    root.RenderUpdateNeeded = 1;
    root.RenderUpdate(7.0f, &cam, 0x5);
    CHECK(strcmp(g_log, "MCGGMC") == 0);
    CHECK(g_mask == 0x123); CHECK(g_maxFade == 55.0f);
    CHECK(g_groups == 2); CHECK(g_near[0] == 12.0f); CHECK(g_far[0] == 14.0f); CHECK(g_flags[0] == 5); CHECK(g_lod[0] == 7.0f); CHECK(g_cam == &cam);
    CHECK(g_clipCentre == &child.BoundingSphere.Center); CHECK(g_clipRadius == 2.0f);
    // too far: the fade distance is below the near distance
    g_logn = 0; g_log[0] = 0; g_groups = 0; g_fade = 11.5f;
    root.RenderUpdate(7.0f, &cam, 0);
    CHECK(strcmp(g_log, "M") == 0); CHECK(g_groups == 0);
    // clipped away
    g_logn = 0; g_log[0] = 0; g_fade = 100.0f; g_clipOr = 0x40;
    root.RenderUpdate(7.0f, &cam, 0);
    CHECK(strcmp(g_log, "MC") == 0); CHECK(g_groups == 0);
    if (fails == 0) printf("00bdda80_TEST PASS\n"); else printf("00bdda80_TEST FAIL\n");
    return fails;
}