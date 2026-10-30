// store a long through the pointer at +0x08 while holding the audio library's global lock (an 8-byte scoped lock object built from the global @ 0x0143BB14)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C15FC0, length 48 bytes.
// Callees: 0x00C0FFC0 PKCProtectionInstance::PKCProtectionInstance(void*), 0x00C0FFE0 PKCProtectionInstance::~PKCProtectionInstance
// The manifest label (C3DMeshFilePrimitiveChunk::SetMaterialIndex) is propagated; the lock class is the one the
// landed audio bodies use (0x00C39830 CMailboxQueue::DispatchMessage names it PKCProtectionInstance), here over a global
// lock target instead of a queue. The target pointer at +0x08 is the only member touched.
//

class PKCCriticalSection;
extern PKCCriticalSection g_lock_0143bb14;              // 0x0143BB14
class PKCProtectionInstance { public:                          // 8-byte scoped lock: target + thread id
    PKCCriticalSection* m_pTarget;
    long m_threadId;
    PKCProtectionInstance(PKCCriticalSection* target);         // 0x00C0FFC0
    ~PKCProtectionInstance();                           // 0x00C0FFE0
};
struct CLockedValueOwner_00c15fc0 {
    unsigned char _pad[0x08];
    long* m_pTarget;                         // +0x08
    void SetTargetValueUnderGlobalLock(long value);
};
void CLockedValueOwner_00c15fc0::SetTargetValueUnderGlobalLock(long value)
{
    PKCProtectionInstance lock(&g_lock_0143bb14);
    *m_pTarget = value;
}