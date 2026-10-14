// ProcessThreadManager_00c35510(CThreadManager*) -- EH-framed (`this` unused, `ret 4`): when the manager is null a log line is built on the stack from the shared buffer @ 0x0139C900 (state 0, in its own block: `.\PKCThreadManager.cpp(6) : Null thread manager`, chained `__fastcall` builders, the optional sink @ 0x0143BB2C, release @ 0x00C0BA40); an empty trace temporary is built in the dead argument slot (@ 0x00C16750), the manager's owner at +0 run through its virtual slot 0, the semaphore member at +8 released (@ 0x00C2A150); returns 0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C35510, oracle length 197 bytes. Built under /EHsc.
// Retail bytes:  ... mov esi,[esp+18h] ; test esi,esi ; jne go ; (log line as in 0x00C62E60 with line 6) ; go: lea ecx,[esp+18h] ; call CTrace::CTrace ; mov ecx,[esi] ; mov edx,[ecx] ; call [edx] ; lea ecx,[esi+8] ; call CSemaphore::Release ; ... xor eax,eax ; ... ret 4
// The sibling of 0x00C62E60. The manifest label is a description; named by the shape.
struct CLogLine_00c35510 {                 // one word: the shared buffer
    void* m_pBuffer;
    CLogLine_00c35510(void* buffer) : m_pBuffer(buffer) { }
    ~CLogLine_00c35510();                      // 0x00C0BA40
    CLogLine_00c35510* __fastcall Append(const char* text);   // 0x00C0DEC0
    CLogLine_00c35510* __fastcall AppendNumber(long value);   // 0x00C0DF00
};

struct CTrace_00c35510 {                       // empty temporary (a debug-break helper): homed in a dead byte (here the argument slot), so the log line keeps a frame local
    CTrace_00c35510();                         // 0x00C16750: calls an import and returns `this`
};

typedef void (__fastcall* PFNLOGSINK_00c35510)(void* buffer);
extern void* g_logBuffer_0139c900;         // 0x0139C900
extern PFNLOGSINK_00c35510 g_pfnLogSink_0143bb2c;   // 0x0143BB2C


struct CThreadOwner_00c35510 {
    virtual void Run();                    // slot 0
};

struct CSemaphore_00c35510 {
    long m_a;
    void Release();                        // 0x00C2A150
};

struct CThreadManager_00c35510 {
    CThreadOwner_00c35510* m_pOwner;       // +0
    long m_4;
    CSemaphore_00c35510 m_semaphore;       // +8
};

struct CThreadHost_00c35510 {
    long ProcessThreadManager(CThreadManager_00c35510* manager);
};

long CThreadHost_00c35510::ProcessThreadManager(CThreadManager_00c35510* manager)
{
    if (!manager) {
        {
            CLogLine_00c35510 line(g_logBuffer_0139c900);
            line.Append(".\\PKCThreadManager.cpp")->Append("(")->AppendNumber(6)->Append(") : ")->Append("Null thread manager")->Append("\n");
            if (g_pfnLogSink_0143bb2c)
                g_pfnLogSink_0143bb2c(line.m_pBuffer);
        }
        CTrace_00c35510();
    }
    manager->m_pOwner->Run();
    manager->m_semaphore.Release();
    return 0;
}
