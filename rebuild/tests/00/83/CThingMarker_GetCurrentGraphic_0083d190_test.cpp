// Behaviour test for 0083d190: src/0083d190.cpp copied verbatim; the state sub-def's graphic is returned when it has a
// bank, else the village def's; every def reference taken is released.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CThingMarker::GetCurrentGraphic() const -- the marker's current map graphic: the state sub-def's graphic when it has one, else the village def's own graphic; returned by value (hidden result pointer)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0083D190, length 231 bytes (`ret 4`; 240 with padding).
// Callees: CDefClassBase::GetSubDef<T> @ 0x0042B0A2 (on the base def, out pointer), CThing::GetDefinitionManager
// @ 0x004C7990 (manifest "CViewBase::GetDisplayEngine" is propagated), CDefinitionManager::GetDef<T>(long,
// CDefPointer<T>*) @ 0x0083B970, the defs' virtual Destroy (slot 1, +4).
// Identity: the catalog label CThingVillage::GetCurrentGraphic is propagated from the twin 0x0083B520; the only caller
// is 0x0083D31E inside the CThingMarker virtual at 0x0083D2E0 (manifest CThingMarker::vfunc_36), so this is the marker's.
// Layout: retail CThing PBaseDef +0x70, DefGlobalIndex (ushort) +0x8c (Ego_r.pdb - 0x10); CThingVillage
// CurrentGraphic +0xd0 (PDB 0xe8 - 0x18); CEngineGraphic is 14 packed bytes {BankIndex, AnimStep, RenderSizeX,
// AdditiveAlpha, Type}; the state sub-def holds one at +0x25 (unaligned), the village def at +0x70 (descriptive).
// Shape: the sub-def out pointer is copied into a scoped releasing holder right after the call and each exit returns
// the graphic straight into the hidden result before the holders release (the dereference drops the null test on the
// first exit); the village def is read through a by-value counted copy
// (`.Object`: add-ref and release share one null test, both count stores kept, as the landed 0x0070B4D0); BankIndex
// is tested unsigned (`> 0` -> `jbe`).
// GetSubDef and GetDef get same-TU `auto_inline(off)` stand-in bodies that write only the out slot: VC7.1 then knows
// neither out pointer escapes, keeps the state def in edi and the village def in esi, and shares one frame word
// between both out pointers (the twin 0x0083B520 recipe).
#pragma pack(push, 1)
struct CEngineGraphic {                                     // 14 bytes
    unsigned long BankIndex;                                // +0x00
    float         AnimStep;                                 // +0x04
    float         RenderSizeX;                              // +0x08
    bool          AdditiveAlpha;                            // +0x0c
    unsigned char Type;                                     // +0x0d
};
#pragma pack(pop)

struct CDefPointeeBase {
    virtual void Destroy0();
    virtual void Destroy();                                 // slot 1 (+0x04)
    long RefCount;                                          // +0x04
    void AddRef() { ++RefCount; }
    void Release() { if (--RefCount == 0) Destroy(); }
};
template <class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) { }
};
template <class T> struct CCountedDefPointer {              // the by-value CDefPointer<T> copy
    T* Object;
    CCountedDefPointer(T* object) : Object(object) { if (Object) Object->AddRef(); }
    ~CCountedDefPointer() { if (Object) Object->Release(); }
};

#pragma pack(push, 1)
struct CVillageStateDef : CDefPointeeBase {                 // the base def's graphic sub-def (descriptive)
    unsigned char  _pad08[0x25 - 0x08];
    CEngineGraphic Graphic;                                 // +0x25
};
#pragma pack(pop)
struct CThingVillageDef : CDefPointeeBase {
    unsigned char  _pad08[0x70 - 0x08];
    CEngineGraphic Graphic;                                 // +0x70 (descriptive)
};
struct CThingBaseDef : CDefPointeeBase {
    bool GetSubDef(CDefPointer<CVillageStateDef>* out) const;   // 0x0042B0A2
};
struct CDefinitionManager {
    bool GetDef_0083d630(long index, CDefPointer<CThingVillageDef>* out);   // 0x0083B970
};

void* FindSubDef_CThingBaseDef_GetSubDef(const CThingBaseDef* def);       // stand-in callees of the stand-in bodies below
void* FindDef_CDefinitionManager_GetDef(long index);
// Stand-in bodies (the real ones are out of line in retail): they write the out slot only, so VC7.1 knows the def
// pointers' addresses do not escape the calls (the landed 0x00848730 idiom).
#pragma auto_inline(off)
__declspec(noinline) bool CThingBaseDef::GetSubDef(CDefPointer<CVillageStateDef>* out) const
{
    void* def = FindSubDef_CThingBaseDef_GetSubDef(this);
    if (!def)
        return false;
    out->Object = (CVillageStateDef*)def;
    return true;
}
__declspec(noinline) bool CDefinitionManager::GetDef_0083d630(long index, CDefPointer<CThingVillageDef>* out)
{
    void* def = FindDef_CDefinitionManager_GetDef(index);
    if (!def)
        return false;
    out->Object = (CThingVillageDef*)def;
    return true;
}
#pragma auto_inline(on)

