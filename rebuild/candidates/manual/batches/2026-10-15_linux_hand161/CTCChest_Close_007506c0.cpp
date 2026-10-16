// CTCChest::Close() -- close an open chest: clear ChestOpen, set the open/close helper to its closed state, force a frame update on the thing and flag its interface-9 component
//
// Retail 0x007506C0, 97 bytes (plain `ret`). Callees: CThing::SetForceFrameUpdate(bool) @ 0x004C7C60,
// CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x0040F020.
// Layout: as the landed CTCChest::vfunc_4 0x00750730: ChestOpener (COpenCloseHelper, four bools)
// +0x1c (PDB +0x20 less the 4 bytes of the retail CTCContainer vector), ChestOpen +0x30; the
// interface-9 component's flag byte +0x10.
// Shape: the guard and the stores share one zero register (`xor al,al ; cmp cl,al`); the helper's
// four bools are set by an inline COpenCloseHelper::Close() (so they store before the call setup);
// the interface-9 component comes from the `__forceinline` out-parameter GetInterface (the thing
// re-read into esi, the bit tested on the whole word `mov eax,[esi+20h] ; test ah,2`, the component
// through eax).
struct CTCBase;

struct CInterfaceMap {                      // CVectorMap<ETCInterfaceType, CTCBase*> {first, last}
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);   // 0x0040F020
};

struct CTCFlagged {                         // the interface-9 component
    unsigned char _pad[0x10];
    bool          m_flag;                   // +0x10
};

struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];         // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;             // +0x44
    unsigned long HasInterface(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
    void SetForceFrameUpdate(bool force);   // 0x004C7C60
};

struct COpenCloseHelper {                   // PDB COpenCloseHelper (first four members)
    bool Opened;                            // +0x00
    bool Closed;                            // +0x01
    bool Opening;                           // +0x02
    bool Closing;                           // +0x03
    void Close()
    {
        Opened = false;
        Closed = false;
        Opening = false;
        Closing = true;
    }
};

struct CTCChest {                           // vtable 0x0126957C
    void*            __vftable;             // +0x00
    CThing*          ParentThing;           // +0x04 (CTCBase)
    unsigned char    _pad08[0x1c - 0x08];   // +0x08 CTCContainerRewardHero
    COpenCloseHelper ChestOpener;           // +0x1c
    unsigned char    _pad20[0x30 - 0x20];
    bool             ChestOpen;             // +0x30
    void Close();
};

void CTCChest::Close()
{
    if (ChestOpen)
    {
        ChestOpen = false;
        ChestOpener.Close();
        ParentThing->SetForceFrameUpdate(true);
        CTCFlagged* flagged;
        if (ParentThing->GetInterface(&flagged, 9))
            flagged->m_flag = true;
    }
}
