#include <cstdio>
static int g_dtors = 0;
struct V { float X, Y, Z; };
struct Mesh { float maxX; ~Mesh() { ++g_dtors; }
    bool IsInContainmentVolume(const char* name, const V& p, float tol) const { return name[0] == 'A' && p.X < maxX + tol; } };
struct Phys { void* params; float maxX;
    void* GetPhysicsMeshParameters() const { return params; }
    Mesh GetPhysicsMeshObject() const { Mesh m; m.maxX = maxX; return m; } };
struct Thing { Phys* PhysicsTC; };
struct F { V Pos; const char* VolumeName;
    bool operator()(Thing* t) const {
        Phys* p = t->PhysicsTC;
        if (p && p->GetPhysicsMeshParameters()) {
            Mesh m = p->GetPhysicsMeshObject();
            if (m.IsInContainmentVolume(VolumeName, Pos, 0.0f)) return true;
        }
        return false; } };
int main()
{
    int dummy;
    Phys p = { &dummy, 5.0f };
    Thing t = { &p }, none = { 0 };
    F f = { { 1, 0, 0 }, "A" };
    bool ok = f(&t) && !f(&none);
    f.Pos.X = 6; ok = ok && !f(&t);
    p.params = 0; f.Pos.X = 1; ok = ok && !f(&t);
    if (!ok) { std::printf("006a7b20_TEST FAIL\n"); return 1; }
    std::printf("006a7b20_TEST PASS\n");
    return 0;
}