struct CThing {
    unsigned char  _pad00[0x70];
    CThingBaseDef* PBaseDef;                                // +0x70
    unsigned char  _pad74[0x8c - 0x74];
    unsigned short DefGlobalIndex;                          // +0x8c
    CDefinitionManager* GetDefinitionManager();            // 0x004C7990
};

struct CThingMarker : CThing {
    CEngineGraphic GetCurrentGraphic();
};

template <class T> struct CHeldDef {                       // a def reference held for the rest of the scope
    T* Object;
    CHeldDef(T* object) : Object(object) { }
    ~CHeldDef() { if (Object) { if (--Object->RefCount == 0) Object->Destroy(); } }
};

CEngineGraphic CThingMarker::GetCurrentGraphic()
{
    CDefPointer<CVillageStateDef> out;
    bool found = PBaseDef->GetSubDef(&out);
    CHeldDef<CVillageStateDef> state(out.Object);
    if (found && state.Object->Graphic.BankIndex > 0)
        return state.Object->Graphic;
    CDefPointer<CThingVillageDef> defOut;
    GetDefinitionManager()->GetDef_0083d630(DefGlobalIndex, &defOut);
    CHeldDef<CThingVillageDef> def(CCountedDefPointer<CThingVillageDef>(defOut.Object).Object);
    return def.Object->Graphic;
}

// ---- harness ----
static int g_destroyed = 0;
void CDefPointeeBase::Destroy0() { }
void CDefPointeeBase::Destroy() { ++g_destroyed; }
static CVillageStateDef g_state;
static CThingVillageDef g_village;
static bool g_haveState = true;
static long g_lastIndex = -1;
void* FindSubDef_CThingBaseDef_GetSubDef(const CThingBaseDef*) { if (!g_haveState) return 0; ++g_state.RefCount; return &g_state; }
void* FindDef_CDefinitionManager_GetDef(long index) { g_lastIndex = index; ++g_village.RefCount; return &g_village; }
static CDefinitionManager g_mgr;
CDefinitionManager* CThing::GetDefinitionManager() { return &g_mgr; }
static void SetGraphic(CEngineGraphic& g, unsigned long bank, float step) { memset(&g, 0, sizeof g); g.BankIndex = bank; g.AnimStep = step; g.RenderSizeX = 2.0f; g.AdditiveAlpha = true; g.Type = 7; }
int main()
{
    CHECK(sizeof(CEngineGraphic) == 14);
    CThingBaseDef base;
    CThingMarker m;
    memset(&m, 0, sizeof m);
    m.PBaseDef = &base; m.DefGlobalIndex = 0x321;
    g_state.RefCount = 1; g_village.RefCount = 1;
    SetGraphic(g_state.Graphic, 5, 0.5f);
    SetGraphic(g_village.Graphic, 9, 0.25f);
    CEngineGraphic r = m.GetCurrentGraphic();
    CHECK(r.BankIndex == 5 && r.AnimStep == 0.5f && r.Type == 7 && r.AdditiveAlpha && r.RenderSizeX == 2.0f);
    CHECK(g_state.RefCount == 1 && g_village.RefCount == 1 && g_lastIndex == -1 && g_destroyed == 0);
    g_state.Graphic.BankIndex = 0;
    r = m.GetCurrentGraphic();
    CHECK(r.BankIndex == 9 && r.AnimStep == 0.25f && g_lastIndex == 0x321);
    CHECK(g_state.RefCount == 1 && g_village.RefCount == 1 && g_destroyed == 0);
    g_haveState = false; g_village.Graphic.BankIndex = 11;
    CHECK(m.GetCurrentGraphic().BankIndex == 11 && g_state.RefCount == 1 && g_village.RefCount == 1 && g_destroyed == 0);
    g_haveState = true; g_state.RefCount = 0; g_village.RefCount = 0; g_state.Graphic.BankIndex = 0;
    CHECK(m.GetCurrentGraphic().BankIndex == 11 && g_destroyed == 2 && g_state.RefCount == 0 && g_village.RefCount == 0);
    g_state.Graphic.BankIndex = 3;
    CHECK(m.GetCurrentGraphic().BankIndex == 3 && g_destroyed == 3);
    if (fails == 0) printf("0083d190_TEST PASS\n");
    return fails;
}