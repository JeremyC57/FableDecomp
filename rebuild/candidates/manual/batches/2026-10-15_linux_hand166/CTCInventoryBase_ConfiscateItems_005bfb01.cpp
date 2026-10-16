// CTCInventoryBase::ConfiscateItems(vector<NInventory::CItem>& items) -- moves every confiscatable stack of `items` into ConfiscatedItems and removes it from the inventory (virtual slot 79)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005BFB01, length 149 bytes. Frame-pointer size TU: optimize("y",off) + ("s",on).
// Callees: vector<CItem>::reserve @ 0x005C2013 (manifest "CActiveFile::ReserveSpace", propagated), the item-type test
// @ 0x005D9790 (__fastcall, manifest "IsBuyable"), vector<CItem>::push_back @ 0x005C2996, virtual slot 79 (+0x13C,
// RemoveItemsOfType(type, count)).
// Shape: sizes are the signed element difference (`sub ; cdq ; idiv 14h`) compared unsigned; `items[i]` is STLport
// `*(begin() + i)` (begin re-read every pass, the byte offset strength-reduced into the dead `items` argument slot).
// Layout: retail CTCInventoryBase ConfiscatedItems {first,last,end} at +0xAC; CItem is the PDB 0x14-byte record.
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

struct CTCInventoryBase_Retail {
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09();
    virtual void v10();
    virtual void v11();
    virtual void v12();
    virtual void v13();
    virtual void v14();
    virtual void v15();
    virtual void v16();
    virtual void v17();
    virtual void v18();
    virtual void v19();
    virtual void v20();
    virtual void v21();
    virtual void v22();
    virtual void v23();
    virtual void v24();
    virtual void v25();
    virtual void v26();
    virtual void v27();
    virtual void v28();
    virtual void v29();
    virtual void v30();
    virtual void v31();
    virtual void v32();
    virtual void v33();
    virtual void v34();
    virtual void v35();
    virtual void v36();
    virtual void v37();
    virtual void v38();
    virtual void v39();
    virtual void v40();
    virtual void v41();
    virtual void v42();
    virtual void v43();
    virtual void v44();
    virtual void v45();
    virtual void v46();
    virtual void v47();
    virtual void v48();
    virtual void v49();
    virtual void v50();
    virtual void v51();
    virtual void v52();
    virtual void v53();
    virtual void v54();
    virtual void v55();
    virtual void v56();
    virtual void v57();
    virtual void v58();
    virtual void v59();
    virtual void v60();
    virtual void v61();
    virtual void v62();
    virtual void v63();
    virtual void v64();
    virtual void v65();
    virtual void v66();
    virtual void v67();
    virtual void v68();
    virtual void v69();
    virtual void v70();
    virtual void v71();
    virtual void v72();
    virtual void v73();
    virtual void v74();
    virtual void v75();
    virtual void v76();
    virtual void v77();
    virtual void v78();
    virtual void RemoveItemsOfType(long itemType, long count); // slot 79 (+0x13C)
    unsigned char _pad_0x04[0xA8];
    CItemVector ConfiscatedItems;                              // +0xAC
    void ConfiscateItems(CItemVector& items);
};
void CTCInventoryBase_Retail::ConfiscateItems(CItemVector& items)
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
