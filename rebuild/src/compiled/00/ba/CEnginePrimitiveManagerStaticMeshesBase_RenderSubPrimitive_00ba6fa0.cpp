// CEnginePrimitiveManagerStaticMeshesBase::RenderSubPrimitive(CEngineSubPrimitiveBase& sub, ERenderPassType pass) -- a cloth sub-primitive (byte +0x20) renders through its primitive's simulation info (+0x64): the simulation found by the renderer's current mesh index is rendered with (renderer, sub, pass); otherwise the plain mesh render
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BA6FA0, length 80 bytes.
// Callees: CEnginePrimitiveSimulationInfo::FindSimulation @ 0x00B812C0 (on the info at primitive+0x64: the index at renderer(+0x18 of sub)->+0x20->+0->+0xc),
// CEngineClothSimulationStaticMesh::Render @ 0x00B812F0 (on the simulation: renderer, sub, pass), RenderMeshPrimitive @ 0x00BA5260 (on `this`, ecx passed
// straight through: sub, pass). Shape: the cloth arm never touches `this`; the info pointer stays in ecx for the find, the renderer in edi for both calls.
struct CEngineClothSimulationStaticMesh;
struct CEngineSubPrimitiveBase;

struct CEnginePrimitiveSimulationInfo {
    CEngineClothSimulationStaticMesh* FindSimulation(long index);   // 0x00B812C0
};

struct CEngineClothSimulationStaticMesh {
    void Render(void* renderer, CEngineSubPrimitiveBase& sub, long pass);   // 0x00B812F0
};

struct CEngineInternalPrimitiveStaticMesh_00ba6fa0 {
    unsigned char                  _pad[0x64];
    CEnginePrimitiveSimulationInfo* m_pSimulationInfo;   // +0x64
};

struct SMeshSlot_00ba6fa0 { void* m_a; void* m_b; void* m_c; long m_index; };   // +0xc = the current mesh index

struct CMeshRenderer_00ba6fa0 {
    unsigned char      _pad[0x20];
    SMeshSlot_00ba6fa0** m_ppCurrent;              // +0x20
    long GetCurrentMeshIndex() const { return (*m_ppCurrent)->m_index; }   // inline: the index lands in eax for the push
};

struct CEngineSubPrimitiveBase {
    unsigned char                              _pad0[0xc];
    CEngineInternalPrimitiveStaticMesh_00ba6fa0* m_pPrimitive;   // +0x0c
    unsigned char                              _pad10[8];
    CMeshRenderer_00ba6fa0*                    m_pRenderer;     // +0x18
    unsigned char                              _pad1c[4];
    bool                                       m_cloth;         // +0x20
};

struct CEnginePrimitiveManagerStaticMeshesBase {
    void RenderMeshPrimitive(CEngineSubPrimitiveBase& sub, long pass);   // 0x00BA5260
    void RenderSubPrimitive(CEngineSubPrimitiveBase& sub, long pass);
};

void CEnginePrimitiveManagerStaticMeshesBase::RenderSubPrimitive(CEngineSubPrimitiveBase& sub, long pass)
{
    if (sub.m_cloth) {
        CEnginePrimitiveSimulationInfo* info = sub.m_pPrimitive->m_pSimulationInfo;
        if (info) {
            CMeshRenderer_00ba6fa0* renderer = sub.m_pRenderer;
            CEngineClothSimulationStaticMesh* simulation = info->FindSimulation(renderer->GetCurrentMeshIndex());
            if (simulation)
                simulation->Render(renderer, sub, pass);
        }
    } else {
        RenderMeshPrimitive(sub, pass);
    }
}