// CTCInventoryBase::ConfiscateAllItems() -- collects every non-empty item stack into a local vector and hands it to the virtual ConfiscateItems
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005BFA8E, length 56 bytes. Frame-pointer size TU: optimize("y",off) + ("s",on).
// Callees: CTCInventoryBase::GetAllNonEmptyInventoryItems(vector<CItem>&, bool) @ 0x005BFA44 (with true), virtual slot 85
// (+0x154, "ConfiscateItems" in the CTCInventoryClothing vtable 0x0124356C), std::vector<NInventory::CItem>::~vector @ 0x005C1E30.
// Shape: the 12-byte retail vector local is zeroed through `xor eax,eax` + three stores (inline constructor) and destroyed
// out of line; the module has no C++ EH, so there is no unwind frame around the local.
#pragma optimize("y", off)
#pragma optimize("s", on)
struct CItem_Retail { unsigned char _body[0x14]; };
struct CItemVector {
    CItem_Retail* first;
    CItem_Retail* last;
    CItem_Retail* end_of_storage;
    CItemVector() : first(0), last(0), end_of_storage(0) {}
    ~CItemVector();                                              // 0x005C1E30
};
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
    virtual void v79();
    virtual void v80();
    virtual void v81();
    virtual void v82();
    virtual void v83();
    virtual void v84();
    virtual void ConfiscateItems(CItemVector& items);            // slot 85 (+0x154)
    void GetAllNonEmptyInventoryItems(CItemVector& items, bool includeAll);   // 0x005BFA44
    void ConfiscateAllItems();
};
void CTCInventoryBase_Retail::ConfiscateAllItems()
{
    CItemVector items;
    GetAllNonEmptyInventoryItems(items, true);
    ConfiscateItems(items);
}
