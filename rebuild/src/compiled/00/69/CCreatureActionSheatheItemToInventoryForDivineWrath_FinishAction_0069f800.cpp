// CCreatureAction_SheatheItemToInventory_ForDivineWrath::FinishAction() -- run the base sheathe-to-inventory finish, then if the creature has the spell component (interface type 0x85) whose flag at +0x18 is clear, trigger it (@ 0x0078F820)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0069F800, length 83 bytes.
// Callees: CCreatureAction_SheatheItemToInventory::FinishAction @ 0x0069F670 (non-virtual base call), CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PCreature at +8),
// CVectorMap::LowerBound @ 0x0040F020 (the inlined GetInterface lookup for type 0x85 = bit 5 of the bitset dword at +0x30), the component's trigger @ 0x0078F820.
// Shape: the landed 0x00849320 lookup with a byte-flag test on the component before the call.
class CTCBase;
struct CInterfaceMap {                      // a thing's sorted {type, component} vector map at +0x44
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);   // 0x0040F020
};
class CThing { public:
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
class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};
class CTCDivineWrath { public:
    unsigned char _pad[0x18];
    unsigned char m_triggered;              // +0x18
public:
    void CancelMove();   // 0x0078F820
};

class CCreatureAction_SheatheItemToInventory { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void FinishAction();            // slot 4: 0x0069F670
    unsigned char _pad04[4];
    CIntelligentPointer<CThing> PCreature;      // +0x08
};
class CCreatureAction_SheatheItemToInventory_ForDivineWrath : public CCreatureAction_SheatheItemToInventory { public:
    virtual void FinishAction();
};

void CCreatureAction_SheatheItemToInventory_ForDivineWrath::FinishAction()
{
    CCreatureAction_SheatheItemToInventory::FinishAction();
    CTCDivineWrath* pSpell;
    if (PCreature.GetPItem()->GetInterface(&pSpell, 0x85)) {
        if (!pSpell->m_triggered)
            pSpell->CancelMove();
    }
}