// CAudioDevice_00cadc10::InitializeFromSource(ISourceInfo*) -- EH-framed: holds the critical section at +0x1c through a one-word scoped lock (state 0); the source's `__stdcall` virtual slot 5 (`this` pushed) fills two 8-byte records on the stack; a negative result is returned as is, else the four words are copied to +0x38..+0x44, the word at +0x48 cleared and 0 returned
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CADC10, oracle length 169 bytes. Built under /EHsc.
// Retail bytes:  ... lea edi,[esi+1Ch] ; push edi ; mov [esp+10h],edi ; call [__imp_EnterCriticalSection] ; mov eax,[esp+30h] ; mov ecx,[eax] ; lea edx,[esp+18h] ; push edx ; lea edx,[esp+14h] ; push edx ; push eax ; mov dword ptr [esp+34h],0 ; call [ecx+14h] ; mov ebx,eax ; test ebx,ebx ; push edi ; jge ok ; call [__imp_LeaveCriticalSection] ; ... mov eax,ebx ; ... ret 4 ; ok: mov eax,[esp+14h] ; mov ecx,[esp+18h] ; mov edx,[esp+1Ch] ; mov [esi+38h],eax ; mov eax,[esp+20h] ; mov [esi+3Ch],ecx ; mov [esi+40h],edx ; mov [esi+44h],eax ; mov dword ptr [esi+48h],0 ; call [__imp_LeaveCriticalSection] ; ... xor eax,eax ; ... ret 4
// The lock's out-of-line destructor (@ 0x00CA48C0, `Leave([ecx])`) is what the unwind funclet
// calls; the inline path leaves through the cached pointer in edi. The manifest label is a
// description (a COM-style initialisation); the class is named by address.
extern "C" __declspec(dllimport) void __stdcall EnterCriticalSection(void* section);
extern "C" __declspec(dllimport) void __stdcall LeaveCriticalSection(void* section);

struct CSectionLock_00cadc10 {             // scoped critical-section holder
    void* m_pSection;
    CSectionLock_00cadc10(void* section) : m_pSection(section) { EnterCriticalSection(m_pSection); }
    ~CSectionLock_00cadc10() { LeaveCriticalSection(m_pSection); }
};

struct SPair_00cadc10 { void* m_first; void* m_second; };

struct ISourceInfo_00cadc10 {              // COM-style: `this` pushed
    virtual long __stdcall v00(); virtual long __stdcall v01(); virtual long __stdcall v02();
    virtual long __stdcall v03(); virtual long __stdcall v04();
    virtual long __stdcall GetInfo(SPair_00cadc10* first, SPair_00cadc10* second);   // slot 5
};

struct CAudioDevice_00cadc10 {
    unsigned char _pad[0x1c];
    unsigned char m_section[0x18];         // +0x1c
    long m_34;
    void* m_firstA;                        // +0x38
    void* m_firstB;                        // +0x3c
    void* m_secondA;                       // +0x40
    void* m_secondB;                       // +0x44
    long m_48;
    long InitializeFromSource(ISourceInfo_00cadc10* source);
};

long CAudioDevice_00cadc10::InitializeFromSource(ISourceInfo_00cadc10* source)
{
    CSectionLock_00cadc10 lock(m_section);
    SPair_00cadc10 first;
    SPair_00cadc10 second;
    long result = source->GetInfo(&first, &second);
    if (result < 0)
        return result;
    m_firstA = first.m_first;
    m_firstB = first.m_second;
    m_secondA = second.m_first;
    m_secondB = second.m_second;
    m_48 = 0;
    return 0;
}