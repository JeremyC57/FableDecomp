// CEngineClothSimulation2DAnimatedMesh::CalcAndSetRunState() -- the 2D animated-mesh cloth always runs: store run state 0 and report that the state is not 1 (true)
// Leaf: ?CalcAndSetRunState@CEngineClothSimulation2DAnimatedMesh@@MAE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B80780, 10 bytes, thiscall, `ret`: mov dword ptr [ecx+4],0 ; mov al,1 ; ret
// Identity (proven): slot 8 of vtable 0x012A39EC (RTTI CEngineClothSimulation2DAnimatedMesh, rtti-vtables.tsv); Ego_r.pdb
//   ?CalcAndSetRunState@CEngineClothSimulation2DAnimatedMesh@@MAE_NXZ (protected virtual) -- the static/animated twins
//   0x00B81180 / 0x00B81230 are the landed CalcAndSetRunState overrides of the same slot.
// Layout: as the landed twins (CEngineSimulationBase::RunState +0x04, an ERunState kept as `long`).
// Shape: the inline SetRunState store, then the sibling's `RunState != 1` test folded to the constant true.
class CEngineSimulationBase {
public:
    virtual void v00();
    long          RunState;                 // +0x04 (ERunState)
    unsigned char SimStepOffset;            // +0x08
    void SetRunState(long state) { RunState = state; }
};

class CEngineClothSimulationMeshBase : public CEngineSimulationBase {
public:
    unsigned char _pad0c[0xb4 - 0x0c];      // ParticleSims (12-byte vector), transforms, MeshInfo, PrimitiveIndex, Owner
};

class CEngineClothSimulationAnimatedMesh : public CEngineClothSimulationMeshBase {
};

class CEngineClothSimulation2DAnimatedMesh : public CEngineClothSimulationAnimatedMesh {
protected:
    virtual bool CalcAndSetRunState();
};

bool CEngineClothSimulation2DAnimatedMesh::CalcAndSetRunState()
{
    SetRunState(0);
    return RunState != 1;
}