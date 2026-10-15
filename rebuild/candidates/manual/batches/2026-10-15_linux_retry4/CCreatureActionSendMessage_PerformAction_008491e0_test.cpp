#include <cstdio>
// Behaviour test for CCreatureAction_SendMessage::PerformAction (0x008491E0): posts the message only while the receiver is alive.
struct CMessageEventDef;
struct C3DVector { float X, Y, Z; };
struct CThing { unsigned char m_flags91; };
struct CBaseIntelligentPointer { CThing* m_pItem; CThing* GetPItem() const { return m_pItem; } };
inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p) { if (p.GetPItem() != 0) { if (!(p.GetPItem()->m_flags91 & 1)) return true; } return false; }
static int g_posts = 0; static CMessageEventDef* g_def = 0; static const C3DVector* g_pos = 0; static CThing* g_sender = 0; static CThing* g_receiver = 0; static long g_extra = 0;
struct CMessageEventManager {
    void* AddMessage(CMessageEventDef* def, const C3DVector* position, CThing* sender, CThing* receiver, long extra)
    { ++g_posts; g_def = def; g_pos = position; g_sender = sender; g_receiver = receiver; g_extra = extra; return 0; }
};
struct CWorld { CMessageEventManager* MessageEventManager; };
static CMessageEventManager g_manager; static CWorld g_world = { &g_manager };
struct CCreatureActionBase { CBaseIntelligentPointer PCreature; CWorld* GetWorld() { return &g_world; } };
struct CCreatureAction_SendMessage : CCreatureActionBase {
    CMessageEventDef* MessageDef; C3DVector Position; CBaseIntelligentPointer PReceiver; long Extra;
    void PostMessage(CMessageEventDef* def, CThing* sender, CThing* receiver)
    { CWorld* world = GetWorld(); world->MessageEventManager->AddMessage(def, &Position, sender, receiver, Extra); }
    void PerformAction()
    {
        if (IS_THING_ALIVE(PReceiver)) {
            PCreature.GetPItem();
            CThing* receiver = PReceiver.GetPItem();
            CThing* sender = PCreature.GetPItem();
            PostMessage(MessageDef, sender, receiver);
        }
    }
};
int main()
{
    CThing creature = { 0 }, receiver = { 0 };
    CCreatureAction_SendMessage a; a.PCreature.m_pItem = &creature; a.PReceiver.m_pItem = &receiver; a.MessageDef = (CMessageEventDef*)0x1234; a.Extra = 77;
    a.PerformAction();
    int ok = g_posts == 1 && g_def == (CMessageEventDef*)0x1234 && g_pos == &a.Position && g_sender == &creature && g_receiver == &receiver && g_extra == 77;
    receiver.m_flags91 = 1; a.PerformAction(); if (g_posts != 1) ok = 0;      // dead receiver: nothing sent
    a.PReceiver.m_pItem = 0; a.PerformAction(); if (g_posts != 1) ok = 0;      // no receiver: nothing sent
    std::printf(ok ? "008491e0_TEST PASS\n" : "008491e0_TEST FAIL\n"); return ok ? 0 : 1;
}
