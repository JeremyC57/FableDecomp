// CMailbox_00c48860::Take() -- EH-framed by-value pop (the manifest's "PKCMailbox_ProcessMessage" is propagated): the counting semaphore at +0x1c is acquired (@ 0x00C2A090), the mailbox's own section held through a profiling lock (@ 0x00C0FFC0 / 0x00C0FFE0, state 1); an empty queue (slot 2 count, unsigned) raises the warning at PKCMailbox.h line 63 (MFC-style string in a nested block, log sink at 0x0143BB2C, the trace temporary on a free frame byte), then the queue's slot-0 virtual pops the message straight into the hidden result (its constructed flag at +0xc set before the lock is released)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C48860, oracle length 254 bytes. Built under /EHsc.
// Retail bytes:  ... sub esp,14h ; push esi ; mov esi,ecx ; lea ecx,[esi+1Ch] ; mov dword ptr [esp+0Ch],0 ; call CSemaphore::Acquire ; push esi ; lea ecx,[esp+14h] ; call CProfileLock::CProfileLock ; mov ecx,[esi+18h] ; mov eax,[ecx] ; mov dword ptr [esp+20h],1 ; call [eax+8] ; test eax,eax ; ja pop ; mov ecx,[g_pchNil] ; mov [esp+8],ecx ; mov edx,offset "d:\dev\audio\ver_fable_1.5_b\libpk\PKCMailbox.h" ; lea ecx,[esp+8] ; mov byte ptr [esp+20h],2 ; call Append ; ... mov edx,3Fh ; ... call AppendNumber ; ... call Append (text) ; ... call Append ("\n") ; mov eax,[g_pfnLogSink] ; test eax,eax ; je skip ; mov ecx,[esp+8] ; call eax ; skip: lea ecx,[esp+8] ; mov byte ptr [esp+20h],1 ; call ~CString ; lea ecx,[esp+7] ; call CTrace::CTrace ; pop: mov ecx,[esi+18h] ; mov esi,[esp+28h] ; mov edx,[ecx] ; push esi ; call [edx] ; lea ecx,[esp+10h] ; mov dword ptr [esp+0Ch],1 ; mov byte ptr [esp+20h],0 ; call ~CProfileLock ; ... mov eax,esi ; ... ret 4
// The sibling peek at 0x00C48960 returns the result through the same hidden pointer. The classes are named by address.
class PKCCriticalSection;
extern char* g_pchNil_0139c900;            // 0x0139C900
typedef void (__fastcall* PFNLOGSINK_00c48860)(const char* text);
extern PFNLOGSINK_00c48860 g_pfnLogSink_0143bb2c;   // 0x0143BB2C

struct CString_00c48860 {                  // MFC-style string: one word, the shared empty data for the default constructor
    char* m_pchData;
    CString_00c48860() : m_pchData(g_pchNil_0139c900) { }
    ~CString_00c48860();                       // 0x00C0BA40
    CString_00c48860* __fastcall Append(const char* text);   // 0x00C0DEC0
    CString_00c48860* __fastcall AppendNumber(long value);   // 0x00C0DF00
};

struct CTrace_00c48860 {                   // empty temporary (a debug-break helper): lands on a free frame byte
    CTrace_00c48860();                         // 0x00C16750
};

class PKCProtectionInstance { public:             // 8 bytes: section + entry tick
    void* m_pSection;
    unsigned long m_tick;
    PKCProtectionInstance(PKCCriticalSection* section);  // 0x00C0FFC0
    ~PKCProtectionInstance();              // 0x00C0FFE0
};

struct CMessage_00c48860 {                 // one word, released by its destructor
    void* m_p;
    ~CMessage_00c48860();
};

struct IQueue_00c48860 {
    virtual CMessage_00c48860 Pop();       // slot 0: by value through the hidden pointer
    virtual void Slot4();
    virtual unsigned long Count();         // slot 8
};

struct CSemaphore_00c48860 { void Acquire(); };   // 0x00C2A090

struct CMailbox_00c48860 {
    unsigned char _pad[0x18];
    IQueue_00c48860* m_pQueue;             // +0x18
    CSemaphore_00c48860 m_semaphore;       // +0x1c
    CMessage_00c48860 Take();
};

CMessage_00c48860 CMailbox_00c48860::Take()
{
    m_semaphore.Acquire();
    PKCProtectionInstance lock((PKCCriticalSection*)this);
    if (!(m_pQueue->Count() > 0)) {
        {
            CString_00c48860 line;
            line.Append("d:\\dev\\audio\\ver_fable_1.5_b\\libpk\\PKCMailbox.h")->Append("(")->AppendNumber(63)->Append(") : ")->Append("MESSAGE TEXT")->Append("\n");
            if (g_pfnLogSink_0143bb2c)
                g_pfnLogSink_0143bb2c(line.m_pchData);
        }
        CTrace_00c48860();
    }
    return m_pQueue->Pop();
}