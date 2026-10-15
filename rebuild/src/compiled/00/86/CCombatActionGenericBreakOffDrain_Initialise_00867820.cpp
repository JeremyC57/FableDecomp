// CCombatAction_GenericBreakOffDrain::Initialise() -- picks the BREAK_OUT combat animation, removes mode 69 from the creature's mode manager, then runs the base Initialise
// Leaf: ?Initialise@CCombatAction_GenericBreakOffDrain
// Retail: Fable.exe (TLC, VC7.1) @ 0x00867820, 144-byte slot (142 bytes of code). Initialise is vtable slot 12.
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0, CCombatActionBase::SetCombatAnimation(const CCharString&)
//          @ 0x008574D0 (manifest label "CTCInGameMenu::SetPDef" is propagated), CCharString::~CCharString @ 0x0099EAE0,
//          CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PCreature at +8), CVectorMap::LowerBound @ 0x0040F020 (inlined
//          GetInterface for type 0x31), CTCCreatureModeManager::RemoveMode @ 0x0073ABA0, CCombatActionBase::Initialise @ 0x00857520.
// Shape: the animation name is a named CCharString in its own block (`push -1 ; push lit` then `lea` re-addressing);
// its frame word is reused for the lookup key. The GetInterface result is NOT tested: RemoveMode runs on both
// paths (on the absent path with the uninitialised out pointer read from the frame -- the landed out-parameter
// template reproduces that garbage read), so the RemoveMode + base Initialise tail is duplicated per branch.
struct CCharString {
    void* m_pData;
    CCharString(const char* text, long length);         // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};

struct CTCBase;
struct CInterfaceMap {                      // a thing's sorted {type, component} vector map at +0x44
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);   // 0x0040F020
};
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];         // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;             // +0x44
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
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                        // +4
    CThing* GetPItem() const;               // 0x00A01B50
};
struct CTCCreatureModeManager {
    void RemoveMode(unsigned long mode);    // 0x0073ABA0
};

struct CCombatActionBase {                  // retail sizeof 0x114
    virtual void v00();
    virtual void Initialise();              // slot 12: 0x00857520
    unsigned char _pad04[4];                // +0x04
    CBaseIntelligentPointer PCreature;      // +0x08 CIntelligentPointer<CThingCreatureBase>
    unsigned char _pad10[0x114 - 0x10];     // +0x10
    void SetCombatAnimation(const CCharString& name);   // 0x008574D0
};

struct CCombatAction_GenericBreakOffDrain : CCombatActionBase {
    virtual void Initialise();
};

void CCombatAction_GenericBreakOffDrain::Initialise()
{
    {
        CCharString name("BREAK_OUT", -1);
        SetCombatAnimation(name);
    }
    CTCCreatureModeManager* pModes;
    PCreature.GetPItem()->GetInterface(&pModes, 0x31);
    pModes->RemoveMode(69);
    CCombatActionBase::Initialise();
}