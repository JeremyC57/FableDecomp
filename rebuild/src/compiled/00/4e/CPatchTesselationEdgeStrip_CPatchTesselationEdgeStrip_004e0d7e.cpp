// CPatchTesselationEdgeStrip::~CPatchTesselationEdgeStrip() -- frees the blocks of the three array records at +0x40/+0x34/+0x28, then runs the inlined CDefPointeeBase destructor (vtable store + base tail jump)
// Retail: Fable.exe (TLC, VC7.1) @ 0x004E0D7E, 59 bytes. Pragma: optimize("s") (`pop ecx` cleanup after each free).
// Callees: free @ 0x00BFEA14 (CRT), base destructor @ 0x009FC550 (labelled "~CMemoryBlock"); vtable 0x01230BA0 is
// CDefPointeeBase (rtti-vtables.tsv).
// Shape: a def class over CDefPointeeBase. The derived class stores no vtable of its own (`__declspec(novtable)`),
// its three 12-byte array members are released last-to-first, then CDefPointeeBase's empty inline destructor
// stores its vtable right before the `jmp` to the root destructor (same model as the landed 0x0044F81A).
// The "CPatchTesselationEdgeStrip" dtor label is propagated over byte-identical def destructors.
#pragma optimize("s", on)

extern "C" void __cdecl free(void* memory);         // 0x00BFEA14

struct CHeapArray {                                 // 12-byte array record: data, size, capacity
    void* m_pData;
    long  m_size;
    long  m_capacity;
    __forceinline ~CHeapArray() { if (m_pData != 0) free(m_pData); }
};

struct CBase_009fc550 {
    virtual void v00();
    ~CBase_009fc550();                              // 0x009FC550
};

struct CDefPointeeBase : CBase_009fc550 {           // vtable 0x01230BA0
    virtual void v00();
    ~CDefPointeeBase() { }
};

struct __declspec(novtable) CPatchTesselationEdgeStrip : CDefPointeeBase {
    unsigned char m_pad04[0x24];
    CHeapArray    m_arrayA;                         // +0x28
    CHeapArray    m_arrayB;                         // +0x34
    CHeapArray    m_arrayC;                         // +0x40
    virtual void v00();
    ~CPatchTesselationEdgeStrip();
};

CPatchTesselationEdgeStrip::~CPatchTesselationEdgeStrip()
{
}