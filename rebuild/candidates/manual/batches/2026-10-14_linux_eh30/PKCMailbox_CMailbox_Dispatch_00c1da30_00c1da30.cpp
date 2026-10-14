// CMailbox_00c1da30::Dispatch(message) -- EH-framed (the manifest's "MailboxQueue_DispatchMessage" is propagated): waits on the event at +0x1c (@ 0x00C2A090: WaitForSingleObject with the infinite timeout and its own warning), then under the mailbox's own profiling lock (@ 0x00C0FFC0 / 0x00C0FFE0, state 0) an empty queue handler at +0x18 (slot 2 count, unsigned) raises the "the queue is out of step with the mailbox handler" warning (PKCMailbox.h line 63: an MFC-style string in a nested block handed to the log sink at 0x0143BB2C, then the empty trace temporary @ 0x00C16750 on a free frame byte; state 1 covers the string); the message is then handed to the handler's slot 0 and returned
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C1DA30, oracle length 241 bytes. Built under /EHsc.
// Retail bytes:  ... sub esp,10h ; push esi ; mov esi,ecx ; lea ecx,[esi+1Ch] ; call CEvent::Wait ; push esi ; lea ecx,[esp+10h] ; call CProfileLock::CProfileLock ; mov ecx,[esi+18h] ; mov eax,[ecx] ; mov dword ptr [esp+1Ch],0 ; call [eax+8] ; test eax,eax ; ja ok ; mov ecx,[g_pchNil] ; mov [esp+8],ecx ; mov edx,offset "d:\dev\audio\ver_fable_1.5_b\libpk\PKCMailbox.h" ; lea ecx,[esp+8] ; mov byte ptr [esp+1Ch],1 ; call Append ; ... mov edx,3Fh ; ... call AppendNumber ; ... call Append ("the queue is out of step with the mailbox handler") ; ... call Append ("\n") ; mov eax,[g_pfnLogSink] ; test eax,eax ; je skip ; mov ecx,[esp+8] ; call eax ; skip: lea ecx,[esp+8] ; mov byte ptr [esp+1Ch],0 ; call ~CString ; lea ecx,[esp+7] ; call CTrace::CTrace ; ok: mov ecx,[esi+18h] ; mov esi,[esp+24h] ; mov edx,[ecx] ; push esi ; call [edx] ; lea ecx,[esp+0Ch] ; mov dword ptr [esp+1Ch],-1 ; call ~CProfileLock ; ... mov eax,esi ; ... ret 4
// The classes are named by address.
extern char* g_pchNil_0139c900;            // 0x0139C900
typedef void (__fastcall* PFNLOGSINK_00c1da30)(const char* text);
extern PFNLOGSINK_00c1da30 g_pfnLogSink_0143bb2c;   // 0x0143BB2C

struct CString_00c1da30 {                  // MFC-style string: one word, the shared empty data for the default constructor
    char* m_pchData;
    CString_00c1da30() : m_pchData(g_pchNil_0139c900) { }
    ~CString_00c1da30();                       // 0x00C0BA40
    CString_00c1da30* __fastcall Append(const char* text);   // 0x00C0DEC0
    CString_00c1da30* __fastcall AppendNumber(long value);   // 0x00C0DF00
};

struct CTrace_00c1da30 {                   // empty temporary (a debug-break helper): lands on a free frame byte
    CTrace_00c1da30();                         // 0x00C16750
};

struct CProfileLock_00c1da30 {             // 8 bytes: section + entry tick
    void* m_pSection;
    unsigned long m_tick;
    CProfileLock_00c1da30(void* section);  // 0x00C0FFC0
    ~CProfileLock_00c1da30();              // 0x00C0FFE0
};

struct CEvent_00c1da30 {                   // +0x1c: a Win32 event handle
    void* m_hEvent;
    void Wait();                           // 0x00C2A090
};

struct IMailboxHandler_00c1da30 {
    virtual void Handle(long message);     // slot 0
    virtual void Slot4();
    virtual unsigned long Count();         // slot 8
};

struct CMailbox_00c1da30 {
    unsigned char _pad[0x18];
    IMailboxHandler_00c1da30* m_pHandler;  // +0x18
    CEvent_00c1da30 m_event;               // +0x1c
    long Dispatch(long message);
};

long CMailbox_00c1da30::Dispatch(long message)
{
    m_event.Wait();
    CProfileLock_00c1da30 lock(this);
    if (!(m_pHandler->Count() > 0)) {
        {
            CString_00c1da30 line;
            line.Append("d:\\dev\\audio\\ver_fable_1.5_b\\libpk\\PKCMailbox.h")->Append("(")->AppendNumber(63)->Append(") : ")->Append("the queue is out of step with the mailbox handler")->Append("\n");
            if (g_pfnLogSink_0143bb2c)
                g_pfnLogSink_0143bb2c(line.m_pchData);
        }
        CTrace_00c1da30();
    }
    m_pHandler->Handle(message);
    return message;
}
