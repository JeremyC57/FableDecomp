// SBuffer_009a0630::`scalar deleting destructor'(unsigned int flags) -- free the array at +0 (`operator delete[]` @ 0x00BFEB1C) when present and clear it, clear the count at +4 and the 31-bit count at +8 (its top flag bit kept), and free the record for flag bit 0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009A0630, length 66 bytes.
// Leaf: ??_GSBuffer_009a0630@@QAEPAXI@Z
// The chunk label is propagated; named by effect. Shape: the two clears scheduled after the flag test, `this` in esi, `ret 4`.
void __cdecl operator delete(void* block);         // 0x00BFE9BC
void __cdecl operator delete[](void* block);       // 0x00BFEB1C
struct SBuffer_009a0630 {
    char*         m_pBuffer;                       // +0x00
    long          m_count;                         // +0x04
    unsigned long m_capacity : 31;                 // +0x08 (the top bit is a flag: `and ecx,80000000h`)
    unsigned long m_flag : 1;
    ~SBuffer_009a0630()
    {
        if (m_pBuffer) {
            delete[] m_pBuffer;
            m_pBuffer = 0;
        }
        m_count = 0;
        m_capacity = 0;
    }
};

void Kill_009a0630(SBuffer_009a0630* object)                      // forces the scalar deleting destructor into the object; not part of the retail body
{
    delete object;
}
