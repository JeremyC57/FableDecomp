// CThingFilter_NoiseCameFromThing::operator()(CThing* thing) -- true when the thing's interface-0x77 component's intelligent pointer (+0x28) holds the filter's Thing
// Leaf: ??RCThingFilter_NoiseCameFromThing
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00809210, length 82 bytes (+ int3 padding).
// Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x004365B0, CBaseIntelligentPointer::GetPItem @ 0x00A01B50.
// Layout: PDB CThingFilter_NoiseCameFromThing { CThing* Thing; } (+0); interface 0x77 -> bitset word 3 (+0x2C) bit 23
// (`test dword ptr [eax+2Ch],800000h`); the component holds a CIntelligentPointer at +0x28 (`lea ecx,[eax+28h]`).
// A second instantiation of the same filter shape as 0x006A81F0 (different component type).
struct CTCBase;

struct CInterfaceMap {                              // CVectorMap<ETCInterfaceType, CTCBase*> {first, last}
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) const;           // 0x004365B0
};

struct CThing {
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

struct CBaseIntelligentPointer {
    void*   __vftable;
    CThing* PItem;
    CThing* GetPItem() const;                       // 0x00A01B50
};

struct CTCInterface77 {                             // interface 0x77 component (class not established)
    unsigned char _pad[0x28];
    CBaseIntelligentPointer m_source;               // +0x28
};

struct CThingFilter_NoiseCameFromThing {
    CThing* Thing;                                  // +0x00
    bool operator()(CThing* thing) const;
};

bool CThingFilter_NoiseCameFromThing::operator()(CThing* thing) const
{
    CTCInterface77* component;
    if (thing->GetInterface(0x77, &component) && component->m_source.GetPItem() == Thing)
        return true;
    return false;
}