// Behaviour test for 00706880: src/00706880.cpp copied verbatim (the function body and its same-TU stand-ins), the remaining
// declared callees stubbed below so calls and arguments are observable. CTCHeroAttachableAppearanceModifiers::AddAppearanceModifier(long defIndex)
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CTCHeroAttachableAppearanceModifiers::AddAppearanceModifier(long defIndex) -- append a modifier def to the worn list of its appearance type, flag the change and stamp the frame
// Leaf: AddAppearanceModifier
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00706880, length 200 bytes (+ int3 padding). __fastcall, `ret 4`.
//         Ego_r.pdb: `?AddAppearanceModifier@CTCHeroAttachableAppearanceModifiers@@QAEXJ@Z`.
// Callees: CTCBase::GetDefinitions() const @ 0x006869D0, CDefinitionManager::GetDef<CThingObjectDef>(long, CDefPointer<const
//          T>*) const @ 0x0042B040, CDefClassBase::GetSubDef<CAppearanceModifierDef>(CDefPointer<const T>*) const @ 0x0057FF92,
//          vector<CModifierEntry>::_M_insert_overflow(pos, const T&, const __true_type&, size_type, bool) @ 0x00708E40,
//          CTCBase::GetWorld() const @ 0x00686A80, CWorld::GetWorldFrame (static) @ 0x0049D870, the defs' virtual slot 1.
// Shape: two plain def out slots (thing first, then appearance: the zero stores go out in that order), the lookups
//        chained in one condition (GetDef && GetSubDef) and both defs released by hand through the slots; the entry
//        {defIndex, true} is a temporary built before the inline STLport push_back (placement copy when there is room,
//        else _M_insert_overflow with the empty tag homed in the dead argument slot); one ebx 1 serves the flag byte,
//        both overflow constants and `Changed |= 1`. Same-TU stand-in bodies above the function for GetDef / GetSubDef
//        (COMDAT template instances) and _M_insert_overflow (0x00708E40): with declarations only VC7.1 gives the entry
//        temporary its own frame word instead of the dead thing-def slot, and the registers rotate.
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

template<class T> class CDefOutPointer {           // a plain out slot (the def is released by hand)
public:
    T* Object;
    CDefOutPointer() : Object(0) {}
};

class CDefClassBase {
public:
    virtual void v00();
    virtual void Destroy() const;                   // slot 1
    mutable long RefCount;                          // +0x04
    template<class T> bool GetSubDef(CDefOutPointer<const T>* out) const;   // 0x0057FF92 for CAppearanceModifierDef
};
class CAppearanceModifierDef : public CDefClassBase {
public:
    unsigned char _pad08[0x50 - 0x08];
    long Type;                                      // +0x50 EHeroAttachableAppearanceModifierType
};
class CThingObjectDef : public CDefClassBase { };

class CDefinitionManager {
public:
    template<class T> bool GetDef(long index, CDefOutPointer<const T>* out) const;   // 0x0042B040 for CThingObjectDef
};
class CDefinitionHolder : public CDefinitionManager { };

struct CWorld {
    static long GetWorldFrame();                    // 0x0049D870
};

struct CModifierEntry {                             // CTCHeroAttachableAppearanceModifiers::CModifierEntry (8 bytes)
    long DefIndex;                                  // +0x00 global def index
    bool Active;                                    // +0x04
    CModifierEntry(long defIndex, bool active) : DefIndex(defIndex), Active(active) {}
};

struct __true_type { __true_type() {} };           // empty tag with a user constructor (no byte store)
inline void* operator new(unsigned int, void* where) { return where; }

template <class T> struct CRetailVector {           // STLport vector (12 bytes)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    void _M_insert_overflow(T* position, const T& x, const __true_type&, unsigned long fill_len, bool atend);   // 0x00708E40
    void push_back(const T& x)
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) T(x);
            ++_M_finish;
        }
        else
            _M_insert_overflow(_M_finish, x, __true_type(), 1, true);
    }
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
    void AddAppearanceModifier(long defIndex);
};

