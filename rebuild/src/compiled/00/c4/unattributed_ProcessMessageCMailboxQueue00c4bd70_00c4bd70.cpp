// CMailboxQueue_00c4bd70::ProcessMessage(void*) -- under a scoped queue lock (ctor @ 0x00C0FFC0, dtor @ 0x00C0FFE0): 1 when the mailbox at +0x18 has a pending item (virtual slot 3), else release the semaphore at +0x1C (@ 0x00C2A150), push the message (virtual slot 1) and return 0; EH-framed
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C4BD70, oracle length 155 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; sub esp,8 ; push esi ; mov esi,ecx ; push esi ; lea ecx,[esp+8] ; call PKCProtectionInstance::PKCProtectionInstance ; mov ecx,[esi+18h] ; mov eax,[ecx] ; mov dword ptr [esp+14h],0 ; call [eax+0Ch] ; test eax,eax ; je E ; lea ecx,[esp+4] ; mov dword ptr [esp+14h],-1 ; call ~PKCProtectionInstance ; mov eax,1 ; pop esi ; mov ecx,[esp+8] ; mov fs:[0],ecx ; add esp,14h ; ret 4 ; E: lea ecx,[esi+1Ch] ; call CSemaphore::Release ; mov ecx,[esi+18h] ; mov eax,[message] ; mov edx,[ecx] ; push eax ; call [edx+4] ; lea ecx,[esp+4] ; mov dword ptr [esp+14h],-1 ; call ~PKCProtectionInstance ; mov ecx,[esp+0Ch] ; xor eax,eax ; pop esi ; mov fs:[0],ecx ; add esp,14h ; ret 4
// Mailbox queue step under an 8-byte scoped lock (constructed over the queue, EH state 0 while it
// lives, destroyed on both exits): when the mailbox at +0x18 reports a pending item through its
// virtual slot 3 the call returns 1; otherwise the semaphore at +0x1C is released (@ 0x00C2A150),
// the message is pushed through virtual slot 1 and 0 is returned. Built under /EHsc (the
// `__ehhandler` is not compared; the unwind funclet lands separately through the funclet pass).
// The manifest label is propagated; the queue class is named by address. One of 8
// bodies of this shape.
class PKCCriticalSection;
struct CMailboxQueue_00c4bd70;

class PKCProtectionInstance { public:                        // 8-byte scoped lock over the queue
    CMailboxQueue_00c4bd70* m_pQueue;
    long m_state;
    PKCProtectionInstance(PKCCriticalSection* queue);   // 0x00C0FFC0
    ~PKCProtectionInstance();                         // 0x00C0FFE0
};

struct CSemaphore {
    long m_value;
    void Release();                        // 0x00C2A150
};

struct CMailbox {
    virtual void v00();
    virtual void Push(void* message);      // slot 1
    virtual void v08();
    virtual void* Peek();                  // slot 3
};

struct CMailboxQueue_00c4bd70 {
    unsigned char _pad[0x18];
    CMailbox* m_pMailbox;                  // +0x18
    CSemaphore m_semaphore;                // +0x1C
    long ProcessMessage_CMailboxQueue_00c4bd70(void* message);
};

long CMailboxQueue_00c4bd70::ProcessMessage_CMailboxQueue_00c4bd70(void* message)
{
    PKCProtectionInstance lock((PKCCriticalSection*)this);
    if (m_pMailbox->Peek())
        return 1;
    m_semaphore.Release();
    m_pMailbox->Push(message);
    return 0;
}