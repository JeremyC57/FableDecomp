// CCreatureAction_ScreamerIntoAggressive::Initialise() -- push a new NCreatureMode::CModeScreamerAggressive onto the creature's mode manager (built with the target when the target is alive, else creature-only), post message 0xC (flags 0xF) at the creature's position from the creature to the target, name the "INTO_COMBAT" animation and run the base Initialise
// Grade: DIFFER(321v331) nd=57 (was 61) -- IsDead accessor (Ego_r.pdb ?IsDead@CThing@@QBE_NXZ) instead of the raw m_flags91 test; levers tried: ternary new (57), lookup after the mode (122), engine/CCountedPointer.h (372: the header's CCountedPointer(T*) inlines where retail calls 0x00718A00 out of line, so the local declaration-only model stays), inline_depth(0) (263); remaining: this/map/&PTarget register roles (retail ebp/esi/ebx) and the first-arm new spilled to a second frame word
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0096DBC0, length 331 bytes (+ int3 padding).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PCreature +8, PTarget +0xa8), CVectorMap::LowerBound @ 0x0040F020 (inlined
// untested GetInterface 0x31 = CTCCreatureModeManager, bit 17 of +0x24), operator new @ 0x00BFEA1A (0x84-byte mode),
// NCreatureMode::CModeScreamerAggressive(CThing*, CThing*) @ 0x0096DD10 and (CThing*) @ 0x007412B0 (vtable 0x012681EC; the manifest
// label "CModeJackOfBladesFloat" is propagated), CCountedPointer<CModeBase>::CCountedPointer(CModeBase*) @ 0x00718A00 (out of line,
// built in the argument area), CTCCreatureModeManager::AddMode @ 0x0073ACC0, CCreatureActionBase::GetPWorld @ 0x00692940, CThing::
// GetPosition @ 0x004C73D0, CMessageEventManager::AddMessage @ 0x00687540 (the world's manager at +0x60), CCharString ctor/dtor
// @ 0x0099EBF0 / 0x0099EAE0, CCombatActionBase::SetAnimationName @ 0x008574D0 and Initialise @ 0x00857520.
// Layout: retail CCombatActionBase is 0x114 bytes: PCreature +0x08, PTarget +0xa8.
// Shape: IS_THING_ALIVE on PTarget picks the mode ctor (each arm null-checks its `new`; the first arm spills the block to the frame);
// the landed CCreatureAction_GiveItemToThing 0x00842F70 message model (two named creature locals, the manager named before the
// arguments); the animation name is an implicit conversion temporary.
struct C3DVector;
struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    bool IsDead() const { return (m_flags91 & 1) != 0; }   // Ego_r.pdb ?IsDead@CThing@@QBE_NXZ
    const C3DVector& GetPosition() const;       // 0x004C73D0
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
    CThing* m_pItem;                            // +0x04
    CThing* GetPItem() const;                   // 0x00A01B50
};
inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!p.GetPItem()->IsDead())
            return true;
    }
    return false;
}
struct CCharString {
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                             // 0x0099EAE0
};
struct CModeBase { };
struct CModeScreamerAggressive : CModeBase {    // retail sizeof 0x84, vtable 0x012681EC
    unsigned char _body[0x84];
    CModeScreamerAggressive(CThing* creature, CThing* target);   // 0x0096DD10
    CModeScreamerAggressive(CThing* creature);  // 0x007412B0
};
struct SCountBlock;
template <class T> struct CCountedPointer {
    T*           m_pObject;                     // +0
    SCountBlock* m_pCount;                      // +4
    CCountedPointer(T* object);                 // 0x00718A00
    CCountedPointer(const CCountedPointer& other);
};
struct CTCCreatureModeManager {
    bool AddMode(CCountedPointer<CModeBase> mode);   // 0x0073ACC0
};
struct CMessageEvent;
struct CMessageEventManager {
    CMessageEvent* AddMessage(long type, unsigned long flags, const C3DVector* position, CThing* thing1, CThing* thing2, float a, float b, long priority);   // 0x00687540
};
struct CWorld {
    unsigned char _pad[0x60];
    CMessageEventManager* m_pMessageEventManager;   // +0x60
};
struct CCombatActionBase {                      // retail sizeof 0x114
    virtual void v00();
    virtual void Initialise();                  // 0x00857520
    unsigned char           _pad04[4];
    CBaseIntelligentPointer PCreature;          // +0x08
    unsigned char           _pad10[0xa8 - 0x10];
    CBaseIntelligentPointer PTarget;            // +0xa8
    unsigned char           _padb0[0x114 - 0xb0];
    CWorld* GetPWorld() const;                  // 0x00692940
    void SetAnimationName(const CCharString& name);   // 0x008574D0
};
struct CCreatureAction_ScreamerIntoAggressive : CCombatActionBase {
    virtual void Initialise();
};

void CCreatureAction_ScreamerIntoAggressive::Initialise()
{
    CTCCreatureModeManager* modeManager;
    PCreature.GetPItem()->GetInterface(&modeManager, 0x31);
    CModeScreamerAggressive* mode;
    if (IS_THING_ALIVE(PTarget))
        mode = new CModeScreamerAggressive(PCreature.GetPItem(), PTarget.GetPItem());
    else
        mode = new CModeScreamerAggressive(PCreature.GetPItem());
    modeManager->AddMode(CCountedPointer<CModeBase>(mode));
    CThing* pCreatureThing = PCreature.GetPItem();
    CThing* pCreature = PCreature.GetPItem();
    CMessageEventManager* pManager = GetPWorld()->m_pMessageEventManager;
    pManager->AddMessage(0xc, 0xf, &pCreature->GetPosition(), pCreatureThing, PTarget.GetPItem(), 0.0f, 0.0f, 0);
    SetAnimationName("INTO_COMBAT");
    CCombatActionBase::Initialise();
}
