// Behaviour test for 00706370: src/00706370.cpp copied verbatim; RemoveAppearanceModifier erases every entry with the def
// index from the worn list of the modifier def's type, sets Changed and stamps FrameLastChanged; both defs are released.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CTCHeroAttachableAppearanceModifiers::RemoveAppearanceModifier(long defIndex) -- take every entry of a modifier def out of the worn list of its type, flag the change and stamp the frame
// Leaf: RemoveAppearanceModifier
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00706370, length 185 bytes. __fastcall, `ret 4`.
//         Ego_r.pdb: `?RemoveAppearanceModifier@CTCHeroAttachableAppearanceModifiers@@QAEXJ@Z`. Label: the catalog's
//         "AddAppearanceModifier" is its sibling (the body erases matching entries).
// Callees: CTCBase::GetDefinitions() const @ 0x006869D0, CDefinitionManager::GetDef<CThingObjectDef>(long, CDefPointer<const
//          T>*) const @ 0x0042B040, CDefClassBase::GetSubDef<CAppearanceModifierDef>(const T**) const @ 0x0057FF92,
//          vector<CModifierEntry>::erase(iterator) @ 0x00708600, CTCBase::GetWorld() const @ 0x00686A80,
//          CWorld::GetWorldFrame (static) @ 0x0049D870, the defs' virtual slot 1.
// Shape: the thing def is a CDefPointer local (inline dtor, re-read from its slot after the erase calls), the appearance
//        def a raw pointer that GetSubDef fills through `const T**` (released by hand before the thing def); GetDef and
//        GetSubDef are same-TU noinline stand-in bodies that write only the out slot, so both defs stay in registers
//        (esi, ebx) between calls; one ebx zero clears both out slots after the pushes and stands for the never-filled
//        appearance def on the first miss path; the erase loop re-reads end() after its first test; Changed is the low
//        bit of +0x14.
// Layout: local model -- engine/CTCHeroAttachableAppearanceModifiers.h carries the 16-byte PDB containers; retail
//         FrameLastChanged +0x10, Changed +0x14 bit 0, AppearanceModifiers +0x30; retail CAppearanceModifierDef type +0x50.
template<class T> class CDefPointer {
public:
    T* Object;
    CDefPointer() : Object(0) {}
    ~CDefPointer()
    {
        if (Object && --Object->RefCount == 0)
            Object->Destroy();
    }
    T* operator->() const { return Object; }
};

