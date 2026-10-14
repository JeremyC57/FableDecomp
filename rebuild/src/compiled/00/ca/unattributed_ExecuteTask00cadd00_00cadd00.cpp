// CThreadedManager_00cadd00::ExecuteTask(void*, void*) -- EH-framed: holds the critical section at +0x1c through a scoped lock (state 0); 0x80004005 while the state at +0x48 is 1, else the first task runs through the `__stdcall` virtual slot 13 (`this` pushed) with 0, the words at +0x38/+0x3c and a tag string, and when a second task is given and the first result is not negative it runs the same way with the words at +0x40/+0x44
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CADD00, oracle length 179 bytes. Built under /EHsc.
// Retail bytes:  ... cmp dword ptr [esi+48h],1 ; mov dword ptr [esp+14h],0 ; jne go ; push ebx ; call [__imp_LeaveCriticalSection] ; pop esi ; mov eax,80004005h ; ... ret 8 ; go: mov ecx,[esi+3Ch] ; mov edx,[esi+38h] ; mov eax,[esi] ; push edi ; push offset tag ; push ecx ; mov ecx,[a] ; push edx ; push 0 ; push ecx ; push esi ; call [eax+34h] ; mov edi,eax ; mov eax,[b] ; test eax,eax ; je done ; test edi,edi ; jl done ; mov ecx,[esi+44h] ; mov edx,[esi] ; push offset tag ; push ecx ; mov ecx,[esi+40h] ; push ecx ; push 0 ; push eax ; push esi ; call [edx+34h] ; mov edi,eax ; done: push ebx ; call [__imp_LeaveCriticalSection] ; mov ecx,[esp+10h] ; mov eax,edi ; ...
// The slot is a COM-style `__stdcall` member (`this` on the stack). The manifest label is
// propagated; the class is named by address.
extern "C" __declspec(dllimport) void __stdcall EnterCriticalSection(void* section);
extern "C" __declspec(dllimport) void __stdcall LeaveCriticalSection(void* section);

struct CSectionLock_00cadd00 {             // scoped critical-section holder
    void* m_pSection;
    CSectionLock_00cadd00(void* section) : m_pSection(section) { EnterCriticalSection(m_pSection); }
    ~CSectionLock_00cadd00() { LeaveCriticalSection(m_pSection); }
};

struct CThreadedManager_00cadd00 {
    virtual void __stdcall v00(); virtual void __stdcall v01(); virtual void __stdcall v02(); virtual void __stdcall v03();
    virtual void __stdcall v04(); virtual void __stdcall v05(); virtual void __stdcall v06(); virtual void __stdcall v07();
    virtual void __stdcall v08(); virtual void __stdcall v09(); virtual void __stdcall v10(); virtual void __stdcall v11();
    virtual void __stdcall v12();
    virtual long __stdcall Execute(void* task, long mode, void* first, void* second, const char* tag);   // slot 13
    unsigned char _pad[0x18];
    unsigned char m_section[0x18];         // +0x1c
    long m_34;
    void* m_firstA;                        // +0x38
    void* m_firstB;                        // +0x3c
    void* m_secondA;                       // +0x40
    void* m_secondB;                       // +0x44
    long m_state;                          // +0x48
    long ExecuteTask(void* a, void* b);
};

long CThreadedManager_00cadd00::ExecuteTask(void* a, void* b)
{
    CSectionLock_00cadd00 lock(m_section);
    if (m_state == 1)
        return 0x80004005;
    long result = Execute(a, 0, m_firstA, m_firstB, "ExecuteTask");
    if (b && result >= 0)
        result = Execute(b, 0, m_secondA, m_secondB, "ExecuteTask");
    return result;
}