// CBufferedReader_00cab6d0::`scalar deleting destructor'(unsigned int) -- return the buffer at +0x3c when one is held, restore the vtable, optional delete
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CAB6D0, length 41 bytes.
// Callees: FreeBuffer(void*) @ 0x00CB1F20 (on `this`; manifest "CBankFileAsyncData::FreeBuffer"), operator delete @ 0x00BFE9BC when bit 0 of the flag is set.
// Vtable 0x012BF6E0 is not in rtti-vtables.tsv; the chunk label "CEngineComponent::`scalar_deleting_destructor'" is propagated. Shape: the buffer pointer is
// loaded and tested around the vtable store; the destructor is `__forceinline` so its body lands inside the `??_G`.
void __cdecl operator delete(void* block);         // 0x00BFE9BC

struct CBufferedReader_00cab6d0 {                  // vtable 0x012BF6E0
    virtual void v00();
    unsigned char _pad04[0x38];
    void*         m_pBuffer;                       // +0x3c
    void FreeBuffer(void* buffer);                 // 0x00CB1F20
    __forceinline virtual ~CBufferedReader_00cab6d0()
    {
        if (m_pBuffer != 0)
            FreeBuffer(m_pBuffer);
    }
};

// VC7.1 emits a class's vtable (and the deleting destructor it references) in every translation
// unit that constructs the object; this helper is that construction, nothing more.
void* MakeOne()
{
    return new CBufferedReader_00cab6d0;
}
