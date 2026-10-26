// SPoolHandle_00a8d490::`scalar deleting destructor'(unsigned int flags) -- return the handle at +4 to its pool at +0 (CHandleBasedMemoryPool::Free @ 0x00A02B30) when the pool is set, clear the pool and mark the handle -1, then free the record for flag bit 0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A8D490, length 53 bytes.
// Leaf: ??_GSPoolHandle_00a8d490@@QAEPAXI@Z
// The chunk label is propagated; named by effect. Shape: the pool loaded straight into ecx, `this` in esi, `ret 4`.
void __cdecl operator delete(void* block);         // 0x00BFE9BC
class CHandleBasedMemoryPool { public:
    void Free(long handle);                        // 0x00A02B30
};
struct SPoolHandle_00a8d490 {
    CHandleBasedMemoryPool* m_pPool;               // +0x00
    long                    m_handle;              // +0x04
    ~SPoolHandle_00a8d490()
    {
        if (m_pPool) {
            m_pPool->Free(m_handle);
            m_pPool = 0;
            m_handle = -1;
        }
    }
};

void Kill_00a8d490(SPoolHandle_00a8d490* object)                      // forces the scalar deleting destructor into the object; not part of the retail body
{
    delete object;
}