// CCreatureAction_SheatheItemToInventory_ForUnholyPower::FinishAction() -- run the base sheathe-to-inventory finish, then if the creature has the unholy-power component (interface type 0x86) whose flag at +0x18 is clear, cancel its abilities (@ 0x00794D90)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0069F8B0, length 96 bytes.
// Callees: CCreatureAction_SheatheItemToInventory::FinishAction @ 0x0069F670 (non-virtual base call), CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PCreature at +8),
// CVectorMap::LowerBound @ 0x0040F020 (the inlined GetInterface lookup for type 0x86 = bit 6 of the bitset dword at +0x30), CTCUnholyPower::CancelAbilities @ 0x00794D90
// (manifest leaf "CTCUnholyPower_CancelAbilities").
// Shape: the twin of the landed 0x0069F800 (DivineWrath, type 0x85, trigger @ 0x0078F820): one `push ecx` frame for the lookup key, a byte-flag test on the component before the call.
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:                      // a thing's sorted {type, component} vector map at +0x44
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type);   // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];         // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;             // +0x44
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            CInterfaceMap* map = &m_interfaces;   // named pointer: retail advances esi to the map once
            CInterfaceMap::SEntry* it = map->LowerBound(type);
            if (it == map->last || interfaceType < it->first)
                it = map->last;
            *ppOut = (T*)it->second;
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
class CTCUnholyPower { public:
    unsigned char _pad[0x18];
    unsigned char m_isCancelled;            // +0x18
    void CancelAbilities();                 // 0x00794D90
};

class CCreatureAction_SheatheItemToInventory { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void FinishAction();            // slot 4: 0x0069F670
    unsigned char _pad04[4];
    CIntelligentPointer<CThing> PCreature;      // +0x08
};
class CCreatureAction_SheatheItemToInventory_ForUnholyPower : public CCreatureAction_SheatheItemToInventory { public:
    virtual void FinishAction();
};

void CCreatureAction_SheatheItemToInventory_ForUnholyPower::FinishAction()
{
    CCreatureAction_SheatheItemToInventory::FinishAction();
    CTCUnholyPower* pPower;
    if (PCreature.GetPItem()->GetInterface(&pPower, 0x86)) {
        if (!pPower->m_isCancelled)
            pPower->CancelAbilities();
    }
}