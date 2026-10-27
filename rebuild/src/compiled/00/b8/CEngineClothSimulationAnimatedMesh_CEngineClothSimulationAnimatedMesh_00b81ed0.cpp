// CEngineClothSimulationAnimatedMesh::CEngineClothSimulationAnimatedMesh(const C3DMeshInfo* meshInfo, unsigned long index, CEngineInternalPrimitiveAnimatedMesh* primitive) -- forward the three arguments to the CEngineClothSimulationMeshBase constructor, then store this class's vtable
// Leaf: ??0CEngineClothSimulationAnimatedMesh@@QAE@PBVC3DMeshInfo@@KPAVCEngineInternalPrimitiveAnimatedMesh@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00b81ed0, 37 bytes, thiscall, `ret 0Ch`.
// Identity (proven): it stores vtable 0x012A39C0 (RTTI .?AVCEngineClothSimulationAnimatedMesh@@, base CEngineClothSimulationMeshBase) after the base constructor 0x00B81AD0; Ego_r.pdb ??0CEngineClothSimulationAnimatedMesh@@QAE@PBVC3DMeshInfo@@KPAVCEngineInternalPrimitiveAnimatedMesh@@@Z (reloc_bindings spells the index `I`, the PDB `K`); caller CEngineInternalPrimitiveAnimatedMesh::InitialiseSimulation 0x00BCFCB0.
// Callees: the base constructor 0x00B81AD0; vtable 0x012A39C0.
class C3DMeshInfo;
class CEngineInternalPrimitiveMeshBase { };
class CEngineInternalPrimitiveStaticMesh : public CEngineInternalPrimitiveMeshBase { };
class CEngineInternalPrimitiveAnimatedMesh : public CEngineInternalPrimitiveMeshBase { };
class CEngineClothSimulationMeshBase {
public:
    CEngineClothSimulationMeshBase(const C3DMeshInfo* meshInfo, unsigned long index, CEngineInternalPrimitiveMeshBase* primitive);   // 0x00B81AD0
    virtual ~CEngineClothSimulationMeshBase();
};
class CEngineClothSimulationAnimatedMesh : public CEngineClothSimulationMeshBase {
public:
    CEngineClothSimulationAnimatedMesh(const C3DMeshInfo* meshInfo, unsigned long index, CEngineInternalPrimitiveAnimatedMesh* primitive);
    virtual ~CEngineClothSimulationAnimatedMesh();
};
CEngineClothSimulationAnimatedMesh::CEngineClothSimulationAnimatedMesh(const C3DMeshInfo* meshInfo, unsigned long index, CEngineInternalPrimitiveAnimatedMesh* primitive)
    : CEngineClothSimulationMeshBase(meshInfo, index, primitive)
{
}