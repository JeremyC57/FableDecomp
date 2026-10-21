// Behaviour test for 00706450: src/00706450.cpp copied verbatim; true when a worn modifier of the type has an appearance
// def whose CoversBodyAreaFlags intersect the asked flags; every def reference taken is released.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CTCHeroAttachableAppearanceModifiers::IsWearingAppearanceModifierCoveringBodyArea(long bodyAreaFlags, EHeroAttachableAppearanceModifierType type) const -- whether any worn modifier of the type has an appearance def covering one of the body areas
// Leaf: IsWearingAppearanceModifierCoveringBodyArea
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00706450, length 185 bytes (+ int3 padding). __fastcall, `ret 8`.
//         Ego_r.pdb: `?IsWearingAppearanceModifierCoveringBodyArea@CTCHeroAttachableAppearanceModifiers@@QBE_NJW4EHero
//         AttachableAppearanceModifierType@@@Z`. Label: the catalog's "CTCGold::GetGold" is propagated; the row sits in the
//         appearance-modifier TU (0x00706370 / 0x00706880 AddAppearanceModifier) and its caller (CTCHeroStats, 0x0057D462)
//         passes (def +0x40 flags, 2).
// Callees: CTCBase::GetDefinitions() const @ 0x006869D0, CDefinitionManager::GetDef<CThingObjectDef>(long, CDefPointer<const
//          T>*) const @ 0x0042B040, CDefClassBase::GetSubDef<CAppearanceModifierDef>(CDefPointer<const T>*) const @
//          0x0057FF92, the defs' virtual slot 1 (destroy on the last release).
// Shape: a pointer walk over AppearanceModifiers[type] (end re-read each pass); per entry two CDefPointer locals (the thing
//        def in the dead `type` slot, the appearance def in the push-ecx word) tested in one `&&` chain, their inline
//        destructors (`dec [reg+4] ; jne`) duplicated on the `return true` path (the appearance def's null test folded away
//        after the flag read). GetDef and GetSubDef are same-TU noinline stand-in bodies that write only the out slot, so
//        the defs are held in registers and eax is re-zeroed each pass instead of a hoisted ebx zero.
// Layout: local model -- engine/CTCHeroAttachableAppearanceModifiers.h carries the 16-byte PDB containers; retail
//         AppearanceModifiers +0x30 (12-byte vector of 12-byte vectors of 8-byte CModifierEntry {def index, ...});
//         retail CAppearanceModifierDef::CoversBodyAreaFlags +0x40 (PDB +0x48).
template<class T> class CDefPointer {
public:
    T* Object;
    CDefPointer() : Object(0) {}
    ~CDefPointer() { if (Object && --Object->RefCount == 0) Object->Destroy(); }
    T* operator->() const { return Object; }
};

class CDefClassBase {
public:
    virtual void v00();
    virtual void Destroy() const;                   // slot 1
    mutable long RefCount;                          // +0x04
    template<class T> bool GetSubDef(CDefPointer<const T>* out) const;   // 0x0057FF92 for CAppearanceModifierDef
};
void* Find_CDefClassBase_GetSubDef(const void* owner);   // stand-in callee of the stand-in body below
#pragma auto_inline(off)
template <class T> __declspec(noinline) bool CDefClassBase::GetSubDef(CDefPointer<const T>* out) const
{
    void* def = Find_CDefClassBase_GetSubDef(this);
    if (!def)
        return false;
    out->Object = (const T*)def;
    return true;
}
#pragma auto_inline(on)

class CAppearanceModifierDef : public CDefClassBase {
public:
    unsigned char _pad08[0x40 - 0x08];
    long CoversBodyAreaFlags;                       // +0x40
};
class CThingObjectDef : public CDefClassBase { };

class CDefinitionManager {
public:
    template<class T> bool GetDef(long index, CDefPointer<const T>* out) const;   // 0x0042B040 for CThingObjectDef
};
void* Lookup_CDefinitionManager_GetDef(const void* manager, long index);   // stand-in callee
#pragma auto_inline(off)
template <class T> __declspec(noinline) bool CDefinitionManager::GetDef(long index, CDefPointer<const T>* out) const
{
    void* def = Lookup_CDefinitionManager_GetDef(this, index);
    if (!def)
        return false;
    out->Object = (const T*)def;
    return true;
}
#pragma auto_inline(on)
class CDefinitionHolder : public CDefinitionManager { };

template <class T> struct CRetailVector {           // STLport vector (12 bytes)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
};

struct CModifierEntry {                             // CTCHeroAttachableAppearanceModifiers::CModifierEntry (8 bytes)
    long DefIndex;                                  // +0x00 global def index
    long Data;                                      // +0x04
};

