// CGuiWindowManager::RemoveExclusiveForegroundWindow() -- EH-framed: holds the critical section at +0x94, adds a reference to the foreground window at +0x6c (COM-style `__stdcall` slot 1) when present and returns it (re-read after the call)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CA4B90, oracle length 93 bytes. Built under /EHsc.
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; push ecx ; push esi ; push edi ; mov esi,ecx ; lea edi,[esi+94h] ; push edi ; mov [esp+0Ch],edi ; call [__imp_EnterCriticalSection] ; mov eax,[esi+6Ch] ; test eax,eax ; mov dword ptr [esp+14h],0 ; je skip ; mov ecx,[eax] ; push eax ; call [ecx+4] ; skip: mov esi,[esi+6Ch] ; push edi ; call [__imp_LeaveCriticalSection] ; mov ecx,[esp+0Ch] ; pop edi ; mov eax,esi ; pop esi ; mov fs:[0],ecx ; add esp,10h ; ret
// The scoped critical-section lock of 0x00CAF890; the window pointer is re-read for the return
// value because the virtual call may have changed it. The class comes from the manifest label.
struct CRITICAL_SECTION_00ca4b90;
extern "C" __declspec(dllimport) void __stdcall EnterCriticalSection(CRITICAL_SECTION_00ca4b90*);   // IAT 0x0143FE0C
extern "C" __declspec(dllimport) void __stdcall LeaveCriticalSection(CRITICAL_SECTION_00ca4b90*);   // IAT 0x0143FE10

struct CCriticalSectionLock_00ca4b90 {
    CRITICAL_SECTION_00ca4b90* m_pSection;
    CCriticalSectionLock_00ca4b90(CRITICAL_SECTION_00ca4b90* section) : m_pSection(section) { EnterCriticalSection(section); }
    ~CCriticalSectionLock_00ca4b90() { LeaveCriticalSection(m_pSection); }
};

struct IGuiWindow_00ca4b90 {               // COM-style: `this` is pushed as the first stack argument
    virtual long __stdcall QueryInterface();
    virtual unsigned long __stdcall AddRef();   // slot 1
};

struct CGuiWindowManager {
    unsigned char _pad[0x6c];
    IGuiWindow_00ca4b90* m_pForeground;    // +0x6c
    unsigned char _pad2[0x24];
    unsigned char m_section[4];            // +0x94: the critical section
    IGuiWindow_00ca4b90* RemoveExclusiveForegroundWindow();
};

IGuiWindow_00ca4b90* CGuiWindowManager::RemoveExclusiveForegroundWindow()
{
    CCriticalSectionLock_00ca4b90 lock((CRITICAL_SECTION_00ca4b90*)m_section);
    if (m_pForeground)
        m_pForeground->AddRef();
    return m_pForeground;
}