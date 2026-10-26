// CCreatureAction_OrderSecuritySweep::PerformAction() -- look up the creature's villager component (interface type 0x23), get its village and recalculate the security sweep
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00849250, length 84 bytes.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PCreature at +8), CVectorMap::LowerBound @ 0x0040F020 (the inlined GetInterface lookup for type 0x23 = bit 3 of
// the bitset dword at +0x24), GFGetPVillageInterface @ 0x007EE540 (register argument, landed naming of 0x0093EDF0), CTCVillage::RecalculateSecuritySweep(long) @ 0x00659C32.
// Shape: the lookup's result is not tested -- the villager pointer is read from the frame slot even when the interface is absent (retail reads an unset local there).
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
struct CTCVillager;
class CTCVillage { public:
    void RecalculateSecuritySweep(long flags);   // 0x00659C32
};
CTCVillage* __fastcall GFGetPVillageInterface(CTCVillager* villager);    // 0x007EE540

class CCreatureAction_OrderSecuritySweep { public:
    virtual void v00(); virtual void v01(); virtual void v02();
    virtual void PerformAction();           // slot 3
    unsigned char _pad04[4];
    CIntelligentPointer<CThing> PCreature;      // +0x08
};

void CCreatureAction_OrderSecuritySweep::PerformAction()
{
    CTCVillager* pVillager;
    PCreature.GetPItem()->GetInterface(&pVillager, 0x23);
    CTCVillage* pVillage = GFGetPVillageInterface(pVillager);
    if (pVillage)
        pVillage->RecalculateSecuritySweep(0);
}