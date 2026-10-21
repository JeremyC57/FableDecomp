// CEngineScene::PreloadPrimitives(const CShadowClipVolume& volume, const C3DVector& position, unsigned long flags) -- preload everything inside a clip volume: every scene map's top grid level from cell (0,0), then each moving primitive whose bounding sphere is inside the volume through its type's primitive manager; returns whether anything was preloaded
// Leaf: PreloadPrimitives
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B930A0, length 484 bytes (+ int3 padding), `ret 0xc`.
// Callees: CEngineSceneGrid::PreloadPrimitives(x, y, volume, position, flags) 0x00B92070; the primitive's virtual slot 18
//   (+0x48, GetBoundingSphereWorldSpace) and the type manager's virtual slot 9 (+0x24, PreloadPrimitive; slot table as the landed CEngineSceneGrid::PreloadPrimitives 0x00B91F50) on the type table 0x01436E84
//   (PrimitiveManagers +0x10).
// Layout: as the landed sibling CEngineScene::RenderEntireScene 0x00B943F0 (MapList header +0x04, outer/inner STLport tree
//   nodes, scene map GridLevels.first +0x18, FirstMovingPrimitive +0x20; CEngineInternalPrimitiveBase Type +0x08,
//   NextPrimitive +0x38). Ego_r.pdb CShadowClipVolume (152 bytes): ClipPlanes CPlane[8] +0, ClipPlaneCount +0x80,
//   BoundingSphere +0x84, ClipToSphere +0x94. No engine header carries these as typed members.
// Shape: the sphere is one function-scope local cleared member by member each iteration (a loop-scoped zeroing ctor lets
//   VC7.1 sink the four stores below the vptr load and push); the grid pointer is named before the call's arguments.
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
    unsigned char                 _pad0c[0x38 - 0x0c];
    CEngineInternalPrimitiveBase* NextPrimitive;    // +0x38
};

struct CEngineSceneGrid {
    bool PreloadPrimitives(unsigned long x, unsigned long y, const CShadowClipVolume& volume,
                           const C3DVector& position, unsigned long flags);   // 0x00B92070
};
struct CGridPointer { CEngineSceneGrid* m_pObject; long* m_pCount; };
struct CEngineSceneMap {
    unsigned char  _pad[0x18];
    CGridPointer*  GridLevelsFirst;               // +0x18 (GridLevels.first)
};

struct CMapNode { unsigned long Colour; CMapNode *Parent, *Left, *Right; long Key; CEngineSceneMap* Data; void* Info; };
struct CMapTree { CMapNode* Head; unsigned long Count; };
struct COuterNode { unsigned long Colour; COuterNode *Parent, *Left, *Right; long Key; CMapTree Maps; };
template <class T> static T* Next(T* node)
{
    if (node->Right) { node = node->Right; while (node->Left) node = node->Left; }
    else { T* parent = node->Parent; while (node == parent->Right) { node = parent; parent = parent->Parent; } if (node->Right != parent) node = parent; }
    return node;
}

struct CEngineScene {
    unsigned long                 Base;           // +0x00
    COuterNode*                   MapListHead;    // +0x04
    unsigned long                 MapListCount;   // +0x08
    unsigned char                 _pad0c[0x14];
    CEngineInternalPrimitiveBase* FirstMovingPrimitive;   // +0x20
    bool PreloadPrimitives(const CShadowClipVolume& volume, const C3DVector& position, unsigned long flags);
};

bool CEngineScene::PreloadPrimitives(const CShadowClipVolume& volume, const C3DVector& position, unsigned long flags)
{
    bool preloaded = false;
    for (COuterNode* outer = MapListHead->Left; outer != MapListHead; outer = Next(outer))
        for (CMapNode* inner = outer->Maps.Head->Left; inner != outer->Maps.Head; inner = Next(inner))
        {
            CEngineSceneGrid* grid = inner->Data->GridLevelsFirst->m_pObject;
            preloaded |= grid->PreloadPrimitives(0, 0, volume, position, flags);
        }
    C3DBoundingSphere sphere;
    for (CEngineInternalPrimitiveBase* primitive = FirstMovingPrimitive; primitive; primitive = primitive->NextPrimitive) {
        sphere.Center.X = 0.0f;
        sphere.Center.Y = 0.0f;
        sphere.Center.Z = 0.0f;
        sphere.Radius = 0.0f;
        if (primitive->GetBoundingSphereWorldSpace(sphere) && volume.IsSphereInside(sphere))
            preloaded |= g_pEnginePrimitiveTypeInfo->PrimitiveManagers[primitive->Type]->PreloadPrimitive(*primitive, position, flags);
    }
    return preloaded;
}
