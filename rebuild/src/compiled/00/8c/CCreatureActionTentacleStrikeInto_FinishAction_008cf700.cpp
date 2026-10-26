// CCreatureAction_TentacleStrikeInto::FinishAction() -- base FinishAction, add creature mode 0x53 through the owner's mode manager, queue the tentacle's strike damage, then make a CCreatureAction_TentacleStrikeLoop the owner's current action
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008CF700, length 203 bytes (+ int3 padding), plain `ret` (virtual).
// Callees: CCombatActionBase::FinishAction @ 0x00859A20, CBaseIntelligentPointer::GetPItem @ 0x00A01B50, CVectorMap::LowerBound
// @ 0x0040F020 (inlined GetInterface 0x31 = CTCCreatureModeManager), CTCCreatureModeManager::AddMode(unsigned long) @ 0x0073AE10,
// CTCKrakenTentacle::QueueStrikeDamage @ 0x007F1170, CCombatActionBase::CCombatActionBase(CThingCreatureBase*, long,
// const CSoundPair&) @ 0x00858030 (action code 150), CThingCreatureBase::SetCurrentAction @ 0x006644F0, ~CCombatActionBase
// @ 0x0062E900.
// Shape: the checked mode-manager lookup (component in eax); the next action is a 0x120-byte temporary built in the
// SetCurrentAction call (as 0x008CF610 / landed 0x008C5370): base call, vtable 0x01284EF4, then the init-list members
// StartTick = -1, Interrupt = false, StartHealth = 0 through the zero register shared with the CSoundPair() temporary.
// Layout: Ego_r.pdb PTCTentacle +0x12c (retail +0x114); CCreatureAction_TentacleStrikeLoop StartTick / Interrupt /
// StartHealth at PDB +0x12c / +0x130 / +0x134, retail +0x114 / +0x118 / +0x11c (CCombatActionBase 0x114 in retail).
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
    bool AddMode(unsigned long mode);           // 0x0073AE10
};
class CTCKrakenTentacle { public:
    void QueueStrikeDamage();                   // 0x007F1170
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
};

class CCreatureAction_TentacleStrikeLoop : public CCombatActionBase { public:   // vtable 0x01284EF4
    long  StartTick;                            // +0x114
    bool  Interrupt;                            // +0x118
    float StartHealth;                          // +0x11c
    virtual void v00();
    CCreatureAction_TentacleStrikeLoop(CThingCreatureBase* owner)
        : CCombatActionBase(owner, 150, CSoundPair()), StartTick(-1), Interrupt(false), StartHealth(0) { }
};

class CCreatureAction_TentacleStrikeInto : public CCombatActionBase { public:
    CTCKrakenTentacle* PTCTentacle;             // +0x114 (PDB +0x12c)
    void FinishAction();
};

void CCreatureAction_TentacleStrikeInto::FinishAction()
{
    CCombatActionBase::FinishAction();
    CTCCreatureModeManager* modeManager;
    if (PCreature.GetPItem()->GetInterface(&modeManager, 0x31))
        modeManager->AddMode(0x53);
    PTCTentacle->QueueStrikeDamage();
    PCreature.GetPItem()->SetCurrentAction(CCreatureAction_TentacleStrikeLoop(PCreature.GetPItem()));
}