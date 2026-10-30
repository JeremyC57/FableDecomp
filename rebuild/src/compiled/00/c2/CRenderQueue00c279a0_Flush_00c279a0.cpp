// CRenderQueue_00c279a0::Flush() -- under the profile lock at 0x0143BAFC, clear the pending count and, unless the queue is suspended, submit its buffer with Render @ 0x00C35DD0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C279A0, length 59 bytes.
// Callees: PKCProtectionInstance::PKCProtectionInstance(void*) @ 0x00C0FFC0 / ~PKCProtectionInstance @ 0x00C0FFE0 (the 8-byte scoped lock, landed shape), the `__stdcall`
// submit @ 0x00C35DD0 (buffer at +0xc, flag 0; manifest "CDrawAutoPickups_ShouldRender" is propagated). Shape: the flag byte at +0x24 is tested
// before the zero store at +0x10 is emitted (the scheduler sinks the store next to the branch). The class is named by address (the chunk label
// "CDraw2DText::ClearPrimitivesIfNecessary" is propagated).
class PKCCriticalSection;
class PKCProtectionInstance { public:                              // 8 bytes: section + entry tick (GOTCHAS wave 23f)
    void* m_pSection;
    long  m_entryTick;
    PKCProtectionInstance(PKCCriticalSection* section);                   // 0x00C0FFC0
    ~PKCProtectionInstance();                               // 0x00C0FFE0
};

extern PKCCriticalSection g_soundLock;   // 0x0143BAFC
void __stdcall RenderBuffer_00c35dd0(void* buffer, long flags);  // 0x00C35DD0

struct CRenderQueue_00c279a0 {
    unsigned char _pad[0x0c];
    void*         m_pBuffer;                       // +0x0c
    long          m_pendingCount;                  // +0x10
    unsigned char _pad14[0x10];
    unsigned char m_suspended;                     // +0x24
    void Flush();
};

void CRenderQueue_00c279a0::Flush()
{
    PKCProtectionInstance lock(&g_soundLock);
    m_pendingCount = 0;
    if (!m_suspended)
        RenderBuffer_00c35dd0(m_pBuffer, 0);
}