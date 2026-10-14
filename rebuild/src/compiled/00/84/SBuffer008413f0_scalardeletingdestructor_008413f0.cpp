// SBuffer_008413f0::`scalar deleting destructor'(unsigned int flags) -- free the array at +0 (`operator delete[]` @ 0x00BFEB1C) when present and clear it, clear the count at +4, and free the record for flag bit 0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008413F0, length 53 bytes.
// Leaf: ??_GSBuffer_008413f0@@QAEPAXI@Z
// The chunk label is propagated; named by effect. Shape: the count clear scheduled after the flag test, `this` in esi, `ret 4`.
void __cdecl operator delete(void* block);         // 0x00BFE9BC
void __cdecl operator delete[](void* block);       // 0x00BFEB1C
struct SBuffer_008413f0 {
    char* m_pBuffer;                               // +0x00
    long  m_count;                                 // +0x04
    ~SBuffer_008413f0()
    {
        if (m_pBuffer) {
            delete[] m_pBuffer;
            m_pBuffer = 0;
        }
        m_count = 0;
    }
};

void Kill_008413f0(SBuffer_008413f0* object)                      // forces the scalar deleting destructor into the object; not part of the retail body
{
    delete object;
}