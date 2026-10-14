// CAIFilter_NoOtherYellForHelp::Evaluate() -- slot-0 virtual: true when the creature's message manager holds no recent message of type 11 (another yell for help)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009825F0, length 30 bytes.
// Callees: CMessageEventManager::FindMostRecentMessage<CMessageEventFilter_IsOfType> @ 0x004AB8B0
// (manifest: thiscall on the manager, one pointer argument -- the filter functor, built in the
// `push ecx` frame slot as a one-dword object holding the message type 11).
// The filter's +4 is the creature whose message manager sits at +0x60; the manager is reached
// through an inline accessor (the creature rides in eax and the filter address in edx -- a
// direct member chain allocates them the other way round). Result `neg eax ; sbb eax,eax ;
// inc eax` is the full-register `== 0` of the returned pointer; no arguments (`ret`).
struct CMessageEvent;

struct CMessageEventFilter_IsOfType {
    long m_type;
    CMessageEventFilter_IsOfType(long type) : m_type(type) { }
};

struct CMessageEventManager {
    CMessageEvent* FindMostRecentMessage(CMessageEventFilter_IsOfType* filter);   // 0x004AB8B0
};

struct CThingCreature {
    unsigned char         _pad[0x60];
    CMessageEventManager* m_pMessageManager; // +0x60
    CMessageEventManager* GetMessageManager() { return m_pMessageManager; }
};

struct CAIFilter_NoOtherYellForHelp {       // vptr at +0x00
    CThingCreature* m_pCreature;            // +0x04
    virtual bool Evaluate();
};

bool CAIFilter_NoOtherYellForHelp::Evaluate()
{
    CMessageEventFilter_IsOfType filter(11);
    return m_pCreature->GetMessageManager()->FindMostRecentMessage(&filter) == 0;
}