// CKrakenData_00c6a340::~CKrakenData_00c6a340() -- EH-framed destructor: the two 0x28-byte elements at +0x40 (state 1, element destructor @ 0x00C69990) and the three 0x14-byte elements at +4 (state 0, @ 0x00C69680) destroyed through `__ehvec_dtor`, then the out-of-line chunk base destructor (@ 0x00C74450)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C6A340, oracle length 109 bytes. Built under /EHsc.
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; push ecx ; push esi ; mov esi,ecx ; mov [esp+4],esi ; push offset ~CLarge ; push 2 ; push 28h ; lea eax,[esi+40h] ; push eax ; mov dword ptr [esp+20h],1 ; call __ehvec_dtor ; push offset ~CSmall ; push 3 ; push 14h ; lea ecx,[esi+4] ; push ecx ; mov byte ptr [esp+20h],0 ; call __ehvec_dtor ; mov ecx,esi ; mov dword ptr [esp+10h],-1 ; call ~CChunkBase ; mov ecx,[esp+8] ; pop esi ; mov fs:[0],ecx ; add esp,10h ; ret
// No vtable of its own is stored, so the class is `__declspec(novtable)`; the arrays go last to
// first with the EH state counting down. The manifest label is propagated garbage; the classes
// are named by address (the elements are the chunk holders of 0x00C69990 / 0x00C69680).
struct CChunkBase_00c6a340 {
    virtual void v00();
    ~CChunkBase_00c6a340();                // 0x00C74450
};

struct CSmall_00c6a340 {                   // 0x14 bytes
    unsigned char _body[0x14];
    ~CSmall_00c6a340();                    // 0x00C69680
};

struct CLarge_00c6a340 {                   // 0x28 bytes
    unsigned char _body[0x28];
    ~CLarge_00c6a340();                    // 0x00C69990
};

struct __declspec(novtable) CKrakenData_00c6a340 : CChunkBase_00c6a340 {
    CSmall_00c6a340 m_small[3];            // +4
    CLarge_00c6a340 m_large[2];            // +0x40
    ~CKrakenData_00c6a340();
};

CKrakenData_00c6a340::~CKrakenData_00c6a340()
{
}
