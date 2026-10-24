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
// ---- the candidate model (copied from src/007e1570.cpp) ----
// NGraphicAppearance::CTCGraphicAppearanceMultipleStaticMeshes::OnCreate() -- vtable slot 4: once the owner exists, fetch the component's CMultiStaticMeshDef sub-def from the owner's def
// Leaf: ?OnCreate@CTCGraphicAppearanceMultipleStaticMeshes@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007E1570, 22 bytes (+10 int3 padding).
// Name: slot 4 of the vtable 0x0126FFCC (rtti-vtables.tsv); slot 4 of the sibling CTCGraphicAppearanceSprite
// vtable is the landed OnCreate 0x0077C480 (empty), so this slot is the component's OnCreate.
// Callees: CDefClassBase::GetSubDef<CMultiStaticMeshDef>(CDefPointer<T>&) @ 0x007E1400 (landed as
// GetSubDef_007e1400; its bool result is dropped).
// Layout: as the landed constructor 0x007E14C0: CTCBase (vptr, ParentThing +4), the appearance-component
// base vptr +0x0C, PrimHandles +0x10, PDef (CDefPointer<CMultiStaticMeshDef const>) +0x1C. Retail CThing holds its
// def pointer (a CDefClassBase*) at +0x70.
// Shape: `this` is copied to eax first (kept for the `add eax,1Ch` argument), the owner's def is loaded
// into ecx and null-tested, the call is the `if` body; plain `ret`.
struct CDefClassBase;

struct CThing {
    unsigned char  _pad_0x00[0x70];
    CDefClassBase* PDef;                                // +0x70 (retail)
};

struct CSubDef;

struct CDefPointer {                                    // CDefPointer<T>, as the landed GetSubDef 0x007E1400
    CSubDef* m_p;
};

struct CDefClassBase {
    bool GetSubDef_007e1400(CDefPointer& out);          // 0x007E1400
};

struct CTCBase {
    virtual void v00();
    CThing* ParentThing;                                // +0x04
    bool    ToBeRemoved;                                // +0x08
    bool    IsInGlobalUpdateTCs;                        // +0x09
};

namespace NGraphicAppearance {

struct CPrimitive;

struct CAGraphicAppearanceComponent {
    virtual void v00();
};

struct CTCGraphicAppearanceMultipleStaticMeshes : CTCBase, CAGraphicAppearanceComponent {
    CPrimitive** PrimHandles[3];                        // +0x10 retail vector<CPrimitive*>
    CDefPointer  PDef;                                  // +0x1C
    void OnCreate();
};

void CTCGraphicAppearanceMultipleStaticMeshes::OnCreate()
{
    if (ParentThing->PDef)
        ParentThing->PDef->GetSubDef_007e1400(PDef);
}

} // namespace NGraphicAppearance

// ---- stubs for the external callees ----

static int g_subDefCalls = 0;
static CDefClassBase* g_subDefThis = 0;
bool CDefClassBase::GetSubDef_007e1400(CDefPointer& out)
{
    ++g_subDefCalls;
    g_subDefThis = this;
    out.m_p = reinterpret_cast<CSubDef*>(this);
    return true;
}
void CTCBase::v00() {}
void NGraphicAppearance::CAGraphicAppearanceComponent::v00() {}

static void run()
{

    CThing thing;
    thing.PDef = 0;
    NGraphicAppearance::CTCGraphicAppearanceMultipleStaticMeshes tc;
    tc.ParentThing = &thing;
    tc.PDef.m_p = 0;
    tc.OnCreate();                                   // no owner def: nothing fetched
    CHECK(g_subDefCalls == 0 && tc.PDef.m_p == 0);
    CDefClassBase def;
    thing.PDef = &def;
    tc.OnCreate();
    CHECK(g_subDefCalls == 1 && g_subDefThis == &def);
    CHECK(tc.PDef.m_p == reinterpret_cast<CSubDef*>(&def));
}

int main()
{
    run();
    if (!g_ok) { std::printf("007e1570_TEST FAIL\n"); return 1; }
    std::printf("007e1570_TEST PASS\n");
    return 0;
}