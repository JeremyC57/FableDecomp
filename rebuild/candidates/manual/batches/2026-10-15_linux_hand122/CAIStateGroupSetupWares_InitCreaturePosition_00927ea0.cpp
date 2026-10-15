// CAIStateGroup_SetupWares::InitCreaturePosition() -- shopkeeper arrives: stock or clear the shop display
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00927EA0, length 131 bytes (+ int3 padding).
// Callees: CVectorMap::LowerBound @ 0x0040F020 (inlined GetInterface on the brain's creature, type 0x90 = bit
// 16 of the presence word at +0x30), CTCShopKeeper::GetShopTC @ 0x00798F50, CTCShop::IsOpeningHours
// @ 0x00773950, CTCShop::RemoveStockItemsFromShop @ 0x00773E10, CTCShop::StockDisplay @ 0x00774C60,
// CTCShop::SetStockState(long) @ 0x00773C00, CTCShop::IsPermanentlyStocked @ 0x007721A0 (manifest labels).
//
// Shape: the shopkeeper component comes from the bool GetInterface (a miss jumps straight to the final
// `xor al,al` tail); its shop (kept in esi) is null-tested. During opening hours the stock is cleared,
// re-displayed and the stock state reset (0); outside them a shop that is not permanently stocked is emptied.
// Every path returns false (this override never places the creature); retail tail-duplicates the
// `pop esi ; xor al,al` epilogue into the opening-hours arm. No engine header exists for this class.
struct CTCBase;

// A thing's component table: the presence bitset at +0x20 (tested inline, bit N of dword N/32) and the sorted
// {type, component} vector map at +0x44 searched with LowerBound @ 0x0040F020 -- the landed out-parameter
// GetInterface template (0x007504E0 / 0x00930DC0 shape): the component lands in eax, and a miss leaves the
// caller's out pointer unwritten (the frame slot that also holds the key).
struct CInterfaceMap {                          // CVectorMap<ETCInterfaceType, CTCBase*> {first, last}
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};

struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad1[0x45];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    bool HasTC(long interfaceType) const
    {
        return (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) != 0;
    }
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;   // named pointer: retail advances esi to the map once
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};

struct CBaseIntelligentPointer {                // CIntelligentPointer<T>: vptr + item
    void*   __vftable;                          // +0x00
    CThing* m_pItem;                            // +0x04
    CThing* GetPItem() const;                   // 0x00A01B50
};

// IS_THING_ALIVE(ptr): the pointer is set and the thing's dead bit (+0x91 bit 0) is clear (inlined).
inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}

struct CAIBrain {
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};

struct CTCShop {
    bool IsOpeningHours();                      // 0x00773950
    void RemoveStockItemsFromShop();            // 0x00773E10
    void StockDisplay();                        // 0x00774C60
    void SetStockState(long state);             // 0x00773C00
    bool IsPermanentlyStocked();                // 0x007721A0
};

struct CTCShopKeeper {                          // interface type 0x90 component
    CTCShop* GetShopTC();                       // 0x00798F50
};

struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    bool Terminated;                            // +0x10
    unsigned char Priority;                     // +0x11
    bool Enabled;                               // +0x12
};

struct CAIStateGroup_SetupWares : CAIStateGroupBase {
    bool InitCreaturePosition();
};

bool CAIStateGroup_SetupWares::InitCreaturePosition()
{
    CTCShopKeeper* shopKeeper;
    if (PBrain->PCreature->GetInterface(&shopKeeper, 0x90)) {
        CTCShop* shop = shopKeeper->GetShopTC();
        if (shop != 0) {
            if (shop->IsOpeningHours()) {
                shop->RemoveStockItemsFromShop();
                shop->StockDisplay();
                shop->SetStockState(0);
            } else if (!shop->IsPermanentlyStocked()) {
                shop->RemoveStockItemsFromShop();
            }
        }
    }
    return false;
}
