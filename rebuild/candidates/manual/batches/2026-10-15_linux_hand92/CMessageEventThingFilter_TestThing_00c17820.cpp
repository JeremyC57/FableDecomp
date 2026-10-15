// CMessageEventThingFilter::TestThing(CThing*) -- wraps a thing in a message event record and runs the bound message-event filter chain on it, returning the verdict
// Leaf: TestThing
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C17820, length 33 bytes (the 48-byte catalog row ends in int3 padding).
// Callees: message event constructor @ 0x00C31990 ({0, thing}; manifest label "CSBAgents" is propagated),
//          CFilter_Bind4<...>::operator()(CMessageEvent&) @ 0x00C2EE80 (the bound filter test, bool).
// Callers: 0x00C172AD (`mov edx,[esp+44h] ; lea ecx,[esp] ; call` then `test al,al`) -- a `__fastcall`
//          member: the filter object in ecx, the thing in edx, the bool result passed straight through.
// Label: the manifest's "CTCInventory::Copy" is propagated over a byte-identical body; this lives in the
//        no-RTTI 0x00C1xxxx-0x00C3xxxx script module, so the class name is descriptive.
// Shape: the event record is a named local (retail re-derives `lea eax,[esp+4]` after the constructor
//        instead of reusing the constructor's eax).

struct CThing;

struct CMessageEvent {
    long    m_type;                               // +0x00 (0 = none)
    CThing* m_pThing;                             // +0x04
    CMessageEvent(CThing* thing);                 // 0x00C31990
};

struct CMessageEventThingFilter {
    bool Test(CMessageEvent& event);              // 0x00C2EE80
    bool __fastcall TestThing(CThing* thing);
};

bool __fastcall CMessageEventThingFilter::TestThing(CThing* thing)
{
    CMessageEvent event(thing);
    return Test(event);
}
