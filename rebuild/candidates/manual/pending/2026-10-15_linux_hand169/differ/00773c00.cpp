// CTCShop::SetStockState(long state) -- on a change: store it, tell the keeper's CTCShopKeeper component (interface 0x90) to repress opinions unless the state is 1, then refresh the player GUI
// Leaf: SetStockState
// Grade: DIFFER(142v142) -- order only: retail loads the component into ecx before `cmp edi,1 ; setne dl ; push edx`, ours computes the bool first (setne cl ; push ecx) then loads ecx (direct, named local and inline GetTC forms tried); lane gap: looked-up component used as `this` loaded before the argument
// Retail: Fable.exe 0x00773C00, 140 bytes (`ret 4`; the GUI refresh is a `mov [esp+4],0 ; jmp` tail call).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (x3), CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound
// @ 0x0040F020 (inline GetInterface), CTCShopKeeper::RepressOpinions(bool) @ 0x007996B0, CPlayerGui::Refresh(long)
// @ 0x004378C0 (`mov ecx,[ecx+160h] ; jmp`; manifest "_Cons_val<...>" is propagated) on the global @ 0x013B8790.
// Layout: retail CTCShop: StockState +0x0C, PShopKeeper +0x10; CThing presence bitset +0x20 (0x90 = bit 16 of +0x30),
// interface map +0x44, dead flag +0x91 bit 0.
// Shape: the new state is stored before the keeper tests; `state != 1` is pushed as `setne dl`.
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
    void RepressOpinions(bool repress);             // 0x007996B0
};
struct CPlayerGui {
    void Refresh(long flag);                        // 0x004378C0
};
extern CPlayerGui* g_pPlayerGui;                    // 0x013B8790
struct CTCShop {
    virtual void v00();
    void*                   ParentThing;            // +0x04 (CTCBase)
    bool                    ToBeRemoved;            // +0x08
    bool                    IsInGlobalUpdateTCs;    // +0x09
    long                    StockState;             // +0x0c
    CBaseIntelligentPointer PShopKeeper;            // +0x10
    void SetStockState(long state);
};

void CTCShop::SetStockState(long state)
{
    if (state != StockState) {
        StockState = state;
        if (IS_THING_ALIVE(PShopKeeper)) {
            CThing* keeper = PShopKeeper.GetPItem();
            if (keeper->HasInterface(0x90)) {
                keeper->GetTC<CTCShopKeeper>(0x90)->RepressOpinions(state != 1);
            }
        }
        g_pPlayerGui->Refresh(0);
    }
}
