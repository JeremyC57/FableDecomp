// Grade: DIFFER(338v338) -- case 3's two discarded HasTC tests load the thing into eax and form `lea ecx,[eax+20h]` in retail; ours loads ecx directly and adds 20h (named/const-ref/accessor forms tried)
// CTCInventory::DeleteMenuComponents() -- tear down the open inventory screen: base clean-up, clear the edit boxes, close the screen-specific menu, then (if a screen was open) delete every menu component except the two persistent ones, keep only those two, reset the UI manager and the global list registry, and mark no screen open
// Leaf: DeleteMenuComponents
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
class CThing {
public:
    unsigned char _pad00[0x20];
    CPresenceBits present;                                  // +0x20 std::bitset of interface types
    unsigned char _pad21[0x44 - 0x21];
    CInterfaceMap m_interfaces;                             // +0x44
    bool HasTC(long type) { return present.test(type); }
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
struct CComponent {
    virtual ~CComponent();                                  // slot 0 (scalar deleting destructor)
};
struct CManager {
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

struct CTCInventory {
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
    case 3:
        ClosePCInventoryMenu();
        ParentThing->HasTC(0x13);
        ParentThing->HasTC(0x12);
        break;
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
