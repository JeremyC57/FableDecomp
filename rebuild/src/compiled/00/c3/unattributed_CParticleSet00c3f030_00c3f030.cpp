// CParticleSet_00c3f030::~CParticleSet_00c3f030() -- EH-framed destructor: the array of six 0x17c-byte elements at +0xc34 destroyed through `__ehvec_dtor` (element destructor @ 0x00C592C0), then the out-of-line base destructor (@ 0x00C57EF0)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C3F030, oracle length 92 bytes. Built under /EHsc.
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; push ecx ; push esi ; mov esi,ecx ; mov [esp+4],esi ; push offset ~CElement ; push 6 ; push 17Ch ; lea eax,[esi+0C34h] ; push eax ; mov dword ptr [esp+20h],0 ; call __ehvec_dtor ; mov ecx,esi ; mov dword ptr [esp+10h],-1 ; call ~CBase ; mov ecx,[esp+8] ; pop esi ; mov fs:[0],ecx ; add esp,10h ; ret
// No vtable of its own is stored, so the class is `__declspec(novtable)`. The manifest label is
// a propagated `_Dest_val` name; the classes are named by address.
struct CParticleSetBase_00c3f030 {
    virtual void v00();
    ~CParticleSetBase_00c3f030();          // 0x00C57EF0
};

struct CElement_00c3f030 {                 // 0x17c bytes
    unsigned char _body[0x17c];
    ~CElement_00c3f030();                  // 0x00C592C0
};

struct __declspec(novtable) CParticleSet_00c3f030 : CParticleSetBase_00c3f030 {
    unsigned char _pad[0xc30];
    CElement_00c3f030 m_items[6];          // +0xc34
    ~CParticleSet_00c3f030();
};

CParticleSet_00c3f030::~CParticleSet_00c3f030()
{
}