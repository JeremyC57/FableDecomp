// CAudioStream_00ca4cc0::Restart() -- EH-framed: holds the critical section at +0x94 through a scoped lock (state 0); 0 when nothing is queued at +0x64, else the handle at +0xc0 is cleared, the source at +0x50 stopped when present (@ 0x00CADDF0), the state at +0x74 set to 1 and the start command (1) issued with the record at +0xc (@ 0x00CA7930); the result survives the unlock in esi
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CA4CC0, oracle length 156 bytes. Built under /EHsc.
// Retail bytes:  ... call [__imp_EnterCriticalSection] ; mov eax,[esi+64h] ; test eax,eax ; mov dword ptr [esp+14h],0 ; jne go ; push edi ; call [__imp_LeaveCriticalSection] ; pop edi ; xor eax,eax ; ... ret ; go: mov ecx,[esi+50h] ; test ecx,ecx ; mov dword ptr [esi+0C0h],0 ; je skip ; call CSource::Stop ; skip: lea eax,[esi+0Ch] ; push eax ; push 0 ; push 1 ; mov ecx,esi ; mov dword ptr [esi+74h],1 ; call Issue ; push edi ; mov esi,eax ; call [__imp_LeaveCriticalSection] ; ...
// The sibling of the auto-stop check landed at 0x00CA7080 (same lock, same issue call). The
// manifest label is propagated; the class is named by address.
extern "C" __declspec(dllimport) void __stdcall EnterCriticalSection(void* section);
extern "C" __declspec(dllimport) void __stdcall LeaveCriticalSection(void* section);

struct CSectionLock_00ca4cc0 {             // scoped critical-section holder
    void* m_pSection;
    CSectionLock_00ca4cc0(void* section) : m_pSection(section) { EnterCriticalSection(m_pSection); }
    ~CSectionLock_00ca4cc0() { LeaveCriticalSection(m_pSection); }
};

struct CSource_00ca4cc0 { void Stop(); };  // 0x00CADDF0

struct CAudioStream_00ca4cc0 {
    unsigned char _pad[0xc];
    unsigned char m_record[0x44];          // +0xc
    CSource_00ca4cc0* m_pSource;           // +0x50
    unsigned char _pad2[0x10];
    void* m_pQueued;                       // +0x64
    unsigned char _pad3[0xc];
    long m_state;                          // +0x74
    unsigned char _pad4[0x1c];
    unsigned char m_section[0x18];         // +0x94
    unsigned char _pad5[0x14];
    void* m_hHandle;                       // +0xc0
    long Issue(long command, void* target, void* record);   // 0x00CA7930
    long Restart();
};

long CAudioStream_00ca4cc0::Restart()
{
    CSectionLock_00ca4cc0 lock(m_section);
    if (!m_pQueued)
        return 0;
    m_hHandle = 0;
    if (m_pSource)
        m_pSource->Stop();
    m_state = 1;
    return Issue(1, 0, m_record);
}