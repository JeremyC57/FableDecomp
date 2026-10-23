// Behaviour test for 0075c670 CTCCameraPointDefinitionBase::OnSerialise: final.cpp body verbatim; the persist context,
// the parent position and the leaf serialiser are stubs recording their calls.
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __stdcall
#define __declspec(x)
#define __forceinline inline
#endif
extern "C" int __cdecl printf(const char* format, ...);
extern "C" int __cdecl strcmp(const char* a, const char* b);
//   `mov eax,ebp` for Up then `add ebp,0xc` for Forward) while the reads stay folded on this; C3DVector has a user copy ctor.
// Leaf: OnSerialise
//
// Retail 0x0075C670, 714 bytes (`ret 8`).
// Callees: CThing::GetPosition @ 0x004C73D0, CPersistContext::Transfer<bool> @ 0x004045C0, Transfer<float> @ 0x00410620
// ("FOV", default 72), Transfer<C3DVector> @ 0x004859AD (manifest label Transfer<CFloatRange> is a propagated name);
// virtual OnSerialseLeaf (vtable +0x84, slot 33).
// Layout = Ego_r.pdb CTCCameraPointDefinitionBase (64 bytes, packed: CoordBase +0x16, CoordAxis +0x22 unaligned), as the
// landed TransformPosition (0x0075C420): CutInto +0x0c, CutOutOf +0x0d, TestAngleBeforeActivation +0x0e, FOV +0x10,
// SelfTerminate +0x14, HeroIsSubject +0x15, UsingRelativeCoords +0x3a, UsingRelativeOrientation +0x3b.
// Shape: CoordBase is saved relative to the parent's position under an "IsCoordBaseRelativeToParent" flag (always written
// true) and rebuilt on load; the axes are transferred through a local copy (Up default +Z, Forward default +X) and copied
// back.
struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) {}
    C3DVector operator-(const C3DVector& b) const { return C3DVector(X - b.X, Y - b.Y, Z - b.Z); }
    C3DVector operator+(const C3DVector& b) const { return C3DVector(X + b.X, Y + b.Y, Z + b.Z); }
};
struct CRightHandedSet {
    CRightHandedSet() {}
    CRightHandedSet(const C3DVector& up, const C3DVector& forward) { Up = up; Forward = forward; }
    C3DVector Up;                               // +0x00
    C3DVector Forward;                          // +0x0c
};
struct CThing {
    const C3DVector& GetPosition() const;       // 0x004C73D0
};
struct CPersistContext {
    unsigned char _pad00[0x18];
    long Mode;                                  // +0x18
    bool IsSaving() const { return Mode == 1 || Mode == 3; }
    void Transfer(const char* name, bool& value, const bool& defaultValue);             // 0x004045C0
    void Transfer(const char* name, float& value, const float& defaultValue);           // 0x00410620
    void Transfer(const char* name, C3DVector& value, const C3DVector& defaultValue);   // 0x004859AD
};
#pragma pack(push, 1)
class CTCCameraPointDefinitionBase {
public:
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09();
    virtual void v10();
    virtual void v11();
    virtual void v12();
    virtual void v13();
    virtual void v14();
    virtual void v15();
    virtual void v16();
    virtual void v17();
    virtual void v18();
    virtual void v19();
    virtual void v20();
    virtual void v21();
    virtual void v22();
    virtual void v23();
    virtual void v24();
    virtual void v25();
    virtual void v26();
    virtual void v27();
    virtual void v28();
    virtual void v29();
    virtual void v30();
    virtual void v31();
    virtual void v32();
    virtual void OnSerialseLeaf(CPersistContext& context);   // slot 33 (+0x84)
    CThing* ParentThing;                        // +0x04
    bool    ToBeRemoved;                        // +0x08
    bool    IsInGlobalUpdateTCs;                // +0x09
    unsigned char _pad0a[2];
    bool    CutInto;                            // +0x0c
    bool    CutOutOf;                           // +0x0d
    bool    TestAngleBeforeActivation;          // +0x0e
    unsigned char _pad0f;
    float   FOV;                                // +0x10
    bool    SelfTerminate;                      // +0x14
    bool    HeroIsSubject;                      // +0x15
    C3DVector CoordBase;                        // +0x16
    CRightHandedSet CoordAxis;                  // +0x22
    bool    UsingRelativeCoords;                // +0x3a
    bool    UsingRelativeOrientation;           // +0x3b
    bool    Activated;                          // +0x3c
    void OnSerialise(CPersistContext& context, bool transfer);
};
#pragma pack(pop)

void CTCCameraPointDefinitionBase::OnSerialise(CPersistContext& context, bool transfer)
{
    C3DVector parentPos = ParentThing->GetPosition();
    context.Transfer("CutInto", CutInto, false);
    context.Transfer("CutOutOf", CutOutOf, false);
    context.Transfer("TestAngleBeforeActivation", TestAngleBeforeActivation, false);
    context.Transfer("SelfTerminate", SelfTerminate, true);
    context.Transfer("HeroIsSubject", HeroIsSubject, false);
    context.Transfer("FOV", FOV, 72.0f);
    bool relative = true;
    context.Transfer("IsCoordBaseRelativeToParent", relative, false);
    if (context.IsSaving()) {
        C3DVector base = CoordBase - parentPos;
        context.Transfer("CoordBase", base, C3DVector(0.0f, 0.0f, 0.0f));
    } else {
        C3DVector base;
        context.Transfer("CoordBase", base, C3DVector(0.0f, 0.0f, 0.0f));
        if (relative)
            CoordBase = base + parentPos;
        else
            CoordBase = base;
    }
    C3DVector up = CoordAxis.Up;
    CRightHandedSet* axis = &CoordAxis;
    context.Transfer("CoordAxisUp", up, C3DVector(0.0f, 0.0f, 1.0f));
    C3DVector forward = CoordAxis.Forward;
    context.Transfer("CoordAxisFwd", forward, C3DVector(1.0f, 0.0f, 0.0f));
    axis->Up = up;
    axis->Forward = forward;
    context.Transfer("UsingRelativeCoords", UsingRelativeCoords, false);
    context.Transfer("UsingRelativeOrientation", UsingRelativeOrientation, false);
    OnSerialseLeaf(context);
}

