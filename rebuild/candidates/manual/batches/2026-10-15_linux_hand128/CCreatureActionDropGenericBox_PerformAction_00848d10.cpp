// CCreatureAction_DropGenericBox::PerformAction() -- the creature drops what it carries in its primary slot (carrying component 0x46) and leaves creature mode 0x22 (mode manager 0x31)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00848D10, length 160 bytes, plain `ret`.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50, CVectorMap::LowerBound @ 0x0040F020,
//          CTCCarrying::RemoveThingFromPrimarySlot(bool) @ 0x006A1E00, CTCCreatureModeManager::RemoveMode(ulong)
//          @ 0x0073ABA0.
// Shape: PCreature's address is taken once (edi) for both lookups; two out-parameter GetInterface calls (void
//   template, out pointers uninitialised so the absent path reads the shared dead frame word); bit 0x46 is a byte
//   test of +0x28 (`test byte ptr [eax+28h],40h`), bit 0x31 a dword test of +0x24; the first call is reached from a
//   merged join, the second is tail-duplicated.
// Layout: CCreatureActionBase (retail 0xa8 bytes) with PCreature at +8; nothing else of the class is touched.
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
    template <class T> __forceinline void GetInterface(T** ppOut, long interfaceType)   // leaves *ppOut alone when absent
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
        }
    }
};

struct CBaseIntelligentPointer {            // CIntelligentPointer<T>: vptr + item
    void*   __vftable;                      // +0x00
    CThing* m_pItem;                        // +0x04
    CThing* GetPItem() const;               // 0x00A01B50
};

struct CTCCarrying {                        // interface type 0x46 component
    void RemoveThingFromPrimarySlot(bool drop);   // 0x006A1E00
};

struct CTCCreatureModeManager {             // interface type 0x31 component
    void RemoveMode(unsigned long mode);    // 0x0073ABA0
};

struct CCreatureActionBase {                // retail sizeof 0xa8
    virtual void v00(); virtual void v01(); virtual void v02();
    virtual void PerformAction();
    unsigned char _pad04[4];                // +0x04
    CBaseIntelligentPointer PCreature;      // +0x08 CIntelligentPointer<CThingCreatureBase>
    unsigned char _pad10[0xa8 - 0x10];
};

struct CCreatureAction_DropGenericBox : CCreatureActionBase {
    virtual void PerformAction();
};

void CCreatureAction_DropGenericBox::PerformAction()
{
    CTCCarrying* carrying;
    PCreature.GetPItem()->GetInterface(&carrying, 0x46);
    carrying->RemoveThingFromPrimarySlot(true);
    CTCCreatureModeManager* modes;
    PCreature.GetPItem()->GetInterface(&modes, 0x31);
    modes->RemoveMode(0x22);
}
