// Behaviour test for 00917a10: final.cpp copied verbatim; the building, its sim-definition pointer, the physics
// bounding info and the inside test are stubbed so each exit and the def release are observable.
#include <cstdio>
#include <cstring>
#include <cstdlib>
#ifndef _MSC_VER
#define __forceinline inline
#define __declspec(x)
#define __fastcall
#define __cdecl
#define _snprintf snprintf
#endif
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
#define CHECKLOG(e) do { if (strcmp(g_log, e) != 0) { ++fails; printf("FAIL %d: log=%s\n  expected=%s\n", __LINE__, g_log, e); } } while (0)
static char g_log[4096];
static void note(const char* s) { if (strlen(g_log) + strlen(s) + 2 < sizeof(g_log)) { strcat(g_log, s); strcat(g_log, ";"); } }
// ---- source (verbatim) ----
// CAIStateGroup_Clean::IsRunnable() -- runnable when there is a building whose sim definition is cleanable (flag 0x20 clear), which has physics, and whose bounding centre lies inside it
// Leaf: IsRunnable@CAIStateGroup_Clean
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00917A10, length 184 bytes (+ int3 padding).
// Callees: own virtual slot 14 (+0x38, the building), CThingBuilding::PeekPSimDefinition @ 0x0082D2D0 (landed; CDefPointer
// <CSimBuildingDef> by value), CTCPhysicsBase::GetBoundingInfo(C3DVector*, float*) @ 0x006AF640, CThingBuilding::IsPointInside
// @ 0x0082D000.
// Shape: the def CDefPointer local is released inline on every exit (null test, then the def's inline
// ReleaseTemporary() through the ref-count base at +4 in the `if (--Count == 0) Destroy()` form: `add eax,4 ; dec ;
// mov [eax],esi ; test ; jne ; call [vtbl+4]` -- the decremented count is stored before the test; a DecRef() +
// GetRefCount() pair sinks the store below it); the inside test result is held in bl across that release.
class C3DVector { public: float X, Y, Z; };

class CTCPhysicsBase
{ public:
    void GetBoundingInfo(C3DVector* centre, float* radius);    // 0x006AF640
};

struct CRefCounted                                  // reference count base at +4 of a def
{
    long Count;
    void DecRef() { --Count; }
    long GetRefCount() const { return Count; }
};

class CSimBuildingDef
{ public:
    virtual void v00();
    virtual void Destroy();                         // slot 1 (+0x04)
    CRefCounted Ref;                                // +0x04
    unsigned char _pad08[0x3c - 0x08];
    unsigned long Flags;                            // +0x3c (bit 5: not cleanable)
    void ReleaseTemporary() { if (--Ref.Count == 0) Destroy(); }
};

template <class T>
class CDefPointer
{ public:
    T* Object;
    ~CDefPointer() { if (Object) Object->ReleaseTemporary(); }
    T* operator->() const { return Object; }
};

class CThingBuilding
{ public:
    unsigned char _pad00[0x60];
    CTCPhysicsBase* Physics;                        // +0x60
    bool IsPointInside(const C3DVector& point);     // 0x0082D000
    CDefPointer<CSimBuildingDef> PeekPSimDefinition();      // 0x0082D2D0 (by value, hidden pointer)
};

struct CAIStateGroup_Clean
{
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10();
    virtual bool IsRunnable();                      // slot 11 (+0x2c)
    virtual void v12();
    virtual void OnActivate();                      // slot 13 (+0x34)
    virtual CThingBuilding* GetBuilding();          // slot 14 (+0x38)
    unsigned char _pad04[0x20 - 0x04];
    float FloorHeight;                              // +0x20
    bool Activated;                                 // +0x24
};

bool CAIStateGroup_Clean::IsRunnable()
{
    if (!GetBuilding())
        return false;
    CDefPointer<CSimBuildingDef> def = GetBuilding()->PeekPSimDefinition();
    if (def.Object && (def->Flags & 0x20))
        return false;
    if (!GetBuilding()->Physics)
        return false;
    float radius;
    C3DVector pos;
    CThingBuilding* building = GetBuilding();
    building->Physics->GetBoundingInfo(&pos, &radius);
    return GetBuilding()->IsPointInside(pos);
}

