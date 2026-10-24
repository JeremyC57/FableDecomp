// CGuiWindowManager_00ca4bf0::ReleaseForegroundWindow() -- EH-framed: holds the critical section at +0x94, releases the COM window at +0x6c and clears it when present, returns 0
// Leaf: ReleaseForegroundWindow_00ca4bf0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CA4BF0, oracle length 97 bytes. Built under /EHsc.
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; push ecx ; push esi ; push edi ; mov esi,ecx ; lea edi,[esi+94h] ; push edi ; mov [esp+0Ch],edi ; call [__imp_EnterCriticalSection] ; mov eax,[esi+6Ch] ; test eax,eax ; mov dword ptr [esp+14h],0 ; je skip ; mov ecx,[eax] ; push eax ; call [ecx+8] ; mov dword ptr [esi+6Ch],0 ; skip: push edi ; call [__imp_LeaveCriticalSection] ; mov ecx,[esp+0Ch] ; pop edi ; xor eax,eax ; pop esi ; mov fs:[0],ecx ; add esp,10h ; ret
// The scoped critical-section lock of 0x00CAF890 around a COM `Release`. The manifest label is
// propagated garbage; the class is the window manager of 0x00CA4B90, named by address.
struct CRITICAL_SECTION_00ca4bf0;
extern "C" __declspec(dllimport) void __stdcall EnterCriticalSection(CRITICAL_SECTION_00ca4bf0*);   // IAT 0x0143FE0C
extern "C" __declspec(dllimport) void __stdcall LeaveCriticalSection(CRITICAL_SECTION_00ca4bf0*);   // IAT 0x0143FE10

struct CCriticalSectionLock_00ca4bf0 {
    CRITICAL_SECTION_00ca4bf0* m_pSection;
    CCriticalSectionLock_00ca4bf0(CRITICAL_SECTION_00ca4bf0* section) : m_pSection(section) { EnterCriticalSection(section); }
    ~CCriticalSectionLock_00ca4bf0() { LeaveCriticalSection(m_pSection); }
};
struct IUnknown_00ca4bf0 {                 // COM-style: `this` is pushed as the first stack argument
    virtual long __stdcall QueryInterface();
    virtual unsigned long __stdcall AddRef();
    virtual unsigned long __stdcall Release();   // slot 2
};

struct CGuiWindowManager_00ca4bf0 {
    unsigned char _pad[0x6c];
    IUnknown_00ca4bf0* m_pForeground;      // +0x6c
    unsigned char _pad2[0x24];
    unsigned char m_section[4];            // +0x94
    long ReleaseForegroundWindow();
};

long CGuiWindowManager_00ca4bf0::ReleaseForegroundWindow()
{
    CCriticalSectionLock_00ca4bf0 lock((CRITICAL_SECTION_00ca4bf0*)m_section);
    if (m_pForeground) {
        m_pForeground->Release();
        m_pForeground = 0;
    }
    return 0;
}