// CCreatureAction_SheatheItemToInventory_ForEnflame::FinishAction() -- run the base sheathe-to-inventory finish, then if the creature has the spell component (interface type 0x95) whose flag at +0xc is clear, trigger it (@ 0x007A44C0)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0069F750, length 86 bytes.
// Callees: CCreatureAction_SheatheItemToInventory::FinishAction @ 0x0069F670 (non-virtual base call), CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PCreature at +8),
// CVectorMap::LowerBound @ 0x0040F020 (the inlined GetInterface lookup for type 0x95 = bit 21 of the bitset dword at +0x30), the component's trigger @ 0x007A44C0.
// Shape: the landed 0x00849320 lookup with a byte-flag test on the component before the call.
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
struct CTCSpellComponent_0069f750 {
    unsigned char _pad[0xc];
    unsigned char m_triggered;              // +0xc
    void Trigger();                         // 0x007A44C0
};

class CCreatureAction_SheatheItemToInventory { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void FinishAction();            // slot 4: 0x0069F670
    unsigned char _pad04[4];
    CIntelligentPointer<CThing> PCreature;      // +0x08
};
class CCreatureAction_SheatheItemToInventory_ForEnflame : public CCreatureAction_SheatheItemToInventory { public:
    virtual void FinishAction();
};

void CCreatureAction_SheatheItemToInventory_ForEnflame::FinishAction()
{
    CCreatureAction_SheatheItemToInventory::FinishAction();
    CTCSpellComponent_0069f750* pSpell;
    if (PCreature.GetPItem()->GetInterface(&pSpell, 0x95)) {
        if (!pSpell->m_triggered)
            pSpell->Trigger();
    }
}