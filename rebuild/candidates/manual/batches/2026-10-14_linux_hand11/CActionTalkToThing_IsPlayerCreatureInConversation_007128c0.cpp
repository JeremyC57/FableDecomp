// NTCScriptedControl::CActionTalkToThing::IsPlayerCreatureInConversation() -- true when either the target thing (through the intelligent pointer at +0xc) or the controlled creature reports the "in conversation" state
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007128C0, length 43 bytes.
// Callees: virtual slot 11 (`call [eax+0x2c]`) of the CIntelligentPointer member at +0xc (GetPItem).
// Layout: +4 the owning scripted control (its +4 is the creature), +0xc CIntelligentPointer<CThing>;
// the conversation state is a byte at +0x10 of a thing, read through an inline enum getter (reading
// the byte directly spills the thing pointer through ecx; the enum getter keeps retail's
// `mov dl,[eax+0x10] ; mov cl,2 ; cmp dl,cl`). The result is a long (`xor eax,eax` / `mov eax,1`);
// the hoisted constant serves both byte compares of the `||`.
enum EConversationState { CONVERSATION_NONE, CONVERSATION_STARTING, CONVERSATION_ACTIVE };

struct CThing {
    unsigned char _pad[0x10];
    unsigned char m_conversationState;                  // +0x10
    EConversationState GetConversationState() const { return (EConversationState)m_conversationState; }
};

struct CIntelligentPointer {
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005();
    virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009(); virtual void v010();
    virtual CThing* GetPItem();                         // slot 11 (+0x2c)
    CThing* PItem;                                      // +0x04
};

struct CTCScriptedControl {
    void*   __vftable;
    CThing* m_pCreature;                                // +0x04
};

struct CActionTalkToThing {
    virtual void v00();
    CTCScriptedControl* m_pControl;                     // +0x04
    unsigned char       _pad[0x4];                      // +0x08
    CIntelligentPointer m_target;                       // +0x0c
    long IsPlayerCreatureInConversation();
};

long CActionTalkToThing::IsPlayerCreatureInConversation()
{
    return m_target.GetPItem()->GetConversationState() == CONVERSATION_ACTIVE
        || m_pControl->m_pCreature->GetConversationState() == CONVERSATION_ACTIVE;
}
