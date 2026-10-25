// NInventory::CTCInventoryBase::ReturnAllConfiscatedItems() -- gives back every confiscated item stack, then empties ConfiscatedItems
// Leaf: ?ReturnAllConfiscatedItems@CTCInventoryBase@NInventory@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005BF91A, length 79 bytes; size TU (`push 14h ; pop ecx` divisor, no frame:
// ebp is the stride register). Virtual slot 73 of the inventory screens (CTCInventoryClothing vtable 0x0124356C).
// Callees: CTCInventoryBase::AddItem(const CItem&) @ 0x005BF4A3, std::vector<CItem>::erase(first, last) @ 0x005C1ECC.
// Retail CTCInventoryBase: no engine header; ConfiscatedItems (std::vector<NInventory::CItem>, 0x14-byte elements) at +0xAC
// (PDB +0xBC minus four 4-byte-narrower vectors before it).
// Shape: the trailing erase reads both bounds through the vector pointer (`push [esi+4] ; mov ecx,esi ; push [esi]`), i.e. the
// erase(begin(), end()) sits in an inline member of the vector (`clear()`); written at the call site it re-addresses
// `[edi+0B0h]` from `this`.
// Name: the class is NInventory::CTCInventoryBase (RTTI vtable 0x01253DBC .?AVCTCInventoryBase@NInventory@@, slot 73);
// Ego_r.exe ?ReturnAllConfiscatedItems@CTCInventoryBase@NInventory@@UAEXXZ 0x006229CE.
#pragma optimize("s", on)
struct CItem_Retail { unsigned char _body[0x14]; };
struct CItemVector {
    CItem_Retail* first;
    CItem_Retail* last;
    CItem_Retail* end_of_storage;
    unsigned long size() const { return (unsigned long)(last - first); }
    CItem_Retail& operator[](unsigned long n) { return first[n]; }
    CItem_Retail* begin() { return first; }
    CItem_Retail* end() { return last; }
    CItem_Retail* erase(CItem_Retail* from, CItem_Retail* to);   // 0x005C1ECC
    void clear() { erase(begin(), end()); }
};
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
    virtual void ReturnAllConfiscatedItems();                         // slot 73 (+0x124)
    unsigned char _pad_0x04[0xA8];
    CItemVector   ConfiscatedItems;                              // +0xAC
    void AddItem(const CItem_Retail& item);                      // 0x005BF4A3
};

} // namespace NInventory
void NInventory::CTCInventoryBase::ReturnAllConfiscatedItems()
{
    for (unsigned long i = 0; i < ConfiscatedItems.size(); ++i)
        AddItem(ConfiscatedItems[i]);
    ConfiscatedItems.clear();
}