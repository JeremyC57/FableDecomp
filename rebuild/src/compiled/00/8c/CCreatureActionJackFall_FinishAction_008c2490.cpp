// CCreatureAction_JackFall::FinishAction() -- drop mode 0x33 from Jack's mode manager if set, base FinishAction, then make "go underground" the creature's current action
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008C2490, length 160 bytes.
// Model: rebuild/include/engine/CCreatureAction_JackFall.h is the flattened PDB layout and sits high against the bytes (CCombatActionBase is 0x12c bytes in the PDB, 0x114 in retail: retail containers are 12 bytes), so `this` is a local model with the retail offsets.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PCreature at +8, three times), CVectorMap::LowerBound @ 0x0040F020 (inlined
// GetInterface for type 0x31), CTCCreatureModeManager::HasMode @ 0x0073A8A0, CTCCreatureModeManager::RemoveMode @ 0x0073ABA0,
// CCombatActionBase::FinishAction @ 0x00859A20 (direct base call), CCreatureAction_GoUnderground::CCreatureAction_GoUnderground(CThingCreatureBase*)
// @ 0x008C3810, CThingCreatureBase::SetCurrentAction(CCreatureActionBase*) @ 0x006644F0, CCombatActionBase::~CCombatActionBase @ 0x0062E900.
// Shape: frame 0x118 = the lookup key word + the 0x114-byte action temporary built inside the call (`&CAction(creature)`, the landed
// 0x0094DBC0 PerformActionAndWait shape); the creature handed to the constructor is fetched BEFORE the base FinishAction (named local in
// esi), the creature whose action is set after it (chained, in edi). GetInterface's result is ignored (absent arm reads the key's word).
// The temporary is destroyed through the base destructor: GoUnderground declares none of its own.
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
    T* GetPItem() const { return reinterpret_cast<T*>(CBaseIntelligentPointer::GetPItem()); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem(reinterpret_cast<CBaseObject*>(item)); }
};
class CTCCreatureModeManager { public:             // interface type 0x31
    bool HasMode(unsigned long mode);       // 0x0073A8A0
    void RemoveMode(unsigned long mode);    // 0x0073ABA0
};

class CThingCreatureBase;
class CCreatureActionBase { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void FinishAction();            // slot 4
    unsigned char _pad04[4];                // +0x04
    CIntelligentPointer<CThing> PCreature;      // +0x08 CIntelligentPointer<CThingCreatureBase>
    unsigned char _pad10[0xa8 - 0x10];      // +0x10
};
class CThingCreatureBase { public:
    bool SetCurrentAction(CCreatureActionBase* action);   // 0x006644F0
};
class CCombatActionBase : public CCreatureActionBase { public:          // retail sizeof 0x114
    unsigned char _combat[0x114 - 0xa8];    // +0xa8
    virtual ~CCombatActionBase();                   // 0x0062E900
    virtual void FinishAction();            // 0x00859A20
    CThingCreatureBase* GetCreature() const { return (CThingCreatureBase*)PCreature.GetPItem(); }
};
namespace N_008c3810 { struct CCreatureAction_GoUnderground : CCombatActionBase {
    CCreatureAction_GoUnderground(CThingCreatureBase* creature);   // 0x008C3810
}; } using N_008c3810::CCreatureAction_GoUnderground;
class CCreatureAction_JackFall : public CCombatActionBase { public:
    virtual void FinishAction();
};

void CCreatureAction_JackFall::FinishAction()
{
    CTCCreatureModeManager* modes;
    PCreature.GetPItem()->GetInterface(&modes, 0x31);
    if (modes->HasMode(0x33))
        modes->RemoveMode(0x33);
    CThingCreatureBase* creature = GetCreature();
    CCombatActionBase::FinishAction();
    GetCreature()->SetCurrentAction(&CCreatureAction_GoUnderground(creature));
}