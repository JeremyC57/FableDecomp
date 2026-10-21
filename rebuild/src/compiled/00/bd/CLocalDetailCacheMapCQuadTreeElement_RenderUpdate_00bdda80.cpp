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
struct C3DVector {
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
struct C3DBoundingSphere {
    C3DVector Center;                                  // +0x00
    float     Radius;                                  // +0x0c
};
struct CEngineCamera {
    unsigned char _pad00[0x54];
    C3DVector     Position;                            // +0x54
    bool ClipSphere(const C3DVector& centre, float radius, unsigned long* flags);   // 0x00B30480
};
struct CEngineLocalDetailGenerator {
    float ModifyFadeDistanceForVideoOptions(unsigned long primitiveTypeMask, float fadeDistance);   // 0x00B597E0
};
extern CEngineLocalDetailGenerator* g_pLocalDetailGenerator;   // 0x01436E88

struct CLocalDetailCacheMap {
    struct CObjectCacheGroupCollection {
        void* __vftable;
        CObjectCacheGroupCollection* Next;             // +0x04
        void RenderUpdateGroups(CEngineCamera* camera, float nearDistance, float farDistance, unsigned long clipFlags, float lodDistance);   // 0x00BDD930
    };
    struct CQuadTreeElement {
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