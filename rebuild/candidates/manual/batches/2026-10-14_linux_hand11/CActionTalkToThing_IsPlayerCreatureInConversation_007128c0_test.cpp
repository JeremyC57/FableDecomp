#include <cstdio>

enum EConversationState { CONVERSATION_NONE, CONVERSATION_STARTING, CONVERSATION_ACTIVE };
struct CThing { unsigned char _pad[0x10]; unsigned char m_conversationState; EConversationState GetConversationState() const { return (EConversationState)m_conversationState; } };
struct CIntelligentPointer { virtual void v000() { } virtual void v001() { } virtual void v002() { } virtual void v003() { } virtual void v004() { } virtual void v005() { }
    virtual void v006() { } virtual void v007() { } virtual void v008() { } virtual void v009() { } virtual void v010() { }
    virtual CThing* GetPItem() { return PItem; } CThing* PItem; };
struct CTCScriptedControl { void* __vftable; CThing* m_pCreature; };
struct CActionTalkToThing { virtual void v00() { } CTCScriptedControl* m_pControl; unsigned char _pad[4]; CIntelligentPointer m_target;
    long IsPlayerCreatureInConversation()
    { return m_target.GetPItem()->GetConversationState() == CONVERSATION_ACTIVE || m_pControl->m_pCreature->GetConversationState() == CONVERSATION_ACTIVE; } };
int main()
{
    CThing target, creature; target.m_conversationState = 0; creature.m_conversationState = 0;
    CTCScriptedControl c; c.m_pCreature = &creature; CActionTalkToThing a; a.m_pControl = &c; a.m_target.PItem = &target;
    if (a.IsPlayerCreatureInConversation() != 0) { std::printf("007128c0_TEST FAIL\n"); return 1; }
    target.m_conversationState = 2; if (a.IsPlayerCreatureInConversation() != 1) { std::printf("007128c0_TEST FAIL\n"); return 1; }
    target.m_conversationState = 1; creature.m_conversationState = 2; if (a.IsPlayerCreatureInConversation() != 1) { std::printf("007128c0_TEST FAIL\n"); return 1; }
    std::printf("007128c0_TEST PASS\n"); return 0;
}
