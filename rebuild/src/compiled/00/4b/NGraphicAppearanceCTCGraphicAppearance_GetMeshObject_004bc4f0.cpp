// NGraphicAppearance::CTCGraphicAppearance::GetMeshObject(const CMeshInfo& info, float interpolation, C3DMeshObject& out) const -- build the mesh object for a mesh entry at the owner's interpolated render position and orientation (the current orientation when interpolation == 1), scaled by the entry, the appearance and 1/100, into the caller's object
// Leaf: ?GetMeshObject@CTCGraphicAppearance@NGraphicAppearance@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x004BC4F0, 303 bytes, thiscall `ret 0Ch`. Ego_r.pdb
//   `?GetMeshObject@CTCGraphicAppearance@NGraphicAppearance@@IBEXABVCMeshInfo@2@MAAVC3DMeshObject@@@Z`; the catalog label
//   "CTCGraphicAppearance_CreateAbilityMeshObject" is descriptive.
// Callees: the file-static GetRenderPositionForThing(const CThing&, float) @ 0x004BBF00 (landed; __fastcall, hidden
//   C3DVector in ecx, thing in edx), the mesh bank's GetMeshInfo @ 0x00A24270 on 0x013B8A04 (manifest
//   "CTCInventoryAbilities::GetAbilityLevel", propagated), the physics TC's GetRHSet() slot 72 (+0x120, a reference) and
//   GetRHSet(float) slot 71 (+0x11C, by value; landed 0x006B1450), C3DMeshObject(const C3DMeshInfo*, const C3DVector&,
//   const CRightHandedSet&, float) @ 0x00A36E70, C3DMeshObject::operator= @ 0x004C3380, operator delete @ 0x00BFE9BC,
//   CBaseClass::~CBaseClass @ 0x0099A2E0. Floats: 1.0f @ 0x0122DED8, 0.01f @ 0x01230A5C.
// Layout: as the landed sibling GetMeshObjectAssumingObjectAt 0x004BD160 (CMeshInfo Index +0 / SizeX +4, appearance
//   Scale +0x34, C3DMeshObject 0x78 with the Mesh counted pointer at +8); CThing PhysicsTC +0x60 (retail).
// Shape: the physics pointer is read first, then the render position, then the mesh info; `interpolation == 1.0f`
//   picks the overload; the common scale argument is hoisted and pushed before the branch; each arm assigns a
//   temporary destroyed inline. MSVC lays the GetRHSet overload set out in reverse declaration order, so the
//   by-reference GetRHSet() is declared first to land in slot 72.
void __cdecl operator delete(void* p);                      // 0x00BFE9BC

class C3DVector {
public:
    float X, Y, Z;
    C3DVector() {}
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) {}
};
class CRightHandedSet {
public:
    C3DVector Up;                                           // +0x00
    C3DVector Forward;                                      // +0x0c
};
class C3DMeshInfo;

struct SCountBlock {
    long m_references;                                      // +0
    void (__fastcall* m_pfnDestroy)(void*);                 // +4
    void* m_pObject;                                        // +8
};
class CCountedPointer {
public:
    void* m_pObject;                                        // +0
    SCountBlock* m_pCount;                                  // +4
    ~CCountedPointer()
    {
        if (m_pCount) {
            --m_pCount->m_references;
            if (m_pCount->m_references == 0) {
                m_pCount->m_pfnDestroy(m_pCount->m_pObject);
                delete m_pCount;
            }
        }
        m_pObject = 0;
        m_pCount = 0;
    }
};

#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
class C3DMeshObject : public CBaseClass {                        // vtable 0x01239860, 0x78 bytes
public:
    const C3DMeshInfo* MeshInfo;                            // +0x4
    CCountedPointer    Mesh;                                // +0x8 CCountedPointer<const C3DMeshStats>
    unsigned char      _rest[0x78 - 0x10];                  // +0x10 PositionMatrix .. Scale
    C3DMeshObject(const C3DMeshInfo* info, const C3DVector& position, const CRightHandedSet& orientation, float scale);   // 0x00A36E70
    C3DMeshObject& operator=(const C3DMeshObject& other);   // 0x004C3380
    virtual ~C3DMeshObject();
};
C3DMeshObject::~C3DMeshObject()
{
}

class CMeshDataBank {
public:
    const C3DMeshInfo* GetMeshInfo(long index);             // 0x00A24270
};
extern CMeshDataBank* g_pMeshDataBank;                      // 0x013B8A04

#define VSLOT(n) virtual void vslot##n();
#define VSLOT10(n) VSLOT(n##0) VSLOT(n##1) VSLOT(n##2) VSLOT(n##3) VSLOT(n##4) VSLOT(n##5) VSLOT(n##6) VSLOT(n##7) VSLOT(n##8) VSLOT(n##9)
class CTCPhysicsBase {
public:
    VSLOT10(0) VSLOT10(1) VSLOT10(2) VSLOT10(3) VSLOT10(4) VSLOT10(5) VSLOT10(6)
    VSLOT(70)
    // MSVC lays an overload set out in reverse declaration order: GetRHSet(float) lands in slot 71, GetRHSet() in 72
    virtual const CRightHandedSet& GetRHSet() const;              // slot 72 (+0x120)
    virtual CRightHandedSet GetRHSet(float interpolation) const;   // slot 71 (+0x11C)
};
class CThing {
public:
    unsigned char   _pad00[0x60];
    CTCPhysicsBase* PhysicsTC;                              // +0x60 (retail; PDB +0x70)
};

namespace NGraphicAppearance {
C3DVector __fastcall GetRenderPositionForThing(const CThing& thing, float interpolation);   // 0x004BBF00 (file-static)

class CMeshInfo {
public:
    long          Index;                                    // +0x0
    float         SizeX;                                    // +0x4
    unsigned char PrimaryMesh : 1;                          // +0x8
};

class CTCGraphicAppearance {
public:
    void*         __vfptr;                                  // +0x00
    CThing*       ParentThing;                              // +0x04 (CTCBase)
    unsigned char _pad08[0x34 - 0x08];
    float         Scale;                                    // +0x34
    void GetMeshObject(const CMeshInfo& info, float interpolation, C3DMeshObject& out) const;
};

void CTCGraphicAppearance::GetMeshObject(const CMeshInfo& info, float interpolation, C3DMeshObject& out) const
{
    const CTCPhysicsBase* physics = ParentThing->PhysicsTC;
    C3DVector position = GetRenderPositionForThing(*ParentThing, interpolation);
    const C3DMeshInfo* meshInfo = g_pMeshDataBank->GetMeshInfo(info.Index);
    if (interpolation == 1.0f)
        out = C3DMeshObject(meshInfo, position, physics->GetRHSet(), info.SizeX * Scale * 0.01f);
    else
        out = C3DMeshObject(meshInfo, position, physics->GetRHSet(interpolation), info.SizeX * Scale * 0.01f);
}

}