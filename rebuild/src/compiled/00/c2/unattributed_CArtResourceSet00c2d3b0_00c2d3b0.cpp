// CArtResourceSet_00c2d3b0::~CArtResourceSet_00c2d3b0() -- EH-framed destructor: an out-of-line reset with 0 (@ 0x00C2D210, state 1), the member at +0x324 destroyed (@ 0x00C167C0, state 0), then the two 0x180-byte elements at +0x18 destroyed through `__ehvec_dtor` (element destructor @ 0x00C38B20)
// Vendor naming V2 (plan phase 1, one address, one name): 0x00C38B20 ??1CActivationReceptor_00c38b20@@QAE@XZ (was ??1CElement_00c2d3b0@@QAE@XZ)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C2D3B0, oracle length 105 bytes. Built under /EHsc.
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; push ecx ; push esi ; mov esi,ecx ; mov [esp+4],esi ; push 0 ; mov dword ptr [esp+14h],1 ; call Reset ; lea ecx,[esi+324h] ; mov byte ptr [esp+10h],0 ; call ~CMember ; push offset ~CElement ; push 2 ; push 180h ; add esi,18h ; push esi ; mov dword ptr [esp+20h],-1 ; call __ehvec_dtor ; mov ecx,[esp+8] ; pop esi ; mov fs:[0],ecx ; add esp,10h ; ret
// Members are destroyed last to first: the member at +0x324 under state 0 (the array alive), then
// the array (state -1: nothing else remains). The class stores no vtable. The manifest label is
// propagated garbage; the class is named by address.
// Funclet fix (2026-11-09, chunk 896): retail's unwind funclet 0x01213B0A is `mov ecx,[ebp-10h] ; add ecx,320h ; jmp 0x00C162A0`, and 0x00C162A0 is `add ecx,4 ; jmp 0x00C167C0`: the state-1 object is a holder at +0x320 whose implicit destructor destroys the member at +0x324. The member alone at +0x324 gave `add ecx,324h` (the parent body is unchanged).
struct CActivationReceptor_00c38b20 {                 // 0x180 bytes
    unsigned char _body[0x180];
    ~CActivationReceptor_00c38b20();                  // 0x00C38B20
};

struct CMember_00c2d3b0 {                  // +0x324
    unsigned char _body[4];
    ~CMember_00c2d3b0();                   // 0x00C167C0
};

struct CUnprovenHolder_00c162a0 {            // +0x320: its implicit destructor destroys the member at +4; the unwind funclet
    unsigned char _pad[4];                 // calls the out-of-line copy (retail 0x00C162A0: add ecx,4 ; jmp 0x00C167C0)
    CMember_00c2d3b0 m_member;             // +0x324
};

struct CArtResourceSet_00c2d3b0 {
    unsigned char _pad[0x18];
    CActivationReceptor_00c38b20 m_items[2];          // +0x18
    unsigned char _pad2[0x8];
    CUnprovenHolder_00c162a0 m_holder;       // +0x320 (the member at +0x324 is its +4)
    void Reset(long mode);                 // 0x00C2D210
    ~CArtResourceSet_00c2d3b0();
};

CArtResourceSet_00c2d3b0::~CArtResourceSet_00c2d3b0()
{
    Reset(0);
}