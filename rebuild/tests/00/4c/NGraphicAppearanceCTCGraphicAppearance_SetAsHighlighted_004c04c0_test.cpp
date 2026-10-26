// Behaviour test for 004c04c0: src/004c04c0.cpp copied verbatim; SetAsHighlighted builds a mesh effect with the two
// mode bytes of the parent def's highlight sub-def (0x80 / 0 without it), hands it to SetPMeshEffect with the flags,
// priority and owner, and releases the sub-def.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// NGraphicAppearance::CTCGraphicAppearance::SetAsHighlighted(float, float, const CRGBColour&, bool, bool, EAppearanceMeshEffectPriority, void*) -- build a highlight mesh effect (its two mode bytes from the thing's appearance sub-def, else 0x80/0) and install it through SetPMeshEffect
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004C04C0, length 155 bytes (+5 int3 padding), `ret 1Ch`.
// FableWin.pdb: ?SetAsHighlighted@CTCGraphicAppearance@NGraphicAppearance@@QAEXMMABVCRGBColour@@_N1W4EAppearanceMeshEffectPriority@2@PAX@Z
// Callees: CDefClassBase::GetSubDef<...> @ 0x0042B0A2 (manifest "GetSubDef<CAbilityDef>", propagated label; the sub-def carries
// two mode bytes at +0x54/+0x55), operator new @ 0x00BFEA1A, the highlight mesh-effect ctor @ 0x004C3000 (manifest label
// "CEnginePrimitive2DText", propagated; six arguments), CTCGraphicAppearance::SetPMeshEffect @ 0x004C0240.
// Shape: the sub-def pointer starts null in the `push ecx` frame word and shares the zero register with the second mode
// value; GetSubDef is a same-TU noinline stand-in body that fills it through `T**` (writes only the out slot), so it is
// read into ebp right after the call and released by hand at the end (null test, `dec [ebp+4] ; jne`, slot 1). The parent
// thing is read into a local first (the def is staged in eax, `mov ecx,eax` after the out push). The counted pointer
// argument is built in the argument area (declared copy ctor) from the raw `new` result.
// Layout: CTCBase ParentThing +0x04; the thing's def CDefPointer +0x70.
class CRGBColour { public: unsigned char B, G, R, A; };

class CDefPointeeBase { public:
    virtual void Destroy();                     // slot 0
    virtual void Delete();                      // slot 1: `call [eax+4]`
    long m_refCount;                            // +0x04
};
template <class T>
class CDefPointer { public:
    CDefPointeeBase* Object;
    CDefPointer() : Object(0) {}
    T* operator->() const { return (T*)Object; }
};
struct CHighlightDef : CDefPointeeBase {
    unsigned char _pad08[0x54 - 0x08];
    unsigned char HighlightMode;                // +0x54
    unsigned char HighlightExtra;               // +0x55
};
class CDefClassBase : public CDefPointeeBase { public:
    bool GetSubDef(CHighlightDef** out);   // 0x0042B0A2
};
void* Find_CDefClassBase_GetSubDef(const void* owner);   // stand-in callee of the stand-in body below
#pragma auto_inline(off)
__declspec(noinline) bool CDefClassBase::GetSubDef(CHighlightDef** out)
{
    void* def = Find_CDefClassBase_GetSubDef(this);
    if (!def)
        return false;
    *out = (CHighlightDef*)def;
    return true;
}
#pragma auto_inline(on)
class CThing { public:
    unsigned char _pad00[0x70];
    CDefPointer<CDefClassBase> PDef;            // +0x70
};

class CEngineMeshEffect { public:
    virtual void v00();
    long m_refCount;
    unsigned char _pad08[0x1c - 0x08];
    CEngineMeshEffect(float a, float b, const CRGBColour& colour, bool flag, long mode, long extra);   // 0x004C3000
};
template <class T>
class CIVCountedPointer { public:
    T* Data;
    CIVCountedPointer(T* p) : Data(p) {}
    CIVCountedPointer(const CIVCountedPointer& o);
};
enum EAppearanceMeshEffectPriority { MESH_EFFECT_PRIORITY_DEFAULT };

