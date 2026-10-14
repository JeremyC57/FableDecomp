// CMailbox_00c4bc80::GetResult() -- EH-framed: the semaphore member at +0x1c is waited on (@ 0x00C2A090); under a scoped lock over `this` (state 0) an empty queue at +0x18 (virtual slot 2 not above 0) logs `d:\\dev\\audio\\ver_fable_1.5_b\\libpk\\PKCMailbox.h(63) : the queue is out of step with the mailbox handler` through a stack string (in its own block, state 1; chained `__fastcall` builders, optional sink @ 0x0143BB2C, release @ 0x00C0BA40) with an empty trace temporary on a dead frame byte (@ 0x00C16750); returns the queue's virtual slot 0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C4BC80, oracle length 236 bytes. Built under /EHsc.
// Retail bytes:  ... lea ecx,[esi+1Ch] ; call CSemaphore::Wait ; push esi ; lea ecx,[esp+10h] ; call CScopedLock::CScopedLock ; mov ecx,[esi+18h] ; mov eax,[ecx] ; mov dword ptr [esp+1Ch],0 ; call [eax+8] ; test eax,eax ; ja pop ; mov ecx,[g_pchNil] ; mov [esp+8],ecx ; mov edx,offset file ; lea ecx,[esp+8] ; mov byte ptr [esp+1Ch],1 ; call Append ; ... AppendNumber(3Fh) ... ; mov eax,[g_pfnLogSink] ; test eax,eax ; je rel ; mov ecx,[esp+8] ; call eax ; rel: lea ecx,[esp+8] ; mov byte ptr [esp+1Ch],0 ; call ~CString ; lea ecx,[esp+7] ; call CTrace::CTrace ; pop: mov ecx,[esi+18h] ; mov edx,[ecx] ; call [edx] ; lea ecx,[esp+0Ch] ; mov esi,eax ; mov dword ptr [esp+1Ch],-1 ; call ~CScopedLock ; ... mov eax,esi ... ret
// Byte-identical twin of 0x0x00C49FF0. The manifest label is a description; the class is named by
// address.
struct CLockTarget_00c4bc80 { };          // the lock keys on the object itself
struct CScopedLock_00c4bc80 {              // 8-byte scoped lock
    CLockTarget_00c4bc80* m_pTarget;
    long m_state;
    CScopedLock_00c4bc80(CLockTarget_00c4bc80* target);   // 0x00C0FFC0
    ~CScopedLock_00c4bc80();                   // 0x00C0FFE0
};

extern char* g_pchNil_0139c900;            // 0x0139C900
typedef void (__fastcall* PFNLOGSINK_00c4bc80)(const char* text);
extern PFNLOGSINK_00c4bc80 g_pfnLogSink_0143bb2c;   // 0x0143BB2C

struct CString_00c4bc80 {                  // MFC-style string: one word, the shared empty data for the default constructor
    char* m_pchData;
    CString_00c4bc80() : m_pchData(g_pchNil_0139c900) { }
    ~CString_00c4bc80();                       // 0x00C0BA40
    CString_00c4bc80* __fastcall Append(const char* text);   // 0x00C0DEC0
    CString_00c4bc80* __fastcall AppendNumber(long value);   // 0x00C0DF00
};

struct CTrace_00c4bc80 {                   // empty temporary (a debug-break helper): lands on a dead frame byte
    CTrace_00c4bc80();                         // 0x00C16750
};

struct CResultQueue_00c4bc80 {
    virtual void* Pop();                   // slot 0
    virtual void v01();
    virtual unsigned long GetCount();      // slot 2
};

struct CSemaphore_00c4bc80 {
    long m_0;
    void Wait();                           // 0x00C2A090
};

struct CMailbox_00c4bc80 : CLockTarget_00c4bc80 {
    unsigned char _pad[0x18];
    CResultQueue_00c4bc80* m_pQueue;           // +0x18
    CSemaphore_00c4bc80 m_semaphore;           // +0x1c
    void* GetResult();
};

void* CMailbox_00c4bc80::GetResult()
{
    m_semaphore.Wait();
    CScopedLock_00c4bc80 lock(this);
    if (!(m_pQueue->GetCount() > 0)) {
        {
            CString_00c4bc80 line;
            line.Append("d:\\dev\\audio\\ver_fable_1.5_b\\libpk\\PKCMailbox.h")->Append("(")->AppendNumber(63)->Append(") : ")->Append("the queue is out of step with the mailbox handler")->Append("\n");
            if (g_pfnLogSink_0143bb2c)
                g_pfnLogSink_0143bb2c(line.m_pchData);
        }
        CTrace_00c4bc80();
    }
    return m_pQueue->Pop();
}
