// CMailboxQueue_00c48c80::ProcessMessage(void*) -- EH-framed: under a scoped lock over `this`, 1 when the mailbox at +0x18 has a pending item (virtual slot 3), else the semaphore at +0x1c is released (@ 0x00C2A150), the message pushed into the mailbox (virtual slot 1) and 0 returned
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C48C80, oracle length 153 bytes. Built under /EHsc.
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; sub esp,8 ; push esi ; mov esi,ecx ; push esi ; lea ecx,[esp+8] ; call PKCProtectionInstance::PKCProtectionInstance ; mov ecx,[esi+18h] ; mov eax,[ecx] ; mov dword ptr [esp+14h],0 ; call [eax+0Ch] ; test eax,eax ; je push ; lea ecx,[esp+4] ; mov dword ptr [esp+14h],-1 ; call ~PKCProtectionInstance ; mov eax,1 ; pop esi ; ... ret 4 ; push: lea ecx,[esi+1Ch] ; call CSemaphore::Release ; mov ecx,[esi+18h] ; mov eax,[message] ; mov edx,[ecx] ; push eax ; call [edx+4] ; lea ecx,[esp+4] ; mov dword ptr [esp+14h],-1 ; call ~PKCProtectionInstance ; mov ecx,[esp+0Ch] ; xor eax,eax ; pop esi ; ...
// A second copy of the mailbox-queue body landed at 0x00C49B90 (same offsets, same slots). The
// manifest label is propagated; the class is named by address.
class PKCCriticalSection;
class PKCProtectionInstance { public:                       // 8-byte scoped lock
    void* m_pTarget;
    long m_state;
    PKCProtectionInstance(PKCCriticalSection* target);             // 0x00C0FFC0
    ~PKCProtectionInstance();                        // 0x00C0FFE0
};

struct CMailbox_00c48c80 {
    virtual void v00();
    virtual void Push(void* message);      // slot 1
    virtual void v02();
    virtual long HasPending();             // slot 3
};

struct CSemaphore_00c48c80 {
    void Release();                        // 0x00C2A150
};

struct CMailboxQueue_00c48c80 {
    unsigned char _pad[0x18];
    CMailbox_00c48c80* m_pMailbox;             // +0x18
    CSemaphore_00c48c80 m_semaphore;           // +0x1c
    long ProcessMessage(void* message);
};

long CMailboxQueue_00c48c80::ProcessMessage(void* message)
{
    PKCProtectionInstance lock((PKCCriticalSection*)this);
    if (m_pMailbox->HasPending())
        return 1;
    m_semaphore.Release();
    m_pMailbox->Push(message);
    return 0;
}