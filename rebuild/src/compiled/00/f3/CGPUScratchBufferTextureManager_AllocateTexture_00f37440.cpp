// CGPUScratchBufferTextureManager::AllocateTexture(long,const C2DExtentsI&,long,const CPixelFormat&,unsigned long,bool) -- allocate a texture, retrying without usage flags
// Retail00f37440 224bytes; no manager header exists; actual texture/pixel headers retained. Callees F372B0,BFEA1A,9FA280,F36F40.
#include "engine/CTexture.h"
#include <string.h>
#include "engine/CPixelFormat.h"
class C2DExtentsI { public: long Width, Height; };
extern char g_HeadlessRendering; // 0143e968
struct CTextureNative : CTexture {
    CTextureNative() { PD3DTexture=0; memset(reinterpret_cast<unsigned char*>(static_cast<CTexture*>(this))+offsetof(CTexture,ByteLength),0,sizeof(ByteLength)+sizeof(AllocationSource)); }
    bool Initialise(const C2DExtentsI&,long,const CPixelFormat&,unsigned long,long,bool,bool); // 009fa280
};
class CSurface;
class CGPUScratchBufferTextureManager { public:
    struct CResourceEntry {
        long ResourceType; CTextureNative* Texture; CSurface* Surface; long PageIndex;
        C2DExtentsI Dimensions; long NoLevels; CPixelFormat PixelFormat;
        long MultiSampleType; unsigned long UseageFlags; long MemoryPool;
    };
    void* Vtable; CResourceEntry* FirstResource; CResourceEntry* LastResource; CResourceEntry* EndResource;
    long AllocResourceEntry(long); // 00f372b0
    void FreeResourceEntry(long); // 00f36f40
    long AllocateTexture(long,const C2DExtentsI&,long,const CPixelFormat&,unsigned long,bool);
};
long CGPUScratchBufferTextureManager::AllocateTexture(long page,const C2DExtentsI& dimensions,long levels,const CPixelFormat& format,unsigned long usage,bool managed)
{
    long index=AllocResourceEntry(page);
    const CPixelFormat& pixel=format;
    long count=levels;
    CResourceEntry* entry=&FirstResource[index];
    entry->Dimensions=dimensions;
    entry->NoLevels=count;
    entry->PixelFormat=pixel;
    entry->UseageFlags=usage;
    entry->Texture=new CTextureNative;
    entry->ResourceType=1;
    entry->MemoryPool=managed ? 1 : 0;
    if (!g_HeadlessRendering && !entry->Texture->Initialise(dimensions,count,pixel,usage,entry->MemoryPool,true,false)) {
        if (!entry->Texture->Initialise(dimensions,count,pixel,0,entry->MemoryPool,true,false)) {
            FreeResourceEntry(index);
            return -1;
        }
    }
    return index;
}