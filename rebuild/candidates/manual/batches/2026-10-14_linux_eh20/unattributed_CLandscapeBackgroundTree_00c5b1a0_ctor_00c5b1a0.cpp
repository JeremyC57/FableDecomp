// CLandscapeBackgroundTree_00c5b1a0::CLandscapeBackgroundTree_00c5b1a0() -- EH-framed constructor: the out-of-line base constructor (@ 0x00C59BA0, state 0 after it), own vtable (0x012B5C60), the member at +0xb4 built out of line (@ 0x00C5D0A0, state 1), the two 0x24-byte elements at +0xdc built in an inline loop (@ 0x00C6B9A0, trivially destructible), then two global matchers registered with their tables (@ 0x00C5CE00: @ 0x0143BF88 with @ 0x0143CBC0, 0x80, 2, 1, 0, 0 and @ 0x0143BF68 with @ 0x0143C5C0, 0x80, 2, 6, 0, 0)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C5B1A0, oracle length 169 bytes. Built under /EHsc.
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; push ecx ; push ebx ; push esi ; mov esi,ecx ; push edi ; mov [esp+0Ch],esi ; call CBase::CBase ; lea ecx,[esi+0B4h] ; mov dword ptr [esp+18h],0 ; mov dword ptr [esi],offset vtable ; call CSoundInit::CSoundInit ; mov byte ptr [esp+18h],1 ; lea edi,[esi+0DCh] ; mov ebx,2 ; loop: mov ecx,edi ; call CElement::CElement ; add edi,24h ; dec ebx ; jne loop ; push 0 ; push 0 ; push 1 ; push 2 ; push 80h ; push offset tableA ; mov ecx,offset matcherA ; call CMatcher::Register ; push 0 ; push 0 ; push 6 ; push 2 ; push 80h ; push offset tableB ; mov ecx,offset matcherB ; call CMatcher::Register ; mov ecx,[esp+10h] ; pop edi ; mov eax,esi ; pop esi ; pop ebx ; mov fs:[0],ecx ; add esp,10h ; ret
// The root constructor of the tree node landed at 0x00C555E0. An array whose elements have a
// constructor but no destructor is built with an inline counted loop (no `__ehvec_ctor`).
struct CTreeBase_00c5b1a0 {                // 0xb4 bytes
    virtual void v00();
    unsigned char _body[0xb0];
    CTreeBase_00c5b1a0();                  // 0x00C59BA0
    ~CTreeBase_00c5b1a0();
};

struct CSoundInit_00c5b1a0 {               // +0xb4, 0x28 bytes
    unsigned char _body[0x28];
    CSoundInit_00c5b1a0();                 // 0x00C5D0A0
    ~CSoundInit_00c5b1a0();
};

struct CElement_00c5b1a0 {                 // +0xdc, 0x24 bytes each, trivially destructible
    unsigned char _body[0x24];
    CElement_00c5b1a0();                   // 0x00C6B9A0
};

struct CMatcherTable_00c5b1a0;
struct CMatcher_00c5b1a0 {
    void Register(CMatcherTable_00c5b1a0* table, long size, long a, long b, long c, long d);   // 0x00C5CE00
};
extern CMatcher_00c5b1a0 g_matcherA_0143bf88;        // 0x0143BF88
extern CMatcher_00c5b1a0 g_matcherB_0143bf68;        // 0x0143BF68
extern CMatcherTable_00c5b1a0 g_tableA_0143cbc0;     // 0x0143CBC0
extern CMatcherTable_00c5b1a0 g_tableB_0143c5c0;     // 0x0143C5C0

struct CLandscapeBackgroundTree_00c5b1a0 : CTreeBase_00c5b1a0 {   // vtable 0x012B5C60
    CSoundInit_00c5b1a0 m_sound;           // +0xb4
    CElement_00c5b1a0 m_elements[2];       // +0xdc
    CLandscapeBackgroundTree_00c5b1a0();
};

CLandscapeBackgroundTree_00c5b1a0::CLandscapeBackgroundTree_00c5b1a0()
    : m_sound()
{
    g_matcherA_0143bf88.Register(&g_tableA_0143cbc0, 0x80, 2, 1, 0, 0);
    g_matcherB_0143bf68.Register(&g_tableB_0143c5c0, 0x80, 2, 6, 0, 0);
}
