// Behaviour test for 004c04c0: the row body verbatim over fixture models of the shared-header interfaces it calls.
// NGraphicAppearance::CTCGraphicAppearance::SetAsHighlighted(float, float, const CRGBColour&, bool, bool, EAppearanceMeshEffectPriority, void*) -- build a smooth-outline mesh effect (its texture alpha values from the thing's appearance def, else 0x80 / 0) and install it through SetPMeshEffect
#include <stdio.h>
#include <string.h>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// Fixture models of the shared-header interfaces: a counted def (Refs counts references, Delete counted), the counted CDefPointer
// handle (copy / assignment add-ref, release on destruction), the definition manager's GetDef<T> (fills the handle with an add-ref,
// returns whether found) and CDefClassBase::GetSubDef<T> (the sub-def table of the def, by a type slot of T).
class CDefPointeeBase { public:
    long Refs; int Deleted; CDefPointeeBase* Sub[8];
    CDefPointeeBase() : Refs(1), Deleted(0) { memset(Sub, 0, sizeof(Sub)); }
    void Delete() { ++Deleted; }
};
template <class T> class CDefPointer { public:
    CDefPointeeBase* Object;
    CDefPointer() : Object(0) { }
    CDefPointer(const CDefPointer& o) : Object(o.Object) { if (Object) ++Object->Refs; }
    ~CDefPointer() { Release(); }
    CDefPointer& operator=(const CDefPointer& o) { if (Object != o.Object) { Release(); Object = o.Object; if (Object) ++Object->Refs; } return *this; }
    void Release() { if (Object) { if (--Object->Refs == 0) Object->Delete(); Object = 0; } }
    T* operator->() const { return static_cast<T*>(Object); }
    T& operator*() const { return *static_cast<T*>(Object); }
    T* Get() const { return static_cast<T*>(Object); }
    bool IsNull() const { return Object == 0; }
    void Set(CDefPointeeBase* p) { if (p) ++p->Refs; Release(); Object = p; }
};
class CDefClassBase : public CDefPointeeBase { public:
    template <class T> bool GetSubDef(CDefPointer<const T>* out) const
    {
        CDefPointeeBase* p = Sub[T::Slot];
        if (!p) return false;
        out->Set(p); return true;
    }
};
class CGameDefinitionManager { public:
    CDefPointeeBase* Table[16]; mutable long Calls; mutable long LastIndex;
    static CGameDefinitionManager& Get();
    template <class T> bool GetDef(long index, CDefPointer<const T>* out) const
    {
        ++Calls; LastIndex = index;
        if (index > 0 && index < 16 && Table[index]) { out->Set(Table[index]); return true; }
        return false;
    }
};
static CGameDefinitionManager g_manager;
CGameDefinitionManager& CGameDefinitionManager::Get() { return g_manager; }
static void ResetManager() { memset(&g_manager, 0, sizeof(g_manager)); }
class CThingBaseDef : public CDefClassBase { public: enum { Slot = 0 }; };
class CRGBColour { public: unsigned long Value; };
class CAppearanceDef : public CDefClassBase { public: enum { Slot = 1 }; unsigned char OutlineEffectAlphaRef, OutlineEffectAlphaBias; };
class CEngineMeshEffect { public: long IVRefCount; int Kind; CEngineMeshEffect() : IVRefCount(1), Kind(0) { } virtual ~CEngineMeshEffect() { } };
static int g_effectsDeleted = 0;
template <class T> class CIVCountedPointer { public:
    T* Object;
    CIVCountedPointer(T* object) : Object(object) { }
    CIVCountedPointer(const CIVCountedPointer& o) : Object(o.Object) { if (Object) ++Object->IVRefCount; }
    ~CIVCountedPointer() { if (Object && --Object->IVRefCount == 0) { ++g_effectsDeleted; delete Object; } }
};
class CEngineMeshEffectOutlineSmooth : public CEngineMeshEffect { public:
    float WidthWorldSpace, WidthScreenSpace; CRGBColour Colour; unsigned char StencilBit; long AlphaRef, AlphaBias;
    CEngineMeshEffectOutlineSmooth(float w, float s, const CRGBColour& c, unsigned char stencil, long ref, long bias)
        : WidthWorldSpace(w), WidthScreenSpace(s), Colour(c), StencilBit(stencil), AlphaRef(ref), AlphaBias(bias) { Kind = 7; }
};
enum EAppearanceMeshEffectPriority { MESH_EFFECT_PRIORITY_3 = 3 };
class CThing { public:
    CDefPointer<const CThingBaseDef> PBaseDef;
    const CThingBaseDef& PeekBaseDef() const { return *PBaseDef; }
};
namespace NGraphicAppearance {
static CEngineMeshEffectOutlineSmooth g_last(0, 0, CRGBColour(), 0, 0, 0); static bool g_flags[3]; static long g_priority; static void* g_owner; static int g_sets;
class CTCGraphicAppearance { public:
    CThing* ParentThing;
    void SetPMeshEffect(CIVCountedPointer<CEngineMeshEffect> effect, bool a, bool b, bool c, EAppearanceMeshEffectPriority priority, void* owner)
    {
        ++g_sets; g_last = *static_cast<CEngineMeshEffectOutlineSmooth*>(effect.Object);
        g_flags[0] = a; g_flags[1] = b; g_flags[2] = c; g_priority = priority; g_owner = owner;
    }
    void SetAsHighlighted(float, float, const CRGBColour&, bool, bool, EAppearanceMeshEffectPriority, void*);
};
}
using namespace NGraphicAppearance;
// ---- row body (verbatim) ----
namespace NGraphicAppearance {

void CTCGraphicAppearance::SetAsHighlighted(float widthWorldSpace, float widthScreenSpace, const CRGBColour& colour, bool p4, bool p5,
                                            EAppearanceMeshEffectPriority priority, void* owner)
{
    CThing* thing = ParentThing;
    const CThingBaseDef& base_def = thing->PeekBaseDef();
    CDefPointer<const CAppearanceDef> appearance;
    long alphaRef = 0x80;
    long alphaBias = 0;
    if (base_def.GetSubDef(&appearance)) {
        alphaRef = appearance->OutlineEffectAlphaRef;
        alphaBias = appearance->OutlineEffectAlphaBias;
    }
    SetPMeshEffect(CIVCountedPointer<CEngineMeshEffect>(new CEngineMeshEffectOutlineSmooth(widthWorldSpace, widthScreenSpace, colour, 0,
                                                                                          alphaRef, alphaBias)),
                   p4, p5, false, priority, owner);
}

}
// ---- harness ----
int main()
{
    ResetManager();
    CThingBaseDef thingDef; CAppearanceDef app; app.OutlineEffectAlphaRef = 0x40; app.OutlineEffectAlphaBias = 0x11;
    CThing thing; thing.PBaseDef.Object = &thingDef;
    NGraphicAppearance::CTCGraphicAppearance appearance; appearance.ParentThing = &thing;
    CRGBColour colour = { 0xFF00FF00 }; int key;
    appearance.SetAsHighlighted(1.5f, 2.5f, colour, true, false, MESH_EFFECT_PRIORITY_3, &key);   // no appearance sub-def: 0x80 / 0
    CHECK(g_sets == 1 && g_last.Kind == 7 && g_last.AlphaRef == 0x80 && g_last.AlphaBias == 0 && g_last.StencilBit == 0);
    CHECK(g_last.WidthWorldSpace == 1.5f && g_last.WidthScreenSpace == 2.5f && g_last.Colour.Value == 0xFF00FF00);
    CHECK(g_flags[0] && !g_flags[1] && !g_flags[2] && g_priority == 3 && g_owner == &key && g_effectsDeleted == 1);
    thingDef.Sub[1] = &app;
    appearance.SetAsHighlighted(1.0f, 1.0f, colour, false, true, MESH_EFFECT_PRIORITY_3, 0);
    CHECK(g_sets == 2 && g_last.AlphaRef == 0x40 && g_last.AlphaBias == 0x11 && !g_flags[0] && g_flags[1]);
    CHECK(app.Refs == 1 && app.Deleted == 0 && g_effectsDeleted == 2);
    thing.PBaseDef.Object = 0;
    if (fails == 0) printf("004c04c0_TEST PASS\n");
    return fails;
}
