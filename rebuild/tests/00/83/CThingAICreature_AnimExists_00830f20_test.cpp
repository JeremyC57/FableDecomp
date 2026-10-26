// Behaviour test for 00830f20: src/00830f20.cpp copied verbatim; the creature def's animation table is searched with the
// current mode's filter; on a miss, unless restricted to the current mode, with a no-group filter; the def is released once.
#include <cstdio>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CThingAICreature::AnimExists(const CCharString& name, bool currentModeOnly) const -- looks the animation name up in the creature def's animation table with the current mode's filter; failing that, unless restricted to the current mode, with the no-group filter
// Leaf: HasAnimation_slot73
// Retail: Fable.exe (TLC, VC7.1) @ 0x00830F20, 181 bytes (+11 int3). `ret 8`, bool in al. CThingAICreature vtable 0x0127293C slot 73 (+0x124).
//         Catalog name CThingAICreature::vfunc_73.
// Callees: CDefClassBase::GetSubDef(CDefPointer<T>*) 0x0042B0A2 on the base def (manifest "GetSubDef<CAbilityDef>" is a propagated
//          instantiation), the mode manager's current mode 0x0073A6E0 (manifest "CPixelShader::CountReferences" propagated; landed
//          as GetCurrentMode), CMode virtual slot 4 (+0x10, the animation filter), the def table's find 0x005DC2E0 (manifest
//          "CAnimationDef_FindAnimationByName" propagated), CDefClassBase virtual slot 1 (Delete) on the def copy's last reference.
//         Vtables: CAnimFilterNoGroup 0x0122FFDC, CAnimFilterBase 0x0122FFCC.
// Shape: the def holder (frame word, zeroed first) is released at each of the three exits (null test, folded decrement, Delete);
//        the no-group filter temporary is built in the second argument's dead home slot; the table address (def+0x34) stays in edi.
//        GetSubDef gets a same-TU `auto_inline(off)` stand-in body that writes only the out slot: VC7.1 then knows the
//        holder does not escape and keeps the def in esi across the mode/filter/find calls (an extern declaration
//        re-reads the holder for each release). The fallback is an `if (!currentModeOnly) { ... }` block so the false
//        return is laid out last, as retail.
// Layout: CThing PBaseDef +0x70; the +0x134 pointer is the mode manager (descriptive); the sub-def's animation table at +0x34.
class CCharString { public: void* PStringData; };
class CAnimFilterBase {
public:                             // vtable 0x0122FFCC
    virtual ~CAnimFilterBase() { }
    virtual void v01();
    virtual void v02();
};
class CAnimFilterNoGroup : public CAnimFilterBase {
public:        // vtable 0x0122FFDC
    CAnimFilterNoGroup() { }
};
class CAnimationSet {                                // Ego_r CAnimationSet: the creature def's animation set
public:
    bool AnimExists(const CCharString& name, const CAnimFilterBase& filter) const;   // 0x005DC2E0
};
struct CDefClassBase {
    virtual void v00();
    virtual void Delete();                           // slot 1 (+0x04)
    long RefCount;                                   // +0x04
    bool GetSubDef(void* out) const;                 // 0x0042B0A2
};
struct CCreatureDef : CDefClassBase {                // descriptive
    unsigned char   _pad08[0x34 - 0x08];
    CAnimationSet   Animations;                      // +0x34
};
struct CDefPointer {                                 // CDefPointer<const CCreatureDef>
    CCreatureDef* Object;
    CDefPointer() : Object(0) { }
    ~CDefPointer()
    {
        CCreatureDef* old = Object;
        if (old) {
            if (--old->RefCount == 0)
                old->Delete();
        }
    }
};
void* FindSubDef_CDefClassBase_GetSubDef(const CDefClassBase* def);   // stand-in callee of the stand-in body below
// Stand-in body (the real one is out of line in retail): it writes the out slot only, so VC7.1 knows the def
// pointer's address does not escape the call (the landed 0x00848730 idiom).
#pragma auto_inline(off)
__declspec(noinline) bool CDefClassBase::GetSubDef(void* out) const
{
    void* def = FindSubDef_CDefClassBase_GetSubDef(this);
    if (!def)
        return false;
    *(void**)out = def;
    return true;
}
#pragma auto_inline(on)

