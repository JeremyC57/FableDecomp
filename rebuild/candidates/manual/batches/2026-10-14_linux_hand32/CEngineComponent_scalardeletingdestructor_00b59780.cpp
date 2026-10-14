// CEngineComponent::`scalar deleting destructor'(unsigned int) -- clear the owner's back-pointer to this component (when registered), restore the vtable, optional delete
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B59780, length 41 bytes.
// Callees: operator delete @ 0x00BFE9BC when bit 0 of the flag is set. Vtable 0x012A2544 = CEngineComponent (rtti-vtables.tsv). Shape: the back-pointer at +4
// is loaded and tested around this class's vtable store (scheduling); `*m_ppSlot = 0` is an immediate store. PDB CEngineComponent is 12 bytes (vptr + two
// words); the destructor is `__forceinline` so its body lands inside the `??_G`.
void __cdecl operator delete(void* block);         // 0x00BFE9BC

struct CEngineComponent {                          // vtable 0x012A2544
    CEngineComponent** m_ppRegisteredSlot;         // +0x04 the engine's slot holding this component, or null
    long               m_id;                       // +0x08
    __forceinline virtual ~CEngineComponent()
    {
        if (m_ppRegisteredSlot != 0)
            *m_ppRegisteredSlot = 0;
    }
};

// VC7.1 emits a class's vtable (and the deleting destructor it references) in every translation
// unit that constructs the object; this helper is that construction, nothing more.
void* MakeOne()
{
    return new CEngineComponent;
}
