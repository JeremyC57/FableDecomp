// CFileInstaller::FreeBuffer(SInstallBuffer* buffer) -- when the buffer is marked in use (+4): clear the mark, count a free buffer (+0x30), drop the pending count (+0x34) by one if the buffer's "pending" byte (+5) is set, and signal the two events at +0x84 and +0x88
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009C3330, length 69 bytes.
// Callees: KERNEL32 SetEvent (IAT 0x0143FDE0, twice through edi: `__declspec(dllimport)`). Shape: the in-use byte through cl, the pending byte through dl
// turned into 0/1 with `setne cl` and subtracted, edi saved only around the two signals, `ret 4`. The manifest label is the class's.
extern "C" __declspec(dllimport) int __stdcall SetEvent(void* event);   // IAT 0x0143FDE0

struct SInstallBuffer_009c3330 {
    long m_0;
    bool m_inUse;                                  // +0x04
    bool m_pending;                                // +0x05
};

class CFileInstaller { public:
    unsigned char _pad[0x30];
    long          m_freeBuffers;                   // +0x30
    long          m_pendingBuffers;                // +0x34
    unsigned char _pad38[0x4c];
    void*         m_hBufferFreedEvent;             // +0x84
    void*         m_hWakeEvent;                    // +0x88
    void FreeBuffer(SInstallBuffer_009c3330* buffer);
};

void CFileInstaller::FreeBuffer(SInstallBuffer_009c3330* buffer)
{
    if (buffer->m_inUse) {
        buffer->m_inUse = false;
        ++m_freeBuffers;
        m_pendingBuffers -= (buffer->m_pending ? 1 : 0);
        SetEvent(m_hBufferFreedEvent);
        SetEvent(m_hWakeEvent);
    }
}