class CDefClassBase {
public:
    virtual void v00();
    virtual void Destroy() const;                   // slot 1
    mutable long RefCount;                          // +0x04
    template<class T> bool GetSubDef(const T** out) const;   // 0x0057FF92 for CAppearanceModifierDef
};
void* Find_CDefClassBase_GetSubDef(const void* owner);   // stand-in callee of the stand-in body below
// Stand-in body (the real one is out of line in retail): it writes the out slot only, so VC7.1 knows the def
// pointer's address does not escape the call (the landed 0x00848730 idiom).
#pragma auto_inline(off)
template <class T> __declspec(noinline) bool CDefClassBase::GetSubDef(const T** out) const
{
    void* def = Find_CDefClassBase_GetSubDef(this);
    if (!def)
        return false;
    *out = (const T*)def;
    return true;
}
#pragma auto_inline(on)
class CAppearanceModifierDef : public CDefClassBase {
public:
    unsigned char _pad08[0x50 - 0x08];
    long Type;                                      // +0x50 EHeroAttachableAppearanceModifierType
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

struct CWorld {
    static long GetWorldFrame();                    // 0x0049D870
};

struct CModifierEntry {                             // CTCHeroAttachableAppearanceModifiers::CModifierEntry (8 bytes)
    long DefIndex;                                  // +0x00 global def index
    long Data;                                      // +0x04
};

template <class T> struct CRetailVector {           // STLport vector (12 bytes)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T* erase(T* position);                          // 0x00708600 for CModifierEntry
};

class CThing;
class CTCHeroAttachableAppearanceModifiers {
public:
    virtual void v00();
    CThing* ParentThing;                            // +0x04 (CTCBase)
    unsigned char _pad08[0x10 - 0x08];
    unsigned long FrameLastChanged;                 // +0x10
    unsigned char Changed : 1;                      // +0x14 bit 0
    unsigned char RecreatePrimitives : 1;
    unsigned char AppearanceModifiersLoaded : 1;
    unsigned char _pad15[0x30 - 0x15];
    CRetailVector< CRetailVector<CModifierEntry> > AppearanceModifiers;   // +0x30
    const CDefinitionHolder* GetDefinitions() const;   // 0x006869D0 (CTCBase)
    CWorld* GetWorld() const;                       // 0x00686A80 (CTCBase)
    void RemoveAppearanceModifier(long defIndex);
};

void CTCHeroAttachableAppearanceModifiers::RemoveAppearanceModifier(long defIndex)
{
    CDefPointer<const CThingObjectDef> thingDef;
    const CAppearanceModifierDef* appearanceDef = 0;
    if (GetDefinitions()->GetDef(defIndex, &thingDef)) {
        if (thingDef->GetSubDef(&appearanceDef)) {
            CRetailVector<CModifierEntry>& entries = AppearanceModifiers._M_start[appearanceDef->Type];
            for (CModifierEntry* it = entries._M_start; it != entries._M_finish; ) {
                if (it->DefIndex == defIndex)
                    it = entries.erase(it);
                else
                    ++it;
            }
            Changed = true;
        }
    }
    FrameLastChanged = GetWorld()->GetWorldFrame();
    if (appearanceDef && --appearanceDef->RefCount == 0)
        appearanceDef->Destroy();
}
// ---- harness ----
static int g_destroyed = 0, g_erases = 0;
static const void* g_destroyedLast = 0;
static void* g_thingDef = 0; static void* g_appDef = 0; static long g_lookupIndex = -1;
static const void* g_lookupMgr = 0; static const void* g_findOwner = 0;
static CDefinitionHolder g_defs;
void CDefClassBase::v00() { }
void CDefClassBase::Destroy() const { ++g_destroyed; g_destroyedLast = this; }
void* Lookup_CDefinitionManager_GetDef(const void* manager, long index)
{ g_lookupMgr = manager; g_lookupIndex = index; if (g_thingDef) ++((CDefClassBase*)g_thingDef)->RefCount; return g_thingDef; }
void* Find_CDefClassBase_GetSubDef(const void* owner)
{ g_findOwner = owner; if (g_appDef) ++((CDefClassBase*)g_appDef)->RefCount; return g_appDef; }
long CWorld::GetWorldFrame() { return 777; }
CWorld* CTCHeroAttachableAppearanceModifiers::GetWorld() const { return 0; }
const CDefinitionHolder* CTCHeroAttachableAppearanceModifiers::GetDefinitions() const { return &g_defs; }
void CTCHeroAttachableAppearanceModifiers::v00() { }
template <> CModifierEntry* CRetailVector<CModifierEntry>::erase(CModifierEntry* position)
{ ++g_erases; memmove(position, position + 1, (char*)_M_finish - (char*)(position + 1)); --_M_finish; return position; }
static void reset() { g_destroyed = g_erases = 0; g_destroyedLast = 0; g_lookupIndex = -1; g_lookupMgr = 0; g_findOwner = 0; }
int main()
{
    CThingObjectDef thingDef; thingDef.RefCount = 1;
    CAppearanceModifierDef appDef; appDef.RefCount = 1; appDef.Type = 1;
    CModifierEntry list0[2] = { { 5, 0 }, { 6, 0 } };
    CModifierEntry list1[5] = { { 5, 1 }, { 9, 2 }, { 5, 3 }, { 5, 4 }, { 7, 5 } };
    CRetailVector<CModifierEntry> lists[2];
    lists[0]._M_start = list0; lists[0]._M_finish = list0 + 2; lists[0]._M_end_of_storage = list0 + 2;
    lists[1]._M_start = list1; lists[1]._M_finish = list1 + 5; lists[1]._M_end_of_storage = list1 + 5;
    static unsigned char buf[sizeof(CTCHeroAttachableAppearanceModifiers)];
    CTCHeroAttachableAppearanceModifiers& mods = *(CTCHeroAttachableAppearanceModifiers*)buf;
    mods.AppearanceModifiers._M_start = lists; mods.AppearanceModifiers._M_finish = lists + 2;
    unsigned char* flags = buf + 0x14;

    reset(); g_thingDef = &thingDef; g_appDef = &appDef; *flags = 0x06; mods.FrameLastChanged = 0;
    mods.RemoveAppearanceModifier(5);
    CHECK(g_lookupMgr == &g_defs && g_lookupIndex == 5 && g_findOwner == &thingDef);
    CHECK(g_erases == 3 && lists[1]._M_finish == list1 + 2 && list1[0].DefIndex == 9 && list1[1].DefIndex == 7);
    CHECK(lists[0]._M_finish == list0 + 2);
    CHECK(*flags == 0x07 && mods.FrameLastChanged == 777);
    CHECK(thingDef.RefCount == 1 && appDef.RefCount == 1 && g_destroyed == 0);

    // nothing matching: no erase, still flagged
    reset(); *flags = 0;
    mods.RemoveAppearanceModifier(42);
    CHECK(g_erases == 0 && *flags == 1 && lists[1]._M_finish == list1 + 2);

    // no sub-def: list untouched, not flagged, frame stamped
    reset(); g_appDef = 0; *flags = 0; mods.FrameLastChanged = 0;
    mods.RemoveAppearanceModifier(6);
    CHECK(g_erases == 0 && *flags == 0 && mods.FrameLastChanged == 777 && thingDef.RefCount == 1 && g_destroyed == 0);

    // no thing def
    reset(); g_thingDef = 0; g_appDef = &appDef; mods.FrameLastChanged = 0;
    mods.RemoveAppearanceModifier(6);
    CHECK(g_findOwner == 0 && g_erases == 0 && mods.FrameLastChanged == 777 && appDef.RefCount == 1);

    // last references: both destroyed, appearance def first
    reset(); g_thingDef = &thingDef; thingDef.RefCount = 0; appDef.RefCount = 0; appDef.Type = 0;
    mods.RemoveAppearanceModifier(6);
    CHECK(g_erases == 1 && lists[0]._M_finish == list0 + 1 && g_destroyed == 2 && g_destroyedLast == &thingDef);

    if (fails == 0) printf("00706370_TEST PASS\n");
    return fails;
}