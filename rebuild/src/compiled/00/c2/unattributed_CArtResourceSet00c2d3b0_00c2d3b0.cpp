// CArtResourceSet_00c2d3b0::~CArtResourceSet_00c2d3b0() -- EH-framed destructor: an out-of-line reset with 0 (@ 0x00C2D210, state 1), the member at +0x324 destroyed (@ 0x00C167C0, state 0), then the two 0x180-byte elements at +0x18 destroyed through `__ehvec_dtor` (element destructor @ 0x00C38B20)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C2D3B0, oracle length 105 bytes. Built under /EHsc.
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; push ecx ; push esi ; mov esi,ecx ; mov [esp+4],esi ; push 0 ; mov dword ptr [esp+14h],1 ; call Reset ; lea ecx,[esi+324h] ; mov byte ptr [esp+10h],0 ; call ~CMember ; push offset ~CElement ; push 2 ; push 180h ; add esi,18h ; push esi ; mov dword ptr [esp+20h],-1 ; call __ehvec_dtor ; mov ecx,[esp+8] ; pop esi ; mov fs:[0],ecx ; add esp,10h ; ret
// Members are destroyed last to first: the member at +0x324 under state 0 (the array alive), then
// the array (state -1: nothing else remains). The class stores no vtable. The manifest label is
// propagated garbage; the class is named by address.
struct CElement_00c2d3b0 {                 // 0x180 bytes
    unsigned char _body[0x180];
    ~CElement_00c2d3b0();                  // 0x00C38B20
};

struct CMember_00c2d3b0 {                  // +0x324
    unsigned char _body[4];
    ~CMember_00c2d3b0();                   // 0x00C167C0
};

struct CArtResourceSet_00c2d3b0 {
    unsigned char _pad[0x18];
    CElement_00c2d3b0 m_items[2];          // +0x18
    unsigned char _pad2[0xc];
    CMember_00c2d3b0 m_member;             // +0x324
    void Reset(long mode);                 // 0x00C2D210
    ~CArtResourceSet_00c2d3b0();
};

CArtResourceSet_00c2d3b0::~CArtResourceSet_00c2d3b0()
{
    Reset(0);
}