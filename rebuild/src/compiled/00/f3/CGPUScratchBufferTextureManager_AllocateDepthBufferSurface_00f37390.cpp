// CGPUScratchBufferTextureManager::AllocateDepthBufferSurface(long,const C2DExtentsI&,const CPixelFormat&,long) -- allocate and initialise a depth surface resource
// Leaf: ?AllocateDepthBufferSurface@CGPUScratchBufferTextureManager@@
// Retail 00f37390, 162 bytes. No manager header exists; native vectors are 12 bytes versus donor CArray's 16.
// Actual CSurface and CPixelFormat headers retained. CSurface constructor adapter writes its real fields and vtable 0122f84c (RTTI CSurface).
// Callees: AllocResourceEntry 00f372b0; operator new 00bfea1a; CSurface::CreateDepthBufferSurface 009f32a0; FreeResourceEntry 00f36f40.
#include "engine/CSurface.h"
#include "engine/CPixelFormat.h"
class C2DExtentsI { public: long Width, Height; };
extern char g_HeadlessRendering; // 0143e968
extern void* g_CSurfaceVtable[]; // 0122f84c
struct CSurfaceNative : CSurface {
    CSurfaceNative() { __vftable=g_CSurfaceVtable; PD3DSurface=0; AllocationSource=0; }
    bool CreateDepthBufferSurface(const C2DExtentsI&,const CPixelFormat&,long,bool); // 009f32a0
};
class CTexture;
class CGPUScratchBufferTextureManager { public:
    struct CResourceEntry {
        long ResourceType; CTexture* Texture; CSurfaceNative* Surface; long PageIndex;
        C2DExtentsI Dimensions; long NoLevels; CPixelFormat PixelFormat;
        long MultiSampleType; unsigned long UseageFlags; long MemoryPool;
    };
    void* Vtable; CResourceEntry* FirstResource; CResourceEntry* LastResource; CResourceEntry* EndResource;
    long AllocResourceEntry(long); // 00f372b0
    long AllocateDepthBufferSurface(long,const C2DExtentsI&,const CPixelFormat&,long);
private:
    void FreeResourceEntry(long);   // 0x00F36F40
};
long CGPUScratchBufferTextureManager::AllocateDepthBufferSurface(long page,const C2DExtentsI& dimensions,const CPixelFormat& format,long multisample)
{
    long index=AllocResourceEntry(page);
    CResourceEntry* entry=&FirstResource[index];
    entry->Dimensions=dimensions;
    entry->PixelFormat=format;
    entry->MultiSampleType=multisample;
    entry->Surface=new CSurfaceNative;
    entry->ResourceType=2;
    entry->MemoryPool=0;
    if (!g_HeadlessRendering && !entry->Surface->CreateDepthBufferSurface(dimensions,format,multisample,true)) {
        FreeResourceEntry(index);
        return -1;
    }
    return index;
}