// CAIStateGroup_MinionReactToPlayerInteraction::Update() -- take the player's latest interaction request aimed at this creature, offer a trade when the shopkeeper reaction is set, step to a comfortable distance and face the player, wait out any speech, greet as a wife, record the interaction and handle a pending confirmation
// Grade: DIFFER(1487v1513) nd=171 (was 1486v1513 nd=178) -- named binder filter + named message manager in a scoped block; residue: register roles across the body, the shop/rule TestTC pair narrowed, wife double bit test merged, x87/FastInvSqrt reg
// Layout: local model (the engine header flattens the base); ReactionType +0x28, WaitingForConfirmation +0x2c, DidTraderInteraction +0x2d, PositionPlayerWasWhenWaitingForConfirmation +0x2e.
#include <stdlib.h>
struct CTCBase;
extern unsigned long g_InvSqrtMantissaTable[128];      // 0x013CE558
__forceinline float FastInvSqrt(float lengthSquared)
{
    unsigned long bits = *(unsigned long*)&lengthSquared;
    unsigned long estimate = (((0x17c - ((bits >> 23) & 0xff)) << 22) & 0xff800000) |
                             (g_InvSqrtMantissaTable[(bits >> 17) & 0x7f] << 15);
    float r = *(float*)&estimate;
    return (3.0f - r * r * lengthSquared) * r * 0.5f;
}
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); }
    C3DVector operator*(float f) const { return C3DVector(X * f, Y * f, Z * f); }
    __forceinline void Normalise() { float inv = FastInvSqrt(X * X + Y * Y + Z * Z); X *= inv; Y *= inv; Z *= inv; }
};
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
    unsigned char _pad1[0x45];
    unsigned char m_flags91;                    // +0x91
    const C3DVector& GetPosition() const;       // 0x004C73D0
    bool TestTC(long interfaceType) const
    {
        return ((m_presentBits[interfaceType >> 5] >> (interfaceType & 31)) & 1) != 0;
    }
    unsigned long HasTC(long interfaceType) const
    {
        return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31));
    }
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
    template<class T> __forceinline bool GetInterfaceT(T** ppOut, long interfaceType)
    {
        bool has = TestTC(interfaceType);
        if (has) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
        }
        return has;
    }
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;
    CBaseIntelligentPointer();                  // 0x00A01B10
    ~CBaseIntelligentPointer();                 // 0x00A01C10
    CThing* GetPItem() const;                   // 0x00A01B50
    void SetPItem(CThing* item);                // 0x00A01B90
};
template<class T> struct CIntelligentPointer : CBaseIntelligentPointer { };
struct CThingPlayerCreature;
__forceinline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}
struct CIntelligentPointerConstThing : CBaseIntelligentPointer {   // vtable 0x01237F1C
    virtual void v00();
    CIntelligentPointerConstThing(const CBaseIntelligentPointer& o) { SetPItem(o.GetPItem()); }
};
struct CCharString {
    void* PData;
    ~CCharString();                             // 0x0099EAE0
};
struct CThingCreatureBase : CThing {
    const C3DVector& GetPosition() const;       // 0x006A4D00
    bool IsFreeToFinishAction();                // 0x006621C0
    void FinishCurrentAction();                 // 0x00662650
};
struct CMessageEvent {
    virtual ~CMessageEvent();
    unsigned char _pad4[0x3c - 4];
    long Answer;                                // +0x3c
};
struct CMessageEventFilter_HasExtraData {
    long m_type;
    CMessageEventFilter_HasExtraData(long type) : m_type(type) { }
    bool operator()(CMessageEvent* event) const;    // 0x006AF1C0
};
struct SMessageNode {
    SMessageNode* m_pNext;
    SMessageNode* m_pPrev;
    CMessageEvent m_value;
};
struct CMessageEventManager {
    void* World;
    SMessageNode* m_pHead;                      // +0x04
    template<class TFilter> CMessageEvent* FindMostRecentMessage(const TFilter& filter)
    {
        for (SMessageNode* node = m_pHead->m_pNext; node != m_pHead; node = node->m_pNext) {
            CMessageEvent* event = &node->m_value;
            if (filter(event))
                return event;
        }
        return 0;
    }
    void RemoveMessage(const CMessageEvent* event)
    {
        for (SMessageNode* node = m_pHead->m_pNext; node != m_pHead; node = node->m_pNext) {
            if (&node->m_value == event) {
                node->m_pPrev->m_pNext = node->m_pNext;
                node->m_pNext->m_pPrev = node->m_pPrev;
                node->m_value.~CMessageEvent();
                free(node);
                return;
            }
        }
    }
};
struct CWorld {
    unsigned char _pad[0x60];
    CMessageEventManager* PMessageEventManager; // +0x60
};
long __fastcall GFGetCurrentFrame();            // 0x0049D870
struct CTCVillage {
    unsigned char _pad[0xa8];
    unsigned long NextFineFrame;                // +0xa8
    long GetFine();                             // 0x006588CF
};
struct CTCVillageMember;
CTCVillage* __fastcall GetVillage(CTCVillageMember* member);   // 0x007EE540
struct CTCHeroStats {
    unsigned char _pad[0x108];
    long FinesPaid;                             // +0x108
    void AddMoney(long amount, bool showMessage);   // 0x0057B327
};
struct CTCHeroOnlineScoreboard {
    void UpdateScoreForActivity(long activity, float value, long extra);   // 0x0055F3B0
};
struct CTCWife {
    void ReceivedConfirmation(bool yes);        // 0x006D1DE0
};
struct CTCTalk {
    void Say(const CCharString& text, long a, long b, bool c, long d);    // 0x0066BED0
};
struct CTCShopKeeper {
    CCharString GetFriendlynessIndicator();     // 0x00799350
    void OnTradeAccepted();                     // 0x00799120
};
struct CPlayerCharacterPointer : CBaseIntelligentPointer { };
struct CPlayer {
    unsigned char _pad[0x34];
    CPlayerCharacterPointer PCharacter;         // +0x34 CIntelligentPointer<CThingPlayerCreature>
};
struct CPlayerManager {
    const CPlayer* GetMainPlayer() const;       // 0x00449960
};
struct CMessageEventFilter_IsOfType {
    long Type;
    CMessageEventFilter_IsOfType(long type) : Type(type) { }
};
struct CMessageEventFilter_IsOriginatedByThing {
    unsigned __int64 UID;
    CMessageEventFilter_IsOriginatedByThing(const CThing* thing);    // 0x006AF290
};
template <class A, class B> struct CFilter_Bind2 {
    const A* m_pFirst;
    const B* m_pSecond;
    CFilter_Bind2(const A& a, const B& b) : m_pFirst(&a), m_pSecond(&b) { }
};
template <class A, class B> inline CFilter_Bind2<A, B> operator&&(const A& a, const B& b) { return CFilter_Bind2<A, B>(a, b); }
struct CMessageManagerFind {
    CMessageEvent* FindMostRecentMessage(const CFilter_Bind2<CMessageEventFilter_IsOfType, CMessageEventFilter_IsOriginatedByThing>& filter);   // 0x008D46D0
    void RemoveMessage(const CMessageEvent* event);   // 0x00493B5B
};
struct CWorldR {
    unsigned char _pad[0x60];
    CMessageManagerFind* PMessageEventManager;  // +0x60
    unsigned char _pad64[0x104 - 0x64];
    long Mode;                                  // +0x104
};
struct CThingPlayerCreature;
struct CIntelligentPointerPlayer : CBaseIntelligentPointer {   // CIntelligentPointer<CThingPlayerCreature>, vtable 0x01231C4C
    virtual void v00();
    CIntelligentPointerPlayer(CThing* item) { SetPItem(item); }
};
struct CCharStringC {
    void* PData;
    CCharStringC(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharStringC();                            // 0x0099EAE0
};
struct CFontBank;
struct CMainGameComponent {
    unsigned char _pad[0x1614c];
    CFontBank* FontBank;                        // +0x1614c
};
extern CMainGameComponent* g_pMainGameComponent;   // 0x013B86A0
struct CSubtitleRenderer {
    void SetText(CFontBank* font, const CCharStringC& text, unsigned long colour, float time, bool a, bool b);   // 0x00492ED1
};
struct CPlayerGui {
    unsigned char _pad00[0x18];
    CSubtitleRenderer SubtitleRenderer;         // +0x18
    unsigned char _pad19[0xfd - 0x19];
    bool InModalMode;                           // +0xfd
};
extern CPlayerGui* g_pPlayerGui;                // 0x013B8790
struct CTCTrade { unsigned char _pad[0x2a]; bool Busy; };        // interface 0xE8 (+0x2a)
struct CTCDistanceRule { void GetLimits(const C3DVector& a, const C3DVector& b, float* x, float* y); };   // interface 7, 0x006CBA10
struct CTCTalkR { bool IsTalking(); };          // 0x006E28A0
struct CTCWifeR { void OnGreet(); };            // 0x006CFC00
struct CTCHeroFlags { unsigned char _pad[0x340]; bool Interacted; };   // interface 0x4F (+0x340)
float __cdecl GetInteractionDistance();         // 0x00661CC0
extern long g_FramesPerSecond;                  // 0x01375550
struct CAIBrain {
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};
struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    unsigned char _pad08[0x0c];
    void Terminate();                           // 0x008FCF40
    const CPlayerManager* GetPPlayerManager() const;   // 0x008FCF80
    CWorldR* GetPThingWorld();                  // 0x008FCFA0
    CWorldR* GetPWorld();                       // 0x008FCFC0
    long AIAction_MoveToPosProximity(long repeat, C3DVector pos, float radius, bool* arrived, long movement,
                                     bool a, bool b, bool c, CThing* leader, bool d, float e);   // 0x0097E890
    long AIAction_DoNothing();                  // 0x0097E200
    long AIAction_LookAtThing(long repeat, CIntelligentPointerConstThing thing, long flags, float speed);   // 0x0097D6F0
};
bool __fastcall IsThingRunningAwayFromThing(const CThing* runner, const CThing* from, const C3DVector* oldPos);   // 0x00864170
#pragma pack(push, 1)
struct CAIStateGroup_MinionReactToPlayerInteraction : CAIStateGroupBase {
    unsigned long NextReactionFrame[5];         // +0x14
    long ReactionType;                          // +0x28
    bool WaitingForConfirmation;                // +0x2c
    bool DidTraderInteraction;                  // +0x2d
    C3DVector PositionPlayerWasWhenWaitingForConfirmation;   // +0x2e
    bool IsTooFarFromThePlayer(const CThing* player);    // 0x0092ECD0
    bool IsTooNearToThePlayer(const CThing* player);     // 0x0092ED40
    void UpdateWaitingForConfirmation(const CBaseIntelligentPointer& player);   // 0x0092EE10
    bool TestOrDoInteraction(bool test, CThing* player); // 0x0092FAB0
    void Update();
};
#pragma pack(pop)

