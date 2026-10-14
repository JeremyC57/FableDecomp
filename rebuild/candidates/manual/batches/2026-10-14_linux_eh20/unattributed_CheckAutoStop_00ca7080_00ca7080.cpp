// CAudioStream_00ca7080::CheckAutoStop() -- EH-framed: holds the critical section at +0x94 through a scoped lock (import enter/leave, EH state 0 while held); when nothing is queued at +0x60, the source at +0x78 has a buffer at +0x18 and its looping byte at +0xa1 is clear, nothing is pending at +0x70 and the state at +0xb0 is 1, the stop command (5) is issued with the source's base at +0xc (null-checked conversion) (@ 0x00CA7930) and the stream reset with 0 (@ 0x00CA4EA0)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CA7080, oracle length 150 bytes. Built under /EHsc.
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; push ecx ; push esi ; push edi ; mov esi,ecx ; lea edi,[esi+94h] ; push edi ; mov [esp+0Ch],edi ; call [__imp_EnterCriticalSection] ; mov eax,[esi+60h] ; test eax,eax ; mov dword ptr [esp+14h],0 ; jne done ; mov eax,[esi+78h] ; mov ecx,[eax+18h] ; test ecx,ecx ; je done ; mov cl,[eax+0A1h] ; test cl,cl ; jne done ; mov ecx,[esi+70h] ; test ecx,ecx ; jne done ; cmp dword ptr [esi+0B0h],1 ; jne done ; test eax,eax ; je null ; add eax,0Ch ; jmp issue ; null: xor eax,eax ; issue: push 0 ; push eax ; push 5 ; mov ecx,esi ; call Issue ; push 0 ; mov ecx,esi ; call Reset ; done: push edi ; call [__imp_LeaveCriticalSection] ; ...
// The source pointer converts to its second base at +0xc with the compiler's null check. The
// manifest label is propagated; the class is named by address.
extern "C" __declspec(dllimport) void __stdcall EnterCriticalSection(void* section);
extern "C" __declspec(dllimport) void __stdcall LeaveCriticalSection(void* section);

struct CSectionLock_00ca7080 {             // scoped critical-section holder
    void* m_pSection;
    CSectionLock_00ca7080(void* section) : m_pSection(section) { EnterCriticalSection(m_pSection); }
    ~CSectionLock_00ca7080() { LeaveCriticalSection(m_pSection); }
};

struct CSourceHead_00ca7080 {              // the 12 bytes in front of the command base
    long m_a, m_b, m_c;
};

struct CCommandTarget_00ca7080 {           // +0xc of the source
    unsigned char _pad[0xc];
    void* m_pBuffer;                       // +0x18 of the source
    unsigned char _pad2[0x85];
    bool m_looping;                        // +0xa1 of the source
};

struct CSource_00ca7080 : CSourceHead_00ca7080, CCommandTarget_00ca7080 { };

struct CAudioStream_00ca7080 {
    unsigned char _pad[0x60];
    void* m_pQueued;                       // +0x60
    unsigned char _pad2[0xc];
    void* m_pPending;                      // +0x70
    long m_74;
    CSource_00ca7080* m_pSource;           // +0x78
    unsigned char _pad3[0x18];
    unsigned char m_section[0x18];         // +0x94
    unsigned char _pad4[4];
    long m_state;                          // +0xb0
    void Issue(long command, CCommandTarget_00ca7080* target, long flags);   // 0x00CA7930
    void Reset(long mode);                 // 0x00CA4EA0
    void CheckAutoStop();
};

void CAudioStream_00ca7080::CheckAutoStop()
{
    CSectionLock_00ca7080 lock(m_section);
    if (!m_pQueued && m_pSource->m_pBuffer && !m_pSource->m_looping && !m_pPending && m_state == 1) {
        Issue(5, m_pSource, 0);
        Reset(0);
    }
}
