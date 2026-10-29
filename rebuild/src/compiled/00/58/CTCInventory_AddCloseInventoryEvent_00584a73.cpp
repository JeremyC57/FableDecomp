// CTCInventory::AddCloseInventoryEvent() -- an active, not yet closing interface-0xF6 menu is asked to close itself; otherwise the base queues the close-inventory game event; then the live GUI is toggled back if it was hidden
// Leaf: AddCloseInventoryEvent
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00584A73, 131 bytes; frame-pointer size TU
// (`#pragma optimize("y", off)` + `optimize("s", on)`).
// Callees: std::bitset::test @ 0x00410DE0, CVectorMap::LowerBound @ 0x0040F020, the menu component's IsActive()
//          @ 0x00625930 (+0x0C), IsClosing() @ 0x00625940 (+0x1C) and RequestClose() @ 0x00625920 (sets +0x1C when
//          active; manifest labels "GetCanItemBeQuickAssigned" / "CTCCreditsUI::Deactivate" are propagated),
//          NInventory::CTCInventoryBase::AddCloseInventoryEvent() @ 0x005BC527 (qualified base call),
//          CPlayerGui::ToggleLiveGui() @ 0x00438310 on the GUI global @ 0x013B8790 (when its +0x2BE LiveGuiMode flag, PDB +0x2CE is set).
// Shape: a `__forceinline` CThing::GetTC<T>(k) doing `T* c = 0; if (HasTC(k)) c = GetTCUnchecked<T>(k); return c;`
// reads ParentThing once (esi) and gives the preloaded `xor ebx,ebx` and the
// interface type held in edi; `this` is spilled to [ebp-8] for the base call, the lookup key takes [ebp-4]; the
// component is used without a null test.
// Layout: CTCBase ParentThing +0x04; retail CThing interface bitset +0x20, map +0x44.
// Layout (supersedes any local-model note above): engine/CPlayerGui.h now has the retail STLport 4.6 offsets, so the data
//   members come from the shared header; the class below keeps only the declarations this body needs.
#define CPlayerGui CPlayerGuiDonor
#include "engine/CPlayerGui.h"
#undef CPlayerGui
#pragma optimize("y", off)
#pragma optimize("s", on)

class CTCBase;

struct CPresenceBits {
    bool test(long bit);                             // 0x00410DE0 (std::bitset::test)
};

enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;                                   // +0x00
    SEntry* last;                                    // +0x04
    SEntry* LowerBound(const ETCInterfaceType& type);            // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CThing { public:
    unsigned char _pad_0x00[0x20];
    CPresenceBits m_present;                         // +0x20
    unsigned char _pad_0x21[0x23];
    CInterfaceMap m_interfaces;                      // +0x44
    bool HasTC(long wanted) { return m_present.test(wanted); }
    template<class T> __forceinline T* GetTCUnchecked(long wanted)
    {
        ETCInterfaceType type = (ETCInterfaceType)(wanted);
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(type);
        if (it == map->last || wanted < it->first)
            it = map->last;
        return (T*)it->second;
    }
    template<class T> __forceinline T* GetTC(long wanted)
    {
        T* component = 0;
        if (HasTC(wanted))
            component = GetTCUnchecked<T>(wanted);
        return component;
    }
};

class CTCQuickAccessMenu { public:                            // interface type 0xF6
    bool IsActive() const;                           // 0x00625930
    bool IsClosing() const;                          // 0x00625940
    void RequestClose();                             // 0x00625920
};

class CPlayerGui : public CPlayerGuiDonor {
 public:
    void ToggleLiveGui();                            // 0x00438310
};

extern CPlayerGui* g_pPlayerGui;                     // 0x013B8790

struct CTCInventoryBase {
    void*   __vftable;                               // +0x00
    CThing* ParentThing;                             // +0x04 CTCBase
    void AddCloseInventoryEvent();                   // 0x005BC527
};

class CTCInventory : public CTCInventoryBase { public:
    void AddCloseInventoryEvent();
};

void CTCInventory::AddCloseInventoryEvent()
{
    CTCQuickAccessMenu* menu = ParentThing->GetTC<CTCQuickAccessMenu>(0xF6);
    if (menu->IsActive() && !menu->IsClosing())
        menu->RequestClose();
    else
        CTCInventoryBase::AddCloseInventoryEvent();
    if (g_pPlayerGui->LiveGuiMode)
        g_pPlayerGui->ToggleLiveGui();
}