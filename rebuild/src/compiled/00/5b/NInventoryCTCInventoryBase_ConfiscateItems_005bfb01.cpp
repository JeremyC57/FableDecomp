// NInventory::CTCInventoryBase::ConfiscateItems(vector<NInventory::CItem>& items) -- moves every confiscatable stack of `items` into ConfiscatedItems and removes it from the inventory (virtual slot 79)
// Leaf: ?ConfiscateItems@CTCInventoryBase@NInventory@@MAEX
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005BFB01, length 149 bytes. Frame-pointer size TU: optimize("y",off) + ("s",on).
// Callees: vector<CItem>::reserve @ 0x005C2013 (manifest "CActiveFile::ReserveSpace", propagated), the item-type test
// @ 0x005D9790 (__fastcall, manifest "IsBuyable"), vector<CItem>::push_back @ 0x005C2996, virtual slot 79 (+0x13C,
// RemoveItemsOfType(type, count)).
// Shape: sizes are the signed element difference (`sub ; cdq ; idiv 14h`) compared unsigned; `items[i]` is STLport
// `*(begin() + i)` (begin re-read every pass, the byte offset strength-reduced into the dead `items` argument slot).
// Layout: retail CTCInventoryBase ConfiscatedItems {first,last,end} at +0xAC; CItem is the PDB 0x14-byte record.
// Name: the class is NInventory::CTCInventoryBase (RTTI vtable 0x01253DBC .?AVCTCInventoryBase@NInventory@@, slot 85);
// Ego_r.exe ?ConfiscateItems@CTCInventoryBase@NInventory@@MAEXABV?$vector@VCItem@NInventory@@...@Z 0x0062293A (protected virtual;
// the PDB parameter is `const std::vector<NInventory::CItem>&`, the local CItemVector model keeps a non-const reference, so
// the Leaf pins the prefix).
#pragma optimize("y", off)
#pragma optimize("s", on)
#include "engine/CItem.h"

struct CItemVector {                                           // std::vector<NInventory::CItem>
    CItem* _M_start;
    CItem* _M_finish;
    CItem* _M_end_of_storage;
    CItem* begin() { return _M_start; }
    unsigned long size() const { return (unsigned long)(_M_finish - _M_start); }
    CItem& operator[](unsigned long n) { return *(begin() + n); }
    void reserve(unsigned long n);                             // 0x005C2013
    void push_back(const CItem& item);                         // 0x005C2996
};
bool __fastcall IsConfiscatable(long itemType);                // 0x005D9790

namespace NInventory {

class CTCInventoryBase {
public:
    virtual ~CTCInventoryBase();                                  // slot 0
    virtual void _v01();
    virtual void _v02();
    virtual void _v03();
    virtual void _v04();
    virtual void _v05();
    virtual void _v06();
    virtual void _v07();
    virtual void _v08();
    virtual void _v09();
    virtual void _v10();
    virtual void _v11();
    virtual void _v12();
    virtual void _v13();
    virtual void _v14();
    virtual void _v15();
    virtual void _v16();
    virtual void _v17();
    virtual void _v18();
    virtual void _v19();
    virtual void _v20();
    virtual void _v21();
    virtual void _v22();
    virtual void _v23();
    virtual void _v24();
    virtual void _v25();
    virtual void _v26();
    virtual void _v27();
    virtual void _v28();
    virtual void _v29();
    virtual void _v30();
    virtual void _v31();
    virtual void _v32();
    virtual void _v33();
    virtual void _v34();
    virtual void _v35();
    virtual void _v36();
    virtual void _v37();
    virtual void _v38();
    virtual void _v39();
    virtual void _v40();
    virtual void _v41();
    virtual void _v42();
    virtual void _v43();
    virtual void _v44();
    virtual void _v45();
    virtual void _v46();
    virtual void _v47();
    virtual void _v48();
    virtual void _v49();
    virtual void _v50();
    virtual void _v51();
    virtual void _v52();
    virtual void _v53();
    virtual void _v54();
    virtual void _v55();
    virtual void _v56();
    virtual void _v57();
    virtual void _v58();
    virtual void _v59();
    virtual void _v60();
    virtual void _v61();
    virtual void _v62();
    virtual void _v63();
    virtual void _v64();
    virtual void _v65();
    virtual void _v66();
    virtual void _v67();
    virtual void _v68();
    virtual void _v69();
    virtual void _v70();
    virtual void _v71();
    virtual void _v72();
    virtual void _v73();
    virtual void _v74();
    virtual void _v75();
    virtual void _v76();
    virtual void _v77();
    virtual void _v78();
    virtual void RemoveItemsOfType(long itemType, long count); // slot 79 (+0x13C)
    virtual void _v80();
    virtual void _v81();
    virtual void _v82();
    virtual void _v83();
    virtual void _v84();
    unsigned char _pad_0x04[0xA8];
    CItemVector ConfiscatedItems;                              // +0xAC
protected:
    virtual void ConfiscateItems(CItemVector& items);                 // slot 85 (+0x154)
};

} // namespace NInventory
void NInventory::CTCInventoryBase::ConfiscateItems(CItemVector& items)
{
    ConfiscatedItems.reserve(ConfiscatedItems.size() + items.size());
    for (unsigned long i = 0; i < items.size(); ++i) {
        CItem& item = items[i];
        if (IsConfiscatable(item.ItemType)) {
            ConfiscatedItems.push_back(item);
            RemoveItemsOfType(item.ItemType, item.NumberOfItems);
        }
    }
}