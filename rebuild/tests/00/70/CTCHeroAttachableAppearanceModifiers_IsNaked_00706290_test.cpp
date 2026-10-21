// Behaviour test for 00706290: src/00706290.cpp copied verbatim; IsNaked returns false as soon as a worn type-2 entry's
// thing def has an appearance-modifier sub-def with at least one morph graphic, true otherwise; every def is released.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CTCHeroAttachableAppearanceModifiers::IsNaked() const -- true unless one of the worn type-2 appearance modifiers has an appearance-modifier sub-def with morph graphics
// Leaf: IsNaked
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00706290, 215 bytes (plain `ret`).
// Label: the catalog row says CTCShop::GetName (propagated); the body sits between CTCHeroAttachableAppearanceModifiers
// members (0x00706040 UpdateHiddenModifiers, 0x00706370 RemoveAppearanceModifier), reads that class's AppearanceModifiers
// vector-of-vectors at +0x30, and its only caller is CGameScriptInterface::IsHeroNaked (0x0089B330), which fetches the hero's
// interface 0x5e component and calls it. Ego_r.pdb has `?IsNaked@CTCHeroAttachableAppearanceModifiers@@QBE_NXZ` (const,
// bool, no arguments) right after UpdateHiddenMaterials in that class's member block, so the name is the PDB's.
// Callees: CTCBase::GetDefinitionManager @ 0x006869D0, CDefinitionManager::GetDef(long, CDefPointer<CThingObjectDef>&)
// @ 0x0042B040, CDefClassBase::GetSubDef<CAppearanceModifierDef>(CDefPointer<CAppearanceModifierDef>&) @ 0x0057FF92;
// the defs' virtual slot 1 (destroy on the last release).
// Layout (retail; the PDB's 16-byte containers are 12 here, as the landed IsWearingAppearanceModifier 0x00706000):
// AppearanceModifiers +0x30 (vector< vector<CModifierEntry{DefIndex, Rendered}> >, element 2 at +0x18); CAppearanceModifierDef
// MorphGraphics (CArray<CEntry>, 12-byte entries) data at +0x34/+0x38 (the CDefClassBase part keeps the PDB 0x28 bytes).
// Shape: both def pointers are zeroed together before the GetDef call; the three tests are one `&&` chain; each CDefPointer
// destructor is a null test plus the pointee's inline `if (--RefCount == 0) Destroy();`, duplicated on the early `return false`.
// GetDef and GetSubDef are same-TU noinline stand-in bodies that write only the out slot (the defs stay in esi/ecx and
// the worn-list pointer keeps ebx). The morph-graphics count is the CArray member's own size() (`this` = def + 0x34), so
// VC7.1 cannot use the dereference to drop the modifier's null test on the early-return path.
struct CDefBase {
    virtual void v00();
    virtual void Destroy();                             // slot 1
    long RefCount;                                      // +0x04
    void Release() { if (--RefCount == 0) Destroy(); }
};
template <class T> struct CDefPointer {                 // a counted def reference (released at scope end)
    T* Object;
    CDefPointer() : Object(0) { }
    ~CDefPointer() { if (Object) Object->Release(); }
    T* operator->() const { return Object; }
};
struct CDefClassBase : CDefBase {
    template <class T> bool GetSubDef(CDefPointer<T>& out) const;   // 0x0057FF92 (CAppearanceModifierDef instantiation)
};
void* Find_CDefClassBase_GetSubDef(const void* owner);   // stand-in callee of the stand-in body below
#pragma auto_inline(off)
template <class T> __declspec(noinline) bool CDefClassBase::GetSubDef(CDefPointer<T>& out) const
{
    void* def = Find_CDefClassBase_GetSubDef(this);
    if (!def)
        return false;
    out.Object = (T*)def;
    return true;
}
#pragma auto_inline(on)
struct CAppearanceModifierGraphicsEntry { unsigned char _data[0x18]; };
struct CAppearanceModifierDef : CDefClassBase {
    unsigned char _pad08[0x34 - 0x08];
    struct CGraphicsArray {                                  // MorphGraphics.Graphics (CArray, 12 bytes)
        CAppearanceModifierGraphicsEntry* _Myfirst;
        CAppearanceModifierGraphicsEntry* _Mylast;
        CAppearanceModifierGraphicsEntry* _Myend;
        long size() const { return _Mylast - _Myfirst; }
    } MorphGraphics;                                          // +0x34
    long GetNumMorphGraphics() const { return MorphGraphics.size(); }
};
struct CThingObjectDef : CDefClassBase { };
struct CDefinitionManager {
    bool GetDef(long globalIndex, CDefPointer<CThingObjectDef>& out);   // 0x0042B040
};
void* Lookup_CDefinitionManager_GetDef(const void* manager, long index);   // stand-in callee
#pragma auto_inline(off)
__declspec(noinline) bool CDefinitionManager::GetDef(long globalIndex, CDefPointer<CThingObjectDef>& out)
{
    void* def = Lookup_CDefinitionManager_GetDef(this, globalIndex);
    if (!def)
        return false;
    out.Object = (CThingObjectDef*)def;
    return true;
}
#pragma auto_inline(on)
struct CModifierEntry {
    long DefIndex;
    bool Rendered;
};
struct CModifierVector {
    CModifierEntry* _Myfirst;
    CModifierEntry* _Mylast;
    CModifierEntry* _Myend;
    CModifierEntry* begin() const { return _Myfirst; }
    CModifierEntry* end() const { return _Mylast; }
};
struct CModifierVectorVector {
    CModifierVector* _Myfirst;
    CModifierVector* _Mylast;
    CModifierVector* _Myend;
    const CModifierVector& operator[](long i) const { return _Myfirst[i]; }
};
struct CThing;
struct CTCBase {
    virtual void v00();
    CThing* ParentThing;                                // +0x04
    bool ToBeRemoved;                                   // +0x08
    bool IsInGlobalUpdateTCs;                           // +0x09
    unsigned char _pad0a[2];
    CDefinitionManager* GetDefinitionManager() const;   // 0x006869D0
};
struct CTCHeroAttachableAppearanceModifiers : CTCBase {
    void* __vftable_0c;                                 // +0x0c secondary vptr
    unsigned long FrameLastChanged;                     // +0x10
    unsigned char Flags14[4];                           // +0x14 Changed / RecreatePrimitives / AppearanceModifiersLoaded
    unsigned char PrimHandles[0xc];                     // +0x18 vector<NGraphicAppearance::CPrimitive*>
    unsigned char HiddenMaterialNames[0xc];             // +0x24 set<CCharString>
    CModifierVectorVector AppearanceModifiers;          // +0x30
    bool IsNaked() const;
};

