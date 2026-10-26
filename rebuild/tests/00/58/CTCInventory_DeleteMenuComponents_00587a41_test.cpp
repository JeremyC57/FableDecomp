#include <stdio.h>
#include <string.h>
// CTCInventory::DeleteMenuComponents() -- tear down the open inventory screen: base clean-up, clear the edit boxes, close the screen-specific menu, then (if a screen was open) delete every menu component except the two persistent ones, keep only those two, reset the UI manager and the global list registry, and mark no screen open
// Leaf: DeleteMenuComponents
// Shape: case 3 fetches components 0x13 and 0x12 and discards them (`HasTC(t) ? FindTCInline(t) : 0` into unused locals):
//   only the two bitset::test calls survive, but through the thing pointer held in eax as retail (a bare discarded HasTC
//   loads ecx and adds 20h); the CVectorMap LowerBound instantiation is defined in this TU (noinline).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00587A41, 338 bytes, plain `ret`. Size TU with a frame pointer:
//   `#pragma optimize("y", off)` + `("s", on)` (`leave`, the switch as a `dec eax` chain).
// Callees: this-calls 0x00556470 and 0x00582414(0) (unnamed CTCInventory helpers), the edit-box list clear @ 0x00592BA7,
//   CTCInventory::ClosePCScoreboard @ 0x00584AF6 / ClosePCMapMenu @ 0x00584FE3 / ClosePCInventoryMenu @ 0x00584B42 (landed),
//   std::bitset<>::test @ 0x00410DE0 and CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x0040F020 (inlined
//   GetInterface, type 0xC6), the 0xC6 component's 0x0047DBB8(1), vector<CComponent*>::erase(first, last) @ 0x00485AA7,
//   push_back @ 0x00486024, NUISystem::CManager::GetInstance @ 0x0041E5F2 and its virtual slots 37 (+0x94) / 29 (+0x74),
//   the global list registry @ 0x013B8A98's clear @ 0x004855A3, the components' scalar deleting destructor (slot 0).
// Layout: retail CTCInventory (12-byte vectors): Components +0x0C, EditBoxes +0x144, the two persistent components
//   +0x214 / +0x218, the open screen +0x21C; retail CThing bitset +0x20, interface map +0x44.
#pragma optimize("y", off)
#pragma optimize("s", on)

class CTCBase;
struct CPresenceBits { bool test(long kind); };            // bitset<>::test 0x00410DE0
struct CInterfaceMap {                                      // CVectorMap<ETCInterfaceType, CTCBase*> at CThing+0x44
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);                   // 0x0040F020
};
#pragma auto_inline(off)
// Same-TU body of the CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound instantiation (a template member, compiled in the
// caller's TU): binary search for the first entry whose type is not below the key read through the pointer.
__declspec(noinline) CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{
    SEntry* it = first;
    long count = last - first;
    while (count > 0) {
        long half = count / 2;
        if (it[half].type < *type) {
            it += half + 1;
            count -= half + 1;
        } else
            count = half;
    }
    return it;
}
#pragma auto_inline(on)

class CThing {
public:
    unsigned char _pad00[0x20];
    CPresenceBits present;                                  // +0x20 std::bitset of interface types
    unsigned char _pad21[0x44 - 0x21];
    CInterfaceMap m_interfaces;                             // +0x44
    bool HasTC(long type) { return present.test(type); }
    // Inline component find (lower bound + not-found -> end), the shape of a fully-inlined GetTC: with its result unused
    // VC7.1 deletes the search but keeps the thing pointer it loaded for it (`mov eax,[esi+4]` / `lea ecx,[eax+20h]`).
    CTCBase* FindTCInline(long type)
    {
        CInterfaceMap::SEntry* it = m_interfaces.first;
        long count = m_interfaces.last - m_interfaces.first;
        while (count > 0) {
            long half = count / 2;
            if (it[half].type < type) { it += half + 1; count -= half + 1; } else count = half;
        }
        if (it == m_interfaces.last || type < it->type) it = m_interfaces.last;
        return it->component;
    }
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (present.test(interfaceType)) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
            return true;
        }
        return false;
    }
};
struct CTCInterfaceC6 {
    void Close(bool b);                                     // 0x0047DBB8
};

namespace NUISystem {
class CComponent { public:
    virtual ~CComponent();                                  // slot 0 (scalar deleting destructor)
};
class CManager { public:
    virtual void m00(); virtual void m01(); virtual void m02(); virtual void m03(); virtual void m04();
    virtual void m05(); virtual void m06(); virtual void m07(); virtual void m08(); virtual void m09();
    virtual void m10(); virtual void m11(); virtual void m12(); virtual void m13(); virtual void m14();
    virtual void m15(); virtual void m16(); virtual void m17(); virtual void m18(); virtual void m19();
    virtual void m20(); virtual void m21(); virtual void m22(); virtual void m23(); virtual void m24();
    virtual void m25(); virtual void m26(); virtual void m27(); virtual void m28();
    virtual void Reset_slot29();                            // slot 29 (+0x74)
    virtual void m30(); virtual void m31(); virtual void m32(); virtual void m33(); virtual void m34();
    virtual void m35(); virtual void m36();
    virtual void Clear_slot37();                            // slot 37 (+0x94)
    static CManager* __fastcall GetInstance();              // 0x0041E5F2
};
}
using NUISystem::CComponent;
using NUISystem::CManager;