class CThing;
class CTCHeroAttachableAppearanceModifiers {
public:
    virtual void v00();
    CThing* ParentThing;                            // +0x04 (CTCBase)
    unsigned char _pad08[0x30 - 0x08];
    CRetailVector< CRetailVector<CModifierEntry> > AppearanceModifiers;   // +0x30
    const CDefinitionHolder* GetDefinitions() const;   // 0x006869D0 (CTCBase)
    bool IsWearingAppearanceModifierCoveringBodyArea(long bodyAreaFlags, long type) const;
};

bool CTCHeroAttachableAppearanceModifiers::IsWearingAppearanceModifierCoveringBodyArea(long bodyAreaFlags, long type) const
{
    const CRetailVector<CModifierEntry>& entries = AppearanceModifiers._M_start[type];
    for (const CModifierEntry* it = entries._M_start; it != entries._M_finish; ++it) {
        CDefPointer<const CThingObjectDef> thingDef;
        CDefPointer<const CAppearanceModifierDef> appearanceDef;
        if (GetDefinitions()->GetDef(it->DefIndex, &thingDef) && thingDef->GetSubDef(&appearanceDef) && (appearanceDef->CoversBodyAreaFlags & bodyAreaFlags))
            return true;
    }
    return false;
}
// ---- harness ----
static int g_destroyed = 0, g_getDefs = 0;
static CThingObjectDef* g_things[8]; static CAppearanceModifierDef* g_mods[8];
static CDefinitionHolder g_defs;
void CDefClassBase::v00() { }
void CDefClassBase::Destroy() const { ++g_destroyed; }
void CTCHeroAttachableAppearanceModifiers::v00() { }
const CDefinitionHolder* CTCHeroAttachableAppearanceModifiers::GetDefinitions() const { return &g_defs; }
void* Lookup_CDefinitionManager_GetDef(const void* manager, long index)
{ CHECK(manager == &g_defs); ++g_getDefs; CThingObjectDef* d = g_things[index]; if (d) ++d->RefCount; return d; }
void* Find_CDefClassBase_GetSubDef(const void* owner)
{ for (int i = 0; i < 8; ++i) if (g_things[i] == owner) { CAppearanceModifierDef* m = g_mods[i]; if (m) ++m->RefCount; return m; }
  return 0; }
int main()
{
    static CThingObjectDef t1, t2, t3; t1.RefCount = t2.RefCount = t3.RefCount = 1;
    static CAppearanceModifierDef m2, m3; m2.RefCount = m3.RefCount = 1;
    m2.CoversBodyAreaFlags = 0x1; m3.CoversBodyAreaFlags = 0x6;
    g_things[1] = &t1; g_things[2] = &t2; g_things[3] = &t3; g_mods[2] = &m2; g_mods[3] = &m3;
    CModifierEntry entries[4] = { { 0, 0 }, { 1, 0 }, { 2, 0 }, { 3, 0 } };
    CRetailVector<CModifierEntry> lists[3]; memset(lists, 0, sizeof lists);
    lists[1]._M_start = entries; lists[1]._M_finish = entries + 4;
    static unsigned char buf[sizeof(CTCHeroAttachableAppearanceModifiers)];
    CTCHeroAttachableAppearanceModifiers& mods = *(CTCHeroAttachableAppearanceModifiers*)buf;
    mods.AppearanceModifiers._M_start = lists;

    CHECK(mods.IsWearingAppearanceModifierCoveringBodyArea(0x4, 1) && g_getDefs == 4);
    CHECK(t1.RefCount == 1 && t2.RefCount == 1 && t3.RefCount == 1 && m2.RefCount == 1 && m3.RefCount == 1 && g_destroyed == 0);
    g_getDefs = 0;
    CHECK(mods.IsWearingAppearanceModifierCoveringBodyArea(0x1, 1) && g_getDefs == 3);
    g_getDefs = 0;
    CHECK(!mods.IsWearingAppearanceModifierCoveringBodyArea(0x8, 1) && g_getDefs == 4 && g_destroyed == 0);
    CHECK(!mods.IsWearingAppearanceModifierCoveringBodyArea(0x4, 2) && !mods.IsWearingAppearanceModifierCoveringBodyArea(0x4, 0));

    // the hit path releases the last references
    t3.RefCount = 0; m3.RefCount = 0;
    CHECK(mods.IsWearingAppearanceModifierCoveringBodyArea(0x2, 1) && g_destroyed == 2);

    if (fails == 0) printf("00706450_TEST PASS\n");
    return fails;
}