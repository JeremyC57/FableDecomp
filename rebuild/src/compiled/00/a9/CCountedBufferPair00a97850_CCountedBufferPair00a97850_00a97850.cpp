// CCountedBufferPair_00a97850::~CCountedBufferPair_00a97850() -- free the buffers at +0x14 and +8 (each when held), then the CIVCountedPointeeBase base's vtable (copy a of two)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A97850, length 43 bytes.
// Callees: free @ 0x00BFEA14 twice. Vtable 0x012354D0 = CIVCountedPointeeBase (rtti-vtables.tsv): the base's inline destructor stores it last; no vtable store of this
// class's own (`__declspec(novtable)`). Shape: members destroyed in reverse declaration order, each `test ; je ; push ; call ; add esp,4`. The chunk label is
// propagated; the class is named by address.
extern "C" void __cdecl free(void* block);        // 0x00BFEA14

class CIVCountedPointeeBase { public:                     // vtable 0x012354D0
    virtual void v00();
    unsigned char _pad[4];                         // +0x04
    ~CIVCountedPointeeBase() { }
};

struct __declspec(novtable) CCountedBufferPair_00a97850 : CIVCountedPointeeBase {
    void*         m_bufferA;                       // +0x08
    unsigned char _pad0c[8];
    void*         m_bufferB;                       // +0x14
    ~CCountedBufferPair_00a97850();
};

CCountedBufferPair_00a97850::~CCountedBufferPair_00a97850()
{
    if (m_bufferB)
        free(m_bufferB);
    if (m_bufferA)
        free(m_bufferA);
}