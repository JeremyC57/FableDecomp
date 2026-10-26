// CGameScriptThing::IncrementScriptCounter() -- increments the counter at +0x14
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008D4670, oracle length 4 bytes.
// Grade: candidate -- generated for a repeated body shape and graded in the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); the batch README records the lane verdict.
//
// Retail bytes:  inc dword ptr [ecx + 0x14] ; ret 
// Tiny-body shape `Increment` synthesised from the instruction pattern (the class comes from the
// manifest label when it is a plain class name, else the method is landed under a descriptive
// placeholder); member names are descriptive. 
class CGameScriptThing { public:
    unsigned char _pad[0x14];
    long m_count;                          // +0x14
    void IncrementScriptCounter();
};

void CGameScriptThing::IncrementScriptCounter()
{
    ++m_count;
}