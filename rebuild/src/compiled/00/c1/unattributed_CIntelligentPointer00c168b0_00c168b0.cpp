// CIntelligentPointer_00c168b0::CIntelligentPointer_00c168b0(CThing*) -- EH-framed constructor: the base zeroes its two pointers inline, then Set(thing) out of line (@ 0x00C16820)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C168B0, oracle length 80 bytes. Built under /EHsc.
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; push ecx ; push esi ; mov esi,ecx ; xor eax,eax ; mov [esp+4],esi ; mov [esi],eax ; mov [esi+4],eax ; mov [esp+10h],eax ; mov eax,[thing] ; push eax ; call Set ; mov ecx,[esp+8] ; mov eax,esi ; pop esi ; mov fs:[0],ecx ; add esp,10h ; ret 4
// The intelligent-pointer base (two pointers, zeroed by its inline constructor; its destructor is
// out of line, so EH state 0 is entered once it exists -- the zero is shared with the state store)
// then the body calls the out-of-line Set with the argument. The manifest label (a persist-traits
// name) is propagated; the class is named by address.
struct CThing;
struct CBaseIntelligentPointer_00c168b0 {
    void* m_pThing;                        // +0
    void* m_pLink;                         // +4
    CBaseIntelligentPointer_00c168b0() : m_pThing(0), m_pLink(0) { }
    ~CBaseIntelligentPointer_00c168b0();
    void Set(CThing* thing);               // 0x00C16820
};

struct CIntelligentPointer_00c168b0 : CBaseIntelligentPointer_00c168b0 {
    CIntelligentPointer_00c168b0(CThing* thing);
};

CIntelligentPointer_00c168b0::CIntelligentPointer_00c168b0(CThing* thing)
{
    Set(thing);
}