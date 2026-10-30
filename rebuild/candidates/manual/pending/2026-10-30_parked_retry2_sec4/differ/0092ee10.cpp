// CAIStateGroup_MinionReactToPlayerInteraction::UpdateWaitingForConfirmation(const CIntelligentPointer<CThingPlayerCreature>&) -- while the player is alive and a confirmation is awaited, consume the player's yes/no answer message (wife, shopkeeper trade, village fine) and keep facing the player until they walk away
// Grade: DIFFER(1030v1059) nd=160 (was nd=204) -- named item in the CIntelligentPointer<const CThing> converting ctor; residue: register roles (retail this=edi, 0x10 constant CSE'd into ebp, manager spilled to [esp+0x1c])
// Layout: local model (the engine header flattens the base); ReactionType +0x28, WaitingForConfirmation +0x2c, DidTraderInteraction +0x2d, PositionPlayerWasWhenWaitingForConfirmation +0x2e.
#include <stdlib.h>
struct CTCBase;
struct C3DVector { float X, Y, Z; };
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
    bool TestTC(long interfaceType) const
    {
        return ((m_presentBits[interfaceType >> 5] >> (interfaceType & 31)) & 1) != 0;
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
    CIntelligentPointerConstThing(const CBaseIntelligentPointer& o) { CThing* item_ = o.GetPItem(); SetPItem(item_); }
};
struct CCharString {
    void* PData;
    ~CCharString();                             // 0x0099EAE0
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
struct CPlayer {
    CThing* GetCharacter();                     // 0x00487DC0
};
struct CPlayerManager {
    CPlayer* GetMainPlayer();                   // 0x00449970
};
struct CAIBrain {
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};
struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    unsigned char _pad08[0x0c];
    void Terminate();                           // 0x008FCF40
    CPlayerManager* GetPPlayerManager();        // 0x008FCF90
    CWorld* GetPWorld();                        // 0x008FCFC0
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
    void UpdateWaitingForConfirmation(const CIntelligentPointer<CThingPlayerCreature>& player);
};
#pragma pack(pop)

void CAIStateGroup_MinionReactToPlayerInteraction::UpdateWaitingForConfirmation(const CIntelligentPointer<CThingPlayerCreature>& player)
{
    do {
        if (!IS_THING_ALIVE(player))
            return;
        if (!WaitingForConfirmation)
            return;
        CMessageEventManager* manager = GetPWorld()->PMessageEventManager;
        CMessageEventFilter_HasExtraData filter(0x10);
        CMessageEvent* event = manager->FindMostRecentMessage(filter);
        if (event) {
            switch (ReactionType) {
            case 4:
                if (event->Answer == 1) {
                    CTCVillageMember* member;
                    if (PBrain->PCreature->GetInterfaceT(&member, 0x23) && GetVillage(member)) {
                        CThing* hero = GetPPlayerManager()->GetMainPlayer()->GetCharacter();
                        CTCVillage* village = GetVillage(member);
                        CTCHeroStats* stats;
                        if (hero->GetInterfaceT(&stats, 4)) {
                            stats->AddMoney(-village->GetFine(), true);
                            stats->FinesPaid = stats->FinesPaid + village->GetFine();
                            CTCHeroOnlineScoreboard* scoreboard;
                            GetPPlayerManager()->GetMainPlayer()->GetCharacter()->GetInterfaceT(&scoreboard, 0x10d);
                            scoreboard->UpdateScoreForActivity(0x25, (float)stats->FinesPaid, 0);
                        }
                        GetPWorld();
                        GetVillage(member)->NextFineFrame = GFGetCurrentFrame() + 450;
                    }
                }
                break;
            case 3:
                ReactionType = 0;
                if (event->Answer == 1) {
                    CTCShopKeeper* shop;
                    if (PBrain->PCreature->GetInterfaceT(&shop, 0x90)) {
                        CCharString text = shop->GetFriendlynessIndicator();
                        CTCTalk* talk;
                        PBrain->PCreature->GetInterfaceT(&talk, 0x10);
                        talk->Say(text, 0, 0, true, 0);
                        shop->OnTradeAccepted();
                        DidTraderInteraction = true;
                        if (AIAction_DoNothing() != 4) {
                            Terminate();
                            return;
                        }
                    }
                }
                break;
            case 1: {
                CTCWife* wife;
                if (PBrain->PCreature->GetInterfaceT(&wife, 0xd7))
                    wife->ReceivedConfirmation(event->Answer == 1);
                break;
            }
            }
            manager->RemoveMessage(event);
            WaitingForConfirmation = false;
        }
        if (IsThingRunningAwayFromThing(player.GetPItem(), PBrain->PCreature, &PositionPlayerWasWhenWaitingForConfirmation))
            return;
    } while (AIAction_LookAtThing(2, player, 0x13, 0.0027777778f) != 1);
}
