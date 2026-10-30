// CQueueOwner_00c279e0::Flush() -- clear the word at +0x10, take the named scoped lock (0x00C0FFC0 with the empty wide string @ 0x0143BAFC), process the queue at +0xc (0x00C35C50 with 0, 0), release the lock (0x00C0FFE0)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C279E0, length 64 bytes.
// Callees: the lock constructor @ 0x00C0FFC0 (an 8-byte local, the name; labelled "CTCHeroExperience::AddSuccessfulBlockParry", propagated), the `__stdcall` queue processor
// @ 0x00C35C50 (labelled "CEngineLandscapePatch::DeleteForegroundVertexBuffers", propagated), the lock destructor @ 0x00C0FFE0 ("EventQueue_ProcessWithTimeout", propagated).
// Shape: the clear scheduled between the name push and the constructor call, `this` in esi. The chunk label "CGameScriptInterface::AddGossip" is propagated; the class is named by address.
class PKCCriticalSection;
extern PKCCriticalSection g_soundLock;   // 0x0143BAFC (the sound critical section, chunk 586)

class PKCProtectionInstance { public:                 // 8 bytes on the stack
    unsigned char _body[8];
    PKCProtectionInstance(PKCCriticalSection* name);   // 0x00C0FFC0
    ~PKCProtectionInstance();                     // 0x00C0FFE0
};
void __stdcall ProcessQueue_00c35c50(void* queue, long a, long b);   // 0x00C35C50

struct CQueueOwner_00c279e0 {
    unsigned char _pad[0xc];
    void*         m_pQueue;                        // +0x0c
    long          m_pending;                       // +0x10
    void Flush();
};

void CQueueOwner_00c279e0::Flush()
{
    m_pending = 0;
    PKCProtectionInstance lock(&g_soundLock);
    ProcessQueue_00c35c50(m_pQueue, 0, 0);
}