static C3DVector g_parentPos(1.0f, 2.0f, 3.0f);
const C3DVector& CThing::GetPosition() const { return g_parentPos; }
static int g_calls = 0, g_leaf = 0, g_fail = 0;
static bool g_loadRelative = true;
static C3DVector g_savedBase(0, 0, 0), g_savedUp(0, 0, 0), g_savedFwd(0, 0, 0);
static float g_fovDefault = 0;
void CPersistContext::Transfer(const char* name, bool& value, const bool& defaultValue)
{
    ++g_calls;
    if (!IsSaving() && strcmp(name, "IsCoordBaseRelativeToParent") == 0) value = g_loadRelative;
    if (strcmp(name, "SelfTerminate") == 0 && defaultValue != true) ++g_fail;
}
void CPersistContext::Transfer(const char* name, float& value, const float& defaultValue)
{
    ++g_calls;
    if (strcmp(name, "FOV") == 0) g_fovDefault = defaultValue;
}
void CPersistContext::Transfer(const char* name, C3DVector& value, const C3DVector& defaultValue)
{
    ++g_calls;
    if (IsSaving()) {
        if (strcmp(name, "CoordBase") == 0) g_savedBase = value;
        if (strcmp(name, "CoordAxisUp") == 0) g_savedUp = value;
        if (strcmp(name, "CoordAxisFwd") == 0) g_savedFwd = value;
    } else {
        if (strcmp(name, "CoordBase") == 0) value = C3DVector(10.0f, 20.0f, 30.0f);
        if (strcmp(name, "CoordAxisUp") == 0) { value = C3DVector(0.0f, 1.0f, 0.0f); if (defaultValue.Z != 1.0f) ++g_fail; }
        if (strcmp(name, "CoordAxisFwd") == 0) { value = C3DVector(0.0f, 0.0f, -1.0f); if (defaultValue.X != 1.0f) ++g_fail; }
    }
}
#define V(n) void CTCCameraPointDefinitionBase::n() {}
V(v00) V(v01) V(v02) V(v03) V(v04) V(v05) V(v06) V(v07) V(v08) V(v09) V(v10) V(v11) V(v12) V(v13) V(v14) V(v15)
V(v16) V(v17) V(v18) V(v19) V(v20) V(v21) V(v22) V(v23) V(v24) V(v25) V(v26) V(v27) V(v28) V(v29) V(v30) V(v31) V(v32)
void CTCCameraPointDefinitionBase::OnSerialseLeaf(CPersistContext&) { ++g_leaf; }

static bool Eq(const C3DVector& v, float x, float y, float z) { return v.X == x && v.Y == y && v.Z == z; }

static int Check()
{
    CThing parent;
    CTCCameraPointDefinitionBase cam;
    cam.ParentThing = &parent;
    cam.CoordBase = C3DVector(5.0f, 6.0f, 7.0f);
    cam.CoordAxis.Up = C3DVector(0.0f, 0.0f, 1.0f);
    cam.CoordAxis.Forward = C3DVector(1.0f, 0.0f, 0.0f);
    CPersistContext save; save.Mode = 1;
    cam.OnSerialise(save, true);
    if (!Eq(g_savedBase, 4.0f, 4.0f, 4.0f)) return 1;
    if (!Eq(g_savedUp, 0.0f, 0.0f, 1.0f) || !Eq(g_savedFwd, 1.0f, 0.0f, 0.0f)) return 2;
    if (!Eq(cam.CoordAxis.Up, 0.0f, 0.0f, 1.0f) || !Eq(cam.CoordAxis.Forward, 1.0f, 0.0f, 0.0f)) return 3;
    if (g_calls != 12 || g_leaf != 1 || g_fovDefault != 72.0f) return 4;
    CPersistContext load; load.Mode = 0;
    g_loadRelative = true;
    cam.OnSerialise(load, true);
    if (!Eq(cam.CoordBase, 11.0f, 22.0f, 33.0f)) return 5;
    if (!Eq(cam.CoordAxis.Up, 0.0f, 1.0f, 0.0f) || !Eq(cam.CoordAxis.Forward, 0.0f, 0.0f, -1.0f)) return 6;
    g_loadRelative = false;
    cam.OnSerialise(load, true);
    if (!Eq(cam.CoordBase, 10.0f, 20.0f, 30.0f)) return 7;
    if (g_leaf != 3 || g_fail != 0) return 8;
    return 0;
}

int main()
{
    int r = Check();
    if (r == 0) printf("0075c670_TEST PASS\n"); else printf("0075c670_TEST FAIL %d\n", r);
    return r;
}