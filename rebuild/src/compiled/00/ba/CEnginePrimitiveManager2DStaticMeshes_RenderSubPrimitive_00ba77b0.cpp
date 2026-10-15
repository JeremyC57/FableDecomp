// CEnginePrimitiveManager2DStaticMeshes::RenderSubPrimitive(CEngineSubPrimitiveBase& sub, ERenderPassType pass) -- a flagged 2D static mesh is first rendered once through the static-mesh base in pass 1, then the base logic runs for the requested pass (cloth sub-primitives through their simulation, others as plain meshes)
// Leaf: ?RenderSubPrimitive@CEnginePrimitiveManager2DStaticMeshes
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BA77B0, length 95 bytes (112 with padding).
// Callees: CEnginePrimitiveManagerStaticMeshesBase::RenderSubPrimitive @ 0x00BA6FA0 (landed; called non-virtually with pass 1
// when bit 0 of the sub-primitive's +4 flags is set), CEnginePrimitiveSimulationInfo::FindSimulation @ 0x00B812C0,
// CEngineClothSimulationStaticMesh::Render @ 0x00B812F0, CEnginePrimitiveManagerStaticMeshesBase::RenderMeshPrimitive
// @ 0x00BA5260.
// Shape: after the optional base call the base body is repeated inline (the landed 0x00BA6FA0 model): the cloth byte at
// +0x20 selects the simulation path (info at primitive+0x64, the renderer at +0x18 kept in edi for both calls, the current mesh
// index through an inline accessor), else the plain mesh render on `this` (kept in edi across the first call).
struct CEngineClothSimulationStaticMesh;
struct CEngineSubPrimitiveBase;

struct CEnginePrimitiveSimulationInfo {
    CEngineClothSimulationStaticMesh* FindSimulation(long index);   // 0x00B812C0
};

struct CEngineClothSimulationStaticMesh {
    void Render(void* renderer, CEngineSubPrimitiveBase& sub, long pass);   // 0x00B812F0
};

struct CEngineInternalPrimitiveStaticMesh {
    unsigned char                   _pad[0x64];
    CEnginePrimitiveSimulationInfo* m_pSimulationInfo;   // +0x64
};

struct SMeshSlot { void* m_a; void* m_b; void* m_c; long m_index; };   // +0xc = the current mesh index

struct CMeshRenderer {
    unsigned char _pad[0x20];
    SMeshSlot**   m_ppCurrent;                     // +0x20
    long GetCurrentMeshIndex() const { return (*m_ppCurrent)->m_index; }
};

struct CEngineSubPrimitiveBase {
    unsigned char                       _pad0[0x4];
    unsigned char                       m_flags;   // +0x04 (bit 0: also render in pass 1)
    unsigned char                       _pad5[0x7];
    CEngineInternalPrimitiveStaticMesh* m_pPrimitive;   // +0x0C
    unsigned char                       _pad10[8];
    CMeshRenderer*                      m_pRenderer;    // +0x18
    unsigned char                       _pad1c[4];
    bool                                m_cloth;        // +0x20
};

struct CEnginePrimitiveManagerStaticMeshesBase {
    void RenderMeshPrimitive(CEngineSubPrimitiveBase& sub, long pass);   // 0x00BA5260
    void RenderSubPrimitive(CEngineSubPrimitiveBase& sub, long pass);    // 0x00BA6FA0
};

struct CEnginePrimitiveManager2DStaticMeshes : CEnginePrimitiveManagerStaticMeshesBase {
    void RenderSubPrimitive(CEngineSubPrimitiveBase& sub, long pass);
};

void CEnginePrimitiveManager2DStaticMeshes::RenderSubPrimitive(CEngineSubPrimitiveBase& sub, long pass)
{
    if (sub.m_flags & 1)
        CEnginePrimitiveManagerStaticMeshesBase::RenderSubPrimitive(sub, 1);
    if (sub.m_cloth) {
        CEnginePrimitiveSimulationInfo* info = sub.m_pPrimitive->m_pSimulationInfo;
        if (info) {
            CMeshRenderer* renderer = sub.m_pRenderer;
            CEngineClothSimulationStaticMesh* simulation = info->FindSimulation(renderer->GetCurrentMeshIndex());
            if (simulation)
                simulation->Render(renderer, sub, pass);
        }
    } else {
        RenderMeshPrimitive(sub, pass);
    }
}