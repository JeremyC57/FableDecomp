// CCreatureAction_SendMessage::PerformAction() -- if the receiver is alive, post the action's message (def, position, sender = this creature, receiver, extra) to the world's message-event manager
// Grade: DIFFER(106v110) -- retail calls GetWorld() BEFORE pushing the five AddMessage arguments (receiver/sender/def held in ebx/edi/ebp, Extra and &Position read after the call); the lane pushes the arguments first and fetches the manager last
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008491E0, length 110 bytes (+ int3 padding).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PReceiver at +0xb8 twice for the alive test and once for the argument,
// PCreature at +8 twice), CCreatureActionBase::GetWorld @ 0x00692940 (the manifest "_Dest_val<...>" label is propagated; the result's +0x60 is the
// message-event manager), CMessageEventManager::AddMessage(CMessageEventDef*, const C3DVector*, CThing*, CThing*, long) @ 0x00687730.
// Shape: the landed IS_THING_ALIVE idiom on PReceiver; then retail calls PCreature.GetPItem() once and DROPS the result (the eax of the first
// call is overwritten by the receiver fetch) -- a discarded expression statement in the source; the AddMessage arguments are evaluated right
// to left (receiver into ebx, sender into edi, the def word into ebp) before the manager is fetched through GetWorld().
// Members: MessageDef at +0xa8 is a counted def pointer (the copy constructor 0x00982750 bumps the count at +4), Position at +0xac,
// PReceiver at +0xb8 (an intelligent pointer), Extra at +0xc0. No engine header for this class; modelled locally (retail CCreatureActionBase is 0xa8 bytes).
struct CMessageEventDef;
struct C3DVector { float X, Y, Z; };
struct CThing {
    unsigned char _pad0[0x91];
    unsigned char m_flags91;                // +0x91 bit 0: dead
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                        // +0x04
    CThing* GetPItem() const;               // 0x00A01B50
};

// IS_THING_ALIVE(ptr): the pointer is set and the thing's dead bit (+0x91 bit 0) is clear (inlined, re-fetching GetPItem).
inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}

struct CMessageEventManager {
    void* AddMessage(CMessageEventDef* def, const C3DVector* position, CThing* sender, CThing* receiver, long extra);   // 0x00687730
};
struct CWorld {
    unsigned char _pad0[0x60];
    CMessageEventManager* MessageEventManager;   // +0x60
};

struct CCreatureActionBase {                // retail sizeof 0xa8
    virtual void v00();
    unsigned char _pad04[4];                // +0x04
    CBaseIntelligentPointer PCreature;      // +0x08 CIntelligentPointer<CThingCreatureBase>
    unsigned char _pad10[0xa8 - 0x10];      // +0x10
    CWorld* GetWorld();                     // 0x00692940
};

struct CCreatureAction_SendMessage : CCreatureActionBase {
    CMessageEventDef* MessageDef;           // +0xa8 (raw word of a counted def pointer)
    C3DVector Position;                     // +0xac
    CBaseIntelligentPointer PReceiver;      // +0xb8 CIntelligentPointer<CThing>
    long Extra;                             // +0xc0
    virtual void PerformAction();
};

void CCreatureAction_SendMessage::PerformAction()
{
    if (IS_THING_ALIVE(PReceiver)) {
        PCreature.GetPItem();               // evaluated and discarded in retail
        CThing* receiver = PReceiver.GetPItem();
        CThing* sender = PCreature.GetPItem();
        CMessageEventDef* def = MessageDef;
        GetWorld()->MessageEventManager->AddMessage(def, &Position, sender, receiver, Extra);
    }
}
