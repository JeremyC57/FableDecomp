// CEngine::GetScratchZBufferD24S8(unsigned long size) -- the shared D24S8 depth surface for a power-of-two size class: index = floor(log2(size)); grow the cache array when needed and create the size x size depth buffer on first use
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B27EE0, length 289 bytes (`ret 4`).
// Callees: CArray<CSurface*>::resize(unsigned int, const value&) @ 0x00B2AAB0, operator new @ 0x00BFEA1A, CPixelFormat::CPixelFormat
// (D3DFORMAT) @ 0x009E3830 (catalogued as CPixelFormat::Initialise), CSurface::InitialiseAsDepthBuffer(const C2DExtentsI&,
// const CPixelFormat&, ESurfaceMultisampleType, bool) @ 0x009F32A0. Vtable 0x0122F84C = CSurface (rtti-vtables.tsv).
// Layout: engine/CSurface.h (vptr, PD3DSurface +4, AllocationSource +8); the retail ScratchZBuffersD24S8 array (12-byte CArray)
// is at +0x14c (PDB CEngine +0x154: earlier containers are narrower), so `this` is modelled locally.
// Shape: the landed HighestSetBit 0x0099FDB0 inlined (full form: `or esi,-1` for 0, 31 for the top bit); the resize fill value
// and the pixel format both live in the dead `size` argument slot (size itself stays in ebx); the format is a named local
// built before the call, the extents a call temporary (built after the pushes); every element access re-reads the array.
#include "engine/CSurface.h"
#include "engine/CPixelFormat.h"
#include "rebuild_abi.h"

void* __cdecl operator new(unsigned int size);     // 0x00BFEA1A
extern void* g_CSurfaceVtable[];                   // 0x0122F84C

__forceinline long HighestSetBit(unsigned long value)
{
    if (value == 0)
        return -1;
    if (value >= 0x80000000)
        return 31;
    ++value;
    long r = ((long)((1 << 16) - value) >> 31) & 16;
    r += ((long)((1 << (r + 8)) - value) >> 31) & 8;
    r += ((long)((1 << (r + 4)) - value) >> 31) & 4;
    r += ((long)((1 << (r + 2)) - value) >> 31) & 2;
    r += ((long)((1 << (r + 1)) - value) >> 31) & 1;
    return r;
}

struct C2DExtentsI {
    long Width, Height;
    C2DExtentsI(long w, long h) : Width(w), Height(h) { }
};

struct CPixelFormatNative : CPixelFormat {
    CPixelFormatNative(long d3dFormat);            // 0x009E3830
};

struct CSurfaceNative : CSurface {
    CSurfaceNative() { __vftable = g_CSurfaceVtable; PD3DSurface = 0; AllocationSource = 0; }
    bool InitialiseAsDepthBuffer(const C2DExtentsI& extents, const CPixelFormat& format, long multisample, bool lockable);   // 0x009F32A0
};

template <class T> struct CRetailArray {           // retail CArray: {first, last, end}
    T* first; T* last; T* end_of_storage;
    unsigned int size() const { return last - first; }
    T& operator[](unsigned int n) { return first[n]; }
    void resize(unsigned int count, const T& value);   // 0x00B2AAB0
};

struct CEngine {
    unsigned char                  _pad[0x14c];
    CRetailArray<CSurfaceNative*>  ScratchZBuffersD24S8;   // +0x14c
    CSurfaceNative* GetScratchZBufferD24S8(unsigned long size);
};

CSurfaceNative* CEngine::GetScratchZBufferD24S8(unsigned long size)
{
    unsigned long level = HighestSetBit(size);
    if (level >= ScratchZBuffersD24S8.size())
        ScratchZBuffersD24S8.resize(level + 1, 0);
    if (ScratchZBuffersD24S8[level] == 0) {
        ScratchZBuffersD24S8[level] = new CSurfaceNative;
        CPixelFormatNative format(0x4b);           // D3DFMT_D24S8
        ScratchZBuffersD24S8[level]->InitialiseAsDepthBuffer(C2DExtentsI(size, size), format, 0, false);
    }
    return ScratchZBuffersD24S8[level];
}