bool CTCHeroAttachableAppearanceModifiers::IsNaked() const
{
    const CModifierVector& worn = AppearanceModifiers[2];
    for (CModifierEntry* e = worn.begin(); e != worn.end(); ++e) {
        CDefPointer<CThingObjectDef> def;
        CDefPointer<CAppearanceModifierDef> modifier;
        if (GetDefinitionManager()->GetDef(e->DefIndex, def) && def->GetSubDef(modifier) && modifier->GetNumMorphGraphics() > 0)
            return false;
    }
    return true;
}
// ---- harness ----
static int g_destroyed = 0, g_getDefs = 0;
static CThingObjectDef* g_things[8]; static CAppearanceModifierDef* g_mods[8];
static CDefinitionManager g_manager;
void CDefBase::v00() { }
void CDefBase::Destroy() { ++g_destroyed; }
void CTCBase::v00() { }
CDefinitionManager* CTCBase::GetDefinitionManager() const { return &g_manager; }
void* Lookup_CDefinitionManager_GetDef(const void* manager, long index)
{ CHECK(manager == &g_manager); ++g_getDefs; CThingObjectDef* d = g_things[index]; if (d) ++d->RefCount; return d; }
void* Find_CDefClassBase_GetSubDef(const void* owner)
{ for (int i = 0; i < 8; ++i) if (g_things[i] == owner) { CAppearanceModifierDef* m = g_mods[i]; if (m) ++m->RefCount; return m; }
  return 0; }
int main()
{
    static CThingObjectDef t1, t2, t3; t1.RefCount = t2.RefCount = t3.RefCount = 1;
    static CAppearanceModifierDef m2, m3; m2.RefCount = m3.RefCount = 1;
    CAppearanceModifierGraphicsEntry graphics[2];
    m2.MorphGraphics._Myfirst = m2.MorphGraphics._Mylast = graphics;           // none
    m3.MorphGraphics._Myfirst = graphics; m3.MorphGraphics._Mylast = graphics + 2;   // two
    g_things[1] = &t1; g_things[2] = &t2; g_things[3] = &t3; g_mods[2] = &m2; g_mods[3] = &m3;
    CModifierEntry entries[4] = { { 0, true }, { 1, true }, { 2, true }, { 3, true } };
    CModifierVector lists[3]; memset(lists, 0, sizeof lists);
    static unsigned char buf[sizeof(CTCHeroAttachableAppearanceModifiers)];
    CTCHeroAttachableAppearanceModifiers& mods = *(CTCHeroAttachableAppearanceModifiers*)buf;
    mods.AppearanceModifiers._Myfirst = lists;

    // entries 0 (no def), 1 (no sub-def), 2 (no morph graphics): nothing worn
    lists[2]._Myfirst = entries; lists[2]._Mylast = entries + 3;
    CHECK(mods.IsNaked() && g_getDefs == 3);
    CHECK(t1.RefCount == 1 && t2.RefCount == 1 && m2.RefCount == 1 && g_destroyed == 0);

    // entry 3 has morph graphics: not unclothed, released on the early return
    g_getDefs = 0; lists[2]._Mylast = entries + 4;
    CHECK(!mods.IsNaked() && g_getDefs == 4 && t3.RefCount == 1 && m3.RefCount == 1 && g_destroyed == 0);

    // the early return releases the last references (sub-def first, then the thing def)
    t3.RefCount = 0; m3.RefCount = 0;
    CHECK(!mods.IsNaked() && g_destroyed == 2);

    // nothing worn
    lists[2]._Mylast = entries; g_getDefs = 0;
    CHECK(mods.IsNaked() && g_getDefs == 0);

    if (fails == 0) printf("00706290_TEST PASS\n");
    return fails;
}