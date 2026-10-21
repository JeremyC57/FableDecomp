// CEngineSceneGrid::PreloadPrimitives(unsigned long x, unsigned long y, const CShadowClipVolume& volume, const C3DVector& position, unsigned long flags) -- preload one grid cell when its bounding box is inside the clip volume: each of its primitives whose bounding sphere is inside the volume goes to its type's primitive manager (PreloadStat 2 when loaded, else 1), then the four child cells one level down; returns whether anything was preloaded
// Leaf: PreloadPrimitives
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B92070, length 477 bytes (+ int3 padding), `ret 0x14`.
// Callees: CEngineSceneGrid::GetCellBoundingBoxAndDrawDistance 0x00B919C0, CShadowClipVolume::ClipBoundingBox 0x00B4CED0,
//   itself (recursion on LevelDown); the primitive's virtual slot 18 (+0x48, GetBoundingSphereWorldSpace) and the type
//   manager's virtual slot 9 (+0x24, PreloadPrimitive) on the type table 0x01436E84 (PrimitiveManagers +0x10).
// Layout: as the landed camera overload CEngineSceneGrid::PreloadPrimitives 0x00B91F50 (CEngineSceneGrid LevelDown +0x0c,
//   Size +0x10, GridCells.first +0x14; CEngineSceneGridCell FirstPrimitive +0x04; CEngineInternalPrimitiveBase Type +0x08,
//   PreloadStat +0x29, NextPrimitive +0x38) and Ego_r.pdb CShadowClipVolume (ClipPlanes +0, ClipPlaneCount +0x80,
//   BoundingSphere +0x84, ClipToSphere +0x94); the sphere-in-volume test is the same inline as CEngineScene::PreloadPrimitives
//   0x00B930A0.
// Shape: `inside` is declared at function scope with the box and distance (block-scoped, it takes the dead volume argument
//   slot that retail gives the outer child-loop counter); the sphere is one local cleared member by member per primitive.
struct C3DVector {
    float X, Y, Z;
};

struct C3DBoundingSphere {
    C3DVector Center;
    float     Radius;
    C3DBoundingSphere() {}
};

struct CPlane {
    C3DVector Normal;
    float     D;
};

struct CShadowClipVolume {
    CPlane            ClipPlanes[8];               // +0x00
    long              ClipPlaneCount;              // +0x80
    C3DBoundingSphere BoundingSphere;              // +0x84
    bool              ClipToSphere;                // +0x94
    __forceinline bool IsSphereInside(const C3DBoundingSphere& sphere) const
    {
        if (ClipToSphere) {
            float dx = sphere.Center.X - BoundingSphere.Center.X;
            float dy = sphere.Center.Y - BoundingSphere.Center.Y;
            float dz = sphere.Center.Z - BoundingSphere.Center.Z;
            float radius = sphere.Radius + BoundingSphere.Radius;
            if (dx * dx + dy * dy + dz * dz > radius * radius)
                return false;
        }
        for (long i = 0; i < ClipPlaneCount; ++i) {
            const CPlane& plane = ClipPlanes[i];
            if (sphere.Center.X * plane.Normal.X + sphere.Center.Y * plane.Normal.Y + sphere.Center.Z * plane.Normal.Z
                    - sphere.Radius > plane.D)
                return false;
        }
        return true;
    }
};

class CEngineInternalPrimitiveBase;

struct CEnginePrimitiveManager {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08();
    virtual bool PreloadPrimitive(CEngineInternalPrimitiveBase& primitive, const C3DVector& position, unsigned long flags);   // slot 9
};
struct CEnginePrimitiveTypeInfo {
    unsigned char            _pad[0x10];
    CEnginePrimitiveManager* PrimitiveManagers[47];   // +0x10
};
extern CEnginePrimitiveTypeInfo* g_pEnginePrimitiveTypeInfo;   // 0x01436E84

class CEngineInternalPrimitiveBase {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual bool GetBoundingSphereWorldSpace(C3DBoundingSphere& sphere);   // slot 18 (+0x48)
    unsigned long                 _pad04;
    long                          Type;             // +0x08
    unsigned char                 _pad0c[0x29 - 0x0c];
    unsigned char                 PreloadStat;      // +0x29
    unsigned char                 _pad2a[0x38 - 0x2a];
    CEngineInternalPrimitiveBase* NextPrimitive;    // +0x38
};

struct C3DBoundingBox {
    C3DVector MinCoord, MaxCoord;
};

struct CShadowClipVolumeMethods : CShadowClipVolume {
    bool ClipBoundingBox(const C3DBoundingBox& box, bool& inside) const;   // 0x00B4CED0
};

struct CEngineSceneGridCell {
    CEngineSceneGridCell*         Parent;           // +0x00
    CEngineInternalPrimitiveBase* FirstPrimitive;   // +0x04
};

struct CEngineSceneGrid {
    void*                  _vptr;                   // +0x00
    unsigned long          Level;                   // +0x04
    CEngineSceneGrid*      LevelUp;                 // +0x08
    CEngineSceneGrid*      LevelDown;               // +0x0c
    unsigned long          Size;                    // +0x10
    CEngineSceneGridCell** GridCells;               // +0x14 (GridCells.first)
    bool GetCellBoundingBoxAndDrawDistance(C3DBoundingBox& box, float& distance, unsigned long x, unsigned long y);   // 0x00B919C0
    bool PreloadPrimitives(unsigned long x, unsigned long y, const CShadowClipVolume& volume, const C3DVector& position,
                           unsigned long flags);
};

bool CEngineSceneGrid::PreloadPrimitives(unsigned long x, unsigned long y, const CShadowClipVolume& volume,
                                         const C3DVector& position, unsigned long flags)
{
    CEngineSceneGridCell* cell = GridCells[y * Size + x];
    bool preloaded = false;
    C3DBoundingBox bounds;
    float distance;
    bool inside;
    if (cell && GetCellBoundingBoxAndDrawDistance(bounds, distance, x, y)) {
        if (static_cast<const CShadowClipVolumeMethods&>(volume).ClipBoundingBox(bounds, inside)) {
            C3DBoundingSphere sphere;
            for (CEngineInternalPrimitiveBase* primitive = cell->FirstPrimitive; primitive; primitive = primitive->NextPrimitive) {
                sphere.Center.X = 0.0f;
                sphere.Center.Y = 0.0f;
                sphere.Center.Z = 0.0f;
                sphere.Radius = 0.0f;
                if (primitive->GetBoundingSphereWorldSpace(sphere) && volume.IsSphereInside(sphere)) {
                    bool loaded = g_pEnginePrimitiveTypeInfo->PrimitiveManagers[primitive->Type]->PreloadPrimitive(*primitive, position, flags);
                    primitive->PreloadStat = loaded ? 2 : 1;
                    preloaded |= loaded;
                }
            }
            if (LevelDown)
                for (unsigned long yy = 0; yy < 2; ++yy)
                    for (unsigned long xx = 0; xx < 2; ++xx)
                        preloaded |= LevelDown->PreloadPrimitives(x * 2 + xx, y * 2 + yy, volume, position, flags);
        }
    }
    return preloaded;
}
