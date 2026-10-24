#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <new>
#ifndef _MSC_VER
#define __cdecl
#define __stdcall
#define __fastcall
#define __fastcall
#define __forceinline inline
#define __int64 long long
#endif
static int g_ok = 1;
#define CHECK(c) do { if (!(c)) { g_ok = 0; std::printf("check failed line %d\n", __LINE__); } } while (0)
// ---- the candidate model (copied from src/00814bc0.cpp) ----
// NHitLocations::CPhysicalPrimitiveMesh::GetClosestPointToPos2D(const CThing& thing, const C3DVector& pos, C3DVector& normal) const -- a mesh primitive has no 2D shape: the closest point is the position itself
// Leaf: ?GetClosestPointToPos2D@CPhysicalPrimitiveMesh@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00814BC0, 27 bytes (+5 int3 padding).
// Name: FableWin.pdb ?GetClosestPointToPos2D@CPhysicalPrimitiveMesh@NHitLocations@@UBE?AVC3DVector@@ABVCThing@@ABV3@AAV3@@Z;
// slot 10 of the vtable 0x0125B9B0 (rtti-vtables.tsv; the sphere/cylinder slot 10 is the same method).
// Callees: none.
// Shape: the C3DVector result comes back through the hidden pointer ([esp+4], returned in eax); `pos`
// is the third stack word ([esp+0Ch]) and is copied word by word through edx by C3DVector's memberwise
// USER copy constructor (the implicit copy uses esi and a `mov edx,eax` copy of the result pointer); `thing` and `normal` are unused. `ret 10h`: hidden pointer + three arguments.
struct CThing;

struct C3DVector {
    float X, Y, Z;
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) {}
};

namespace NHitLocations {

struct CPhysicalPrimitiveMesh {
    virtual void v00();
    C3DVector GetClosestPointToPos2D(const CThing& thing, const C3DVector& pos, C3DVector& normal) const;
};

C3DVector CPhysicalPrimitiveMesh::GetClosestPointToPos2D(const CThing& thing, const C3DVector& pos, C3DVector& normal) const
{
    return pos;
}

} // namespace NHitLocations

// ---- stubs for the external callees ----

void NHitLocations::CPhysicalPrimitiveMesh::v00() {}
struct SRaw { float X, Y, Z; };

static void run()
{

    SRaw rawPos = { 1.5f, -2.0f, 3.25f };
    SRaw rawNormal = { 0.0f, 0.0f, 0.0f };
    int thingStorage = 0;
    const CThing& thing = *reinterpret_cast<const CThing*>(&thingStorage);
    const C3DVector& pos = *reinterpret_cast<const C3DVector*>(&rawPos);
    C3DVector& normal = *reinterpret_cast<C3DVector*>(&rawNormal);
    NHitLocations::CPhysicalPrimitiveMesh mesh;
    C3DVector r = mesh.GetClosestPointToPos2D(thing, pos, normal);
    CHECK(r.X == 1.5f && r.Y == -2.0f && r.Z == 3.25f);
    CHECK(rawNormal.X == 0.0f && rawNormal.Y == 0.0f && rawNormal.Z == 0.0f);
}

int main()
{
    run();
    if (!g_ok) { std::printf("00814bc0_TEST FAIL\n"); return 1; }
    std::printf("00814bc0_TEST PASS\n");
    return 0;
}