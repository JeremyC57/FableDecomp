// CAIStateGroup_TavernKeeper::ReceivePayment() -- walk to the jug point, post a payment message to the payee, wait up to 20 s for payment, then play RECEIVE_MONEY_AT_BAR
// Leaf: ReceivePayment
// Grade: DIFFER(445v426) -- VC7.1 RTM sinks the first `FreeBarPoint(); SetPushableByHero(true); return false;` block (WalkToPoint failed / payment cancelled) to the end of the function, retail keeps it as the fall-through of the cancel test (same block-placement gap as 0x0096AF60); vector/action frame slots 8 bytes apart; RemoveMessage this/arg order
// Retail: Fable.exe (TLC, VC7.1) @ 0x009692C0; best of the probes graded under VC7.1 RTM (probe/009692c0d.cpp).
struct CTCBase;
struct C3DVector {
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
};
struct CRightHandedSet { float m[6]; };
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
    unsigned char _pad1[0x91 - 0x4c];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
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
    CThing* m_pItem;
    CBaseIntelligentPointer();                  // 0x00A01B10
    CThing* GetPItem() const;                   // 0x00A01B50
    void SetPItem(CThing* item);                // 0x00A01B90
    ~CBaseIntelligentPointer();                 // 0x00A01C10
};
struct CCharString {
    void* m_pData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                 // 0x0099EAE0
};
float __fastcall GFFloatRandom(float range, unsigned long* seed);   // 0x004978A0
struct CThingCreatureBase;
struct CTCTavernBar {                           // interface 0xb9
    bool GetRandomBarSlot(CThing& creature, C3DVector& position, CRightHandedSet& orientation, long& actionPoint, long& slot);  // 0x007BA3C0
    bool HasEmptyJugSlot(CThing* creature);     // 0x007BA8C0
};
struct CSoundPair {
    long m_first;
    long m_second;
    CSoundPair() : m_first(0), m_second(0) { }
};
struct CCreatureActionBase {
    virtual void v00();
    unsigned char _body[0x9c];                  // sizeof(CCreatureAction_PlayAnimation) = 0xac (retail frame 0xc4)
    ~CCreatureActionBase();                     // 0x00693EF0
};
struct CThingCreatureBase;
struct CCreatureAction_PlayAnimation : CCreatureActionBase {
    unsigned char _anim[0x0c];
    CCreatureAction_PlayAnimation(CThingCreatureBase* creature, const CCharString& anim, const CSoundPair& sound,
        bool a, bool b, bool c, long d, long e);    // 0x008424F0
};
struct CMessageEvent;
struct CMessageEventManager {
    void RemoveMessage(const CMessageEvent* message);      // 0x00493B5B
    CMessageEvent* AddMessage(long type, unsigned long lifetime, C3DVector* pos, CThing* from, CThing* to,
        float a, float b, long c);                      // 0x00687540
};
struct CWorld {
    unsigned char _pad[0x60];
    CMessageEventManager* PMessageManager;      // +0x60
    static long GetWorldFrame();                // 0x0049D870
};
extern long g_FramesPerSecond;                  // 0x01375550
struct CThingCreatureBase;
struct CAIBrain {
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};
struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;
    unsigned char Enabled;
    unsigned char _pad_0x13;
    CWorld* GetPWorld();                        // 0x008FCFC0
    CThingCreatureBase* GetCreatureBase() const { return (CThingCreatureBase*)PBrain->PCreature; }
    bool IsStopping();                          // 0x008FD010
    void SetPushableByHero(bool pushable);      // 0x0097E330
    long AIAction_DoNothing();                  // 0x0097E200
    long AIAction_PerformActionAndWait(CCreatureActionBase* action, bool b);   // 0x0097DB70
};
__forceinline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}
struct CAIStateGroup_TavernKeeper : CAIStateGroupBase {
    CBaseIntelligentPointer PJug;               // +0x14 CIntelligentPointer<CThingObject>
    CBaseIntelligentPointer PBar;               // +0x1c CIntelligentPointer<CThingObject>
    CBaseIntelligentPointer PPub;               // +0x24 CIntelligentPointer<CThingBuilding>
    CBaseIntelligentPointer PPayee;             // +0x2c CIntelligentPointer<CThingAICreature>
    long JugActionPointIndex;                   // +0x34
    C3DVector JugPos;                           // +0x38
    CRightHandedSet JugRHSet;                   // +0x44
    long JugSlot;                               // +0x5c
    float AverageWalkSpeed;                     // +0x60
    CMessageEvent* PaymentRequestMessage;       // +0x64
    bool WalkToPoint(C3DVector* pos, CRightHandedSet* rh);   // 0x00968230
    bool HasBeenPaid();                         // 0x00968460
    bool GetPaymentCancelledMessage();          // 0x00968510
    void FreeBarPoint();                        // 0x00968950
    bool ReceivePayment();
};

bool CAIStateGroup_TavernKeeper::ReceivePayment()
{
    SetPushableByHero(false);
    if (!WalkToPoint(&JugPos, &JugRHSet) || GetPaymentCancelledMessage()) {
        FreeBarPoint();
        SetPushableByHero(true);
        return false;
    } else {
        {
            CMessageEventManager* requests = GetPWorld()->PMessageManager;
            requests->RemoveMessage(PaymentRequestMessage);
        }
        CThing* creature = PBrain->PCreature;
        C3DVector pos(0.0f, 0.0f, 0.0f);
        CMessageEventManager* messages = GetPWorld()->PMessageManager;
        CMessageEvent* message = messages->AddMessage(0x30, 1000, &pos, creature, PPayee.GetPItem(), 0.0f, 0.0f, 0);
        long fps = g_FramesPerSecond;
        long endFrame = CWorld::GetWorldFrame() + fps * 20;
        while (!HasBeenPaid()) {
            AIAction_DoNothing();
            if (CWorld::GetWorldFrame() > endFrame || IsStopping() || !IS_THING_ALIVE(PPayee)) {
                GetPWorld()->PMessageManager->RemoveMessage(message);
                FreeBarPoint();
                SetPushableByHero(true);
                return false;
            }
        }
        AIAction_PerformActionAndWait(&CCreatureAction_PlayAnimation(GetCreatureBase(),
            "RECEIVE_MONEY_AT_BAR", CSoundPair(), false, true, false, 0, 0), false);
        FreeBarPoint();
        return true;
    }
}
