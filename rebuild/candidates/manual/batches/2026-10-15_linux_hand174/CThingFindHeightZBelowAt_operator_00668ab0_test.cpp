#include <cstdio>
struct V { float X, Y, Z; };
struct Mesh { float hitZ; bool hits;
    bool GetPointIntersectionFromTo(const V&, const V&, V* out) const { if (!hits) return false; out->X = 0; out->Y = 0; out->Z = hitZ; return true; } };
struct Phys { void* params; Mesh m; void* GetPhysicsMeshParameters() const { return params; } Mesh GetPhysicsMeshObject() const { return m; } };
struct Thing { Phys* PhysicsTC; };
struct F { V* From; V* To; float Height;
    bool operator()(Thing* t) {
        Phys* p = t->PhysicsTC;
        if (p && p->GetPhysicsMeshParameters()) {
            Mesh m = p->GetPhysicsMeshObject(); V d;
            if (m.GetPointIntersectionFromTo(*From, *To, &d)) { float z = d.Z + 0.007f; if (z > Height && z < From->Z) { Height = z; return true; } }
        }
        return true; } };
int main()
{
    V from = { 0, 0, 10 }, to = { 0, 0, -10 };
    int dummy;
    Phys p = { &dummy, { 2.0f, true } };
    Thing t = { &p };
    F f = { &from, &to, 0.0f };
    bool ok = f(&t) && f.Height > 2.006f && f.Height < 2.008f;
    p.m.hitZ = 1.0f; ok = ok && f(&t) && f.Height > 2.006f;
    p.m.hitZ = 11.0f; ok = ok && f(&t) && f.Height < 2.008f;
    if (!ok) { std::printf("00668ab0_TEST FAIL\n"); return 1; }
    std::printf("00668ab0_TEST PASS\n");
    return 0;
}
