// CCreatureAction_StabbedToDeath::Initialise() -- base Initialise, then if the creature has the type-9 component, clears bit 2 of that component's flags word at +0xc through its setter
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0087F340, length 96 bytes.
// Callees: CCreatureActionBase::Initialise @ 0x00694D10 (direct base call, as the landed 0x00842F70), CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PCreature at +8),
// CVectorMap::LowerBound @ 0x0040F020 (the inlined GetInterface lookup for type 9 = bit 9 of the bitset dword at +0x20, tested as `test ch,2`),
// the component's flags setter @ 0x007EA270 (`mov [this+0Ch],arg`; the manifest "CObservable::AddExclusiveObserver" label is propagated).
// Shape: one `push ecx` frame for the lookup key; the flag is cleared as a conditional toggle (`test al,4 ; je ; xor eax,4`): retail did not fold it to `and`,
// so the source is the toggle-if-set form, not `& ~4`.
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
class CTCTargeted { public:                  // the type-9 component: a flags word at +0xc with an out-of-line setter
    unsigned char _pad[0x0c];
    unsigned long m_flags;                  // +0x0c
public:
    void SetTargetingFlags(unsigned long);   // 0x007EA270
};

class CCreatureActionBase { public:                // retail sizeof 0xa8
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void Initialise();              // slot 12, 0x00694D10
    unsigned char _pad04[4];                // +0x04
    CIntelligentPointer<CThing> PCreature;      // +0x08 CIntelligentPointer<CThingCreatureBase>
    unsigned char _pad10[0xa8 - 0x10];      // +0x10
};
class CCreatureAction_StabbedToDeath : public CCreatureActionBase { public:
    virtual void Initialise();
};

void CCreatureAction_StabbedToDeath::Initialise()
{
    CCreatureActionBase::Initialise();
    CTCTargeted* pComponent;
    if (PCreature.GetPItem()->GetInterface(&pComponent, 9))
        pComponent->SetTargetingFlags((pComponent->m_flags & 4) ? (pComponent->m_flags ^ 4) : pComponent->m_flags);
}