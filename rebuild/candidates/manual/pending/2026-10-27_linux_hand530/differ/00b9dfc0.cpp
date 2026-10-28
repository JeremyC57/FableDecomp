// CEngineInternalPrimitive2DStaticMesh::Update(const CEnginePrimitive& primitive, unsigned long flags) -- take a 2D static mesh primitive description: the flags, the static mesh info from the main mesh bank, alpha, tint, clip box, the fixed 2D layer, interpolation set and orientation (both orientation frames re-orthogonalised); the bounding sphere is the description's scaled by 100 when it has a radius, else the mesh's own; mark the primitive for re-initialisation
// Grade: DIFFER(341v341) -- MODEL ERROR ONLY, not a codegen residue: the graded build derived `this` from the flattened header and declared Update virtual, adding a second vptr (every member +4: `mov [esi+10h],ebx` for `[esi+0Ch]`); this file is the corrected polymorphic model, NOT regraded (4-grade budget spent). The same body over a vptr-free model graded RELOCATION_MATCH in probe NA (probe/mvdfc0c.cpp, all four variants matched) | next: one regrade of this file is expected to give RELOCATION_MATCH
// Leaf: ?Update@CEngineInternalPrimitive2DStaticMesh@@UAEXABVCEnginePrimitive@@K@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B9DFC0, length 341 bytes, `ret 8`.
// Name: FableWin.pdb `?Update@CEngineInternalPrimitive2DStaticMesh@@UAEXABVCEnginePrimitive@@K@Z` (virtual; catalog spelling agrees).
// Callees: CMeshDataBank::PeekStaticMesh 0x00A24280 (landed), C3DOrientationInterpolator::operator= 0x006B0E00 (landed, FableWin
//   `??4C3DOrientationInterpolator@@QAEAAV0@ABV0@@Z`), CRightHandedSet::MakePerpendicular 0x00A22E10 (FableWin
//   `?MakePerpendicular@CRightHandedSet@@QAEXXZ`; catalog label "CRightHandedSet::MakePerpendicular" agrees).
// Inlines (FableWin.pdb): CEngineResourceManager::PeekMeshBank(EMeshBankNo) const -- the manager is read before the mesh index
//   (a direct `MeshBanks[0]->` call pushes the index first); C3DMeshInfo::GetBoundingSphere() by value -- with the C3DBoundingSphere
//   / C3DVector memberwise copy constructors the copy goes through a frame temporary (the 0x10-byte frame) as in retail.
// Globals: 0x01436E98 the engine resource manager (MeshBanks[3] +0x10, engine/CEngineResourceManager.h; spelling
//   g_pEngineResourceManager_01436e98 is descriptive, see NOTES). Constants 0.0f @ 0x0122DEDC, 100.0f @ 0x01230A08.
// Layout: CEnginePrimitive / CEnginePrimitive2DStaticMesh = Ego_r.pdb (MeshIndex +0x0C, ClipBox +0x10, Alpha +0x20, Orientation
//   +0x24, BoundingSphere +0x58, TintColour +0x68); `this` = the offsets of engine/CEngineInternalPrimitive2DStaticMesh.h plus the
//   base members it leaves as padding (Ego_r.pdb CEngineInternalPrimitiveBase EnginePrimitiveFlags +0x0C, Alpha +0x27,
//   InterpolationSet +0x2C; CEngineInternalPrimitiveMeshBase MeshInfo +0x50, TintColour +0x5C).
// Enumerators: EMeshBankNo 0 and the 2D layer 0x48 have no recovered names (cast literals).
#define CEngineInternalPrimitive2DStaticMesh CEngineInternalPrimitive2DStaticMesh_HeaderLayout
#include "engine/CEngineInternalPrimitive2DStaticMesh.h"
#undef CEngineInternalPrimitive2DStaticMesh
#define CEngineResourceManager CEngineResourceManagerLayout
#include "engine/CEngineResourceManager.h"
#undef CEngineResourceManager