struct CListRegistry { void clear(); };                     // 0x004855A3
extern CListRegistry g_ListRegistry;                        // 0x013B8A98

template <class T> struct CRetailVector {                   // STLport vector (12 bytes)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T* begin() { return _M_start; }
    T* end() { return _M_finish; }
    unsigned long size() const { return _M_finish - _M_start; }
    T& operator[](unsigned long n) const { return _M_start[n]; }
    T* erase(T* first, T* last);                            // 0x00485AA7
    void push_back(const T& x);                             // 0x00486024
    void clear() { erase(begin(), end()); }
};
struct CEditBoxList { void clear(); };                      // 0x00592BA7

class CTCInventory { public:
    void*         __vftable;                                // +0x00
    CThing*       ParentThing;                              // +0x04 CTCBase
    unsigned char _pad008[0x4];
    CRetailVector<CComponent*> Components;                  // +0x0C
    unsigned char _pad018[0x144 - 0x18];
    CEditBoxList  EditBoxes;                                // +0x144
    unsigned char _pad148[0x214 - 0x148];
    CComponent*   PBackground;                              // +0x214
    CComponent*   PMousePointer;                            // +0x218
    long          CurrentScreen;                            // +0x21C

    void BaseDeleteMenuComponents();                        // 0x00556470
    void ClearHighlights(long value);                       // 0x00582414
    void ClosePCScoreboard();                               // 0x00584AF6
    void ClosePCMapMenu();                                  // 0x00584FE3
    void ClosePCInventoryMenu();                            // 0x00584B42
    void Nothing() { }
    void DeleteMenuComponents();
};

void CTCInventory::DeleteMenuComponents()
{
    BaseDeleteMenuComponents();
    ClearHighlights(0);
    EditBoxes.clear();
    switch (CurrentScreen) {
    case 1:
        Nothing();
        break;
    case 3: {
        ClosePCInventoryMenu();
        CTCBase* p13 = ParentThing->HasTC(0x13) ? ParentThing->FindTCInline(0x13) : 0;
        CTCBase* p12 = ParentThing->HasTC(0x12) ? ParentThing->FindTCInline(0x12) : 0;
        break;
    }
    case 4: {
        CThing* thing = ParentThing;
        CTCInterfaceC6* menu;
        if (thing->GetInterface(&menu, 0xc6))
            menu->Close(true);
        break;
    }
    case 6:
        ClosePCMapMenu();
        break;
    case 8:
        ClosePCScoreboard();
        break;
    }
    if (CurrentScreen != 0) {
        for (unsigned long i = 0; i < Components.size(); ++i) {
            CComponent* component = Components[i];
            if (component != PBackground && component != PMousePointer)
                delete component;
        }
        Components.clear();
        Components.push_back(PBackground);
        Components.push_back(PMousePointer);
        CManager::GetInstance()->Clear_slot37();
        CManager::GetInstance()->Reset_slot29();
        g_ListRegistry.clear();
    }
    CurrentScreen = 0;
}

