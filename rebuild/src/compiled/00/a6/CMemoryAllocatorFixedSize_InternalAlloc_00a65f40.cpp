// CMemoryAllocatorFixedSize::InternalAlloc(unsigned long size, void** out) -- allocate a raw block of `size` plus the alignment slack ((Alignment - 1) rounded down to 8) from the process heap; returns the real block size (HeapSize) or 0 when the request overflows 32 bits or the heap fails
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A65F40, length 100 bytes.
// Callees: KERNEL32 GetProcessHeap (IAT 0x0143FD3C, kept in esi), HeapAlloc (IAT 0x0143FE88), HeapSize (IAT 0x0143FE84); flags 1 = HEAP_NO_SERIALIZE.
// Layout: CMemoryAllocatorBase::Alignment at +0xc14 (PDB).
// Shape: the overflow test is a 64-bit sum (`add ; adc` with the high word spilled into the 8-byte frame) while the
//   HeapAlloc size is recomputed in 32 bits; the success path is written nested so it falls through (`if (ok) { ... if (*out) return HeapSize; } return 0;`).
//   GetProcessHeap is used twice, so the dllimport loads it once into esi.
extern "C" __declspec(dllimport) void* __stdcall GetProcessHeap();                                              // IAT 0x0143FD3C
extern "C" __declspec(dllimport) void* __stdcall HeapAlloc(void* heap, unsigned long flags, unsigned long size);   // IAT 0x0143FE88
extern "C" __declspec(dllimport) unsigned long __stdcall HeapSize(void* heap, unsigned long flags, const void* mem); // IAT 0x0143FE84

struct CMemoryAllocatorFixedSize {
    unsigned char _base[0xc14];                    // vptr + CPerformanceStats
    unsigned long Alignment;                       // +0xc14 (CMemoryAllocatorBase)
    unsigned long InternalAlloc(unsigned long size, void** out);
};

unsigned long CMemoryAllocatorFixedSize::InternalAlloc(unsigned long size, void** out)
{
    unsigned long extra = (Alignment - 1) & ~7UL;
    unsigned __int64 total = (unsigned __int64)extra + size;
    if (total <= 0xFFFFFFFF) {
        *out = HeapAlloc(GetProcessHeap(), 1, extra + size);
        if (*out)
            return HeapSize(GetProcessHeap(), 1, *out);
    }
    return 0;
}