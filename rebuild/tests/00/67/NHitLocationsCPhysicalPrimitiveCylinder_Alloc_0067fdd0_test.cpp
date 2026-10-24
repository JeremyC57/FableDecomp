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
// ---- the candidate model (copied from src/0067fdd0.cpp) ----
// NHitLocations::CPhysicalPrimitiveCylinder::Alloc() const -- vtable slot 1: returns a freshly constructed cylinder primitive (BaseName unset)
// Leaf: ?Alloc@CPhysicalPrimitiveCylinder@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0067FDD0, 31 bytes.
// Name: FableWin.pdb ?Alloc@CPhysicalPrimitiveCylinder@NHitLocations@@UBEPAVCPhysicalPrimitiveBase@2@XZ; slot 1 of the
// vtable 0x0125BA4C (rtti-vtables.tsv; slot 0 is the deleting dtor 0x0067F770).
// Callees: operator new @ 0x00BFEA1A (`push 10h`).
// Layout: Ego_r.pdb CPhysicalPrimitiveCylinder (16 bytes) = { vptr, CDefString BaseName +4, float Radius +8, float Height +0xC }.
// Shape: `new CPhysicalPrimitiveCylinder` with the inline default constructor: the vtable store, then the
// CDefString member's own constructor (index -1); Radius and Height are left uninitialised (no store), as retail.
namespace NHitLocations {

struct CDefString {
    long Index;
    CDefString() : Index(-1) {}
};

struct CPhysicalPrimitiveBase {
    virtual ~CPhysicalPrimitiveBase() {}
    virtual CPhysicalPrimitiveBase* Alloc() const = 0;   // slot 1
};

struct CPhysicalPrimitiveCylinder : CPhysicalPrimitiveBase {
    CDefString BaseName;   // +0x04
    float      Radius;     // +0x08
    float      Height;     // +0x0C
    virtual CPhysicalPrimitiveBase* Alloc() const;
};

CPhysicalPrimitiveBase* CPhysicalPrimitiveCylinder::Alloc() const
{
    return new CPhysicalPrimitiveCylinder;
}

} // namespace NHitLocations

// ---- stubs for the external callees ----

static void run()
{

    NHitLocations::CPhysicalPrimitiveCylinder cylinder;
    NHitLocations::CPhysicalPrimitiveBase* p = cylinder.Alloc();
    CHECK(p != 0 && p != &cylinder);
    CHECK(static_cast<NHitLocations::CPhysicalPrimitiveCylinder*>(p)->BaseName.Index == -1);
    delete p;
}

int main()
{
    run();
    if (!g_ok) { std::printf("0067fdd0_TEST FAIL\n"); return 1; }
    std::printf("0067fdd0_TEST PASS\n");
    return 0;
}