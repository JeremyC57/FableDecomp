// CThingFilter_IsPerceptible::operator()(CThing* thing) -- true when the thing has no interface-0x36 component, else when any of that component's three flag bytes (+0x0C, +0x0D, +0x0E) is set
// Leaf: ??RCThingFilter_IsPerceptible
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006A8450, length 94 bytes (+ int3 padding).
// Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x004365B0.
// Layout: interface 0x36 -> bitset word 1 (+0x24) bit 22 (`test dword ptr [eax+24h],400000h`).
// Shape: an absent component returns the constant (`mov al,1`), the `a || b || c` chain is materialised as an int
// (`mov eax,1` / `xor eax,eax`); the present case is the `if` body so the constant tail lands last.
class CTCBase;

struct CInterfaceMap {                              // CVectorMap<ETCInterfaceType, CTCBase*> {first, last}
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) const;           // 0x004365B0
};

class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                 // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                     // +0x44
    template <class T> __forceinline bool GetInterface(long type, T** out)
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31))) {
            long key = type;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&key);
            if (it == map->last || type < it->type)
                it = map->last;
            *out = (T*)it->component;
            return true;
        }
        return false;
    }
};

struct CTCInterface36 {                             // interface 0x36 component (class not established)
    unsigned char _pad[0x0c];
    bool m_flagA;                                   // +0x0c
    bool m_flagB;                                   // +0x0d
    bool m_flagC;                                   // +0x0e
};

class CThingFilter_IsPerceptible { public:
    bool operator()(CThing* thing) const;
};

bool CThingFilter_IsPerceptible::operator()(CThing* thing) const
{
    CTCInterface36* component;
    if (thing->GetInterface(0x36, &component))
        return component->m_flagA || component->m_flagB || component->m_flagC;
    return true;
}