#include <cstdio>
#include <cstring>
#include <cstdlib>
// behaviour test: GetNoDummyObjects gathers the components' meshes, skips index-0 meshes, builds each mesh object at the given
// scale and returns the first non-zero dummy-object count for the name (0 when none), releasing every object it built
struct CCharString { const char* s; };
struct CMeshInfo { long Index; float SizeX; unsigned char PrimaryMesh; };
static int g_live = 0, g_built = 0; static float g_lastScale = 0;
struct C3DMeshObject {
    long index;
    C3DMeshObject() : index(0) { ++g_live; }
    ~C3DMeshObject() { --g_live; }
    long GetNoDummyObjects(const CCharString& name) const { return std::strcmp(name.s, "hand") == 0 && index >= 3 ? index * 10 : 0; }
};
struct CMeshVector {
    CMeshInfo* _M_start; CMeshInfo* _M_finish; CMeshInfo* _M_end;
    CMeshVector() : _M_start(0), _M_finish(0), _M_end(0) {}
    ~CMeshVector() { if (_M_start) std::free(_M_start); }
    void reserve(unsigned long n) { _M_start = _M_finish = (CMeshInfo*)std::malloc(n * 4 * sizeof(CMeshInfo)); _M_end = _M_start + n * 4; }
    void push(long idx) { CMeshInfo m = { idx, 0, 0 }; *_M_finish++ = m; }
    CMeshInfo* begin() const { return _M_start; }
    CMeshInfo* end() const { return _M_finish; }
};
struct CComponent { long a, b; void GetAllMeshes(CMeshVector& v) const { v.push(a); v.push(b); } };
struct CTCGraphicAppearance {
    CComponent* comps; int ncomps;
    void GetAllMeshes(CMeshVector& meshes) const { meshes.reserve(4); for (int i = 0; i < ncomps; ++i) comps[i].GetAllMeshes(meshes); }
    void GetMeshObject(const CMeshInfo& info, float scale, C3DMeshObject& out) const { out.index = info.Index; g_lastScale = scale; ++g_built; }
    long GetNoDummyObjects(const CCharString& name, float scale) const
    {
        CMeshVector meshes;
        GetAllMeshes(meshes);
        for (CMeshInfo* mesh = meshes.begin(); mesh != meshes.end(); ++mesh) {
            if (mesh->Index != 0) {
                C3DMeshObject object;
                GetMeshObject(*mesh, scale, object);
                long count = object.GetNoDummyObjects(name);
                if (count != 0)
                    return count;
            }
        }
        return 0;
    }
};
int main()
{
    CComponent comps[2] = { { 0, 1 }, { 4, 5 } };
    CTCGraphicAppearance a; a.comps = comps; a.ncomps = 2;
    CCharString hand = { "hand" }, foot = { "foot" };
    long r1 = a.GetNoDummyObjects(hand, 2.0f);      // meshes 0 (skipped), 1 (0), 4 (40) -> 40, stops before 5
    bool ok = r1 == 40 && g_built == 2 && g_lastScale == 2.0f && g_live == 0;
    g_built = 0;
    long r2 = a.GetNoDummyObjects(foot, 1.0f);      // no match anywhere -> 0, all three non-zero meshes built
    ok = ok && r2 == 0 && g_built == 3 && g_live == 0;
    if (!ok) { std::printf("004be000_TEST FAIL\n"); return 1; }
    std::printf("004be000_TEST PASS\n");
    return 0;
}