namespace NGraphicAppearance {
class CTCGraphicAppearance {
public:
    virtual void v00();
    CThing* ParentThing;                        // +0x04
    void SetPMeshEffect(CIVCountedPointer<CEngineMeshEffect> effect, bool a, bool b, bool c,
                        EAppearanceMeshEffectPriority priority, void* owner);   // 0x004C0240
    void SetAsHighlighted(float a, float b, const CRGBColour& colour, bool p4, bool p5,
                          EAppearanceMeshEffectPriority priority, void* owner);
};

void CTCGraphicAppearance::SetAsHighlighted(float a, float b, const CRGBColour& colour, bool p4, bool p5,
                                            EAppearanceMeshEffectPriority priority, void* owner)
{
    CThing* thing = ParentThing;
    CDefClassBase* thingDef = thing->PDef.operator->();
    CHighlightDef* def = 0;
    long mode = 0x80;
    long extra = 0;
    if (thingDef->GetSubDef(&def)) {
        mode = def->HighlightMode;
        extra = def->HighlightExtra;
    }
    SetPMeshEffect(CIVCountedPointer<CEngineMeshEffect>(new CEngineMeshEffect(a, b, colour, false, mode, extra)),
                   p4, p5, false, priority, owner);
    if (def && --def->m_refCount == 0)
        def->Delete();
}
}
// ---- harness ----
static int g_deleted = 0, g_set = 0, g_copies = 0;
static void* g_subDef = 0; static const void* g_findOwner = 0;
static float g_a = 0, g_b = 0; static const CRGBColour* g_colour = 0; static bool g_flag = true; static long g_mode = -1, g_extra = -1;
static CEngineMeshEffect* g_created = 0; static CEngineMeshEffect* g_passed = 0;
static bool g_p4, g_p5, g_c; static long g_priority = -1; static void* g_owner = 0; static const void* g_setThis = 0;
void CDefPointeeBase::Destroy() { }
void CDefPointeeBase::Delete() { ++g_deleted; }
void* Find_CDefClassBase_GetSubDef(const void* owner)
{ g_findOwner = owner; if (g_subDef) ++((CDefPointeeBase*)g_subDef)->m_refCount; return g_subDef; }
void CEngineMeshEffect::v00() { }
CEngineMeshEffect::CEngineMeshEffect(float a, float b, const CRGBColour& colour, bool flag, long mode, long extra)
{ g_created = this; g_a = a; g_b = b; g_colour = &colour; g_flag = flag; g_mode = mode; g_extra = extra; m_refCount = 0; }
template <> CIVCountedPointer<CEngineMeshEffect>::CIVCountedPointer(const CIVCountedPointer& o) : Data(o.Data) { ++g_copies; }
namespace NGraphicAppearance {
void CTCGraphicAppearance::v00() { }
void CTCGraphicAppearance::SetPMeshEffect(CIVCountedPointer<CEngineMeshEffect> effect, bool a, bool b, bool c,
                                          EAppearanceMeshEffectPriority priority, void* owner)
{ ++g_set; g_setThis = this; g_passed = effect.Data; g_p4 = a; g_p5 = b; g_c = c; g_priority = priority; g_owner = owner; }
}
int main()
{
    static unsigned char defClassBuf[sizeof(CDefClassBase)];
    static unsigned char thingBuf[sizeof(CThing)]; CThing& thing = *(CThing*)thingBuf;
    thing.PDef.Object = (CDefPointeeBase*)defClassBuf;
    static CHighlightDef highlight; highlight.m_refCount = 1; highlight.HighlightMode = 0x12; highlight.HighlightExtra = 0x34;
    static unsigned char appBuf[sizeof(NGraphicAppearance::CTCGraphicAppearance)];
    NGraphicAppearance::CTCGraphicAppearance& app = *(NGraphicAppearance::CTCGraphicAppearance*)appBuf;
    app.ParentThing = &thing;
    CRGBColour colour = { 1, 2, 3, 4 }; int owner = 0;

    g_subDef = &highlight;
    app.SetAsHighlighted(1.5f, 2.5f, colour, true, false, (EAppearanceMeshEffectPriority)3, &owner);
    CHECK(g_findOwner == defClassBuf && g_a == 1.5f && g_b == 2.5f && g_colour == &colour && !g_flag);
    CHECK(g_mode == 0x12 && g_extra == 0x34 && g_set == 1 && g_setThis == &app && g_passed == g_created && g_created != 0);
    CHECK(g_p4 && !g_p5 && !g_c && g_priority == 3 && g_owner == &owner);
    CHECK(highlight.m_refCount == 1 && g_deleted == 0);
    delete g_created;

    // no sub-def: default modes
    g_subDef = 0;
    app.SetAsHighlighted(0.0f, 1.0f, colour, false, true, (EAppearanceMeshEffectPriority)0, 0);
    CHECK(g_mode == 0x80 && g_extra == 0 && g_set == 2 && !g_p4 && g_p5 && g_owner == 0 && g_deleted == 0);
    delete g_created;

    // last reference: deleted
    g_subDef = &highlight; highlight.m_refCount = 0;
    app.SetAsHighlighted(0.0f, 1.0f, colour, false, false, (EAppearanceMeshEffectPriority)0, 0);
    CHECK(g_mode == 0x12 && highlight.m_refCount == 0 && g_deleted == 1);
    delete g_created;

    if (fails == 0) printf("004c04c0_TEST PASS\n");
    return fails;
}