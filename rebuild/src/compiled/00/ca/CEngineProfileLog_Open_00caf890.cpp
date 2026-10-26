// CEngineProfileLog::Open() -- EH-framed: holds the critical section at +8 (Enter/LeaveCriticalSection through the import table), clears the member at +0x64, runs the out-of-line open @ 0x00CAF750 and returns 0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CAF890, oracle length 82 bytes. Built under /EHsc.
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; push ecx ; push esi ; push edi ; mov esi,ecx ; lea edi,[esi+8] ; push edi ; mov [esp+0Ch],edi ; call [__imp_EnterCriticalSection] ; xor eax,eax ; mov ecx,esi ; mov [esp+14h],eax ; mov [esi+64h],eax ; call DoOpen ; push edi ; call [__imp_LeaveCriticalSection] ; mov ecx,[esp+0Ch] ; pop edi ; xor eax,eax ; pop esi ; mov fs:[0],ecx ; add esp,10h ; ret
// A scoped lock object (one pointer, kept in the frame for the unwind funclet) enters the critical
// section in its inline constructor and leaves it in its inline destructor; EH state 0 covers the
// body. The class comes from the manifest label; the lock and the helper are descriptive.
struct CRITICAL_SECTION_00caf890;
extern "C" __declspec(dllimport) void __stdcall EnterCriticalSection(CRITICAL_SECTION_00caf890*);   // IAT 0x0143FE0C
extern "C" __declspec(dllimport) void __stdcall LeaveCriticalSection(CRITICAL_SECTION_00caf890*);   // IAT 0x0143FE10

struct CCriticalSectionLock_00caf890 {
    CRITICAL_SECTION_00caf890* m_pSection;
    CCriticalSectionLock_00caf890(CRITICAL_SECTION_00caf890* section) : m_pSection(section) { EnterCriticalSection(section); }
    ~CCriticalSectionLock_00caf890() { LeaveCriticalSection(m_pSection); }
};

class CEngineProfileLog { public:
    unsigned char _pad[8];
    unsigned char m_section[0x5c];         // +8: the critical section
    long m_entries;                        // +0x64
    void DoOpen();                         // 0x00CAF750
    long Open();
};

long CEngineProfileLog::Open()
{
    CCriticalSectionLock_00caf890 lock((CRITICAL_SECTION_00caf890*)m_section);
    m_entries = 0;
    DoOpen();
    return 0;
}