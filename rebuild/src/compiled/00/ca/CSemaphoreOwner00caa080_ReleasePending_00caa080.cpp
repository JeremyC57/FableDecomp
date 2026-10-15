// CSemaphoreOwner_00caa080::ReleasePending() -- when releases are pending, releases the semaphore by that count and clears the counter
// Leaf: ReleasePending
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CAA080, length 32 bytes.
// Callees: ReleaseSemaphore (import [0x0143FEB0], `__stdcall`: handle +0x30, count +0x34, null previous-count).
// Label: the manifest's "CTCCarryable::OnKill" is propagated and rejected; no direct caller or vtable reference
// was found, so the owning class (an audio/streaming worker of the 0x00CAxxxx module) is named by address.
// Shape: the count is read once for the test and pushed from the register; the clear follows the call.
extern "C" __declspec(dllimport) int __stdcall ReleaseSemaphore(void* semaphore, long releaseCount, long* previousCount);   // [0x0143FEB0]

struct CSemaphoreOwner_00caa080 {
    unsigned char _pad[0x30];
    void*         m_hSemaphore;                     // +0x30
    long          m_pendingReleases;                // +0x34
    void ReleasePending();
};

void CSemaphoreOwner_00caa080::ReleasePending()
{
    if (m_pendingReleases) {
        ReleaseSemaphore(m_hSemaphore, m_pendingReleases, 0);
        m_pendingReleases = 0;
    }
}