// CKrakenAttacks_00c6a2e0::~CKrakenAttacks_00c6a2e0() -- EH-framed destructor: the array of four 0x28-byte elements at +4 destroyed through the vector destructor iterator (`__ehvec_dtor`, element destructor @ 0x00C69930), then the out-of-line chunk base destructor (@ 0x00C74450)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C6A2E0, oracle length 86 bytes. Built under /EHsc.
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; push ecx ; push esi ; mov esi,ecx ; mov [esp+4],esi ; push offset ~CElement ; push 4 ; push 28h ; lea eax,[esi+4] ; push eax ; mov dword ptr [esp+20h],0 ; call __ehvec_dtor ; mov ecx,esi ; mov dword ptr [esp+10h],-1 ; call ~CChunkBase ; mov ecx,[esp+8] ; pop esi ; mov fs:[0],ecx ; add esp,10h ; ret
// No vtable of its own is stored, so the class is `__declspec(novtable)`; the element type is the
// chunk-holding class destroyed at 0x00C69930. The manifest label is propagated garbage; the
// classes are named by address.
struct CChunkBase_00c6a2e0 {
    virtual void v00();
    ~CChunkBase_00c6a2e0();                // 0x00C74450
};

struct CElement_00c6a2e0 {                 // 0x28 bytes, destroyed @ 0x00C69930
    unsigned char _body[0x28];
    ~CElement_00c6a2e0();
};

struct __declspec(novtable) CKrakenAttacks_00c6a2e0 : CChunkBase_00c6a2e0 {
    CElement_00c6a2e0 m_items[4];          // +4
    ~CKrakenAttacks_00c6a2e0();
};

CKrakenAttacks_00c6a2e0::~CKrakenAttacks_00c6a2e0()
{
}