class C3DVector {
public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }
    C3DVector operator*(float s) const { return C3DVector(X * s, Y * s, Z * s); }
};
class C3DBoundingSphere { public:
    C3DVector Center;                                      // +0x00
    float     Radius;                                      // +0x0c
    C3DBoundingSphere() { }
    C3DBoundingSphere(const C3DBoundingSphere& o) : Center(o.Center), Radius(o.Radius) { }
};
class C2DBoxF { public: float TLX, TLY, BRX, BRY; };
class CRGBColour { public: unsigned char B, G, R, A; };
class CRightHandedSet { public:
    C3DVector Up, Forward;
    void MakePerpendicular();                              // 0x00A22E10
};
class C3DOrientationInterpolator { public:
    void*           __vftable;                             // +0x00
    CRightHandedSet StartOrientation;                      // +0x04
    CRightHandedSet EndOrientation;                        // +0x1c
    C3DOrientationInterpolator& operator=(const C3DOrientationInterpolator& other);   // 0x006B0E00
};
class C3DMeshInfo { public:                                // CMeshDataBankEntry base: vptr, Bank, Index
    unsigned char     _pad00[0x28];
    C3DBoundingSphere BoundingSphere;                      // +0x28
    C3DBoundingSphere GetBoundingSphere() const { return BoundingSphere; }   // FableWin ?GetBoundingSphere@C3DMeshInfo@@QBE?AVC3DBoundingSphere@@XZ
};
class CMeshDataBank { public:
    const C3DMeshInfo* PeekStaticMesh(unsigned long index) const;   // 0x00A24280
};
enum EMeshBankNo { };
class CEngineResourceManager : public CEngineResourceManagerLayout {
public:
    // FableWin.pdb ?PeekMeshBank@CEngineResourceManager@@QBEPBVCMeshDataBank@@W4EMeshBankNo@@@Z (inline here)
    const CMeshDataBank* PeekMeshBank(EMeshBankNo bankNo) const { return MeshBanks[bankNo]; }
};
extern CEngineResourceManager* g_pEngineResourceManager_01436e98;   // 0x01436E98
enum EEngine2DLayer { };
enum EEngineInterpolationSet { };
class CEnginePrimitive { public:
    long                    Type;                          // +0x00 EEnginePrimitiveType
    EEngineInterpolationSet InterpolationSet;              // +0x04
    unsigned long           ThemeChannelDesc;              // +0x08
};
class CEnginePrimitive2DStaticMesh : public CEnginePrimitive { public:
    unsigned long              MeshIndex;                  // +0x0c
    C2DBoxF                    ClipBox;                    // +0x10
    unsigned char              Alpha;                      // +0x20
    C3DOrientationInterpolator Orientation;                // +0x24
    C3DBoundingSphere          BoundingSphere;             // +0x58
    CRGBColour                 TintColour;                 // +0x68
};
// `this`: a genuine polymorphic local model (engine/CEngineInternalPrimitive2DStaticMesh.h flattens the vptr into a data member,
// so deriving from it and declaring the virtual Update adds a second vptr and shifts every member by 4); offsets and names as
// the header / Ego_r.pdb.
class CEngineInternalPrimitive2DStaticMesh {
public:
    virtual void Update(const CEnginePrimitive& primitive, unsigned long flags);
    unsigned char              _pad04[0x0c - 0x04];
    unsigned long              EnginePrimitiveFlags;   // +0x0c CEngineInternalPrimitiveBase
    unsigned char              _pad10[0x27 - 0x10];
    unsigned char              Alpha;                  // +0x27 CEngineInternalPrimitiveBase
    unsigned char              _pad28[0x2c - 0x28];
    EEngineInterpolationSet    InterpolationSet;       // +0x2c CEngineInternalPrimitiveBase
    unsigned char              _pad30[0x50 - 0x30];
    const C3DMeshInfo*         MeshInfo;               // +0x50 CEngineInternalPrimitiveMeshBase
    unsigned char              _pad54[0x5c - 0x54];
    CRGBColour                 TintColour;             // +0x5c CEngineInternalPrimitiveMeshBase
    unsigned char              _pad60[0xac - 0x60];
    bool                       Initialised;            // +0xac
    unsigned char              _padad[0xb0 - 0xad];
    C2DBoxF                    ClipBox;                // +0xb0
    EEngine2DLayer             Layer;                  // +0xc0
    C3DBoundingSphere          BoundingSphere;         // +0xc4
    C3DOrientationInterpolator Orientation;            // +0xd4
};

void CEngineInternalPrimitive2DStaticMesh::Update(const CEnginePrimitive& primitive, unsigned long flags)
{
    EnginePrimitiveFlags = flags;
    const CEnginePrimitive2DStaticMesh& desc = static_cast<const CEnginePrimitive2DStaticMesh&>(primitive);
    MeshInfo = g_pEngineResourceManager_01436e98->PeekMeshBank((EMeshBankNo)0)->PeekStaticMesh(desc.MeshIndex);
    Alpha = desc.Alpha;
    TintColour = desc.TintColour;
    ClipBox = desc.ClipBox;
    Layer = (EEngine2DLayer)0x48;
    InterpolationSet = desc.InterpolationSet;
    Orientation = desc.Orientation;
    Orientation.StartOrientation.MakePerpendicular();
    Orientation.EndOrientation.MakePerpendicular();
    if (flags & 0x80)
        EnginePrimitiveFlags |= 0x80;
    if (desc.BoundingSphere.Radius > 0.0f) {
        BoundingSphere.Center = desc.BoundingSphere.Center * 100.0f;
        BoundingSphere.Radius = desc.BoundingSphere.Radius * 100.0f;
    } else
        BoundingSphere = MeshInfo->GetBoundingSphere();
    Initialised = false;
}