void* AllocateStorage_00708e40(unsigned long bytes);
#pragma auto_inline(off)
template <class T>
__declspec(noinline) void CRetailVector<T>::_M_insert_overflow(T* position, const T& x, const __true_type&, unsigned long fill_len, bool atend)
{
    unsigned long oldSize = (unsigned long)(_M_finish - _M_start);
    unsigned long len = oldSize + (oldSize > fill_len ? oldSize : fill_len);
    T* newStart = (T*)AllocateStorage_00708e40(len * sizeof(T));
    T* out = newStart;
    for (T* it = _M_start; it != position; ++it, ++out)
        new (out) T(*it);
    for (unsigned long i = 0; i < fill_len; ++i, ++out)
        new (out) T(x);
    _M_start = newStart;
    _M_finish = out;
    _M_end_of_storage = newStart + len;
}
#pragma auto_inline(on)
void* FindDef_0042b040(long index);
void* FindSubDef_0057ff92(const void* def);
#pragma auto_inline(off)
template<class T> __declspec(noinline) bool CDefinitionManager::GetDef(long index, CDefOutPointer<const T>* out) const
{
    T* def = (T*)FindDef_0042b040(index);
    if (!def)
        return false;
    ++def->RefCount;
    out->Object = def;
    return true;
}
template<class T> __declspec(noinline) bool CDefClassBase::GetSubDef(CDefOutPointer<const T>* out) const
{
    T* def = (T*)FindSubDef_0057ff92(this);
    if (!def)
        return false;
    ++def->RefCount;
    out->Object = def;
    return true;
}
#pragma auto_inline(on)
void CTCHeroAttachableAppearanceModifiers::AddAppearanceModifier(long defIndex)
{
    CDefOutPointer<const CThingObjectDef> thingOut;
    CDefOutPointer<const CAppearanceModifierDef> appearanceOut;
    if (GetDefinitions()->GetDef(defIndex, &thingOut) && thingOut.Object->GetSubDef(&appearanceOut)) {
        AppearanceModifiers._M_start[appearanceOut.Object->Type].push_back(CModifierEntry(defIndex, true));
        Changed = true;
    }
    FrameLastChanged = GetWorld()->GetWorldFrame();
    if (appearanceOut.Object && --appearanceOut.Object->RefCount == 0)
        appearanceOut.Object->Destroy();
    if (thingOut.Object && --thingOut.Object->RefCount == 0)
        thingOut.Object->Destroy();
}

// ---- stubs and driver ----
void CDefClassBase::v00() {}
void CTCHeroAttachableAppearanceModifiers::v00() {}
#include <stdlib.h>
static int g_destroyed = 0; static const CDefClassBase* g_destroyedDef[4];
void CDefClassBase::Destroy() const { g_destroyedDef[g_destroyed++ & 3] = this; }
static CThingObjectDef g_thingDef; static CAppearanceModifierDef g_appDef;
static bool g_haveThing = true, g_haveSub = true; static long g_findIndex;
void* FindDef_0042b040(long index) { g_findIndex = index; return g_haveThing ? &g_thingDef : 0; }
static const void* g_subOf;
void* FindSubDef_0057ff92(const void* def) { g_subOf = def; return g_haveSub ? &g_appDef : 0; }
static int g_allocs = 0;
void* AllocateStorage_00708e40(unsigned long bytes) { ++g_allocs; return malloc(bytes); }
static CDefinitionHolder g_defs;
const CDefinitionHolder* CTCHeroAttachableAppearanceModifiers::GetDefinitions() const { return &g_defs; }
static CWorld g_world;
CWorld* CTCHeroAttachableAppearanceModifiers::GetWorld() const { return &g_world; }
static long g_frame = 100;
long CWorld::GetWorldFrame() { return g_frame; }
int main()
{
    CRetailVector<CModifierEntry> lists[3];
    for (int i = 0; i < 3; ++i) lists[i]._M_start = lists[i]._M_finish = lists[i]._M_end_of_storage = 0;
    static unsigned char store[sizeof(CTCHeroAttachableAppearanceModifiers)];
    CTCHeroAttachableAppearanceModifiers& m = *(CTCHeroAttachableAppearanceModifiers*)store;
    m.AppearanceModifiers._M_start = lists; m.FrameLastChanged = 0; m.Changed = 0;
    g_thingDef.RefCount = 1; g_appDef.RefCount = 1; g_appDef.Type = 2;
    m.AddAppearanceModifier(77);
    CHECK(g_findIndex == 77); CHECK(g_subOf == &g_thingDef);
    CHECK(lists[2]._M_finish - lists[2]._M_start == 1); CHECK(lists[2]._M_start[0].DefIndex == 77); CHECK(lists[2]._M_start[0].Active == true);
    CHECK(m.Changed == 1); CHECK(m.FrameLastChanged == 100); CHECK(g_allocs == 1);
    CHECK(g_thingDef.RefCount == 1); CHECK(g_appDef.RefCount == 1); CHECK(g_destroyed == 0);
    // a second one with room: the placement copy path
    lists[2]._M_end_of_storage = lists[2]._M_start + 1;
    m.AddAppearanceModifier(78);
    CHECK(lists[2]._M_finish - lists[2]._M_start == 2); CHECK(lists[2]._M_start[1].DefIndex == 78); CHECK(g_allocs == 2);
    // no sub def: nothing added, the frame still stamped, the thing def released
    g_haveSub = false; g_frame = 200; m.Changed = 0;
    m.AddAppearanceModifier(79);
    CHECK(lists[2]._M_finish - lists[2]._M_start == 2); CHECK(m.Changed == 0); CHECK(m.FrameLastChanged == 200);
    CHECK(g_thingDef.RefCount == 1);
    // no def at all
    g_haveThing = false; g_frame = 300;
    m.AddAppearanceModifier(80);
    CHECK(m.FrameLastChanged == 300); CHECK(g_destroyed == 0);
    // the last reference: released defs are destroyed
    g_haveThing = true; g_haveSub = true; g_thingDef.RefCount = 0; g_appDef.RefCount = 0;
    m.AddAppearanceModifier(81);
    CHECK(g_destroyed == 2); CHECK(g_destroyedDef[0] == &g_appDef); CHECK(g_destroyedDef[1] == &g_thingDef);
    if (fails == 0) printf("00706880_TEST PASS\n"); else printf("00706880_TEST FAIL\n");
    return fails;
}