// CCreatureAction_TentacleStrikeOutOf::Initialise() -- remove creature mode 0x53 through the owner's mode manager, clear the tentacle's obstruction and striking state, animation TENTACLE_SMASH_OUT_OF, then the base Initialise
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008CF540, length 201 bytes (+ int3 padding), plain `ret` (virtual).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50, CVectorMap::LowerBound @ 0x0040F020 (inlined GetInterface 0x31 =
// CTCCreatureModeManager, then 0xE3 = CTCKrakenTentacle), CTCCreatureModeManager::RemoveMode(unsigned long) @ 0x0073ABA0,
// CTCKrakenTentacle::RemoveObstruction @ 0x007F0810, CTCKrakenTentacle::SetIsStriking(bool) @ 0x007F0770,
// CCharString::CCharString(const char*, long) @ 0x0099EBF0, CCombatActionBase::SetAnimationName @ 0x008574D0,
// CCharString::~CCharString @ 0x0099EAE0, CCombatActionBase::Initialise @ 0x00857520.
// Shape: the checked mode-manager lookup (component left in eax), the UNCHECKED tentacle lookup in an inline accessor (absent
// arm reads the shared key word, which the block-scoped name string reuses afterwards), the tentacle kept in esi for two calls.
// Layout: owner CIntelligentPointer +8 (CCombatActionBase is 0x114 in retail).
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

class CCharString { public:
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};

struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CThing* GetPItem() const;                   // 0x00A01B50
};

class CTCCreatureModeManager { public:
    void RemoveMode(unsigned long mode);        // 0x0073ABA0
};
class CTCKrakenTentacle { public:
    void RemoveObstruction();                   // 0x007F0810
    void SetIsStriking(bool striking);          // 0x007F0770
};

class CCombatActionBase { public:
    virtual void Initialise();                  // 0x00857520
    unsigned char           _pad04[4];          // +0x04
    CBaseIntelligentPointer PCreature;          // +0x08
    unsigned char           _pad10[0x114 - 0x10];
    void SetAnimationName(const CCharString& name);   // 0x008574D0
    __forceinline CTCKrakenTentacle* GetTentacle() const
    {
        CTCKrakenTentacle* tentacle;
        PCreature.GetPItem()->GetInterface(&tentacle, 0xe3);
        return tentacle;
    }
};

class CCreatureAction_TentacleStrikeOutOf : public CCombatActionBase { public:
    virtual void Initialise();
};

void CCreatureAction_TentacleStrikeOutOf::Initialise()
{
    CTCCreatureModeManager* modeManager;
    if (PCreature.GetPItem()->GetInterface(&modeManager, 0x31))
        modeManager->RemoveMode(0x53);
    CTCKrakenTentacle* tentacle = GetTentacle();
    tentacle->RemoveObstruction();
    tentacle->SetIsStriking(false);
    {
        CCharString name("TENTACLE_SMASH_OUT_OF", -1);
        SetAnimationName(name);
    }
    CCombatActionBase::Initialise();
}