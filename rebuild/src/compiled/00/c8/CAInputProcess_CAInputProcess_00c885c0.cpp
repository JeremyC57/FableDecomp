// CAInputProcess::~CAInputProcess() -- EH-framed destructor: own vtable (0x012BF1C4), the member at +0x80 released out of line (@ 0x00C8A7E0) when the pointer at +0x78 is set (state 1), the member at +4 destroyed (@ 0x00C4B270, state 0), then the inline base destructor restores the base vtable (0x012BEFFC)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C885C0, oracle length 96 bytes. Built under /EHsc.
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; push ecx ; push esi ; mov esi,ecx ; mov [esp+4],esi ; mov dword ptr [esi],offset vtable ; mov eax,[esi+78h] ; test eax,eax ; mov dword ptr [esp+10h],1 ; je skip ; lea ecx,[esi+80h] ; call CHandle::Release ; skip: lea ecx,[esi+4] ; mov byte ptr [esp+10h],0 ; call ~CMember ; mov ecx,[esp+8] ; mov dword ptr [esi],offset base vtable ; pop esi ; mov fs:[0],ecx ; add esp,10h ; ret
// Two tracked objects (the member at +4 and the base) give EH state 1 during the body and state 0
// (narrowed to a byte) during the member destructor; the base's inline empty destructor restores
// its vtable. The class comes from the manifest label; the rest is descriptive.
struct CAInputProcessBase_00c885c0 {       // vtable 0x012BEFFC
    virtual void v00();
    ~CAInputProcessBase_00c885c0() { }
};

struct CInputMember_00c885c0 {             // +4
    unsigned char _body[0x74];
    ~CInputMember_00c885c0();              // 0x00C4B270
};

struct CInputHandle_00c885c0 {             // +0x80
    unsigned char _body[4];
    void Release();                        // 0x00C8A7E0
};

struct CAInputProcess : CAInputProcessBase_00c885c0 {   // vtable 0x012BF1C4
    CInputMember_00c885c0 m_member;        // +4
    void* m_pDevice;                       // +0x78
    unsigned char _pad[4];
    CInputHandle_00c885c0 m_handle;        // +0x80
    ~CAInputProcess();
};

CAInputProcess::~CAInputProcess()
{
    if (m_pDevice)
        m_handle.Release();
}