// ---- stubs ----
static char g_buf[256];
struct CTestDef : CSimBuildingDef {
    virtual void v00() { }
    virtual void Destroy() { note("Destroy"); }
};
void CSimBuildingDef::v00() { }
void CSimBuildingDef::Destroy() { note("base Destroy"); }
static CTestDef g_def;
static CSimBuildingDef* g_defPtr = 0;
static CThingBuilding g_building;
static CTCPhysicsBase g_physics;
static bool g_inside = false;
CDefPointer<CSimBuildingDef> CThingBuilding::PeekPSimDefinition()
{
    note("PeekPSimDefinition");
    CDefPointer<CSimBuildingDef> p; p.Object = g_defPtr;
    if (g_defPtr) ++g_defPtr->Ref.Count;            // the peek hands out a counted reference
    return p;
}
void CTCPhysicsBase::GetBoundingInfo(C3DVector* centre, float* radius) { note("GetBoundingInfo"); centre->X = 1.0f; centre->Y = 2.0f; centre->Z = 3.0f; *radius = 4.0f; }
bool CThingBuilding::IsPointInside(const C3DVector& point)
{
    _snprintf(g_buf, sizeof(g_buf), "IsPointInside(%g,%g,%g)", point.X, point.Y, point.Z); note(g_buf);
    return g_inside;
}
struct CTestGroup : CAIStateGroup_Clean {
    CThingBuilding* building;
    virtual void v00() { } virtual void v01() { } virtual void v02() { } virtual void v03() { } virtual void v04() { }
    virtual void v05() { } virtual void v06() { } virtual void v07() { } virtual void v08() { } virtual void v09() { }
    virtual void v10() { }
    virtual void v12() { }
    virtual void OnActivate() { }
    virtual CThingBuilding* GetBuilding() { note("GetBuilding"); return building; }
};
void CAIStateGroup_Clean::v00() { } void CAIStateGroup_Clean::v01() { } void CAIStateGroup_Clean::v02() { } void CAIStateGroup_Clean::v03() { }
void CAIStateGroup_Clean::v04() { } void CAIStateGroup_Clean::v05() { } void CAIStateGroup_Clean::v06() { } void CAIStateGroup_Clean::v07() { }
void CAIStateGroup_Clean::v08() { } void CAIStateGroup_Clean::v09() { } void CAIStateGroup_Clean::v10() { } void CAIStateGroup_Clean::v12() { }
void CAIStateGroup_Clean::OnActivate() { }
CThingBuilding* CAIStateGroup_Clean::GetBuilding() { return 0; }

int main()
{
    CTestGroup group;
    bool r;
    // The by-value CDefPointer return has no copy constructor in the model, so an unoptimised test build may destroy
    // (release) extra temporaries per peek: measure that per-call net decrement once and use it in the expectations.
    g_defPtr = &g_def; g_def.Ref.Count = 100; g_def.Flags = 0;
    { CDefPointer<CSimBuildingDef> probe = g_building.PeekPSimDefinition(); }
    long perCall = 100 - g_def.Ref.Count;              // 0 when every copy is elided
    CHECK(perCall >= 0 && perCall < 10);

    // 1. no building
    group.building = 0; g_log[0] = 0;
    r = group.IsRunnable();
    CHECK(!r); CHECKLOG("GetBuilding;");

    // 2. not-cleanable def (flag 0x20): released on the early exit, no destroy
    group.building = &g_building; g_building.Physics = &g_physics;
    g_def.Ref.Count = 10; g_def.Flags = 0x20; g_log[0] = 0;
    r = group.IsRunnable();
    CHECK(!r); CHECKLOG("GetBuilding;GetBuilding;PeekPSimDefinition;");
    CHECK(g_def.Ref.Count == 10 - perCall);

    // 3. not-cleanable def held only by the peek: the last release destroys it
    g_def.Ref.Count = perCall; g_log[0] = 0;
    r = group.IsRunnable();
    CHECK(!r); CHECKLOG("GetBuilding;GetBuilding;PeekPSimDefinition;Destroy;");
    CHECK(g_def.Ref.Count == 0);

    // 4. cleanable def, no physics
    g_def.Ref.Count = 10; g_def.Flags = 0; g_building.Physics = 0; g_log[0] = 0;
    r = group.IsRunnable();
    CHECK(!r); CHECKLOG("GetBuilding;GetBuilding;PeekPSimDefinition;GetBuilding;");
    CHECK(g_def.Ref.Count == 10 - perCall);

    // 5. physics, centre outside
    g_building.Physics = &g_physics; g_inside = false; g_def.Ref.Count = 10; g_log[0] = 0;
    r = group.IsRunnable();
    CHECK(!r); CHECKLOG("GetBuilding;GetBuilding;PeekPSimDefinition;GetBuilding;GetBuilding;GetBoundingInfo;GetBuilding;IsPointInside(1,2,3);");
    CHECK(g_def.Ref.Count == 10 - perCall);

    // 6. centre inside: runnable; released at the end
    g_inside = true; g_def.Ref.Count = 10; g_log[0] = 0;
    r = group.IsRunnable();
    CHECK(r); CHECKLOG("GetBuilding;GetBuilding;PeekPSimDefinition;GetBuilding;GetBuilding;GetBoundingInfo;GetBuilding;IsPointInside(1,2,3);");
    CHECK(g_def.Ref.Count == 10 - perCall);

    // 7. inside, released down to zero at the end: destroyed after the inside test
    g_def.Ref.Count = perCall; g_log[0] = 0;
    r = group.IsRunnable();
    CHECK(r); CHECKLOG("GetBuilding;GetBuilding;PeekPSimDefinition;GetBuilding;GetBuilding;GetBoundingInfo;GetBuilding;IsPointInside(1,2,3);Destroy;");

    // 8. no def at all: nothing to release, still runnable when inside
    g_defPtr = 0; g_log[0] = 0;
    r = group.IsRunnable();
    CHECK(r); CHECKLOG("GetBuilding;GetBuilding;PeekPSimDefinition;GetBuilding;GetBuilding;GetBoundingInfo;GetBuilding;IsPointInside(1,2,3);");

    if (fails == 0) printf("00917a10_TEST PASS\n");
    else printf("00917a10_TEST FAIL (%d)\n", fails);
    return fails ? 1 : 0;
}