// CLocalDetailPrimitiveRepeatedMesh::CLocalDetailPrimitiveRepeatedMesh(const CLocalDetailObjectCollectionType* type, CLocalDetailCacheMap* map) -- the local-detail primitive base, this class's vtable, an empty primitive handle, then every count and array pointer cleared
// Leaf: ??0CLocalDetailPrimitiveRepeatedMesh@@QAE@PBVCLocalDetailObjectCollectionType@@PAVCLocalDetailCacheMap@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BFAA80, 93 bytes, thiscall, `ret 8`, returns this.
// Identity (proven): stores vtable 0x012A82AC (RTTI .?AVCLocalDetailPrimitiveRepeatedMesh@@ : CLocalDetailPrimitiveBase :
//   CMovableResource); Ego_r.pdb ??0CLocalDetailPrimitiveRepeatedMesh@@QAE@PBVCLocalDetailObjectCollectionType@@PAVCLocalDetailCacheMap@@@Z.
// Callees: CBaseClass::CBaseClass 0x0099A2D0 (the PrimitiveHandle's base); vtable 0x0122F598 CEnginePrimitiveHandle.
// Layout (Ego_r.pdb): CLocalDetailPrimitiveBase 0x38, MaxObjectScale +0x38, ObjectCount +0x3C, SubSectionsCount +0x40,
//   SubSections +0x44, ObjectMatricies +0x48, ObjectOffsets +0x4C, WindDelayArray +0x50, LandscapeNormalArray +0x54,
//   PrimitiveHandle +0x58 (0x60 bytes).
// Shape: as CLocalDetailPrimitiveMesh 0x00BDFB50, then the eight members cleared in the body (after the handle member is
//   constructed), in retail's store order.
class CBaseClass {
public:
    CBaseClass();                                    // 0x0099A2D0
    virtual ~CBaseClass();                           // 0x0099A2E0
};
class CIVCountedPointeeBase;
template <class T> class CIVCountedPointer {
public:
    CIVCountedPointer() : Object(0) { }
    CIVCountedPointeeBase* Object;
};
class CEnginePersistentPrimitiveBase;
class CEnginePrimitiveHandle : public CBaseClass {
public:
    CEnginePrimitiveHandle() { }
    virtual ~CEnginePrimitiveHandle();
    CIVCountedPointer<CEnginePersistentPrimitiveBase> Data;   // +0x04
};

class C3DVector {
public:
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    float X, Y, Z;
};
class C3DBoundingBox {
public:
    C3DVector Min, Max;
};
class C3DBoundingSphere {
public:
    C3DBoundingSphere() : Center(0.0f, 0.0f, 0.0f), Radius(0.0f) { }
    C3DVector Center;
    float Radius;
};
class CMatrix3x4 {
public:
    float E11, E12, E13, E21, E22, E23, E31, E32, E33, E41, E42, E43;
};

class CMovableResource {
public:
    CMovableResource() { }
    virtual ~CMovableResource();
};
class CLocalDetailObjectCollectionType;
class CLocalDetailCacheMap;

class CLocalDetailPrimitiveBase : public CMovableResource {
public:
    CLocalDetailPrimitiveBase(const CLocalDetailObjectCollectionType* type, CLocalDetailCacheMap* map)
        : Next(0), Type(type), Map(map) { }
    CLocalDetailPrimitiveBase* Next;                 // +0x04
    C3DBoundingBox BoundingBox;                      // +0x08
    C3DBoundingSphere BoundingSphere;                // +0x20
    const CLocalDetailObjectCollectionType* Type;    // +0x30
    CLocalDetailCacheMap* Map;                       // +0x34
};

class CSubsectionElement;
class C4DVector;

class CLocalDetailPrimitiveRepeatedMesh : public CLocalDetailPrimitiveBase {
public:
    CLocalDetailPrimitiveRepeatedMesh(const CLocalDetailObjectCollectionType* type, CLocalDetailCacheMap* map);
    virtual ~CLocalDetailPrimitiveRepeatedMesh();
    float MaxObjectScale;                            // +0x38
    long ObjectCount;                                // +0x3C
    long SubSectionsCount;                           // +0x40
    CSubsectionElement* SubSections;                 // +0x44
    C4DVector* ObjectMatricies;                      // +0x48
    C4DVector* ObjectOffsets;                        // +0x4C
    unsigned char* WindDelayArray;                   // +0x50
    float* LandscapeNormalArray;                     // +0x54
    CEnginePrimitiveHandle PrimitiveHandle;          // +0x58
};

CLocalDetailPrimitiveRepeatedMesh::CLocalDetailPrimitiveRepeatedMesh(const CLocalDetailObjectCollectionType* type, CLocalDetailCacheMap* map)
    : CLocalDetailPrimitiveBase(type, map)
{
    WindDelayArray = 0;
    LandscapeNormalArray = 0;
    MaxObjectScale = 0.0f;
    ObjectCount = 0;
    SubSections = 0;
    SubSectionsCount = 0;
    ObjectMatricies = 0;
    ObjectOffsets = 0;
}