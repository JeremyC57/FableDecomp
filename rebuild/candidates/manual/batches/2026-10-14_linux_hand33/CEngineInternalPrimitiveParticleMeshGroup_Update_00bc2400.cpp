// CEngineInternalPrimitiveParticleMeshGroup::Update(const SUpdate& update, unsigned long flags) -- stores the primitive flags, resolves the update's mesh id to its C3DMeshInfo through the engine data's mesh bank, copies the world transform and the two blend settings
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BC2400, length 82 bytes (96 with padding).
// Callees: the mesh bank's GetMeshInfo(long) @ 0x00A242A0 (on the bank at engine data +0x10; returns `[this+380h][id]`). Globals: the engine data @ 0x01436E98
// (see CEngine::GetLoadingMemoryPool @ 0x00B25C80, landed). Shape: the flags store precedes the call; the bank is a named local so the engine-data read is scheduled before the id argument (a bare object expression loads it after), the 0x30-byte matrix is a POD block copy (`rep movsd`
// of 12 dwords), then BlendOp (+0x44 of the update) and BlendMode (+0x40) are stored in that order. Layout: the retail object is 0x14 bytes shorter than
// engine/CEngineInternalPrimitiveParticleMeshGroup.h (PDB MeshInfo +0xc0, WorldTransform +0xc4, BlendOp +0xf4, BlendMode +0xf8 are at +0xac, +0xb0, +0xe0,
// +0xe4: the particle-group base's STL containers are narrower in retail), so the members are modelled locally at the retail offsets; EnginePrimitiveFlags
// (+0xc) is the CEngineInternalPrimitiveBase member.
struct C3DMeshInfo;

struct CMatrix3x4 { float m_rows[12]; };

struct CMeshBank_00a242a0 {
    C3DMeshInfo* GetMeshInfo(long id);             // 0x00A242A0
};

struct CEngineData_01436e98 {
    unsigned char      _pad[0x10];
    CMeshBank_00a242a0* m_pMeshBank;               // +0x10
};
extern CEngineData_01436e98* g_pEngineData_01436e98;   // 0x01436E98

struct SParticleMeshGroupUpdate {
    unsigned char _pad[0xc];
    long          m_meshId;                        // +0x0c
    CMatrix3x4    m_worldTransform;                // +0x10
    long          m_blendMode;                     // +0x40
    long          m_blendOp;                       // +0x44
};

struct CEngineInternalPrimitiveParticleMeshGroup {
    unsigned char _pad[0xc];
    unsigned long EnginePrimitiveFlags;            // +0x0c (CEngineInternalPrimitiveBase)
    unsigned char _pad10[0x9c];
    C3DMeshInfo*  MeshInfo;                        // +0xac
    CMatrix3x4    WorldTransform;                  // +0xb0
    long          BlendOp;                         // +0xe0
    long          BlendMode;                       // +0xe4
    void Update(const SParticleMeshGroupUpdate& update, unsigned long flags);
};

void CEngineInternalPrimitiveParticleMeshGroup::Update(const SParticleMeshGroupUpdate& update, unsigned long flags)
{
    EnginePrimitiveFlags = flags;
    CMeshBank_00a242a0* bank = g_pEngineData_01436e98->m_pMeshBank;
    MeshInfo = bank->GetMeshInfo(update.m_meshId);
    WorldTransform = update.m_worldTransform;
    BlendOp = update.m_blendOp;
    BlendMode = update.m_blendMode;
}
