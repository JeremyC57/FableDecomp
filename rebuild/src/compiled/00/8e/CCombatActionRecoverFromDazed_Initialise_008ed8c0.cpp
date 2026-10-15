// CCombatAction_RecoverFromDazed::Initialise() -- resumes a player creature's suspended animation, removes mode 67 from its mode manager, then runs the base Initialise
// Leaf: ?Initialise@CCombatAction_RecoverFromDazed
// Retail: Fable.exe (TLC, VC7.1) @ 0x008ED8C0, 144-byte slot (139 bytes of code). Initialise is vtable slot 12.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PCreature at +8, three times through one `lea esi,[edi+8]`),
//          CThingAnimatable::SetSuspendAnimation(bool, long) @ 0x00833A10, CVectorMap::LowerBound @ 0x0040F020 (inlined
//          GetInterface for type 0x31), CTCCreatureModeManager::RemoveMode @ 0x0073ABA0, CCombatActionBase::Initialise @ 0x00857520.
// Shape: the thing-type byte at +0x10 (1 = the player creature, as in the landed CModeKnockedDown::OnDeactivate)
// gates SetSuspendAnimation(false, 0); the GetInterface result is not tested, so RemoveMode + the base call are
// duplicated on both paths (the absent path reads the uninitialised out pointer from the `push ecx` frame word).
struct CTCBase;
struct CInterfaceMap {                      // a thing's sorted {type, component} vector map at +0x44
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);   // 0x0040F020
};
struct CThing {
    unsigned char _pad0[0x10];
    unsigned char m_type;                   // +0x10 thing type (1 = the player creature)
    unsigned char _pad11[0x20 - 0x11];
    unsigned long m_presentBits[9];         // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;             // +0x44
    void SetSuspendAnimation(bool suspend, long reason);   // 0x00833A10 (CThingAnimatable)
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
};

struct CCombatAction_RecoverFromDazed : CCombatActionBase {
    virtual void Initialise();
};

void CCombatAction_RecoverFromDazed::Initialise()
{
    if (PCreature.GetPItem()->m_type == 1) {
        CThing* pCreature = PCreature.GetPItem();   // named: retail fetches it before the argument pushes
        pCreature->SetSuspendAnimation(false, 0);
    }
    CTCCreatureModeManager* pModes;
    PCreature.GetPItem()->GetInterface(&pModes, 0x31);
    pModes->RemoveMode(67);
    CCombatActionBase::Initialise();
}