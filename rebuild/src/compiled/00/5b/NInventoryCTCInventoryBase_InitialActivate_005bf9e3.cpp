// NInventory::CTCInventoryBase::InitialActivate() -- consumes the items and selected-item names restored from a save: re-adds the items, re-selects each named item, then drops both serialised counted pointers
// Leaf: ?InitialActivate@CTCInventoryBase@NInventory@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005BF9E3, length 97 bytes. Size TU: `#pragma optimize("s", on)` (`and dword ptr [m],0`).
// Callees: NInventory::CTCInventoryBase::InitialiseInventoryWithItems(const vector<CItem>&) @ 0x005BF8CB,
// CCountedPointer<vector<NInventory::CItem>>::ReleaseData @ 0x005C0511 and CCountedPointer<vector<CCharString>>::ReleaseData @ 0x005C0563
// (manifest "_Dest_val<...COptimisedPrimitive>", byte twins), CTCBase::GetDefinitionManager const @ 0x006869D0,
// CDefinitionManager::GetDefGlobalIndexFromName(const CCharString&) @ 0x009AD410, CTCInventoryBase::SelectItemWithDefIndex @ 0x005BDFB1.
// Shape: each counted pointer is cleared with an inline `Reset(0)` (out-of-line ReleaseData + one zero store of Data);
// the name vector is re-read through the counted pointer every iteration (the calls may touch it).
// Layout: retail CTCInventoryBase SerialisedItems +0x78, SerialisedSelectedItems +0x80 (shared engine/CCountedPointer.h {Data, Info};
// Ego_r.pdb CCountedPointer<vector<NInventory::CItem>> / CCountedPointer<vector<CCharString>>); CTCBase from engine/CTCBase.h.
// Name: the class is NInventory::CTCInventoryBase (RTTI vtable 0x01253DBC .?AVCTCInventoryBase@NInventory@@, slot 5);
// Ego_r.exe ?InitialActivate@CTCInventoryBase@NInventory@@UAEXXZ 0x00622A5A (CTCBase slot 5 = InitialActivate).
#pragma optimize("s", on)
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA_DECL_ONLY
#include "engine/ETCInterfaceType.h"
#include "engine/CTCBase.h"
#include "engine/CCountedPointer.h"
#include "stlport/fable_containers.h"
#include "engine/CItem.h"
class CCharString { public: void* m_pData; };
class CDefinitionManager { public:
    long GetDefGlobalIndexFromName(const CCharString&) const;   // 0x009AD410
};
class CGameDefinitionManager : public CDefinitionManager { public: };   // RTTI CGameDefinitionManager : CDefinitionManager
namespace NInventory {
class CItem : public ::CItem { };                         // NInventory::CItem (Ego_r.pdb) over the shared engine/CItem.h layout

class CTCInventoryBase : public CTCBase {
public:
    virtual void InitialActivate();                               // CTCBase slot 5 (+0x14)
    unsigned char _pad_0x0c[0x78 - 0x0c];
    CCountedPointer<std::vector<CItem> > SerialisedItems;         // +0x78
    CCountedPointer<std::vector<CCharString> > SerialisedSelectedItems;   // +0x80
protected:
    void SelectItemWithDefIndex(long defIndex);                   // 0x005BDFB1 (Ego_r.pdb IAE)
private:
    void InitialiseInventoryWithItems(const std::vector<CItem>& items);   // 0x005BF8CB (Ego_r.pdb AAE)
};

} // namespace NInventory
void NInventory::CTCInventoryBase::InitialActivate()
{
    if (SerialisedItems.Data) {
        InitialiseInventoryWithItems(*SerialisedItems.Data);
        SerialisedItems.Reset(0);
    }
    if (SerialisedSelectedItems.Data) {
        for (std::vector<CCharString>::iterator it = SerialisedSelectedItems.Data->begin(); it != SerialisedSelectedItems.Data->end(); ++it) {
            long index = GetDefinitionManager().GetDefGlobalIndexFromName(*it);
            if (index > 0)
                SelectItemWithDefIndex(index);
        }
        SerialisedSelectedItems.Reset(0);
    }
}