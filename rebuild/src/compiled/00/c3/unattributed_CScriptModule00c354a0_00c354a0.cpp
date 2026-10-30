// CScriptModule_00c354a0::~CScriptModule_00c354a0() -- EH-framed destructor: the member at +8 cleared out of line (@ 0x00C2A090), an import called with 0x14, the handle at +4 released through an import and nulled, then the member at +8 destroyed (@ 0x00C29FB0)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C354A0, oracle length 105 bytes. Built under /EHsc.
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; push ecx ; push esi ; mov esi,ecx ; push edi ; mov [esp+8],esi ; lea edi,[esi+8] ; mov ecx,edi ; mov dword ptr [esp+14h],0 ; call CMember::Clear ; push 14h ; call [__imp__Sleep@4] ; mov eax,[esi+4] ; push eax ; call [__imp__CloseHandle@4] ; mov ecx,edi ; mov dword ptr [esi+4],0 ; mov dword ptr [esp+14h],-1 ; call ~CMember ; mov ecx,[esp+0Ch] ; pop edi ; pop esi ; mov fs:[0],ecx ; add esp,10h ; ret
// The member's address is kept in edi for both its calls; the imports are `__stdcall`. EH state 0
// covers the body (the member is alive). The class stores no vtable. The manifest label is
// propagated garbage; the class is named by address.
extern "C" __declspec(dllimport) void __stdcall Sleep(unsigned long milliseconds); // IAT 0x0143FE1C = KERNEL32.dll!Sleep
extern "C" __declspec(dllimport) int __stdcall CloseHandle(void* handle);          // IAT 0x0143FE30 = KERNEL32.dll!CloseHandle

struct CMember_00c354a0 {                  // +8
    unsigned char _body[4];
    void Clear();                          // 0x00C2A090
    ~CMember_00c354a0();                   // 0x00C29FB0
};

struct CScriptModule_00c354a0 {
    unsigned char _pad[4];
    void* m_handle;                        // +4
    CMember_00c354a0 m_member;             // +8
    ~CScriptModule_00c354a0();
};

CScriptModule_00c354a0::~CScriptModule_00c354a0()
{
    m_member.Clear();
    Sleep(0x14);
    CloseHandle(m_handle);
    m_handle = 0;
}