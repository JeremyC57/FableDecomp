// CMessageListOwner_00c203a0::CMessageListOwner_00c203a0() -- a dword and two words zeroed, then the message-event
// list at +8 built by its out-of-line constructor.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C203A0, length 27 bytes.
// Callees: std::list<CMessageEvent const*>::list() 0x00C1BD10 (the retail list: a sentinel pointer).
// The manifest label (CAtmosProcess::CAtmosProcess) is propagated: the PDB CAtmosProcess holds a CIntelligentPointer at
// +6, not two words; this object is {long, short, short, list} and is named by what it holds.
struct CMessageEventList {
    void* head;                                          // retail std::list: the sentinel-node pointer
    CMessageEventList();                                 // 0x00C1BD10
};

struct CMessageListOwner_00c203a0 {
    long m_state;                                        // +0
    short m_index;                                       // +4
    short m_count;                                       // +6
    CMessageEventList m_messages;                        // +8
    CMessageListOwner_00c203a0();
};

CMessageListOwner_00c203a0::CMessageListOwner_00c203a0()
    : m_state(0), m_index(0), m_count(0)
{
}