// CTCShop::IsShopKeeperAvailable() const -- TRUE when the shop is open (StockState 0), its keeper is alive, has the CTCShopKeeper component (interface 0x90) and that component's RepressedOpinions flag is set
// Leaf: IsShopKeeperAvailable
// Grade: DIFFER(118v118) -- register choice only: retail reads the component into eax and the flag into cl (`mov eax,[eax+4] ; mov cl,[eax+42h]`), ours ecx / al (direct, local, inline GetTC and one-condition forms tried); scheduling artefact
// Retail: Fable.exe 0x00773AA0, 118 bytes (plain `ret`, BOOL result).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (x3), CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound
// @ 0x0040F020 (inline GetInterface, landed CAIStateGroupBase::CleanupMain 0x008FD050 form).
// Layout: retail CTCShop: StockState +0x0C, PShopKeeper (CIntelligentPointer<CThingCreatureBase>) +0x10; CThing
// presence bitset +0x20 (type 0x90 = bit 16 of +0x30), interface map +0x44, dead flag +0x91 bit 0; retail
// CTCShopKeeper (PDB less the 4 bytes of the debug vector): RepressedOpinions +0x42 (PDB 0x46).
// Shape: IS_THING_ALIVE inlined, the presence test on a third GetPItem, the type key in the `push ecx` slot.
struct CTCBaseAny;
struct CInterfaceMap {                              // CVectorMap<ETCInterfaceType, CTCBase*> {first, last}
    struct SEntry { long type; CTCBaseAny* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);           // 0x0040F020
};
struct CThing {
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                 // +0x20
    CInterfaceMap m_interfaces;                     // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char m_flags91;                        // +0x91 bit 0: dead
    unsigned long HasInterface(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    template <class T> __forceinline T* GetTC(long type)
    {
        long key = type;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&key);
        if (it == map->last || type < it->type)
            it = map->last;
        return (T*)it->component;
    }
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                                // +4
    CThing* GetPItem() const;                       // 0x00A01B50
};
inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}
struct CTCShopKeeper {
    unsigned char _pad00[0x42];
    bool RepressedOpinions;                         // +0x42
};
struct CTCShop {
    virtual void v00();
    void*                   ParentThing;            // +0x04 (CTCBase)
    bool                    ToBeRemoved;            // +0x08
    bool                    IsInGlobalUpdateTCs;    // +0x09
    long                    StockState;             // +0x0c
    CBaseIntelligentPointer PShopKeeper;            // +0x10
    long IsShopKeeperAvailable() const;
};

long CTCShop::IsShopKeeperAvailable() const
{
    if (StockState == 0 && IS_THING_ALIVE(PShopKeeper)) {
        CThing* keeper = PShopKeeper.GetPItem();
        if (keeper->HasInterface(0x90)) {
            if (keeper->GetTC<CTCShopKeeper>(0x90)->RepressedOpinions)
                return 1;
        }
    }
    return 0;
}
