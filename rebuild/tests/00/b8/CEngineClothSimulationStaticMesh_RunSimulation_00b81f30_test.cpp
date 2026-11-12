// Behaviour test for 0x00B81F30 CEngineClothSimulationStaticMesh::RunSimulation: the same logic over a small copy of the model
// (counted mesh pointer, LOD pick, one particle simulation per cloth primitive on first use, clear on a failed set-up, save + step).
#include <cstdio>

struct TSimulation { int SetUp, Saved, Stepped; };
struct TProgram {
    bool FailSetup; int SetupCalls, SaveCalls, StepCalls; const void* LastContext;
    bool SetupSimulation(TSimulation& s) { ++SetupCalls; if (FailSetup) return false; s.SetUp = 1; return true; }
    void SaveStateToInterpolateFrom(TSimulation& s) { ++SaveCalls; ++s.Saved; }
    void SimulationStep(TSimulation& s, const void* context) { ++StepCalls; ++s.Stepped; LastContext = context; }
};
struct TClothPrimitive { TProgram* ParticleProgram; };
struct TPrimitive { TClothPrimitive* Cloth[4]; unsigned long ClothCount; };
struct TMesh { TPrimitive Primitives[2]; };
struct TPointerInfo { unsigned long RefCount; };
struct TMeshPointer {                       // CCountedPointer<const C3DMesh2>
    TMesh* Data; TPointerInfo* Info;
    TMeshPointer() : Data(0), Info(0) { }
    ~TMeshPointer() { if (Info) --Info->RefCount; }
};
struct TMeshInfo {
    bool HasMeshData; void* LODMeshes[4]; unsigned long LODDataSizes[4];
    TMesh* Mesh; TPointerInfo MeshRef; bool FailGetMesh; unsigned long LastLOD, LastFlags; int GetMeshCalls;
    bool HasLOD(unsigned long lod) const { return lod < 4 && LODDataSizes[lod] != 0; }
    unsigned long GetBestCurrentDetailLevel() const
    {
        if (HasMeshData) {
            for (unsigned long i = 0; HasLOD(i); ++i)
                if (LODMeshes[i])
                    return i;
        }
        return (unsigned long)-1;
    }
    bool GetMesh(TMeshPointer& mesh, unsigned long lod, unsigned long flags)
    {
        ++GetMeshCalls; LastLOD = lod; LastFlags = flags;
        if (FailGetMesh) return false;
        mesh.Data = Mesh; mesh.Info = &MeshRef; ++MeshRef.RefCount;
        return true;
    }
};
struct TCloth {
    TSimulation* ParticleSims[4]; unsigned long SimCount;
    TSimulation Storage[4]; int Created;
    TMeshInfo* MeshInfo; unsigned long PrimitiveIndex;
    bool RunState; int UpdateCalls, ClearCalls;
    void UpdateTransform() { ++UpdateCalls; }
    bool CalcAndSetRunState() { return RunState; }
    void ClearParticleSims() { ++ClearCalls; SimCount = 0; }
    void RunSimulation()
    {
        UpdateTransform();
        if (!CalcAndSetRunState()) {
            ClearParticleSims();
            return;
        }
        TMeshPointer mesh;
        if (!MeshInfo->GetMesh(mesh, MeshInfo->GetBestCurrentDetailLevel(), 4))
            return;
        TPrimitive& primitive = mesh.Data->Primitives[PrimitiveIndex];
        if (primitive.ClothCount == 0)
            return;
        if (SimCount == 0) {
            unsigned long count = primitive.ClothCount;
            SimCount = count;                                  // resize(count)
            for (unsigned long i = 0; i < count; ++i) {
                TSimulation* created = &Storage[i];            // new CParticleSimulation
                created->SetUp = created->Saved = created->Stepped = 0;
                ++Created;
                ParticleSims[i] = created;                     // Reset
                TSimulation* simulation = ParticleSims[i];
                TProgram* program = primitive.Cloth[i]->ParticleProgram;
                if (!program->SetupSimulation(*simulation)) {
                    ClearParticleSims();
                    break;
                }
            }
        }
        for (unsigned long i = 0; i < SimCount; ++i) {
            TClothPrimitive* cloth = primitive.Cloth[i];
            TSimulation* simulation = ParticleSims[i];
            cloth->ParticleProgram->SaveStateToInterpolateFrom(*simulation);
            cloth->ParticleProgram->SimulationStep(*simulation, this);
        }
    }
};

