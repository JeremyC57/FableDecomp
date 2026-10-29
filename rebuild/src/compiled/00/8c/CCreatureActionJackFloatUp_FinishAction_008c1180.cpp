// CCreatureAction_JackFloatUp::FinishAction() -- make sure Jack's mode manager holds mode 0x31 (floating), then the base FinishAction
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008C1180, length 101 bytes (+ int3 padding).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PCreature at +8), CVectorMap::LowerBound @ 0x0040F020 (the inlined
// GetInterface lookup for type 0x31 = bit 17 of the presence word at +0x24), CTCCreatureModeManager::IsModeActive @ 0x0073A8A0,
// CTCCreatureModeManager::AddMode @ 0x0073AE10, CCombatActionBase::FinishAction @ 0x00859A20 (direct base call, last).
// Shape: the GetInterface result is NOT tested -- the absent path reads the uninitialised out pointer from the lookup-key
// frame word (`mov esi,[esp+8]`), the landed out-parameter GetInterface template (0x007504E0) reproduces that; the mode
// manager rides in esi for both calls. Retail CCombatActionBase is 0x114 bytes (the PDB header sits 0xc high), so `this`
// is modelled locally like the landed 0x008CB5C0.
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
            CInterfaceMap* map = &m_interfaces;
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
class CTCCreatureModeManager { public:             // interface type 0x31
    bool AddMode(unsigned long mode);       // 0x0073AE10
public:
    bool IsModeActive(unsigned long) const;   // 0x0073A8A0
};

class CCombatActionBase { public:                  // retail sizeof 0x114
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    protected: virtual void FinishAction(); public:            // slot 4: 0x00859A20
    unsigned char _pad04[4];                // +0x04
    CIntelligentPointer<CThing> PCreature;      // +0x08 CIntelligentPointer<CThingCreatureBase>
    unsigned char _pad10[0x114 - 0x10];     // +0x10
};

class CCreatureAction_JackFloatUp : public CCombatActionBase { public:
    virtual void FinishAction();
};

void CCreatureAction_JackFloatUp::FinishAction()
{
    CTCCreatureModeManager* modes;
    PCreature.GetPItem()->GetInterface(&modes, 0x31);
    if (!modes->IsModeActive(0x31))
        modes->AddMode(0x31);
    CCombatActionBase::FinishAction();
}