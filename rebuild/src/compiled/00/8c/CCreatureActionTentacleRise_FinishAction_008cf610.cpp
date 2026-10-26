// CCreatureAction_TentacleRise::FinishAction() -- remove creature mode 0x54 through the owner's mode manager, set the flag byte +0x10 of the owner's interface-9 component, base FinishAction, then make a CCreatureAction_TentacleIdle the owner's current action
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008CF610, length 231 bytes (+ int3 padding), plain `ret` (virtual).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50, CVectorMap::LowerBound @ 0x0040F020 (inlined GetInterface 0x31 =
// CTCCreatureModeManager, then 9), CTCCreatureModeManager::RemoveMode(unsigned long) @ 0x0073ABA0, CCombatActionBase::FinishAction
// @ 0x00859A20, CCombatActionBase::CCombatActionBase(CThingCreatureBase*, long, const CSoundPair&) @ 0x00858030 (action code 50),
// CThingCreatureBase::SetCurrentAction @ 0x006644F0, ~CCombatActionBase @ 0x0062E900 (manifest "~CTCLightningOrb" is propagated).
// Shape: the checked mode-manager lookup (component left in eax), then the UNCHECKED interface-9 lookup in an inline accessor
// (absent arm reads the shared key word); the idle action is a 0x114-byte temporary built in the SetCurrentAction call (the
// landed CCreatureAction_WaspQueenDie::FinishAction 0x008C5370 shape): its inline constructor stores the vtable 0x01284DAC
// after the base call, the CSoundPair() temporary is zeroed through ecx after the first two pushes, and the owner for the
// call's `this` is fetched after the temporary is built.
// Layout: owner CIntelligentPointer +8.
class CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
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
};

class CTCCreatureModeManager { public:
    void RemoveMode(unsigned long mode);        // 0x0073ABA0
};
struct CTCInterface9 {                          // the owner's interface-9 component (descriptive)
    unsigned char _pad[0x10];
    bool          m_flag10;                     // +0x10
};

class CSoundPair { public:
    long m_first;
    long m_second;
    CSoundPair() : m_first(0), m_second(0) { }
};

class CCombatActionBase;
class CThingCreatureBase : public CThing { public:
    bool SetCurrentAction(const CCombatActionBase& action);   // 0x006644F0
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

class CCombatActionBase { public:
    virtual void v00();
    unsigned char _pad04[4];                    // +0x04
    CIntelligentPointer<CThingCreatureBase> PCreature;          // +0x08 CIntelligentPointer<CThingCreatureBase>
    unsigned char _pad10[0x114 - 0x10];         // +0x10
    CCombatActionBase(CThingCreatureBase* owner, long actionCode, const CSoundPair& sound);   // 0x00858030
    virtual ~CCombatActionBase();                       // 0x0062E900
    void FinishAction();                        // 0x00859A20
    __forceinline CTCInterface9* GetInterface9() const
    {
        CTCInterface9* component;
        PCreature.GetPItem()->GetInterface(&component, 9);
        return component;
    }
};

class CCreatureAction_TentacleIdle : public CCombatActionBase { public:   // vtable 0x01284DAC
    virtual void v00();
    CCreatureAction_TentacleIdle(CThingCreatureBase* owner) : CCombatActionBase(owner, 50, CSoundPair()) { }
};

class CCreatureAction_TentacleRise : public CCombatActionBase { public:
    void FinishAction();
};

void CCreatureAction_TentacleRise::FinishAction()
{
    CTCCreatureModeManager* modeManager;
    if (PCreature.GetPItem()->GetInterface(&modeManager, 0x31))
        modeManager->RemoveMode(0x54);
    GetInterface9()->m_flag10 = true;
    CCombatActionBase::FinishAction();
    PCreature.GetPItem()->SetCurrentAction(CCreatureAction_TentacleIdle(PCreature.GetPItem()));
}