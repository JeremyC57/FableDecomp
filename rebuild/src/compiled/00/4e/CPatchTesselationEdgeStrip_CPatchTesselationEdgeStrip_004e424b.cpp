// CPatchTesselationEdgeStrip::~CPatchTesselationEdgeStrip() -- frees the blocks of the four 16-byte array records at +0x58/+0x48/+0x38/+0x28, then runs the inlined CDefPointeeBase destructor (vtable store + base tail jump)
// Retail: Fable.exe (TLC, VC7.1) @ 0x004E424B, 73 bytes. Pragma: optimize("s") (`pop ecx` cleanup after each free).
// Callees: free @ 0x00BFEA14 (CRT), base destructor @ 0x009FC550 (labelled "~CMemoryBlock"); vtable 0x01230BA0 is
// CDefPointeeBase (rtti-vtables.tsv).
// Shape: as 0x004E0D7E with four 16-byte records (data pointer first): the derived class is `__declspec(novtable)`,
// members are released last-to-first, CDefPointeeBase's empty inline destructor stores its vtable before the `jmp`.
// The "CPatchTesselationEdgeStrip" dtor label is propagated over byte-identical def destructors.
#pragma optimize("s", on)

extern "C" void __cdecl free(void* memory);         // 0x00BFEA14

struct CHeapArray16 {                               // 16-byte array record: data, size, capacity, grow step
    void* m_pData;
    long  m_size;
    long  m_capacity;
    long  m_growBy;
    __forceinline ~CHeapArray16() { if (m_pData != 0) free(m_pData); }
};

class CResource { public:
    virtual void v00();
    virtual ~CResource();                              // 0x009FC550
};

class CDefPointeeBase : public CResource { public:           // vtable 0x01230BA0
    virtual void v00();
    ~CDefPointeeBase() { }
};

struct __declspec(novtable) CPatchTesselationEdgeStrip : CDefPointeeBase {
    unsigned char m_pad04[0x24];
    CHeapArray16  m_arrayA;                         // +0x28
    CHeapArray16  m_arrayB;                         // +0x38
    CHeapArray16  m_arrayC;                         // +0x48
    CHeapArray16  m_arrayD;                         // +0x58
    virtual void v00();
    ~CPatchTesselationEdgeStrip();
};

CPatchTesselationEdgeStrip::~CPatchTesselationEdgeStrip()
{
}