static int Fail(int n) { std::printf("case %d\n", n); std::printf("00b81f30_TEST FAIL\n"); return 1; }

int main()
{
    static TProgram programs[3];
    static TClothPrimitive cloths[3];
    static TMesh mesh;
    static TMeshInfo info;
    static TCloth cloth;
    int i;
    for (i = 0; i < 3; ++i) cloths[i].ParticleProgram = &programs[i];
    mesh.Primitives[0].ClothCount = 0;
    mesh.Primitives[1].ClothCount = 3;
    for (i = 0; i < 3; ++i) mesh.Primitives[1].Cloth[i] = &cloths[i];
    info.Mesh = &mesh; info.MeshRef.RefCount = 1;
    cloth.MeshInfo = &info; cloth.PrimitiveIndex = 1;

    // LOD pick: no mesh data -> -1; LOD 0 present but not loaded, LOD 1 loaded -> 1; a gap in the sizes stops the scan -> -1
    info.HasMeshData = false; info.LODDataSizes[0] = 10; info.LODMeshes[0] = &mesh;
    if (info.GetBestCurrentDetailLevel() != (unsigned long)-1) return Fail(1);
    info.HasMeshData = true; info.LODMeshes[0] = 0; info.LODDataSizes[1] = 5; info.LODMeshes[1] = &mesh;
    if (info.GetBestCurrentDetailLevel() != 1) return Fail(2);
    info.LODDataSizes[1] = 0; info.LODDataSizes[2] = 7; info.LODMeshes[2] = &mesh;
    if (info.GetBestCurrentDetailLevel() != (unsigned long)-1) return Fail(3);
    info.LODDataSizes[1] = 5;

    // not running: transform updated, simulations cleared, the mesh is never fetched
    cloth.RunState = false;
    cloth.RunSimulation();
    if (cloth.UpdateCalls != 1 || cloth.ClearCalls != 1 || info.GetMeshCalls != 0) return Fail(4);

    // GetMesh fails: nothing created, nothing cleared
    cloth.RunState = true; info.FailGetMesh = true;
    cloth.RunSimulation();
    if (info.GetMeshCalls != 1 || info.LastLOD != 1 || info.LastFlags != 4 || cloth.Created != 0 || cloth.ClearCalls != 1) return Fail(5);
    if (info.MeshRef.RefCount != 1) return Fail(6);

    // primitive without cloth: the mesh reference is released, nothing else happens
    info.FailGetMesh = false; cloth.PrimitiveIndex = 0;
    cloth.RunSimulation();
    if (cloth.Created != 0 || cloth.SimCount != 0 || info.MeshRef.RefCount != 1) return Fail(7);

    // first run on a cloth primitive: three simulations created and set up, each saved and stepped once with this as context
    cloth.PrimitiveIndex = 1;
    cloth.RunSimulation();
    if (cloth.Created != 3 || cloth.SimCount != 3 || info.MeshRef.RefCount != 1) return Fail(8);
    for (i = 0; i < 3; ++i) {
        if (programs[i].SetupCalls != 1 || programs[i].SaveCalls != 1 || programs[i].StepCalls != 1) return Fail(9);
        if (programs[i].LastContext != &cloth || cloth.Storage[i].SetUp != 1 || cloth.Storage[i].Saved != 1 || cloth.Storage[i].Stepped != 1) return Fail(10);
    }

    // second run: no new set-up, one more save + step each
    cloth.RunSimulation();
    if (cloth.Created != 3) return Fail(11);
    for (i = 0; i < 3; ++i)
        if (programs[i].SetupCalls != 1 || programs[i].SaveCalls != 2 || programs[i].StepCalls != 2) return Fail(12);

    // a failed set-up (second program) clears every simulation and nothing is stepped
    cloth.ClearParticleSims();
    cloth.ClearCalls = 0; cloth.Created = 0;
    programs[1].FailSetup = true;
    cloth.RunSimulation();
    if (cloth.Created != 2 || cloth.ClearCalls != 1 || cloth.SimCount != 0) return Fail(13);
    if (programs[0].SetupCalls != 2 || programs[1].SetupCalls != 2 || programs[2].SetupCalls != 1) return Fail(14);
    for (i = 0; i < 3; ++i)
        if (programs[i].SaveCalls != 2 || programs[i].StepCalls != 2) return Fail(15);
    if (info.MeshRef.RefCount != 1) return Fail(16);

    std::printf("00b81f30_TEST PASS\n");
    return 0;
}