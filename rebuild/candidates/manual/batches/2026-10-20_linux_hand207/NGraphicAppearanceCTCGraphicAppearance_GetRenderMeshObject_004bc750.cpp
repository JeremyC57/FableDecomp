// NGraphicAppearance::CTCGraphicAppearance::GetRenderMeshObject(C3DMeshObject& out, const C3DVector& position, const CRightHandedSet& orientation) const -- for a mesh main graphic (type 4 or 5) build its render mesh object at the given placement; false for any other graphic
// Leaf: GetRenderMeshObject
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004BC750, length 173 bytes (+ int3 padding). __thiscall, `ret 0Ch`.
//         Ego_r.pdb: `?GetRenderMeshObject@CTCGraphicAppearance@NGraphicAppearance@@QBE_NAAVC3DMeshObject@@ABVC3DVector@@AB
//         VCRightHandedSet@@@Z`.
// Callees: CTCBase::GetWorld() const @ 0x00686A80, the main mesh bank's GetMeshInfo(long) @ 0x00A24270,
//          C3DMeshObject::C3DMeshObject(const C3DMeshInfo*, const C3DVector&, const CRightHandedSet&, float) @ 0x00A36E70,
//          C3DMeshObject::operator= @ 0x004C3380, operator delete @ 0x00BFE9BC, CBaseClass::~CBaseClass @ 0x0099A2E0.
//          Float pool 0x01230A5C = 0.01f.
// Shape: the landed GetMeshObjectAssumingObjectAt (0x004BD160) body on the main graphic: `out = C3DMeshObject(info,
//        position, orientation, RenderSizeX * Scale * 0.01f)`, the temporary built in the 0x78-byte frame, assigned out
//        of line and destroyed inline (vtable store, counted-pointer release re-reading its block, both words zeroed,
//        ~CBaseClass); the bank is a named local read through the world (loaded into eax before the index push).
// Layout: retail CTCGraphicAppearance: Scale +0x34, MainGraphic +0x44 (BankIndex +0x44, RenderSizeX +0x4C, Type +0x51);
//         retail CWorld::PMainMeshBank +0x3C; C3DMeshObject 0x78 bytes (landed model).
void __cdecl operator delete(void* p);                      // 0x00BFE9BC

struct C3DVector { float X, Y, Z; };
struct CRightHandedSet;
struct C3DMeshInfo;

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

struct CBaseClass {                                         // vtable 0x01231710
    virtual ~CBaseClass();                                  // 0x0099A2E0
};

struct C3DMeshObject : CBaseClass {                         // vtable 0x01239860, 0x78 bytes
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

struct CMeshDataBank {
    const C3DMeshInfo* GetMeshInfo(long index);             // 0x00A24270
};
struct CWorld {
    unsigned char _pad00[0x3c];
    CMeshDataBank* PMainMeshBank;                           // +0x3C
};

#pragma pack(push, 2)
struct CEngineGraphic {                                     // 14 bytes
    unsigned long BankIndex;                                // +0x00
    float AnimStep;                                         // +0x04
    float RenderSizeX;                                      // +0x08
    bool AdditiveAlpha;                                     // +0x0C
    unsigned char Type;                                     // +0x0D
};
#pragma pack(pop)

namespace NGraphicAppearance {

struct CTCGraphicAppearance {
    unsigned char  _pad00[0x34];
    float          Scale;                                   // +0x34
    unsigned char  _pad38[0x44 - 0x38];
    CEngineGraphic MainGraphic;                             // +0x44
    CWorld* GetWorld() const;                               // 0x00686A80 (CTCBase)
    bool GetRenderMeshObject(C3DMeshObject& out, const C3DVector& position, const CRightHandedSet& orientation) const;
};

bool CTCGraphicAppearance::GetRenderMeshObject(C3DMeshObject& out, const C3DVector& position, const CRightHandedSet& orientation) const
{
    if (MainGraphic.Type != 5 && MainGraphic.Type != 4)
        return false;
    CMeshDataBank* bank = GetWorld()->PMainMeshBank;
    const C3DMeshInfo* meshInfo = bank->GetMeshInfo(MainGraphic.BankIndex);
    out = C3DMeshObject(meshInfo, position, orientation, MainGraphic.RenderSizeX * Scale * 0.01f);
    return true;
}

}
