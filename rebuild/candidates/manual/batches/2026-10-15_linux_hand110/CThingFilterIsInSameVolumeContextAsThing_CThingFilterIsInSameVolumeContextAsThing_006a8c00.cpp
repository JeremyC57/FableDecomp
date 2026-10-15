// CThingFilter_IsInSameVolumeContextAsThing::CThingFilter_IsInSameVolumeContextAsThing(CThing* thing, bool override) -- constructor: Override from the argument, PThingInside = the thing's interface-0x75 component's intelligent-pointer item (+0x0C), else null
// Leaf: ??0CThingFilter_IsInSameVolumeContextAsThing
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006A8C00, length 87 bytes (+ int3 padding).
// Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x004365B0, CBaseIntelligentPointer::GetPItem @ 0x00A01B50.
// Layout: PDB CThingFilter_IsInSameVolumeContextAsThing { CThing* PThingInside; bool Override; } (8 bytes);
// interface 0x75 -> bitset word 3 (+0x2C) bit 21 (`test dword ptr [eax+2Ch],200000h`); the component holds an
// intelligent pointer at +0x0C (`lea ecx,[eax+0Ch]`).
// Shape: Override (+4) is stored before PThingInside (+0) is cleared -- the bool is an init-list member and the pointer
// is assigned in the body (zero first, then the component's item when present).
struct CTCBase;

struct CInterfaceMap {                              // CVectorMap<ETCInterfaceType, CTCBase*> {first, last}
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);           // 0x004365B0
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

struct CTCInterface75 {                             // interface 0x75 component (class not established)
    unsigned char _pad[0x0c];
    CBaseIntelligentPointer m_inside;               // +0x0c
};

struct CThingFilter_IsInSameVolumeContextAsThing {
    CThing* PThingInside;                           // +0x00
    bool    Override;                               // +0x04
    CThingFilter_IsInSameVolumeContextAsThing(CThing* thing, bool override);
};

CThingFilter_IsInSameVolumeContextAsThing::CThingFilter_IsInSameVolumeContextAsThing(CThing* thing, bool override)
    : Override(override)
{
    PThingInside = 0;
    CTCInterface75* component;
    if (thing->GetInterface(0x75, &component))
        PThingInside = component->m_inside.GetPItem();
}