// ---- test stubs ----
static int fails = 0;
#define CHECK(c) do { if (!(c)) { printf("CHECK failed line %d: %s\n", __LINE__, #c); fails++; } } while (0)
static char g_log[512];
static void L(const char* s) { strcat(g_log, s); strcat(g_log, ";"); }
static long g_tests[8]; static int g_ntests; static void* g_testThis; static bool g_hasC6; static int g_deleted;
static void* g_closeThis; static int g_closeArg;
bool CPresenceBits::test(long kind) { if (g_ntests < 8) g_tests[g_ntests] = kind; g_ntests++; g_testThis = this; return kind == 0xc6 ? g_hasC6 : true; }
void CTCInterfaceC6::Close(bool b) { g_closeThis = this; g_closeArg = b; L("closeC6"); }
NUISystem::CComponent::~CComponent() { g_deleted++; }
static NUISystem::CManager g_mgr;
void NUISystem::CManager::m00() {} void NUISystem::CManager::m01() {} void NUISystem::CManager::m02() {} void NUISystem::CManager::m03() {} void NUISystem::CManager::m04() {}
void NUISystem::CManager::m05() {} void NUISystem::CManager::m06() {} void NUISystem::CManager::m07() {} void NUISystem::CManager::m08() {} void NUISystem::CManager::m09() {}
void NUISystem::CManager::m10() {} void NUISystem::CManager::m11() {} void NUISystem::CManager::m12() {} void NUISystem::CManager::m13() {} void NUISystem::CManager::m14() {}
void NUISystem::CManager::m15() {} void NUISystem::CManager::m16() {} void NUISystem::CManager::m17() {} void NUISystem::CManager::m18() {} void NUISystem::CManager::m19() {}
void NUISystem::CManager::m20() {} void NUISystem::CManager::m21() {} void NUISystem::CManager::m22() {} void NUISystem::CManager::m23() {} void NUISystem::CManager::m24() {}
void NUISystem::CManager::m25() {} void NUISystem::CManager::m26() {} void NUISystem::CManager::m27() {} void NUISystem::CManager::m28() {}
void NUISystem::CManager::Reset_slot29() { L("reset29"); }
void NUISystem::CManager::m30() {} void NUISystem::CManager::m31() {} void NUISystem::CManager::m32() {} void NUISystem::CManager::m33() {} void NUISystem::CManager::m34() {}
void NUISystem::CManager::m35() {} void NUISystem::CManager::m36() {}
void NUISystem::CManager::Clear_slot37() { L("clear37"); }
NUISystem::CManager* __fastcall NUISystem::CManager::GetInstance() { return &g_mgr; }
CListRegistry g_ListRegistry;
void CListRegistry::clear() { L("registry"); }
static CComponent* g_store[16];
template <class T> T* CRetailVector<T>::erase(T* first, T* last)
{
    T* out = first; for (T* p = last; p != _M_finish; ++p) *out++ = *p; _M_finish = out; L("erase"); return first;
}
template <class T> void CRetailVector<T>::push_back(const T& x) { *_M_finish++ = x; L("push"); }
void CEditBoxList::clear() { L("editclear"); }
void CTCInventory::BaseDeleteMenuComponents() { L("base"); }
void CTCInventory::ClearHighlights(long value) { L(value == 0 ? "hl0" : "hl?"); }
void CTCInventory::ClosePCScoreboard() { L("score"); }
void CTCInventory::ClosePCMapMenu() { L("map"); }
void CTCInventory::ClosePCInventoryMenu() { L("inv"); }
static void Reset() { g_log[0] = 0; g_ntests = 0; g_deleted = 0; g_closeThis = 0; g_closeArg = -1; g_testThis = 0; }
static unsigned char g_thingBuf[0x100], g_invBuf[sizeof(CTCInventory)];
int main()
{
    CThing* thing = (CThing*)g_thingBuf;
    static CTCInterfaceC6 c6;
    CInterfaceMap::SEntry e[] = { { 0x10, 0 }, { 0xc6, (CTCBase*)&c6 } };
    thing->m_interfaces.first = e; thing->m_interfaces.last = e + 2;
    CTCInventory* inv = (CTCInventory*)g_invBuf; inv->ParentThing = thing;
    CComponent* bg = new CComponent; CComponent* mouse = new CComponent;
    inv->PBackground = bg; inv->PMousePointer = mouse;
    inv->Components._M_start = g_store; inv->Components._M_end_of_storage = g_store + 16;
    // screen 3: close inventory menu, two discarded component tests (0x13, 0x12) on the thing's bitset; delete others
    g_store[0] = bg; g_store[1] = new CComponent; g_store[2] = mouse; g_store[3] = new CComponent; g_store[4] = 0;
    inv->Components._M_finish = g_store + 5; inv->CurrentScreen = 3; Reset();
    inv->DeleteMenuComponents();
    CHECK(strcmp(g_log, "base;hl0;editclear;inv;erase;push;push;clear37;reset29;registry;") == 0);
    CHECK(g_ntests == 2 && g_tests[0] == 0x13 && g_tests[1] == 0x12 && g_testThis == (char*)thing + 0x20);
    CHECK(g_deleted == 2 && inv->Components.size() == 2 && g_store[0] == bg && g_store[1] == mouse && inv->CurrentScreen == 0);
    // screen 4 with the 0xC6 component: Close(true) on it
    inv->CurrentScreen = 4; g_hasC6 = true; Reset();
    inv->DeleteMenuComponents();
    CHECK(strcmp(g_log, "base;hl0;editclear;closeC6;erase;push;push;clear37;reset29;registry;") == 0);
    CHECK(g_closeThis == &c6 && g_closeArg == 1 && g_ntests == 1 && g_tests[0] == 0xc6 && g_deleted == 0);
    // screen 4 without it
    inv->CurrentScreen = 4; g_hasC6 = false; Reset();
    inv->DeleteMenuComponents();
    CHECK(g_closeThis == 0 && strstr(g_log, "closeC6") == 0);
    // screens 6, 8, 1
    inv->CurrentScreen = 6; Reset(); inv->DeleteMenuComponents(); CHECK(strncmp(g_log, "base;hl0;editclear;map;erase;", 29) == 0);
    inv->CurrentScreen = 8; Reset(); inv->DeleteMenuComponents(); CHECK(strncmp(g_log, "base;hl0;editclear;score;erase;", 31) == 0);
    inv->CurrentScreen = 1; Reset(); inv->DeleteMenuComponents(); CHECK(strcmp(g_log, "base;hl0;editclear;erase;push;push;clear37;reset29;registry;") == 0 && g_ntests == 0);
    // no screen open: only the clean-up
    inv->CurrentScreen = 0; Reset(); inv->DeleteMenuComponents();
    CHECK(strcmp(g_log, "base;hl0;editclear;") == 0 && inv->CurrentScreen == 0);
    if (fails == 0) printf("00587a41_TEST PASS\n"); else printf("00587a41_TEST FAIL\n");
    return fails;
}