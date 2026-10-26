// CAIStateGroup_WanderAroundShops::OnReplace() -- leave the current shop's shopper list (when the shop is alive)
// and let the creature's idle scheduler (interface 0xD3) pick a new idle.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00946E70, length 175 bytes (+ int3 padding).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (Shop +0x14, three times), CVectorMap::LowerBound
// @ 0x0040F020 twice (shop 0x53 = bit 19 of +0x28 through the out-parameter GetInterface whose miss path reads the
// never-written slot; idle scheduler 0xD3 = bit 19 of +0x38 through HasTC + entry-returning FindEntry),
// CTCShop::RemoveShopper(CThing*) @ 0x00772120, CTCIdleScheduler::ChooseNewIdle @ 0x00801A50.
// Shape: inline IS_THING_ALIVE on Shop; the shop component lands in eax (out-parameter form), the scheduler in ecx
// straight from the entry (FindEntry form). PDB member names (Shop +0x14, PrettyThing +0x1c, NextFrameBuy +0x24)
// from the engine header, whose layout is padding only, so a local model.
// ---- shared AI state-group model (same shapes as the landed CAIStateGroup_* sources) ----
class CTCBase;
class CThing;
class CWorld;

// A thing's component table: the presence bitset at +0x20 (tested inline, bit N of dword N/32) and the sorted
// {type, component} vector map at +0x44 searched with CVectorMap::LowerBound @ 0x004365B0.
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x004365B0
};

class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad1[0x45];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    unsigned long HasTC(long interfaceType) const
    {
        return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31));
    }
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
    // entry-returning lookup (the caller tests HasTC first and uses the entry's component directly)
    __forceinline CInterfaceMap::SEntry* FindEntry(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        CInterfaceMap::SEntry* sel;
        if (it == map->last) sel = map->last;
        else if (it->type <= interfaceType) sel = it;
        else sel = map->last;
        return sel;
    }
};

class CAIBrain { public:
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};

class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;                     // +0x11
    unsigned char Enabled;                      // +0x12
    unsigned char _pad_0x13;
};

struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CThing* GetPItem() const;                   // 0x00A01B50
};

inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}

class CTCShop { public:                                // interface 0x53
    void RemoveShopper(CThing* shopper);        // 0x00772120
};

class CTCIdleScheduler { public:                       // interface 0xD3
    void ChooseNewIdle();                       // 0x00801A50
};

struct CAIStateGroup_WanderAroundShops : CAIStateGroupBase {
    CBaseIntelligentPointer Shop;               // +0x14 CIntelligentPointer<CThing>
    CBaseIntelligentPointer PrettyThing;        // +0x1c CIntelligentPointer<CThing>
    long NextFrameBuy;                          // +0x24
    virtual void OnReplace();
};

void CAIStateGroup_WanderAroundShops::OnReplace()
{
    if (IS_THING_ALIVE(Shop)) {
        CTCShop* shop;
        Shop.GetPItem()->GetInterface(&shop, 0x53);
        shop->RemoveShopper(PBrain->PCreature);
    }
    if (PBrain->PCreature->HasTC(0xD3))
        ((CTCIdleScheduler*)PBrain->PCreature->FindEntry(0xD3)->component)->ChooseNewIdle();
}