struct CMode {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual const CAnimFilterBase* GetAnimFilter_slot4();   // slot 4 (+0x10)
};
struct CModeManager {                                // descriptive
    CMode* GetCurrentMode();                         // 0x0073A6E0
};
bool Run(bool first, bool second, bool currentOnly);
class CThingAICreature {
public:
    // vptr +0x00 (AnimExists is vtable slot 73)
    unsigned char _pad04[0x70 - 4];
    CDefClassBase* PBaseDef;                         // +0x70 CDefPointer<const CThingBaseDef>::Object
    unsigned char _pad74[0x134 - 0x74];
    CModeManager* m_pModeManager;                    // +0x134 (descriptive)
protected:
    virtual bool AnimExists(const CCharString& name, bool currentModeOnly) const;   // slot 73
    friend bool Run(bool, bool, bool);
};
bool CThingAICreature::AnimExists(const CCharString& name, bool currentModeOnly) const
{
    CDefPointer def;
    PBaseDef->GetSubDef(&def);
    if (def.Object->Animations.AnimExists(name, *m_pModeManager->GetCurrentMode()->GetAnimFilter_slot4()))
        return true;
    if (!currentModeOnly) {
        CAnimFilterNoGroup noGroup;
        return def.Object->Animations.AnimExists(name, noGroup);
    }
    return false;
}

// ---- harness ----
static int g_deleted = 0, g_findCalls = 0, g_subDefCalls = 0;
static bool g_results[2];
static const CAnimFilterBase* g_filters[2];
static const CCharString* g_names[2];
static const CAnimationSet* g_tables[2];
void CAnimFilterBase::v01() { }
void CAnimFilterBase::v02() { }
void CDefClassBase::v00() { }
void CDefClassBase::Delete() { ++g_deleted; }
static CCreatureDef g_def;
void* FindSubDef_CDefClassBase_GetSubDef(const CDefClassBase*) { ++g_subDefCalls; ++g_def.RefCount; return &g_def; }
bool CAnimationSet::AnimExists(const CCharString& name, const CAnimFilterBase& filterRef) const
{
    const CAnimFilterBase* filter = &filterRef;
    int i = g_findCalls++;
    if (i < 2) { g_filters[i] = filter; g_names[i] = &name; g_tables[i] = this; return g_results[i]; }
    return false;
}
struct CModeFilter : CAnimFilterBase { void v01() { } void v02() { } };
static CModeFilter g_modeFilter;
void CMode::v00() { } void CMode::v01() { } void CMode::v02() { } void CMode::v03() { }
const CAnimFilterBase* CMode::GetAnimFilter_slot4() { return &g_modeFilter; }
static CMode g_mode;
CMode* CModeManager::GetCurrentMode() { return &g_mode; }
static CModeManager g_mgr;
static void* VtableOf(const void* p) { return *(void* const*)p; }
bool Run(bool first, bool second, bool currentOnly)
{
    g_findCalls = 0; g_results[0] = first; g_results[1] = second; g_filters[0] = g_filters[1] = 0;
    CThingAICreature c;
    c.PBaseDef = &g_def; c.m_pModeManager = &g_mgr;
    CCharString name; name.PStringData = 0;
    bool r = c.AnimExists(name, currentOnly);
    CHECK(g_names[0] == &name && g_tables[0] == &g_def.Animations);
    CHECK(g_filters[0] == &g_modeFilter);
    return r;
}
int main()
{
    CAnimFilterNoGroup probe;
    g_def.RefCount = 1;                               // the def system's own reference
    CHECK(Run(true, false, false) == true);           // found with the mode filter: one search
    CHECK(g_findCalls == 1 && g_def.RefCount == 1 && g_deleted == 0);
    CHECK(Run(false, true, true) == false);           // restricted to the current mode: no fallback
    CHECK(g_findCalls == 1 && g_def.RefCount == 1);
    CHECK(Run(false, true, false) == true);           // fallback with the no-group filter
    CHECK(g_findCalls == 2 && g_filters[1] != 0 && g_filters[1] != &g_modeFilter);
    CHECK(g_names[1] == g_names[0] && g_tables[1] == &g_def.Animations);
    CHECK(g_def.RefCount == 1);
    CHECK(Run(false, false, false) == false);         // fallback misses too
    CHECK(g_findCalls == 2 && g_def.RefCount == 1 && g_subDefCalls == 4);
    g_def.RefCount = 0;                               // the holder owns the last reference: released -> Delete
    CHECK(Run(true, false, false) == true);
    CHECK(g_deleted == 1 && g_def.RefCount == 0);
    (void)VtableOf(&probe);
    if (fails == 0) printf("00830f20_TEST PASS\n");
    return fails;
}