#include <cstdio>
#include <cstring>
#include <math.h>
#pragma intrinsic(sqrt)
class C3DVector { public:
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector operator-(const C3DVector& v) const { return C3DVector(X - v.X, Y - v.Y, Z - v.Z); }
    float GetLength() const { return (float)sqrt(X * X + Y * Y + Z * Z); }
};
class C3DBoundingSphere { public:
    C3DVector Centre;
    float     Radius;
};
class CEngineCamera { public:
    unsigned char _pad00[0x54];
    C3DVector     Position;                                 // +0x54
};
class CEngineLocalDetailGenerator { public:
    float ModifyFadeDistanceForVideoOptions(unsigned long typeMask, float distance) { return typeMask == 7 ? distance * 2 : distance; }
};
CEngineCamera* g_pEngineCamera;
CEngineLocalDetailGenerator* g_pLocalDetailGenerator;
static double g_timer = 0;
double GFGetFastSubFrameTimer() { return g_timer; }
class CEngineStaticMapFileBlockManager;
struct CObjectTypeCollectionPalette { int dummy; };
class CLocalDetailCacheMap;
struct CObjectCacheGroupCollection {
    CObjectCacheGroupCollection* Next;                      // +0x04
    int updates; bool result;
    bool                         RenderUpdateNeeded : 1;    // +0x36 bit 0
    bool                         Loading : 1;
    bool StaticUpdate(CEngineStaticMapFileBlockManager*, const CObjectTypeCollectionPalette&, double) { ++updates; return result; }
};
struct CQuadTreeElement {
    CObjectCacheGroupCollection* ObjectCacheGroupCollections;   // +0x04
    CQuadTreeElement*            Children[4];                   // +0x08
    C3DBoundingSphere            BoundingSphere;                // +0x18
    CLocalDetailCacheMap*        Map;                           // +0x28
    float                        MaxFadeDistance;               // +0x2c
    unsigned long                PrimitiveTypeMask;             // +0x30
    long                         FileBlockPos;                  // +0x34
    long                         FileBlockSize;                 // +0x38
    long                         OffsetIntoFileBlock;           // +0x3c
    unsigned short               MapX, MapY, Width, Height;     // +0x40
    bool                         HasDynamicAreasFlag : 1;       // +0x48 bit 0
    bool                         RenderUpdateNeeded : 1;        // bit 1
    bool                         StaticContentsLoaded : 1;      // bit 2
    bool                         Loading : 1;                   // bit 3
    int loads, unloads;
    bool LoadStaticContents(CEngineStaticMapFileBlockManager*, const CObjectTypeCollectionPalette&) { ++loads; StaticContentsLoaded = true; return true; }
    void UnloadStaticContents(CEngineStaticMapFileBlockManager*) { ++unloads; StaticContentsLoaded = false; }
    bool Update(CEngineStaticMapFileBlockManager* blocks, const CObjectTypeCollectionPalette& palette, double deadline);
};

bool CQuadTreeElement::Update(CEngineStaticMapFileBlockManager* blocks, const CObjectTypeCollectionPalette& palette, double deadline)
{
    RenderUpdateNeeded = false;
    bool changed = false;
    if (!HasDynamicAreasFlag) {
        C3DVector centre(BoundingSphere.Centre.X, BoundingSphere.Centre.Y, BoundingSphere.Centre.Z);
        float distance = (centre - g_pEngineCamera->Position).GetLength() - BoundingSphere.Radius;
        if (g_pLocalDetailGenerator->ModifyFadeDistanceForVideoOptions(PrimitiveTypeMask, MaxFadeDistance) > distance) {
            if (GFGetFastSubFrameTimer() < deadline)
                changed = LoadStaticContents(blocks, palette);
        } else
            UnloadStaticContents(blocks);
    }
    if (HasDynamicAreasFlag || StaticContentsLoaded) {
        for (int i = 0; i < 4; ++i) {
            if (Children[i]) {
                changed |= Children[i]->Update(blocks, palette, deadline);
                RenderUpdateNeeded |= Children[i]->RenderUpdateNeeded;
            }
        }
    }
    CObjectCacheGroupCollection* collection;
    for (collection = ObjectCacheGroupCollections; collection; collection = collection->Next)
        RenderUpdateNeeded |= collection->RenderUpdateNeeded;
    if (StaticContentsLoaded)
        for (collection = ObjectCacheGroupCollections; collection; collection = collection->Next)
            changed |= collection->StaticUpdate(blocks, palette, deadline);
    return changed;
}

int main()
{
    static CEngineCamera cam; static CEngineLocalDetailGenerator gen;
    g_pEngineCamera = &cam; g_pLocalDetailGenerator = &gen;
    cam.Position.X = 0; cam.Position.Y = 0; cam.Position.Z = 0;
    static CQuadTreeElement root, child;
    std::memset(&root, 0, sizeof root); std::memset(&child, 0, sizeof child);
    root.BoundingSphere.Centre.X = 30; root.BoundingSphere.Centre.Y = 40; root.BoundingSphere.Radius = 10;   // surface 40 away
    root.MaxFadeDistance = 50; root.PrimitiveTypeMask = 1; root.Children[2] = &child;
    child.BoundingSphere.Centre.X = 1000; child.MaxFadeDistance = 1; child.PrimitiveTypeMask = 1;
    child.RenderUpdateNeeded = true;
    static CObjectCacheGroupCollection c1, c2; std::memset(&c1, 0, sizeof c1); std::memset(&c2, 0, sizeof c2);
    c1.Next = &c2; c2.RenderUpdateNeeded = true; c2.result = true;
    root.ObjectCacheGroupCollections = &c1;
    CEngineStaticMapFileBlockManager* blocks = 0; CObjectTypeCollectionPalette pal;
    g_timer = 5;
    bool r = root.Update(blocks, pal, 10.0);           // in range, time left: loads, recurses, collects
    bool ok = r && root.loads == 1 && root.StaticContentsLoaded && child.unloads == 1 && root.RenderUpdateNeeded == true
           && c1.updates == 1 && c2.updates == 1;
    child.RenderUpdateNeeded = true;                   // unloaded child: its flag is cleared by its own update
    root.MaxFadeDistance = 30;                         // out of range: unload, no recursion
    r = root.Update(blocks, pal, 10.0);
    ok = ok && !r && root.unloads == 1 && !root.StaticContentsLoaded && c1.updates == 1 && root.RenderUpdateNeeded;
    root.MaxFadeDistance = 50; g_timer = 20;           // in range but no time left
    r = root.Update(blocks, pal, 10.0);
    ok = ok && !r && root.loads == 1;
    root.HasDynamicAreasFlag = true; child.HasDynamicAreasFlag = true; child.RenderUpdateNeeded = false;
    c2.RenderUpdateNeeded = false;
    r = root.Update(blocks, pal, 10.0);                // dynamic: no load test, child visited
    ok = ok && !r && root.loads == 1 && !root.RenderUpdateNeeded;
    if (!ok) { std::printf("00bdebf0_TEST FAIL\n"); return 1; }
    std::printf("00bdebf0_TEST PASS\n");
    return 0;
}