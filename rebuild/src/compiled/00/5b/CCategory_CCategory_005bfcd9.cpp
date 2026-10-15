// CCategory::CCategory(CDefPointer def, CTCInventoryBase* inventory, long globalDefIndex) -- build an inventory category: no associated thing, an empty item vector sized to the def's NumberOfSlots, the def (shared), the owning inventory, its def index, no selection and the highlight on slot 0
// Leaf: ??0CCategory_Retail
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005BFCD9, length 133 bytes (ret 0Ch); a frame-pointer size TU
// (`leave`, `or dword ptr [esi+20h],-1`) = `#pragma optimize("y", off)` + `("s", on)`. Callers 0x005C01CB, 0x005C0287.
// Callees: CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x00A01B10 (PAssociatedThing's base; vtable
// CIntelligentPointer 0x01238C6C), the item vector's resize(n) @ 0x005C2AA9 (manifest label
// "CTCInventoryStats::OnSerialise" is propagated), CIntelligentPointer<CThing>::CIntelligentPointer(CThing*)
// @ 0x004A96F0, CIntelligentPointer::operator= @ 0x004A9760, CBaseIntelligentPointer::~CBaseIntelligentPointer
// @ 0x00A01C10, CInventoryCategoryDef virtual slot 1 (+0x04, destroy).
// Layout: retail CCategory (0x2C bytes; engine/CCategory.h is the PDB layout with a 16-byte vector, every member
// after it sits 4 bytes lower in retail -- see the landed CCategory sources): PAssociatedThing +0x00,
// ItemsInCategory +0x08, PDef +0x14, PInventory +0x18, GlobalDefIndex +0x1c, SelectedItemIndex +0x20,
// HighlightedItemIndex +0x24, Selected +0x28.
// Shape: the def arrives as a by-value CDefPointer (the def pointer in edi): the member copy bumps its count and the
// parameter's destructor releases it at the very end; NumberOfSlots (+0x40) is read straight through the stored
// member (no scoped reference); `PAssociatedThing = (CThing*)0` builds the 8-byte conversion temporary in
// the frame (an IMPLICIT conversion: its address is re-formed with `lea` after the constructor, an explicit
// temporary reuses the constructor's eax), assigns it out of line and runs its inline destructor's out-of-line base part.
#pragma optimize("y", off)
#pragma optimize("s", on)

struct CThing;
struct CInventoryCategoryDef_Retail {         // the def behind the retail CDefPointer (count at +4, slot 1 destroys)
    virtual void v00();
    virtual void Destroy();                   // slot 1 (+0x04)
    long          m_refCount;                 // +0x04
    unsigned char _pad_0x08[0x38];            // +0x08
    long          NumberOfSlots;              // +0x40
};
class CDefPointer {                           // CDefPointer<const CInventoryCategoryDef>
public:
    CInventoryCategoryDef_Retail* Object;
    __forceinline CDefPointer(const CDefPointer& other) : Object(other.Object)
    {
        if (Object)
            ++Object->m_refCount;
    }
    __forceinline ~CDefPointer()
    {
        if (Object) {
            if (--Object->m_refCount == 0)
                Object->Destroy();
        }
    }
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                          // +0x04
    CBaseIntelligentPointer();                // 0x00A01B10
    ~CBaseIntelligentPointer();               // 0x00A01C10
};
struct CIntelligentPointerThing : CBaseIntelligentPointer {   // CIntelligentPointer<CThing>, vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointerThing() : CBaseIntelligentPointer() { }
    CIntelligentPointerThing(CThing* thing);  // 0x004A96F0
    CIntelligentPointerThing& operator=(const CIntelligentPointerThing& other);   // 0x004A9760
};
struct CItem;
struct CItemVector {                          // retail std::vector<NInventory::CItem>, 12 bytes
    CItem* first;                             // +0x00
    CItem* last;                              // +0x04
    CItem* end_of_storage;                    // +0x08
    CItemVector() : first(0), last(0), end_of_storage(0) { }
    void resize(unsigned long count);         // 0x005C2AA9
};
struct CTCInventoryBase;

class CCategory_Retail {
public:
    CIntelligentPointerThing PAssociatedThing;   // +0x00
    CItemVector       ItemsInCategory;        // +0x08
    CDefPointer       PDef;                   // +0x14
    CTCInventoryBase* PInventory;             // +0x18
    long              GlobalDefIndex;         // +0x1c
    long              SelectedItemIndex;      // +0x20
    long              HighlightedItemIndex;   // +0x24
    bool              Selected;               // +0x28
    bool              DisplayItems;           // +0x29
    CCategory_Retail(CDefPointer def, CTCInventoryBase* inventory, long globalDefIndex);
};

CCategory_Retail::CCategory_Retail(CDefPointer def, CTCInventoryBase* inventory, long globalDefIndex)
    : PAssociatedThing(), ItemsInCategory(), PDef(def), PInventory(inventory), GlobalDefIndex(globalDefIndex),
      SelectedItemIndex(-1), HighlightedItemIndex(0), Selected(false)
{
    ItemsInCategory.resize(PDef.Object->NumberOfSlots);
    PAssociatedThing = (CThing*)0;
}