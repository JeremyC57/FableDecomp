// CCreatureAction_Drink::FinishAction() -- when the drink prop is still alive: kill it, drop whatever the creature carries in its primary slot, and when the jug/cup object is alive hand it back to the creature's carry slot (first removing it from the creature when the creature itself is its carrier; another carrier leaves it alone); then the base FinishAction
// Grade: DIFFER(346v348) -- retail pushes ebx after the first alive test and edi after the carrying lookup (callee-saved pushes inside one branch, the known lane gap); the "other carrier" early exit merges into that pop-less tail
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00847C90, length 348 bytes (+ int3 padding).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50, CThing::Kill(bool) @ 0x004C9B80, CVectorMap::LowerBound @ 0x0040F020
// (inlined GetInterface for CTCCarrying = type 0x46, bit 6 of +0x28) and the const instantiation @ 0x004365B0 (const GetTC for
// CTCCarriedObject = type 0x47, bit 7 of +0x28: `test cl,cl ; jns`), CTCCarrying::RemoveThingFromPrimarySlot(bool) @ 0x006A1E00,
// CTCCarriedObject::GetCarrier @ 0x00705170 (manifest "CGuiControlListBox::GetCurrentSelectedEntryData" is propagated),
// CTCCarrying::RemoveThing(CThing*) @ 0x006A19B0, CTCCarriedObject::GetCarrySlot() @ 0x00705CC0 (manifest "CTCTavernGame::GetBanter"
// is propagated), CTCCarrying::PutThingInCarrySlot(CThing*, long, bool) @ 0x006A1F80, CCreatureActionBase::FinishAction @ 0x00694430.
// Shape: IS_THING_ALIVE on the two intelligent pointers at +0xa8 and +0xb0 (null test, then bit 0 of +0x91); the first lookup is an
// untested out-parameter GetInterface (absent arm reads the dead word), the second a `p = 0; if (HasTC) p = GetTC` preload through
// the const map; `this` is spilled to the frame because ebp holds the carrier for the compare; the "other carrier" arm falls
// straight to the base call. Retail CCombatActionBase/CCreatureActionBase is 0x114 bytes, so `this` is modelled locally.
struct CTCBase;
struct CInterfaceMap {                      // CVectorMap<ETCInterfaceType, CTCBase*> at CThing+0x44
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);   // 0x0040F020
    const SEntry* LowerBound(const long* type) const;  // 0x004365B0 (the const instantiation)
};
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];         // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;             // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char m_flags91;                // +0x91 bit 0: dead
    void Kill(bool immediately);            // 0x004C9B80
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
    template<class T> __forceinline const T* GetTC(long interfaceType) const
    {
        long type = interfaceType;
        const CInterfaceMap* map = &m_interfaces;
        const CInterfaceMap::SEntry* it = map->LowerBound(&type);
        if (it == map->last || interfaceType < it->type)
            it = map->last;
        return (const T*)it->component;
    }
    __forceinline unsigned long HasTC(long interfaceType) const
    {
        return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31));
    }
};
struct CTCCarrying {
    CThing* RemoveThingFromPrimarySlot(bool drop);     // 0x006A1E00
    void RemoveThing(CThing* thing);                   // 0x006A19B0
    void PutThingInCarrySlot(CThing* thing, long slot, bool immediately);   // 0x006A1F80
};
struct CTCCarriedObject {
    unsigned char _pad[0x10];
    CThing* m_pCarrier;                     // +0x10
    CThing* GetCarrier() const;             // 0x00705170
    long GetCarrySlot() const;              // 0x00705CC0
};
struct CBaseIntelligentPointer {            // CIntelligentPointer<CThing>: vptr + item
    virtual void v00();
    CThing* m_pItem;                        // +4
    CThing* GetPItem() const;               // 0x00A01B50
};
inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}

struct CCreatureActionBase {                // retail sizeof 0xa8 (PCreature +0x08; the PDB header is 12 bytes larger)
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void FinishAction();            // slot 4: 0x00694430
    unsigned char _pad04[4];                // +0x04
    CBaseIntelligentPointer PCreature;      // +0x08
    unsigned char _pad10[0xa8 - 0x10];      // +0x10
};
struct CCreatureAction_Drink : CCreatureActionBase {
    CBaseIntelligentPointer PDrinkThing;    // +0xa8 the consumed prop
    CBaseIntelligentPointer PJug;           // +0xb0 the carried cup/jug object
    virtual void FinishAction();
};

void CCreatureAction_Drink::FinishAction()
{
    if (IS_THING_ALIVE(PDrinkThing)) {
        PDrinkThing.GetPItem()->Kill(true);
        CTCCarrying* carrying;
        PCreature.GetPItem()->GetInterface(&carrying, 0x46);
        carrying->RemoveThingFromPrimarySlot(false);
        if (IS_THING_ALIVE(PJug)) {
            const CTCCarriedObject* carried = 0;
            const CThing* jugThing = PJug.GetPItem();
            if (jugThing->HasTC(0x47))
                carried = jugThing->GetTC<CTCCarriedObject>(0x47);
            if (carried->m_pCarrier) {
                CThing* carrier = carried->GetCarrier();
                if (carrier != PCreature.GetPItem()) {
                    CCreatureActionBase::FinishAction();
                    return;
                }
                carrying->RemoveThing(PJug.GetPItem());
            }
            CThing* jug = PJug.GetPItem();
            carrying->PutThingInCarrySlot(jug, carried->GetCarrySlot(), true);
        }
    }
    CCreatureActionBase::FinishAction();
}
