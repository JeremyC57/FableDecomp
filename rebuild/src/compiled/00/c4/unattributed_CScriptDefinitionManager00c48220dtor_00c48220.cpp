// CScriptDefinitionManager_00c48220::~CScriptDefinitionManager_00c48220() -- EH-framed destructor: own vtable (0x012B8034), the pair member at +0x18 destroyed inline (its second at +0x34 @ 0x00C29FB0 at state 1, its first @ 0x00C1C820 at state 0), then the member at +4 destroyed out of line (@ 0x00C48DB0)
// Leaf: ??1CAIStateGroup_CreatureUseCoverBase@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C48220, oracle length 111 bytes. Built under /EHsc.
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; sub esp,8 ; push esi ; mov esi,ecx ; push edi ; mov [esp+8],esi ; mov dword ptr [esi],offset vtable ; lea edi,[esi+18h] ; mov dword ptr [esp+18h],0 ; mov [esp+0Ch],edi ; lea ecx,[edi+1Ch] ; mov byte ptr [esp+18h],1 ; call ~CSecond ; mov ecx,edi ; mov byte ptr [esp+18h],0 ; call ~CFirst ; lea ecx,[esi+4] ; mov dword ptr [esp+18h],-1 ; call ~CMember ; mov ecx,[esp+10h] ; pop edi ; pop esi ; mov fs:[0],ecx ; add esp,14h ; ret
// The destructor counterpart of the constructor at 0x00C485E0 (same vtable): the pair's implicit
// destructor is inlined (its address kept in edi and the frame). The manifest label is propagated
// garbage; the class is named by address.
struct CFirst_00c48220 {                   // 0x1c bytes (a CCharString)
    unsigned char _body[0x1c];
    ~CFirst_00c48220();                    // 0x00C1C820
};

struct CSecond_00c48220 {
    unsigned char _body[4];
    ~CSecond_00c48220();                   // 0x00C29FB0
};

struct SPair_00c48220 {                    // implicit destructor, inlined into the owner
    CFirst_00c48220 first;
    CSecond_00c48220 second;
};

struct CMember_00c48220 {                  // +4
    unsigned char _body[0x14];
    ~CMember_00c48220();                   // 0x00C48DB0
};

struct CScriptDefinitionManager_00c48220 {   // vtable 0x012B8034
    virtual void v00();
    CMember_00c48220 m_member;             // +4
    SPair_00c48220 m_pair;                 // +0x18
    ~CScriptDefinitionManager_00c48220();
};

CScriptDefinitionManager_00c48220::~CScriptDefinitionManager_00c48220()
{
}