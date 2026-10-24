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
// ---- the candidate model (copied from src/0067f790.cpp) ----
// NHitLocations::CPhysicalPrimitiveMesh::Alloc() const -- vtable slot 1: returns a freshly constructed (empty) mesh primitive
// Leaf: ?Alloc@CPhysicalPrimitiveMesh@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0067F790, 24 bytes (+8 int3 padding).
// Name: FableWin.pdb ?Alloc@CPhysicalPrimitiveMesh@NHitLocations@@UBEPAVCPhysicalPrimitiveBase@2@XZ; slot 1 of the
// vtable 0x0125B9B0 (rtti-vtables.tsv: slot 0 the deleting dtor 0x0067F7B0, slot 2 GetToken, slot 3 GetType).
// Callees: operator new @ 0x00BFEA1A (`push 4 ; call ; add esp,4`).
// Shape: `new CPhysicalPrimitiveMesh` with the inline default constructor -- the mesh primitive has no data
// members (Ego_r.pdb: 4 bytes, the vptr only), so the null-checked construction is one vtable store.
namespace NHitLocations {

struct CPhysicalPrimitiveBase {
    virtual ~CPhysicalPrimitiveBase() {}
    virtual CPhysicalPrimitiveBase* Alloc() const = 0;   // slot 1
};

struct CPhysicalPrimitiveMesh : CPhysicalPrimitiveBase {
    virtual CPhysicalPrimitiveBase* Alloc() const;
};

CPhysicalPrimitiveBase* CPhysicalPrimitiveMesh::Alloc() const
{
    return new CPhysicalPrimitiveMesh;
}

} // namespace NHitLocations

// ---- stubs for the external callees ----

static void run()
{

    NHitLocations::CPhysicalPrimitiveMesh mesh;
    NHitLocations::CPhysicalPrimitiveBase* p = mesh.Alloc();
    CHECK(p != 0);
    CHECK(p != &mesh);
    NHitLocations::CPhysicalPrimitiveBase* q = p->Alloc();   // the new object is a mesh too (its Alloc works)
    CHECK(q != 0 && q != p);
    delete q;
    delete p;
}

int main()
{
    run();
    if (!g_ok) { std::printf("0067f790_TEST FAIL\n"); return 1; }
    std::printf("0067f790_TEST PASS\n");
    return 0;
}