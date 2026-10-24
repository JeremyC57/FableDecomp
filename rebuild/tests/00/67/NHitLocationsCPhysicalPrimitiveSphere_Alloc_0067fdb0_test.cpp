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
// ---- the candidate model (copied from src/0067fdb0.cpp) ----
// NHitLocations::CPhysicalPrimitiveSphere::Alloc() const -- vtable slot 1: returns a freshly constructed sphere primitive (BaseName unset)
// Leaf: ?Alloc@CPhysicalPrimitiveSphere@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0067FDB0, 31 bytes (+1 int3 padding).
// Name: FableWin.pdb ?Alloc@CPhysicalPrimitiveSphere@NHitLocations@@UBEPAVCPhysicalPrimitiveBase@2@XZ; slot 1 of the
// vtable 0x0125B9FC (rtti-vtables.tsv; slot 0 is the deleting dtor 0x0067F710).
// Callees: operator new @ 0x00BFEA1A (`push 0Ch`).
// Layout: Ego_r.pdb CPhysicalPrimitiveSphere (12 bytes) = { vptr, CDefString BaseName +4, float Radius +8 }.
// Shape: `new CPhysicalPrimitiveSphere` with the inline default constructor: the vtable store, then the
// CDefString member's own constructor (index -1); Radius is left uninitialised (no store), as retail.
namespace NHitLocations {

struct CDefString {
    long Index;
    CDefString() : Index(-1) {}
};

struct CPhysicalPrimitiveBase {
    virtual ~CPhysicalPrimitiveBase() {}
    virtual CPhysicalPrimitiveBase* Alloc() const = 0;   // slot 1
};

struct CPhysicalPrimitiveSphere : CPhysicalPrimitiveBase {
    CDefString BaseName;   // +0x04
    float      Radius;     // +0x08
    virtual CPhysicalPrimitiveBase* Alloc() const;
};

CPhysicalPrimitiveBase* CPhysicalPrimitiveSphere::Alloc() const
{
    return new CPhysicalPrimitiveSphere;
}

} // namespace NHitLocations

// ---- stubs for the external callees ----

static void run()
{

    NHitLocations::CPhysicalPrimitiveSphere sphere;
    NHitLocations::CPhysicalPrimitiveBase* p = sphere.Alloc();
    CHECK(p != 0 && p != &sphere);
    CHECK(static_cast<NHitLocations::CPhysicalPrimitiveSphere*>(p)->BaseName.Index == -1);
    delete p;
}

int main()
{
    run();
    if (!g_ok) { std::printf("0067fdb0_TEST FAIL\n"); return 1; }
    std::printf("0067fdb0_TEST PASS\n");
    return 0;
}