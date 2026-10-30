// CMailboxQueue_00c399f0::DispatchMessage(void*) -- acquire the semaphore at +0x1C (@ 0x00C2A090), scoped lock, assert the mailbox at +0x18 holds an item (virtual slot 2; else a debug-stream message and a debugger break), dispatch the message through virtual slot 0, return it; EH-framed
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C399F0, oracle length 241 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; sub esp,10h ; push esi ; mov esi,ecx ; lea ecx,[esi+1Ch] ; call CSemaphore::Acquire ; push esi ; lea ecx,[esp+10h] ; call PKCProtectionInstance::PKCProtectionInstance ; mov ecx,[esi+18h] ; mov eax,[ecx] ; mov dword ptr [esp+1Ch],0 ; call [eax+8] ; test eax,eax ; ja D ; mov ecx,[g_pDebugBuffer] ; mov [esp+8],ecx ; mov edx,offset lit ; lea ecx,[esp+8] ; mov byte ptr [esp+1Ch],1 ; call operator<< ; (mov edx,piece ; mov ecx,eax ; call operator<<) x5 ; mov eax,[g_pfnErrorReport] ; test eax,eax ; je S ; mov ecx,[esp+8] ; call eax ; S: lea ecx,[esp+8] ; mov byte ptr [esp+1Ch],0 ; call ~CDebugStream ; lea ecx,[esp+7] ; call CDebugBreak::CDebugBreak ; D: mov ecx,[esi+18h] ; mov esi,[message] ; mov edx,[ecx] ; push esi ; call [edx] ; lea ecx,[esp+0Ch] ; mov dword ptr [esp+1Ch],-1 ; call ~PKCProtectionInstance ; mov ecx,[esp+14h] ; mov eax,esi ; pop esi ; mov fs:[0],ecx ; add esp,1Ch ; ret 4
// Mailbox dispatch: the semaphore at +0x1C is acquired (@ 0x00C2A090), an 8-byte scoped lock
// is taken over the queue (EH state 0), and unless the mailbox at +0x18 reports a positive count
// through its virtual slot 2 an assertion fires: a CDebugStream over the shared debug buffer
// (its own nested scope: state 1 as a byte store, destroyed before the break) receives the file/line/message pieces through
// the `__fastcall` `operator<<` overloads @ 0x00C0DEC0 / 0x00C0DF00, the report callback @
// 0x0143BB2C runs when installed, the stream is destroyed and an empty CDebugBreak temporary
// (@ 0x00C16750, in the high byte of the dead frame dword) breaks into the debugger. The
// message is then dispatched through the mailbox's virtual slot 0 and returned (the slot's
// result is ignored), the lock destroyed (state -1). Built under /EHsc; the funclets land
// through the funclet pass. The manifest label is propagated; the literals are the retail
// strings. One of 6 bodies of this shape.
class PKCCriticalSection;
struct CMailboxQueue_00c399f0;

class PKCProtectionInstance { public:                        // 8-byte scoped lock over the queue
    CMailboxQueue_00c399f0* m_pQueue;
    long m_state;
    PKCProtectionInstance(PKCCriticalSection* queue);   // 0x00C0FFC0
    ~PKCProtectionInstance();                         // 0x00C0FFE0
};

struct CSemaphore {
    long m_value;
    void Acquire();                        // 0x00C2A090
};

struct CMailbox {
    virtual void* Dispatch(void* message); // slot 0
    virtual void v04();
    virtual unsigned long GetCount();      // slot 2
};

struct CDebugStream {                      // 4 bytes: the message buffer
    char* m_pBuffer;
    CDebugStream(char* buffer) : m_pBuffer(buffer) { }
    CDebugStream& __fastcall operator<<(const char* text);   // 0x00C0DEC0
    CDebugStream& __fastcall operator<<(long value);         // 0x00C0DF00
    ~CDebugStream();                       // 0x00C0BA40
};

struct CDebugBreak {                       // empty class whose constructor breaks into the debugger
    CDebugBreak();                         // 0x00C16750
};

extern char* g_pDebugBuffer;               // 0x0139C900
extern void (__fastcall* g_pfnErrorReport)(const char* text);   // 0x0143BB2C

struct CMailboxQueue_00c399f0 {
    unsigned char _pad[0x18];
    CMailbox* m_pMailbox;                  // +0x18
    CSemaphore m_semaphore;                // +0x1C
    void* DispatchMessage_CMailboxQueue_00c399f0(void* message);
};

void* CMailboxQueue_00c399f0::DispatchMessage_CMailboxQueue_00c399f0(void* message)
{
    m_semaphore.Acquire();
    PKCProtectionInstance lock((PKCCriticalSection*)this);
    if (!(m_pMailbox->GetCount() > 0)) {
        {
            CDebugStream s(g_pDebugBuffer);
            s << "d:\\dev\\audio\\ver_fable_1.5_b\\libpk\\PKCMailbox.h" << "(" << 63 << ") : " << "the queue is out of step with the mailbox handler" << "\n";
            if (g_pfnErrorReport)
                g_pfnErrorReport(s.m_pBuffer);
        }
        CDebugBreak();
    }
    m_pMailbox->Dispatch(message);
    return message;
}