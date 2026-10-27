// CEngineClothSimulation2DAnimatedMesh::CEngineClothSimulation2DAnimatedMesh(const C3DMeshInfo* meshInfo, unsigned long index, CEngineInternalPrimitiveAnimatedMesh* primitive) -- forward the three arguments to the CEngineClothSimulationAnimatedMesh constructor, then store this class's vtable
// Leaf: ??0CEngineClothSimulation2DAnimatedMesh@@QAE@PBVC3DMeshInfo@@KPAVCEngineInternalPrimitiveAnimatedMesh@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00b81f00, 37 bytes, thiscall, `ret 0Ch`.
// Identity (proven): it stores vtable 0x012A39EC (RTTI .?AVCEngineClothSimulation2DAnimatedMesh@@, bases CEngineClothSimulationAnimatedMesh <- CEngineClothSimulationMeshBase) after the MeshBase constructor 0x00B81AD0; Ego_r.pdb ??0CEngineClothSimulation2DAnimatedMesh@@QAE@PBVC3DMeshInfo@@KPAVCEngineInternalPrimitiveAnimatedMesh@@@Z; caller CEngineInternalPrimitive2DAnimatedMesh::InitialiseSimulation 0x00BCFDE0.
// Callees: the base constructor 0x00B81AD0 (the CEngineClothSimulationAnimatedMesh constructor inlined; its vtable store is dead); vtable 0x012A39EC.
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
    CEngineClothSimulationAnimatedMesh(const C3DMeshInfo* meshInfo, unsigned long index, CEngineInternalPrimitiveAnimatedMesh* primitive)
        : CEngineClothSimulationMeshBase(meshInfo, index, primitive) { }   // inline here; its own out-of-line copy is 0x00B81ED0
    virtual ~CEngineClothSimulationAnimatedMesh();
};
class CEngineClothSimulation2DAnimatedMesh : public CEngineClothSimulationAnimatedMesh {
public:
    CEngineClothSimulation2DAnimatedMesh(const C3DMeshInfo* meshInfo, unsigned long index, CEngineInternalPrimitiveAnimatedMesh* primitive);
    virtual ~CEngineClothSimulation2DAnimatedMesh();
};
CEngineClothSimulation2DAnimatedMesh::CEngineClothSimulation2DAnimatedMesh(const C3DMeshInfo* meshInfo, unsigned long index, CEngineInternalPrimitiveAnimatedMesh* primitive)
    : CEngineClothSimulationAnimatedMesh(meshInfo, index, primitive)
{
}