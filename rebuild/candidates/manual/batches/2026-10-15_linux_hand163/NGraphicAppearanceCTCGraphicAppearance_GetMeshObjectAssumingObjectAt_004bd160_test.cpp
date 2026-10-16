// behaviour test for 0x004BD160 CTCGraphicAppearance::GetMeshObjectAssumingObjectAt
#include <cstdio>
struct MeshInfo3D { int id; };
static MeshInfo3D g_meshes[4] = { { 0 }, { 1 }, { 2 }, { 3 } };
struct MeshObject { const MeshInfo3D* info; float x; float scale;
    MeshObject(const MeshInfo3D* i, float px, float s) : info(i), x(px), scale(s) { } };
struct Info { long Index; float SizeX; };
struct Appearance {
    float Scale;
    void Get(const Info& info, float pos, MeshObject& out) const
    {
        const MeshInfo3D* meshInfo = &g_meshes[info.Index];
        out = MeshObject(meshInfo, pos, info.SizeX * Scale * 0.01f);
    }
};
int main()
{
    Appearance a = { 2.0f };
    Info i = { 3, 50.0f };
    MeshObject o(0, 0.0f, 0.0f);
    a.Get(i, 7.0f, o);
    bool ok = o.info == &g_meshes[3] && o.x == 7.0f && o.scale > 0.999f && o.scale < 1.001f;
    if (!ok) { std::printf("004bd160_TEST FAIL\n"); return 1; }
    std::printf("004bd160_TEST PASS\n");
    return 0;
}
