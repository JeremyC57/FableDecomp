// CCreatureAction_SummonerLightningOrbOutOfGroundSmash::FinishAction() -- base FinishAction, then if the creature has a mode manager (interface type 0x31) that holds mode 90, remove that mode
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008CB5C0, length 96 bytes (byte-identical to the OrbOutOf body @ 0x008CB4A0 up to the relocations). FinishAction is vtable slot 4 (protected `MAE`); the base is called directly here.
// Callees: CCombatActionBase::FinishAction @ 0x00859A20 (direct base call), CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PCreature at +8),
// CVectorMap::LowerBound @ 0x0040F020 (the inlined GetInterface lookup for type 0x31 = bit 17 of the bitset dword at +0x24),
// CTCCreatureModeManager::IsModeActive(unsigned long) @ 0x0073A8A0 (manifest "CPlayer::HasMode" is propagated; landed naming of 0x00977A30),
// CTCCreatureModeManager::RemoveMode(unsigned long) @ 0x0073ABA0.
// Shape: one `push ecx` frame for the lookup key; the mode manager rides in esi for both calls (the landed 0x008B7640 lookup with the
// HasMode guard of 0x00977A30). No engine header matches the retail layout (CCreatureActionBase is 0xa8 bytes in retail), so `this` is modelled locally.
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
class CTCCreatureModeManager { public:
    void RemoveMode(unsigned long mode);    // 0x0073ABA0
public:
    bool IsModeActive(unsigned long) const;   // 0x0073A8A0
};

class CCombatActionBase { public:                  // retail sizeof 0x114
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void FinishAction();            // slot 4: 0x00859A20
    unsigned char _pad04[4];                // +0x04
    CIntelligentPointer<CThing> PCreature;      // +0x08 CIntelligentPointer<CThingCreatureBase>
    unsigned char _pad10[0x114 - 0x10];     // +0x10
};

class CCreatureAction_SummonerLightningOrbOutOfGroundSmash : public CCombatActionBase { public:
    virtual void FinishAction();
};

void CCreatureAction_SummonerLightningOrbOutOfGroundSmash::FinishAction()
{
    CCombatActionBase::FinishAction();
    CTCCreatureModeManager* pModes;
    if (PCreature.GetPItem()->GetInterface(&pModes, 0x31)) {
        if (pModes->IsModeActive(90))
            pModes->RemoveMode(90);
    }
}