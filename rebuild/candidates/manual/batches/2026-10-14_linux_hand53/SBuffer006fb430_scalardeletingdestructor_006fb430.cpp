// SBuffer_006fb430::`scalar deleting destructor'(unsigned int flags) -- free the array at +4 (`operator delete[]` @ 0x00BFEB1C), clear the pointer and the count at +8, and free the record for flag bit 0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006FB430, length 48 bytes.
// Leaf: ??_GSBuffer_006fb430@@QAEPAXI@Z
// The chunk label is propagated; named by address. Shape: no vtable (a non-polymorphic class deleted with `delete`), `this` in esi, `test byte ptr [esp+8],1`, `ret 4`.
void __cdecl operator delete(void* block);         // 0x00BFE9BC
void __cdecl operator delete[](void* block);       // 0x00BFEB1C
struct SBuffer_006fb430 {
    long  m_0;                                     // +0x00
    char* m_pBuffer;                               // +0x04
    long  m_count;                                 // +0x08
    ~SBuffer_006fb430()
    {
        delete[] m_pBuffer;
        m_pBuffer = 0;
        m_count = 0;
    }
};

void Kill_006fb430(SBuffer_006fb430* object)                      // forces the scalar deleting destructor into the object; not part of the retail body
{
    delete object;
}
