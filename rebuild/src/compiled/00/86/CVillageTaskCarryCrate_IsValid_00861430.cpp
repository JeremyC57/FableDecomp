// CVillageTaskCarryCrate::IsValid() -- the task still makes sense: the crate stack thing exists and has crates to pick up (Pickup), or room for one more (fewer than 2) when dropping off
// Leaf: ?IsValid@CVillageTaskCarryCrate@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00861430, length 121 bytes (padded to 128).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (x2, on PCrateStack +0x10), CVectorMap<ETCInterfaceType,
// CTCBase*>::LowerBound @ 0x004365B0 (the inline GetInterface for TC 0xb5 = CTCCrateStack, present bit `[thing+34h] &
// 200000h`), CTCCrateStack::GetNumCrates @ 0x007B6660.
// Layout: engine/CVillageTaskCarryCrate.h offsets (CreatureAssignedToTask +0x08, PCrateStack +0x10, Pickup +0x18) with the
// intelligent pointers modelled as {vptr, item}.
// Shape: `CTCCrateStack* stack = 0;` before the second GetPItem (its `xor esi,esi` precedes the call); the out-parameter
// GetInterface of the landed agent161 CTCCrateStack model; the crate count goes to GetNumCrates even when null.
class CTCBase;
struct CInterfaceMap {                             // CVectorMap<ETCInterfaceType, CTCBase*>
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) const;          // 0x004365B0
};
class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                // +0x20 interface-present bitset
    CInterfaceMap m_interfaces;                    // +0x44
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
};
struct CBaseIntelligentPointer {                   // CIntelligentPointer<T>: vptr + item (8 bytes)
    virtual void v00();
    CThing* PItem;
    CThing* GetPItem() const;                      // 0x00A01B50
};
class CTCCrateStack { public:
    long GetNumCrates() const;                     // 0x007B6660
};
class CTCCarrying { public:
    CThing* GetThingInPrimarySlot() const;         // 0x006A1440
};
class CVillageTaskCarryCrate { public:                    // engine/CVillageTaskCarryCrate.h layout (flattened pointers there)
    virtual void v00();
    unsigned char           _pad04[4];             // +0x04 (CVillageTaskBase)
    CBaseIntelligentPointer CreatureAssignedToTask;   // +0x08 CIntelligentPointer<const CThing>
    CBaseIntelligentPointer PCrateStack;           // +0x10 CIntelligentPointer<CThing>
    bool                    Pickup;                // +0x18
    long                    Priority;              // +0x1c
    bool IsValid();
    bool IsCreatureValidForTask(CThing* creature);
};
bool CVillageTaskCarryCrate::IsValid()
{
    if (PCrateStack.GetPItem() == 0)
        return false;
    CTCCrateStack* stack = 0;
    PCrateStack.GetPItem()->GetInterface(&stack, 0xb5);
    long crates = stack->GetNumCrates();
    if ((Pickup && crates > 0) || (!Pickup && crates < 2))
        return true;
    return false;
}