// CCreatureAction_SendMessage::PerformAction() -- if the receiver is alive, post the action's message (def, position, sender = this creature, receiver, extra) to the world's message-event manager
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008491E0, length 110 bytes (+ int3 padding).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PReceiver at +0xb8 twice for the alive test and once for the argument,
// PCreature at +8 twice), CCreatureActionBase::GetWorld @ 0x00692940 (the manifest "_Dest_val<...>" label is propagated; the result's +0x60 is the
// message-event manager), CMessageEventManager::AddMessage(CMessageEventDef*, const C3DVector*, CThing*, CThing*, long) @ 0x00687730.
// Shape: the landed IS_THING_ALIVE idiom on PReceiver; the post goes through an inline PostMessage(def, sender, receiver) helper whose
// body FIRST takes `CWorld* world = GetWorld()` into a local: that puts the world call after the three register arguments and before the
// pushes (a chained `GetWorld()->Manager->AddMessage(...)` pushes first and fetches the manager last, 106v110); then retail calls PCreature.GetPItem() once and DROPS the result (the eax of the first
// call is overwritten by the receiver fetch) -- a discarded expression statement in the source; the AddMessage arguments are evaluated right
// to left (receiver into ebx, sender into edi, the def word into ebp) before the manager is fetched through GetWorld().
// Members: MessageDef at +0xa8 is a counted def pointer (the copy constructor 0x00982750 bumps the count at +4), Position at +0xac,
// PReceiver at +0xb8 (an intelligent pointer), Extra at +0xc0. No engine header for this class; modelled locally (retail CCreatureActionBase is 0xa8 bytes).
class CMessageEventDef;
class C3DVector { public: float X, Y, Z; };
class CThing { public:
    unsigned char _pad0[0x91];
    unsigned char m_flags91;                // +0x91 bit 0: dead
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

// IS_THING_ALIVE(ptr): the pointer is set and the thing's dead bit (+0x91 bit 0) is clear (inlined, re-fetching GetPItem).
inline bool IS_THING_ALIVE(const CIntelligentPointer<CThing>& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}

class CMessageEventManager { public:
    void* AddMessage(CMessageEventDef* def, const C3DVector* position, CThing* sender, CThing* receiver, long extra);   // 0x00687730
};
class CWorld { public:
    unsigned char _pad0[0x60];
    CMessageEventManager* MessageEventManager;   // +0x60
};

class CCreatureActionBase { public:                // retail sizeof 0xa8
    virtual void v00();
    unsigned char _pad04[4];                // +0x04
    CIntelligentPointer<CThing> PCreature;      // +0x08 CIntelligentPointer<CThingCreatureBase>
    unsigned char _pad10[0xa8 - 0x10];      // +0x10
    CWorld* GetWorld();                     // 0x00692940
};

class CCreatureAction_SendMessage : public CCreatureActionBase { public:
    CMessageEventDef* MessageDef;           // +0xa8 (raw word of a counted def pointer)
    C3DVector Position;                     // +0xac
    CIntelligentPointer<CThing> PReceiver;      // +0xb8 CIntelligentPointer<CThing>
    long Extra;                             // +0xc0
    virtual void PerformAction();
    void PostMessage(CMessageEventDef* def, CThing* sender, CThing* receiver)
    {
        CWorld* world = GetWorld();
        world->MessageEventManager->AddMessage(def, &Position, sender, receiver, Extra);
    }
};

void CCreatureAction_SendMessage::PerformAction()
{
    if (IS_THING_ALIVE(PReceiver)) {
        PCreature.GetPItem();               // evaluated and discarded in retail
        CThing* receiver = PReceiver.GetPItem();
        CThing* sender = PCreature.GetPItem();
        PostMessage(MessageDef, sender, receiver);
    }
}