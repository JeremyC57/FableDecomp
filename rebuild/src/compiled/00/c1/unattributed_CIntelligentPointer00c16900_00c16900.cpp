// CIntelligentPointer_00c16900::CIntelligentPointer_00c16900(const CIntelligentPointer_00c16900&) -- EH-framed copy constructor: the base zeroes its two pointers inline, then Set(other.Get()) out of line (@ 0x00C16800 / 0x00C16820)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C16900, oracle length 80 bytes. Built under /EHsc.
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; push ecx ; push esi ; mov esi,ecx ; xor eax,eax ; mov [esp+4],esi ; mov [esi],eax ; mov [esi+4],eax ; mov ecx,[other] ; mov [esp+10h],eax ; call Get ; push eax ; mov ecx,esi ; call Set ; mov ecx,[esp+8] ; mov eax,esi ; pop esi ; mov fs:[0],ecx ; add esp,10h ; ret 4
// The intelligent-pointer base (two pointers, zeroed by its inline constructor; its destructor is
// out of line, so EH state 0 is entered once it exists) then the body resolves the other pointer's
// thing out of line and Sets it. The manifest label (`operator=`) is propagated; the class is
// named by address.
struct CThing;
struct CBaseIntelligentPointer_00c16900 {
    void* m_pThing;                        // +0
    void* m_pLink;                         // +4
    CBaseIntelligentPointer_00c16900() : m_pThing(0), m_pLink(0) { }
    ~CBaseIntelligentPointer_00c16900();
    CThing* Get() const;                   // 0x00C16800
    void Set(CThing* thing);               // 0x00C16820
};

struct CIntelligentPointer_00c16900 : CBaseIntelligentPointer_00c16900 {
    CIntelligentPointer_00c16900(const CIntelligentPointer_00c16900& other);
};

CIntelligentPointer_00c16900::CIntelligentPointer_00c16900(const CIntelligentPointer_00c16900& other)
{
    Set(other.Get());
}