void CAIStateGroup_MinionReactToPlayerInteraction::Update()
{
    CMessageEvent* event;
    {
        CFilter_Bind2<CMessageEventFilter_IsOfType, CMessageEventFilter_IsOriginatedByThing> filter =
            CMessageEventFilter_IsOfType(0x14) && CMessageEventFilter_IsOriginatedByThing(PBrain->PCreature);
        CMessageManagerFind* events = GetPThingWorld()->PMessageEventManager;
        event = events->FindMostRecentMessage(filter);
    }
    if (event) {
        long interaction = event->Answer;
        CMessageManagerFind* manager = GetPWorld()->PMessageEventManager;
        manager->RemoveMessage(event);
        CIntelligentPointerPlayer player(GetPPlayerManager()->GetMainPlayer()->PCharacter.GetPItem());
        if (!IS_THING_ALIVE(player)) {
            Terminate();
            return;
        }
        if (ReactionType == 3) {
            long mode = GetPWorld()->Mode;
            if ((mode == 0 || mode == 9) && !g_pPlayerGui->InModalMode) {
                CTCTrade* trade;
                if (!player.GetPItem()->GetInterfaceT(&trade, 0xe8) || !trade->Busy) {
                    {
                        CCharStringC text("TEXT_GUI_TRADE_START_TRADING");
                        g_pPlayerGui->SubtitleRenderer.SetText(g_pMainGameComponent->FontBank, text, 4, 0.0f, false, true);
                    }
                    WaitingForConfirmation = true;
                    PositionPlayerWasWhenWaitingForConfirmation = player.GetPItem()->GetPosition();
                    UpdateWaitingForConfirmation(player);
                }
            }
        }
        bool tooFar = IsTooFarFromThePlayer(player.GetPItem());
        bool tooNear = IsTooNearToThePlayer(player.GetPItem());
        CThing* creature = PBrain->PCreature;
        CTCDistanceRule* rule;
        bool hasRule = creature->TestTC(0x90) && creature->GetInterfaceT(&rule, 7);
        if (hasRule) {
            float x, y;
            rule->GetLimits(PBrain->PCreature->GetPosition(), player.GetPItem()->GetPosition(), &x, &y);
            if (x != 0.0f || y != 0.0f) {
                tooNear = false;
                tooFar = false;
            }
        }
        if (((CThingCreatureBase*)PBrain->PCreature)->IsFreeToFinishAction())
            ((CThingCreatureBase*)PBrain->PCreature)->FinishCurrentAction();
        if (tooFar || tooNear) {
            CThingCreatureBase* me = (CThingCreatureBase*)PBrain->PCreature;
            const C3DVector& playerPos = ((CThingCreatureBase*)player.GetPItem())->GetPosition();
            C3DVector direction = me->GetPosition() - playerPos;
            long fps = g_FramesPerSecond;
            direction.Normalise();
            long endFrame = GFGetCurrentFrame() + fps * 10;
            long result;
            do {
                bool arrived;
                C3DVector offset = direction * GetInteractionDistance();
                result = AIAction_MoveToPosProximity(2, offset + ((CThingCreatureBase*)player.GetPItem())->GetPosition(), 0.1f, &arrived, 3,
                                                     tooFar, false, true, player.GetPItem(), true, 0.0f);
            } while (GFGetCurrentFrame() < endFrame && result == 4);
            if (result != 2 && interaction == 5) {
                Terminate();
                return;
            }
            long startFrame = GFGetCurrentFrame();
            do {
                if (AIAction_LookAtThing(1, player, 0xb, 0.0027777778f) != 2) {
                    Terminate();
                    return;
                }
                if (interaction != 5)
                    break;
            } while (GFGetCurrentFrame() - startFrame < g_FramesPerSecond * 5);
        }
        CTCTalkR* talk;
        PBrain->PCreature->GetInterface(&talk, 0xf);
        if (talk->IsTalking()) {
            do {
                if (AIAction_DoNothing() != 4)
                    break;
            } while (talk->IsTalking());
        }
        CThing* self = PBrain->PCreature;
        if (self->HasTC(0xd7)) {
            CTCWifeR* wife = 0;
            self->GetInterface(&wife, 0xd7);
            wife->OnGreet();
        }
        if (TestOrDoInteraction(false, player.GetPItem()) || DidTraderInteraction) {
            CTCHeroFlags* flags;
            if (PBrain->PCreature->GetInterface(&flags, 0x4f))
                flags->Interacted = true;
        }
        if (WaitingForConfirmation)
            UpdateWaitingForConfirmation(player);
    }
    Terminate();
}
