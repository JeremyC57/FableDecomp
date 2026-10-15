// Behaviour test for 0x00BBA340 (see src/00bba340.cpp): the decal RenderPrimitive queuing a sub-primitive at the camera distance.
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#define CHECK(c) do { if (!(c)) { std::printf("00bba340_TEST FAIL\n"); return 1; } } while (0)

struct C3DVector { float X, Y, Z; };
struct CEngineCamera { C3DVector WorldPosition; };
struct CEngineInternalPrimitiveDecal { C3DVector WorldPos; };
struct CEngineSubPrimitiveDecal { CEngineInternalPrimitiveDecal* Parent; void* NextSubPrimitive; CEngineSubPrimitiveDecal(CEngineInternalPrimitiveDecal& p) : Parent(&p), NextSubPrimitive(0) { } };
struct CEngineSubPrimitiveRenderer {
    int calls; int list; CEngineSubPrimitiveDecal* sub; float distance; void* manager;
    void AddSubPrimitive(int l, CEngineSubPrimitiveDecal* s, float d, void* m) { ++calls; list = l; sub = s; distance = d; manager = m; }
};
static CEngineCamera g_camera = { { 1, 2, 3 } };
static CEngineSubPrimitiveRenderer g_renderer = { 0, -1, 0, 0, 0 };
struct CEnginePrimitiveManagerDecal {
    void* SceneList; bool Enable;
    int RenderPrimitive(CEngineInternalPrimitiveDecal& decal)
    {
        if (!Enable) return 0;
        CEngineSubPrimitiveDecal* sub = new CEngineSubPrimitiveDecal(decal);
        float dx = g_camera.WorldPosition.X - decal.WorldPos.X;
        float dy = g_camera.WorldPosition.Y - decal.WorldPos.Y;
        float dz = g_camera.WorldPosition.Z - decal.WorldPos.Z;
        g_renderer.AddSubPrimitive(0, sub, (float)std::sqrt(dx * dx + dy * dy + dz * dz), this);
        return 2;
    }
};
int main()
{
    CEnginePrimitiveManagerDecal m; m.SceneList = 0; m.Enable = false;
    CEngineInternalPrimitiveDecal d = { { 4, 6, 3 } };
    CHECK(m.RenderPrimitive(d) == 0 && g_renderer.calls == 0);
    m.Enable = true;
    CHECK(m.RenderPrimitive(d) == 2 && g_renderer.calls == 1 && g_renderer.list == 0 && g_renderer.manager == &m);
    CHECK(g_renderer.sub && g_renderer.sub->Parent == &d && g_renderer.sub->NextSubPrimitive == 0 && g_renderer.distance == 5.0f);
    delete g_renderer.sub;
    std::printf("00bba340_TEST PASS\n");
    return 0;
}