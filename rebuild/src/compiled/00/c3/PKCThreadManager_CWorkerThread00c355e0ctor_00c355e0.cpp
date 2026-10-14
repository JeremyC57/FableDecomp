// CWorkerThread_00c355e0::CWorkerThread(owner, params) -- EH-framed constructor (the manifest's "PKCThreadManager::InitializeThread" is propagated; the landed 0x00C48640 allocates it): the owner at +0, the handle at +4 cleared, a (0, 1) event part at +8 (@ 0x00C29FC0; destructor @ 0x00C29FB0, state 0); the optional parameter record supplies the stack size and priority; the thread is created on the landed procedure 0x00C35510 with the worker as its argument and a null handle raises the "Problems starting thread" warning (PKCThreadManager.cpp line 23; the string and the trace temporary live in the dead parameter slot, state 1); a non-zero priority is applied (SetThreadPriority)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C355E0, oracle length 269 bytes. Built under /EHsc.
// Retail bytes:  ... mov eax,[esp+14h] ; push esi ; push edi ; mov esi,ecx ; push 1 ; push 0 ; lea ecx,[esi+8] ; mov [esp+10h],esi ; mov [esi],eax ; mov dword ptr [esi+4],0 ; call CEventPart::CEventPart ; mov ecx,[esp+20h] ; xor eax,eax ; xor edi,edi ; test ecx,ecx ; mov dword ptr [esp+14h],0 ; je a ; mov eax,[ecx] ; mov edi,[ecx+4] ; a: push 0 ; push 0 ; push esi ; push offset ThreadProc ; push eax ; push 0 ; call [__imp_CreateThread] ; test eax,eax ; mov [esi+4],eax ; jne b ; mov ecx,[g_pchNil] ; mov [esp+1Ch],ecx ; mov edx,offset ".\PKCThreadManager.cpp" ; lea ecx,[esp+1Ch] ; mov byte ptr [esp+14h],1 ; call Append ; ... mov edx,17h ; ... call AppendNumber ; ... call Append ("Problems starting thread") ; ... call Append ("\n") ; mov eax,[g_pfnLogSink] ; test eax,eax ; je skip ; mov ecx,[esp+1Ch] ; call eax ; skip: lea ecx,[esp+1Ch] ; mov byte ptr [esp+14h],0 ; call ~CString ; lea ecx,[esp+1Ch] ; call CTrace::CTrace ; b: test edi,edi ; je done ; mov edx,[esi+4] ; push edi ; push edx ; call [__imp_SetThreadPriority] ; done: ... mov eax,esi ; ... ret 8
// The classes are named by address.
extern "C" __declspec(dllimport) void* __stdcall CreateThread(void* attributes, unsigned long stackSize, unsigned long (__stdcall* start)(void*), void* parameter, unsigned long flags, unsigned long* threadId);
extern "C" __declspec(dllimport) int __stdcall SetThreadPriority(void* thread, long priority);
unsigned long __stdcall ThreadProc_00c35510(void* parameter);   // 0x00C35510
extern char* g_pchNil_0139c900;            // 0x0139C900
typedef void (__fastcall* PFNLOGSINK_00c355e0)(const char* text);
extern PFNLOGSINK_00c355e0 g_pfnLogSink_0143bb2c;   // 0x0143BB2C

struct CString_00c355e0 {                  // MFC-style string: one word, the shared empty data for the default constructor
    char* m_pchData;
    CString_00c355e0() : m_pchData(g_pchNil_0139c900) { }
    ~CString_00c355e0();                       // 0x00C0BA40
    CString_00c355e0* __fastcall Append(const char* text);   // 0x00C0DEC0
    CString_00c355e0* __fastcall AppendNumber(long value);   // 0x00C0DF00
};

struct CTrace_00c355e0 {                   // empty temporary (a debug-break helper): lands on a dead frame byte
    CTrace_00c355e0();                         // 0x00C16750
};

struct SThreadParams_00c355e0 {            // 8 bytes: stack size and priority
    unsigned long m_stackSize;
    long m_priority;
};

struct CEventPart_00c355e0 {               // +8
    unsigned char _body[4];
    CEventPart_00c355e0(long a, long b);   // 0x00C29FC0
    ~CEventPart_00c355e0();                // 0x00C29FB0
};

struct CWorkerThread_00c355e0 {            // 12 bytes
    void* m_pOwner;                        // +0
    void* m_hThread;                       // +4
    CEventPart_00c355e0 m_event;           // +8
    CWorkerThread_00c355e0(void* owner, const SThreadParams_00c355e0* params);
};

CWorkerThread_00c355e0::CWorkerThread_00c355e0(void* owner, const SThreadParams_00c355e0* params)
    : m_pOwner(owner), m_hThread(0), m_event(0, 1)
{
    unsigned long stackSize = 0;
    long priority = 0;
    if (params) {
        stackSize = params->m_stackSize;
        priority = params->m_priority;
    }
    m_hThread = CreateThread(0, stackSize, ThreadProc_00c35510, this, 0, 0);
    if (m_hThread == 0) {
        {
            CString_00c355e0 line;
            line.Append(".\\PKCThreadManager.cpp")->Append("(")->AppendNumber(23)->Append(") : ")->Append("Problems starting thread")->Append("\n");
            if (g_pfnLogSink_0143bb2c)
                g_pfnLogSink_0143bb2c(line.m_pchData);
        }
        CTrace_00c355e0();
    }
    if (priority)
        SetThreadPriority(m_hThread, priority);
}