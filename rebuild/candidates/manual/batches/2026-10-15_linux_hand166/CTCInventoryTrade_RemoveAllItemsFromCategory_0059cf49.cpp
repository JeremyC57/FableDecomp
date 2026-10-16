// CTCInventoryTrade::RemoveAllItemsFromCategory(EInventoryCategoryIdentifier category) -- remembers the category and removes every (item type, count) entry of the trade category from the inventory
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0059CF49, length 116 bytes. Frame-pointer size TU: optimize("y",off) + ("s",on).
// Callees: CTCInventoryTrade::GetCategoryIndexForTradeType() @ 0x0059B6F4, CTCInventoryBase::GetCategoryWithIndex(long)
// @ 0x005BCE3C, NInventory::CCategory::GetAllItemsInCategory(vector<pair<long,long> >&) @ 0x005BF2C6, virtual slot 79 (+0x13C,
// RemoveItemFromInventory(long itemType, long count)), the vector's storage freed by 0x00BFEA14.
// Shape: the category is a named local looked up before the vector is declared; a local STLport vector (three zero words sharing `xor ebx,ebx`, which is also the loop index); `(long)size()` gives
// the signed `sar 3 ; jle` count and `*(begin() + i)` re-reads the start word each pass; the inline vector dtor frees the
// storage after a null test.
// Layout: retail CTCInventoryTrade CategoryIdent +0x170 (engine header PDB +0x19C less 0x2C of 16-byte debug containers).
#pragma optimize("y", off)
#pragma optimize("s", on)
void __cdecl FreeStorage(void* p);                             // 0x00BFEA14

struct CItemCount {                                            // std::pair<long,long>: item type, count
    long first;
    long second;
};
struct CItemCountVector {                                      // STLport vector<pair<long,long> >
    CItemCount* _M_start;
    CItemCount* _M_finish;
    CItemCount* _M_end_of_storage;
    CItemCountVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
    ~CItemCountVector() { if (_M_start) FreeStorage(_M_start); }
    CItemCount* begin() const { return _M_start; }
    unsigned long size() const { return _M_finish - _M_start; }
    CItemCount& operator[](unsigned long n) { return *(begin() + n); }
};
namespace NInventory {
struct CCategory {
    void GetAllItemsInCategory(CItemCountVector& items);       // 0x005BF2C6
};
}

struct CTCInventoryTrade {
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
    virtual void RemoveItemFromInventory(long itemType, long count);   // slot 79 (+0x13C)
    unsigned char _pad_0x04[0x16C];
    long CategoryIdent;                                        // +0x170
    long GetCategoryIndexForTradeType();                       // 0x0059B6F4
    NInventory::CCategory* GetCategoryWithIndex(long index);   // 0x005BCE3C (CTCInventoryBase)
    void RemoveAllItemsFromCategory(long category);
};

void CTCInventoryTrade::RemoveAllItemsFromCategory(long category)
{
    CategoryIdent = category;
    NInventory::CCategory* tradeCategory = GetCategoryWithIndex(GetCategoryIndexForTradeType());
    CItemCountVector items;
    tradeCategory->GetAllItemsInCategory(items);
    long count = (long)items.size();
    for (long i = 0; i < count; ++i)
        RemoveItemFromInventory(items[i].first, items[i].second);
}
