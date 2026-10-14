// CTeleportDataOwner_00c4b270::~CTeleportDataOwner_00c4b270() -- EH-framed destructor: own vtable (0x012B8458), an out-of-line cleanup on `this` (@ 0x00C4B010, state 1), the two raw blocks at +4 and +0x1c freed through the global operator delete (@ 0x00BFE9BC, one stack clean-up for both), the pair member at +0x48 destroyed inline (its second at +0x64 @ 0x00C29FB0 at state 2, its first @ 0x00C1C820 at state 0), then the member at +0x34 destroyed out of line (@ 0x00C4BB00)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C4B270, oracle length 137 bytes. Built under /EHsc.
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; sub esp,8 ; push esi ; mov esi,ecx ; push edi ; mov [esp+8],esi ; mov dword ptr [esi],offset vtable ; mov dword ptr [esp+18h],1 ; call Cleanup ; mov eax,[esi+4] ; push eax ; call operator delete ; mov eax,[esi+1Ch] ; push eax ; call operator delete ; lea edi,[esi+48h] ; add esp,8 ; mov [esp+0Ch],edi ; lea ecx,[edi+1Ch] ; mov byte ptr [esp+18h],2 ; call ~CSecond ; mov ecx,edi ; mov byte ptr [esp+18h],0 ; call ~CFirst ; lea ecx,[esi+34h] ; mov dword ptr [esp+18h],-1 ; call ~CMember ; mov ecx,[esp+10h] ; pop edi ; pop esi ; mov fs:[0],ecx ; add esp,14h ; ret
// The pair's implicit destructor is inlined (its `this` parked in the second frame slot); the
// body frees two raw blocks without null checks. The manifest label (`_Dest_val<...TeleportData>`)
// is propagated; the classes are named by address.
struct CFirst_00c4b270 {                   // 0x1c bytes (a CCharString)
    unsigned char _body[0x1c];
    ~CFirst_00c4b270();                    // 0x00C1C820
};

struct CSecond_00c4b270 {
    unsigned char _body[4];
    ~CSecond_00c4b270();                   // 0x00C29FB0
};

struct SPair_00c4b270 {                    // +0x48: implicit destructor, inlined into the owner
    CFirst_00c4b270 first;
    CSecond_00c4b270 second;
};

struct CMember_00c4b270 {                  // +0x34
    unsigned char _body[0x14];
    ~CMember_00c4b270();                   // 0x00C4BB00
};

struct CTeleportDataOwner_00c4b270 {       // vtable 0x012B8458
    virtual void v00();
    char* m_pFirstBlock;                   // +4
    unsigned char _pad[0x14];
    char* m_pSecondBlock;                  // +0x1c
    unsigned char _pad2[0x14];
    CMember_00c4b270 m_member;             // +0x34
    SPair_00c4b270 m_pair;                 // +0x48
    void Cleanup();                        // 0x00C4B010
    ~CTeleportDataOwner_00c4b270();
};

CTeleportDataOwner_00c4b270::~CTeleportDataOwner_00c4b270()
{
    Cleanup();
    delete m_pFirstBlock;
    delete m_pSecondBlock;
}