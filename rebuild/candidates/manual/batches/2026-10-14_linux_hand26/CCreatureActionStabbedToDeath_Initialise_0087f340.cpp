// CCreatureAction_StabbedToDeath::Initialise() -- base Initialise, then if the creature has the type-9 component, clears bit 2 of that component's flags word at +0xc through its setter
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0087F340, length 96 bytes.
// Callees: CCreatureActionBase::Initialise @ 0x00694D10 (direct base call, as the landed 0x00842F70), CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PCreature at +8),
// CVectorMap::LowerBound @ 0x0040F020 (the inlined GetInterface lookup for type 9 = bit 9 of the bitset dword at +0x20, tested as `test ch,2`),
// the component's flags setter @ 0x007EA270 (`mov [this+0Ch],arg`; the manifest "CObservable::AddExclusiveObserver" label is propagated).
// Shape: one `push ecx` frame for the lookup key; the flag is cleared as a conditional toggle (`test al,4 ; je ; xor eax,4`): retail did not fold it to `and`,
// so the source is the toggle-if-set form, not `& ~4`.
struct CTCBase;
struct CInterfaceMap {                      // a thing's sorted {type, component} vector map at +0x44
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);   // 0x0040F020
};
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];         // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;             // +0x44
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
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                        // +4
    CThing* GetPItem() const;               // 0x00A01B50
};
struct CTCType9Component {                  // the type-9 component: a flags word at +0xc with an out-of-line setter
    unsigned char _pad[0x0c];
    unsigned long m_flags;                  // +0x0c
    void SetFlags(unsigned long flags);     // 0x007EA270
};

struct CCreatureActionBase {                // retail sizeof 0xa8
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void Initialise();              // slot 12, 0x00694D10
    unsigned char _pad04[4];                // +0x04
    CBaseIntelligentPointer PCreature;      // +0x08 CIntelligentPointer<CThingCreatureBase>
    unsigned char _pad10[0xa8 - 0x10];      // +0x10
};
struct CCreatureAction_StabbedToDeath : CCreatureActionBase {
    virtual void Initialise();
};

void CCreatureAction_StabbedToDeath::Initialise()
{
    CCreatureActionBase::Initialise();
    CTCType9Component* pComponent;
    if (PCreature.GetPItem()->GetInterface(&pComponent, 9))
        pComponent->SetFlags((pComponent->m_flags & 4) ? (pComponent->m_flags ^ 4) : pComponent->m_flags);
}
