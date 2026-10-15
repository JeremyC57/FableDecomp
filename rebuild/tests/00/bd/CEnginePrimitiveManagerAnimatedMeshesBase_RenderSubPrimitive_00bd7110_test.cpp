// Behaviour test for 0x00BD7110 (see src/00bd7110.cpp): the animated-mesh RenderSubPrimitive dispatch (simulated, missing simulation, plain mesh).
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#define CHECK(c) do { if (!(c)) { std::printf("00bd7110_TEST FAIL\n"); return 1; } } while (0)

struct C3DClothPrimitive { unsigned int SimulationIndex; };
struct C3DClothPrimitivePointer { C3DClothPrimitive* Data; void* Info; C3DClothPrimitive* operator->() const { return Data; } };
struct C3DPrimitive2 { C3DClothPrimitivePointer* ClothPrimitives; };
struct CEngineSimulation { int renders; float time; unsigned int pass; void Render(float t, unsigned int p) { ++renders; time = t; pass = p; } };
struct CEnginePrimitiveSimulationInfo {
    CEngineSimulation** sims; unsigned int count;
    CEngineSimulation* FindSimulation(unsigned int i) { return i < count ? sims[i] : 0; }
};
struct CEngineInternalPrimitiveAnimatedMesh { int InterpolationSet; bool BulletTimeAffectsCloth; CEnginePrimitiveSimulationInfo* SimulationInfo; };
struct CEngineSubPrimitiveAnimatedMesh { CEngineInternalPrimitiveAnimatedMesh* Parent; C3DPrimitive2* Primitive; bool Simulated; };
struct STime { float Time; float a; float b; };
static float g_bulletTime = 0.25f;
static STime g_times[3] = { { 1, 0, 0 }, { 2, 0, 0 }, { 3, 0, 0 } };
struct CEnginePrimitiveManagerAnimatedMeshesBase {
    int meshRenders;
    void RenderMeshPrimitive(CEngineSubPrimitiveAnimatedMesh&, unsigned int) { ++meshRenders; }
    void RenderSubPrimitive(CEngineSubPrimitiveAnimatedMesh& mesh, unsigned int pass)
    {
        if (mesh.Simulated) {
            if (mesh.Parent->SimulationInfo) {
                C3DPrimitive2* primitive = mesh.Primitive;
                CEngineSimulation* sim = mesh.Parent->SimulationInfo->FindSimulation(primitive->ClothPrimitives[0]->SimulationIndex);
                if (sim) {
                    CEngineInternalPrimitiveAnimatedMesh* parent = mesh.Parent;
                    float time;
                    if (parent->BulletTimeAffectsCloth) time = g_bulletTime; else time = g_times[parent->InterpolationSet].Time;
                    sim->Render(time, pass);
                }
            }
        }
        else
            RenderMeshPrimitive(mesh, pass);
    }
};
int main()
{
    CEngineSimulation sim = { 0, 0, 0 }; CEngineSimulation* sims[2] = { 0, &sim };
    CEnginePrimitiveSimulationInfo info = { sims, 2 };
    C3DClothPrimitive cloth = { 1 }; C3DClothPrimitivePointer cp = { &cloth, 0 }; C3DPrimitive2 prim = { &cp };
    CEngineInternalPrimitiveAnimatedMesh parent = { 2, false, &info };
    CEngineSubPrimitiveAnimatedMesh sub = { &parent, &prim, true };
    CEnginePrimitiveManagerAnimatedMeshesBase m; m.meshRenders = 0;
    m.RenderSubPrimitive(sub, 7);
    CHECK(sim.renders == 1 && sim.time == 3.0f && sim.pass == 7 && m.meshRenders == 0);
    parent.BulletTimeAffectsCloth = true; m.RenderSubPrimitive(sub, 1);
    CHECK(sim.renders == 2 && sim.time == 0.25f);
    cloth.SimulationIndex = 0; m.RenderSubPrimitive(sub, 1);
    CHECK(sim.renders == 2);
    parent.SimulationInfo = 0; m.RenderSubPrimitive(sub, 1);
    CHECK(sim.renders == 2 && m.meshRenders == 0);
    sub.Simulated = false; m.RenderSubPrimitive(sub, 1);
    CHECK(m.meshRenders == 1 && sim.renders == 2);
    std::printf("00bd7110_TEST PASS\n");
    return 0;
}