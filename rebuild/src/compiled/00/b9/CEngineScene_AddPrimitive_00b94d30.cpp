// CEngineScene::AddPrimitive(CEngineInternalPrimitiveBase* primitive, unsigned long flags) -- register a primitive with the scene: an enabled persistent primitive with a world bounding box (and only the grid flag of 0x4001 set) goes into the scene map under its 2D centre with its 2D diameter; otherwise it is linked into the persistent or (add-ref'd) non-persistent loose list
// Grade: RELOCATION_MATCH (retry A 11b) -- centre and extents are named 3D vectors built with the operator form ((Min + Max) * 0.5f, (Max - Min) * 0.5f); the unused Z component keeps a 12-byte extents record (the spare frame dword) and the C2DVector argument temporaries give the two centre copies
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B94D30, length 306 bytes (`ret 8`).
// Callees: the primitive's virtual slot 17 (+0x44, GetBoundingBoxWorldSpace(C3DBoundingBox&) -> bool), CEngineScene::GetMapAt
// (const C2DVector&) @ 0x00B94BF0, CEngineSceneMap::AddPrimitive(CEngineInternalPrimitiveBase*, const C2DVector&, float)
// @ 0x00B93B30, CEngineInternalPrimitiveBase::AddToList(CEngineInternalPrimitiveBase**) @ 0x00B8FDF0. 0.5f @ 0x0122F59C.
// Layout: primitive = CEngineInternalPrimitiveBase (IVRefCount +4, RenderLayerMask +0x30, Enable bit 0 / NonPersistent bit 1
// of +0x34); scene fields as the RenderLayerCells model 0x00B93F40 (grid layer mask +0x1c, the two loose lists +0x20/+0x24)
// plus the three counters +0x38 (grid), +0x3c (persistent), +0x40 (non-persistent) -- descriptive names.
#include <math.h>
#pragma intrinsic(sqrt)
class C2DVector {
public:
    float X, Y;
    C2DVector() { }
    C2DVector(float x, float y) : X(x), Y(y) { }
};

struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    __forceinline C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); }
    __forceinline C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    __forceinline C3DVector operator*(float f) const { return C3DVector(X * f, Y * f, Z * f); }
    __forceinline float Length2D() const { return (float)sqrt(X * X + Y * Y); }
};
struct C3DBoundingBox {
    C3DVector Min, Max;
};

class CEngineInternalPrimitiveBase {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16();
    virtual bool GetBoundingBoxWorldSpace(C3DBoundingBox& box);   // slot 17 (+0x44)
    long          IVRefCount;                      // +0x04
    unsigned char _pad08[0x28];
    unsigned long RenderLayerMask;                 // +0x30
    unsigned char Enable : 1;                      // +0x34 bit 0
    unsigned char NonPersistent : 1;               // +0x34 bit 1
    void AddToList(CEngineInternalPrimitiveBase** list);   // 0x00B8FDF0
};

struct CEngineSceneMap {
    void AddPrimitive(CEngineInternalPrimitiveBase* primitive, const C2DVector& centre, float diameter);   // 0x00B93B30
};

struct CEngineScene {
    unsigned char                 _pad00[0x1c];
    unsigned long                 GridLayerMask;            // +0x1c
    CEngineInternalPrimitiveBase* PersistentList;           // +0x20
    CEngineInternalPrimitiveBase* NonPersistentList;        // +0x24
    unsigned char                 _pad28[0x10];
    long                          GridPrimitiveCount;       // +0x38
    long                          PersistentCount;          // +0x3c
    long                          NonPersistentCount;       // +0x40
    CEngineSceneMap* GetMapAt(const C2DVector& position);   // 0x00B94BF0
    void AddPrimitive(CEngineInternalPrimitiveBase* primitive, unsigned long flags);
};

void CEngineScene::AddPrimitive(CEngineInternalPrimitiveBase* primitive, unsigned long flags)
{
    unsigned long layerMask = primitive->RenderLayerMask;
    C3DBoundingBox box;
    if (!primitive->NonPersistent && primitive->Enable && (flags & 0x4001) == 1 && primitive->GetBoundingBoxWorldSpace(box)) {
        C3DVector centre = (box.Min + box.Max) * 0.5f;
        C3DVector extents = (box.Max - box.Min) * 0.5f;
        GetMapAt(C2DVector(centre.X, centre.Y))->AddPrimitive(primitive, C2DVector(centre.X, centre.Y), extents.Length2D() * 2.0f);
        GridLayerMask |= layerMask;
        ++GridPrimitiveCount;
        return;
    }
    if (!primitive->NonPersistent) {
        primitive->AddToList(&PersistentList);
        ++PersistentCount;
    } else {
        ++primitive->IVRefCount;
        primitive->AddToList(&NonPersistentList);
        ++NonPersistentCount;
    }
}