// CJoystickDX::~CJoystickDX() -- EH-framed destructor: the block at +0x3c freed through operator delete, the COM device at +0x18 released and nulled when present, then the member at +4 destroyed out of line (@ 0x00CACAF0)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CAAAC0, oracle length 101 bytes. Built under /EHsc.
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; push ecx ; push esi ; mov esi,ecx ; mov [esp+4],esi ; mov eax,[esi+3Ch] ; push eax ; mov dword ptr [esp+14h],0 ; call operator delete ; mov eax,[esi+18h] ; add esp,4 ; test eax,eax ; je skip ; mov ecx,[eax] ; push eax ; call [ecx+8] ; mov dword ptr [esi+18h],0 ; skip: lea ecx,[esi+4] ; mov dword ptr [esp+10h],-1 ; call ~CMember ; mov ecx,[esp+8] ; pop esi ; mov fs:[0],ecx ; add esp,10h ; ret
// The body (EH state 0, the member at +4 alive) frees a raw block and releases a COM-style
// interface (`__stdcall` slot 2 with `this` pushed). The class stores no vtable. The class comes
// from the manifest label; the rest is descriptive.
struct IUnknown_00caaac0 {                 // COM-style: `this` is pushed as the first stack argument
    virtual long __stdcall QueryInterface();
    virtual unsigned long __stdcall AddRef();
    virtual unsigned long __stdcall Release();   // slot 2
};

struct CMember_00caaac0 {                  // +4
    unsigned char _body[0x14];
    ~CMember_00caaac0();                   // 0x00CACAF0
};

struct CJoystickDX {
    unsigned char _pad[4];
    CMember_00caaac0 m_member;             // +4
    IUnknown_00caaac0* m_pDevice;          // +0x18
    unsigned char _pad2[0x20];
    void* m_pBlock;                        // +0x3c
    ~CJoystickDX();
};

CJoystickDX::~CJoystickDX()
{
    ::operator delete(m_pBlock);
    if (m_pDevice) {
        m_pDevice->Release();
        m_pDevice = 0;
    }
}