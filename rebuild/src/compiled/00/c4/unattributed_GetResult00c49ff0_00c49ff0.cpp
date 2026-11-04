// CMailbox_00c49ff0::GetResult() -- EH-framed: the semaphore member at +0x1c is waited on (@ 0x00C2A090); under a scoped lock over `this` (state 0) an empty queue at +0x18 (virtual slot 2 not above 0) logs `d:\\dev\\audio\\ver_fable_1.5_b\\libpk\\PKCMailbox.h(63) : the queue is out of step with the mailbox handler` through a stack string (in its own block, state 1; chained `__fastcall` builders, optional sink @ 0x0143BB2C, release @ 0x00C0BA40) with an empty trace temporary on a dead frame byte (@ 0x00C16750); returns the queue's virtual slot 0
// Vendor naming wave V1 (one address, one name): 0x00C2A090 ?Wait@PKCSemaphore@@QAEXXZ (was ?Wait@CSemaphore_00c49ff0@@QAEXXZ)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C49FF0, oracle length 236 bytes. Built under /EHsc.
// Retail bytes:  ... lea ecx,[esi+1Ch] ; call CSemaphore::Wait ; push esi ; lea ecx,[esp+10h] ; call CScopedLock::CScopedLock ; mov ecx,[esi+18h] ; mov eax,[ecx] ; mov dword ptr [esp+1Ch],0 ; call [eax+8] ; test eax,eax ; ja pop ; mov ecx,[g_pchNil] ; mov [esp+8],ecx ; mov edx,offset file ; lea ecx,[esp+8] ; mov byte ptr [esp+1Ch],1 ; call Append ; ... AppendNumber(3Fh) ... ; mov eax,[g_pfnLogSink] ; test eax,eax ; je rel ; mov ecx,[esp+8] ; call eax ; rel: lea ecx,[esp+8] ; mov byte ptr [esp+1Ch],0 ; call ~CString ; lea ecx,[esp+7] ; call CTrace::CTrace ; pop: mov ecx,[esi+18h] ; mov edx,[ecx] ; call [edx] ; lea ecx,[esp+0Ch] ; mov esi,eax ; mov dword ptr [esp+1Ch],-1 ; call ~CScopedLock ; ... mov eax,esi ... ret
// Byte-identical twin of 0x0x00C4BC80. The manifest label is a description; the class is named by
// address.
class PKCCriticalSection { };          // the lock keys on the object itself
class PKCProtectionInstance { public:              // 8-byte scoped lock
    PKCCriticalSection* m_pTarget;
    long m_state;
    PKCProtectionInstance(PKCCriticalSection* target);   // 0x00C0FFC0
    ~PKCProtectionInstance();                   // 0x00C0FFE0
};

extern char* g_pchNil_0139c900;            // 0x0139C900
typedef void (__fastcall* PFNLOGSINK_00c49ff0)(const char* text);
extern PFNLOGSINK_00c49ff0 g_pfnLogSink_0143bb2c;   // 0x0143BB2C

class CString { public:                  // MFC-style string: one word, the shared empty data for the default constructor
    char* m_pchData;
    CString() : m_pchData(g_pchNil_0139c900) { }
    ~CString();                       // 0x00C0BA40
    CString& __fastcall operator<<(const char* text);   // 0x00C0DEC0
    CString& __fastcall operator<<(long value);   // 0x00C0DF00
};

struct CDebugBreak {                   // empty temporary (a debug-break helper): lands on a dead frame byte
    CDebugBreak();                         // 0x00C16750
};

struct CResultQueue_00c49ff0 {
    virtual void* Pop();                   // slot 0
    virtual void v01();
    virtual unsigned long GetCount();      // slot 2
};

struct PKCSemaphore {
    long m_0;
    void Wait();                           // 0x00C2A090
};

struct CMailbox_00c49ff0 : PKCCriticalSection {
    unsigned char _pad[0x18];
    CResultQueue_00c49ff0* m_pQueue;           // +0x18
    PKCSemaphore m_semaphore;           // +0x1c
    void* GetResult();
};

void* CMailbox_00c49ff0::GetResult()
{
    m_semaphore.Wait();
    PKCProtectionInstance lock(this);
    if (!(m_pQueue->GetCount() > 0)) {
        {
            CString line;
            line << "d:\\dev\\audio\\ver_fable_1.5_b\\libpk\\PKCMailbox.h" << "(" << 63L << ") : " << "the queue is out of step with the mailbox handler" << "\n";
            if (g_pfnLogSink_0143bb2c)
                g_pfnLogSink_0143bb2c(line.m_pchData);
        }
        CDebugBreak();
    }
    return m_pQueue->Pop();
}