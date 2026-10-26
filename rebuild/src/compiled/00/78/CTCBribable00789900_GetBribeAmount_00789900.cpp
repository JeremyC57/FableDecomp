// CTCBribable_00789900::GetBribeAmount() -- ask the thing's guard component for the bribe it will take, else the component's own default
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00789900, length 68 bytes.
// Callees: CVectorMap::LowerBound @ 0x004365B0 (the inlined interface lookup for type 0xb4 = bit 20 of
// bitset dword +0x34), CTCGuard::GetBribeAmountWillTake @ 0x007B2D50 on the found component.
// The manifest label (CCombatSequence_WillEnflame::OnStart) is wrong: that class's OnStart is 0x00909730
// (its vtable slot 17), and this body is called from CTCCarriedActionUseGoldBag @ 0x007ED64A on the
// component fetched for interface type 0x80, with the result compared `jle`. The object is a thing
// component ({vptr, owning CThing* at +4, ..., default amount at +0xc}); the default is read into a local
// first (retail loads +0xc into eax before the lookup frees ecx) and overwritten by the guard's answer. Named by what it does.
class CTCBase;

struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) const;      // 0x004365B0 (CVectorMap::LowerBound)
};

class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44 sorted {type, component} vector map
    unsigned char _pad1[0x45];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    __forceinline bool HasInterface(long interfaceType) const   // the bitset test alone
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)))
            return true;
        return false;
    }
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)   // forced: two lookups in one body exceed the inliner budget
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

class CTCGuard { public:
    long GetBribeAmountWillTake();             // 0x007B2D50
};

struct CTCBribable_00789900 {
    virtual void v00();
    CThing* m_pThing;                          // +0x04 the owning thing
    long    m_08;                              // +0x08
    long    m_defaultBribeAmount;              // +0x0c returned when the thing has no guard component
    long GetBribeAmount();
};

long CTCBribable_00789900::GetBribeAmount()
{
    long amount = m_defaultBribeAmount;        // read first: retail loads +0xc into eax before the lookup frees ecx
    CTCGuard* pGuard;
    if (m_pThing->GetInterface(&pGuard, 0xb4))
        amount = pGuard->GetBribeAmountWillTake();
    return amount;
}