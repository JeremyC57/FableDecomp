// CGPUScratchBufferTextureManager::AllocateTexture(long,const C2DExtentsI&,long,const CPixelFormat&,unsigned long,bool) -- allocate a texture, retrying without usage flags
// Retail00f37440 224bytes; no manager header exists; actual texture/pixel headers retained. Callees F372B0,BFEA1A,9FA280,F36F40.
// Naming (NF160, 2026-11-20, q4297): the made-up CTextureNative stand-in (a CTexture subclass redeclaring Initialise with a long pool)
//   and the local C2DExtentsI model dropped for the shared engine/CTexture.h retail class (FABLE_RETAIL_CTEXTURE: inline default
//   constructor, Initialise) and engine/C2DExtentsI.h, so the call binds Ego_r.pdb 0x00B6A590 / FableWin.pdb 0x030DA090
//   ?Initialise@CTexture@@QAE_NABVC2DExtentsI@@JABVCPixelFormat@@KW4ESurfacePool@@_N3@Z (was ?Initialise@CTextureNative@@...KJ_N2@Z);
//   CResourceEntry::Texture / MemoryPool take their Ego_r.pdb types CTexture* / ESurfacePool (struct_layouts_egor.tsv +0x04 / +0x28).
#define FABLE_RETAIL_CTEXTURE
#include "engine/CTexture.h"
#include "engine/CPixelFormat.h"
extern char g_HeadlessRendering; // 0143e968
class CSurface;
class CGPUScratchBufferTextureManager { public:
    struct CResourceEntry {
        long ResourceType; CTexture* Texture; CSurface* Surface; long PageIndex;
        C2DExtentsI Dimensions; long NoLevels; CPixelFormat PixelFormat;
        long MultiSampleType; unsigned long UseageFlags; ESurfacePool MemoryPool;
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
    entry->Texture=new CTexture;
    entry->ResourceType=1;
    entry->MemoryPool=managed ? SURFACE_POOL_MANAGED : SURFACE_POOL_DEFAULT_LOCATION;
    if (!g_HeadlessRendering && !entry->Texture->Initialise(dimensions,count,pixel,usage,entry->MemoryPool,true,false)) {
        if (!entry->Texture->Initialise(dimensions,count,pixel,0,entry->MemoryPool,true,false)) {
            FreeResourceEntry(index);
            return -1;
        }
    }
    return index;
}