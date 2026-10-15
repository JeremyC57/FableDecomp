// CMessageReceiver_00c38ac0::ReceiveMessage() -- dispatch the next 12-byte message of the mailbox queue at +0x74 into a frame record, keep its two parameter words at +0x174 / +0x178 and return its id
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C38AC0, length 48 bytes.
// Leaf: ReceiveMessage
// Callees: CMailboxQueue::DispatchMessage(void*) @ 0x00C39670 (landed: semaphore acquire, scoped lock, the mailbox's
// slot 0 fills the caller's buffer, which is returned and ignored here).
// The chunk label "CTCActivationReceptorBase::AddTrigger" is propagated and rejected (the receptor is 0x2c bytes; this owner
// has a mailbox queue at +0x74 and words at +0x174/+0x178, in the 0x00C3xxxx audio-library module); named by address.
// Shape: `sub esp,0Ch` is the message record; after the call its words are read back in the order +4, +8, +0 and the
// first one is the return value.
struct SMailMessage {                          // 12-byte message record
    long m_id;                                 // +0
    long m_param1;                             // +4
    long m_param2;                             // +8
};

struct CMailboxQueue {
    unsigned char _body[0x100];
    void* DispatchMessage(void* message);      // 0x00C39670
};

struct CMessageReceiver_00c38ac0 {
    unsigned char _pad[0x74];
    CMailboxQueue m_queue;                     // +0x74
    long m_param1;                             // +0x174
    long m_param2;                             // +0x178
    long ReceiveMessage();
};

long CMessageReceiver_00c38ac0::ReceiveMessage()
{
    SMailMessage message;
    m_queue.DispatchMessage(&message);
    m_param1 = message.m_param1;
    m_param2 = message.m_param2;
    return message.m_id;
}
