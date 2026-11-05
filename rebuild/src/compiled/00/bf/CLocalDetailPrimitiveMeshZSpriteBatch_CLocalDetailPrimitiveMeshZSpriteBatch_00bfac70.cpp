// CLocalDetailPrimitiveMeshZSpriteBatch::CLocalDetailPrimitiveMeshZSpriteBatch(const CLocalDetailObjectCollectionType* type, CLocalDetailCacheMap* map) -- the local-detail primitive base, this class's vtable, an empty z-sprite primitive handle, then the object and z-sprite arrays and the mesh-handle flag cleared
// Leaf: ??0CLocalDetailPrimitiveMeshZSpriteBatch@@QAE@PBVCLocalDetailObjectCollectionType@@PAVCLocalDetailCacheMap@@@Z
// Plan phase 4 (2026-11-05), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BFAC70, 78 bytes, thiscall, `ret 8`, returns this.
// Identity (proven): stores vtable 0x012A82D8 (RTTI .?AVCLocalDetailPrimitiveMeshZSpriteBatch@@, slot 0 = ??_G 0x00BFBD80);
//   Ego_r.pdb ??0CLocalDetailPrimitiveMeshZSpriteBatch@@QAE@PBVCLocalDetailObjectCollectionType@@PAVCLocalDetailCacheMap@@@Z
//   (the only constructor in the PDB name set; same argument list as the landed sibling ctors 0x00BDFB50 / 0x00BFAA80).
// Callees: CBaseClass::CBaseClass 0x0099A2D0 (the ZSpriteHandle's base); vtable 0x0122F598 CEnginePrimitiveHandle.
// Layout (Ego_r.pdb, 0x50): CLocalDetailPrimitiveBase 0x38, Objects +0x38, ZSpriteEntries +0x3C, ObjectCount +0x40,
//   HasMeshPrimitiveHandles +0x44, ZSpriteHandle CEnginePrimitiveHandle +0x48.
// Shape: the landed 0x00BFAA80 model; ObjectCount is left uninitialised (retail never stores +0x40).
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
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

#include "engine/C3DVector.h"
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

class CLocalDetailPrimitiveMeshZSpriteBatch : public CLocalDetailPrimitiveBase {
public:
    class CObject;
    class CObjectEntry;
    CLocalDetailPrimitiveMeshZSpriteBatch(const CLocalDetailObjectCollectionType* type, CLocalDetailCacheMap* map);
    virtual ~CLocalDetailPrimitiveMeshZSpriteBatch();
    CObject* Objects;                                // +0x38
    CObjectEntry* ZSpriteEntries;                    // +0x3C
    long ObjectCount;                                // +0x40
    bool HasMeshPrimitiveHandles;                    // +0x44
    CEnginePrimitiveHandle ZSpriteHandle;            // +0x48
};

CLocalDetailPrimitiveMeshZSpriteBatch::CLocalDetailPrimitiveMeshZSpriteBatch(const CLocalDetailObjectCollectionType* type, CLocalDetailCacheMap* map)
    : CLocalDetailPrimitiveBase(type, map)
{
    Objects = 0;
    ZSpriteEntries = 0;
    HasMeshPrimitiveHandles = false;
}