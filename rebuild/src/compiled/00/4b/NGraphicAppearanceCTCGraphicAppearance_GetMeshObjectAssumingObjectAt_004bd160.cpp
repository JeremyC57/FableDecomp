// NGraphicAppearance::CTCGraphicAppearance::GetMeshObjectAssumingObjectAt(const CMeshInfo&, const C3DVector&, const CRightHandedSet&, C3DMeshObject&) const -- build the mesh object for a mesh entry placed at the given position/orientation, scaled by the entry, the appearance and 1/100, into the caller's object
// Leaf: GetMeshObjectAssumingObjectAt
//
// Retail 0x004BD160, 155 bytes (`ret 10h`). Ego_r.pdb `?GetMeshObjectAssumingObjectAt@CTCGraphicAppearance@
// NGraphicAppearance@@IBEXABVCMeshInfo@2@ABVC3DVector@@ABVCRightHandedSet@@AAVC3DMeshObject@@@Z`.
// Callees: the mesh bank's mesh-info lookup @ 0x00A24270 on the global bank @ 0x013B8A04 (manifest
// "CTCInventoryAbilities::GetAbilityLevel", propagated), C3DMeshObject::C3DMeshObject(const C3DMeshInfo*,
// const C3DVector&, const CRightHandedSet&, float) @ 0x00A36E70, C3DMeshObject::operator= @ 0x004C3380,
// operator delete @ 0x00BFE9BC, CBase::~CBase @ 0x0099A2E0.
// Layout: CMeshInfo (Ego_r.pdb, 0xc): Index +0, SizeX +4; retail CTCGraphicAppearance Scale +0x34
// (PDB +0x3c: the two maps ahead are 12-byte retail maps); C3DMeshObject (0x78) as the landed deleting
// destructor 0x004C3250 models it (Mesh counted pointer at +8/+0xc).
// Shape: the bank's mesh info is a named local fetched first, then
// `out = C3DMeshObject(meshInfo, position, orientation, scale)`: the temporary is built in the frame,
// assigned out of line, then destroyed inline (vtable store, counted-pointer release with re-reads of
// the block, both words zeroed, ~CBase); the scale is `SizeX * Scale * 0.01f`.
void __cdecl operator delete(void* p);                      // 0x00BFE9BC

class C3DVector { public: float X, Y, Z; };
class CRightHandedSet;
class C3DMeshInfo;

struct SCountBlock {
    long m_references;                                      // +0
    void (__fastcall* m_pfnDestroy)(void*);                 // +4
    void* m_pObject;                                        // +8
};

struct CCountedPointer {
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

struct CBase {
    virtual ~CBase();                                       // 0x0099A2E0
};

class C3DMeshObject : public CBase { public:                              // vtable 0x01239860, 0x78 bytes
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

class CMeshDataBank { public:
    const C3DMeshInfo* GetMeshInfo(long index);             // 0x00A24270
};
extern CMeshDataBank* g_pMeshDataBank;                      // 0x013B8A04

namespace NGraphicAppearance {

class CMeshInfo { public:
    long          Index;                                    // +0x0
    float         SizeX;                                    // +0x4
    unsigned char PrimaryMesh : 1;                          // +0x8
};

class CTCGraphicAppearance { public:
    unsigned char _pad00[0x34];
    float         Scale;                                    // +0x34
    void GetMeshObjectAssumingObjectAt(const CMeshInfo& info, const C3DVector& position,
                                       const CRightHandedSet& orientation, C3DMeshObject& out) const;
};

void CTCGraphicAppearance::GetMeshObjectAssumingObjectAt(const CMeshInfo& info, const C3DVector& position,
                                                         const CRightHandedSet& orientation, C3DMeshObject& out) const
{
    const C3DMeshInfo* meshInfo = g_pMeshDataBank->GetMeshInfo(info.Index);
    out = C3DMeshObject(meshInfo, position, orientation, info.SizeX * Scale * 0.01f);
}

}