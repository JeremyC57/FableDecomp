// CEngineClothSimulationStaticMesh::CEngineClothSimulationStaticMesh(const C3DMeshInfo* meshInfo, unsigned long index, CEngineInternalPrimitiveStaticMesh* primitive) -- forward the three arguments to the CEngineClothSimulationMeshBase constructor, then store this class's vtable
// Leaf: ??0CEngineClothSimulationStaticMesh@@QAE@PBVC3DMeshInfo@@KPAVCEngineInternalPrimitiveStaticMesh@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00b81ea0, 37 bytes, thiscall, `ret 0Ch`.
// Identity (proven): it stores vtable 0x012A3998 (RTTI .?AVCEngineClothSimulationStaticMesh@@, base CEngineClothSimulationMeshBase) after the base constructor 0x00B81AD0; Ego_r.pdb ??0CEngineClothSimulationStaticMesh@@QAE@PBVC3DMeshInfo@@KPAVCEngineInternalPrimitiveStaticMesh@@@Z; caller CEngineInternalPrimitiveStaticMesh::InitialiseSimulation 0x00B9DE90.
// Callees: the base constructor 0x00B81AD0; vtable 0x012A3998.
class C3DMeshInfo;
class CEngineInternalPrimitiveMeshBase { };
class CEngineInternalPrimitiveStaticMesh : public CEngineInternalPrimitiveMeshBase { };
class CEngineInternalPrimitiveAnimatedMesh : public CEngineInternalPrimitiveMeshBase { };
class CEngineClothSimulationMeshBase {
public:
    CEngineClothSimulationMeshBase(const C3DMeshInfo* meshInfo, unsigned long index, CEngineInternalPrimitiveMeshBase* primitive);   // 0x00B81AD0
    virtual ~CEngineClothSimulationMeshBase();
};
class CEngineClothSimulationStaticMesh : public CEngineClothSimulationMeshBase {
public:
    CEngineClothSimulationStaticMesh(const C3DMeshInfo* meshInfo, unsigned long index, CEngineInternalPrimitiveStaticMesh* primitive);
    virtual ~CEngineClothSimulationStaticMesh();
};
CEngineClothSimulationStaticMesh::CEngineClothSimulationStaticMesh(const C3DMeshInfo* meshInfo, unsigned long index, CEngineInternalPrimitiveStaticMesh* primitive)
    : CEngineClothSimulationMeshBase(meshInfo, index, primitive)
{
}