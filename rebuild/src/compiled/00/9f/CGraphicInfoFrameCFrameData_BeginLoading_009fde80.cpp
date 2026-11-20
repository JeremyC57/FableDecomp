// CGraphicInfoFrame::CFrameData::BeginLoading(unsigned long mipmap_level, unsigned long get_flags) -- start loading this frame's texture at
// the finest affordable mip level: clamp the level to the graphic's mip count and small-mip level; a loaded level at least that fine is
// only touched; otherwise allocate a texture (volume, or the finest level from the request down to the limit that fits), abort any
// read in flight, make the new texture the loading one and queue an async read of the level's bytes with the bank's resource bank.
// Retail: Fable.exe (TLC, VC7.1) @ 0x009FDE80, 1265 bytes, thiscall `ret 8`; Ego_r.pdb / FableWin.pdb / Ego_d.pdb
//   ?BeginLoading@CFrameData@CGraphicInfoFrame@@QAEXKK@Z.
// Leaf: ?BeginLoading@CFrameData@CGraphicInfoFrame@@QAEXKK@Z
// Locals: Ego_d.pdb lists every local with its type and scope (header, max_level, texture_manager, pixel_format, allocate_success,
//   managed_texture, C3DExtentsI dimensions, texture2, texture, async_entry, unsigned long total_size, long data_size, data_offset,
//   mipmap_bytesize; loop block { c_level { current_size } }). The 12-byte C3DExtentsI `dimensions` (not three longs) is what gives
//   retail's frame: its Z (the depth, used only by the volume call) homed in the top frame word above the packer slots (0x34 frame; three
//   scalar longs gave 0x30 for twelve earlier agents). No `bank` local in Ego_d: GetBank() at each use.
// Statement structure also evidenced by the Anniversary 360 Test build's PPC line table (third source): early `if (!allocate_success)
//   return;`, the comma for-increment `++mipmap_level, dimensions.X /= 2, dimensions.Y /= 2` (retail loads mipmap_level before the
//   halving), unsigned c_level (jbe 0x009FE200 / jae 0x009FE215), unsigned division of total_size (div at 0x009FE1E7).
// Callees: CBankFileAsync::GetAsyncEntry 0x009D58D0 (Ego_r.pdb ?GetAsyncEntry@CBankFileAsync@@..., the earlier draft's "CreateEntry" is in
//   neither PDB), CTextureManager::AllocateVolumeTexture 0x00A69C20 / AllocateTexture 0x00A69B70, CBankFileAsyncData::Abort 0x009D6350 /
//   WaitUntilFinished 0x009D5B60, CBankFileAsyncEntry::BeginReadDataAutoAlloc 0x009D6100 (the shared engine/CBankFileAsyncEntry.h class,
//   opt-in FABLE_RETAIL_CBANKFILEASYNCENTRY_WATER: Ego_d TPI 0xA2CF layout).
// Inline-budget: CGraphicInfoFrame::TouchFrame is expanded at 0x009FDECA..0x009FDF89; the shared FORCEINLINE body (CGraphicInfo.h
//   FABLE_RETAIL_CGRAPHICINFOFRAME_TOUCHFRAME, PASS22 pointer guard) is used as is; a plain-inline row copy leaves it called (1064 bytes).
// No FITTED items.
#define FABLE_RETAIL_CGRAPHICINFOFRAME_FREE
#define FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS
#define FABLE_RETAIL_CCOUNTEDPOINTER
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_CBANKFILE_ENTRYQUERY
#define FABLE_RETAIL_CGRAPHICINFO_TEXTURETYPE
#define FABLE_RETAIL_CFRAMEDATA_ISLOADING
#define FABLE_RETAIL_CFRAMEDATA_ISLOADING_ISNULL
#define FABLE_RETAIL_CRESOURCE_TOUCHRESOURCE
#define FABLE_RETAIL_CRESOURCE_RESOURCELIST
#define FABLE_RETAIL_CPIXELFORMAT
#define FABLE_RETAIL_CGRAPHICDATABANK_GETPIXELFORMAT
#define FABLE_RETAIL_CTEXTUREMANAGER_ALLOCATETEXTURE
#define FABLE_RETAIL_CTEXTUREMANAGER_ALLOCATEVOLUMETEXTURE
#define FABLE_RETAIL_CMANAGEDTEXTURE_LOCK
#define FABLE_RETAIL_CMANAGEDTEXTURE_GETTEXTURE
#define FABLE_RETAIL_CFRAMEDATA_UPLOADFRAME
#define FABLE_RETAIL_CBANKFILEASYNCDATA_ABORT
#define FABLE_RETAIL_CBANKFILEASYNC_GETASYNCENTRY
#define FABLE_RETAIL_CBANKFILE_ENTRYDATASIZE
#define FABLE_RETAIL_CBANKFILEASYNCDATA_WAITUNTILFINISHED
#define FABLE_RETAIL_CGRAPHICINFO_PEEKGRAPHICHEADER
#define FABLE_RETAIL_CGRAPHICINFOFRAME_GETINDEX
#define FABLE_RETAIL_CGRAPHICINFOFRAME_GETFRAME
#define FABLE_RETAIL_CGRAPHICINFOFRAME_TOUCHFRAME
#define FABLE_RETAIL_CGRAPHICINFOFRAME_GETBANK_DIRECT
#define FABLE_RETAIL_CGRAPHICINFOFRAME_ORIGINAL_TOUCHFRAME_POINTER_GUARD_PASS22
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_GET
#define FABLE_RETAIL_CBANKFILEASYNCENTRY_WATER
#include "engine/CGraphicInfo.h"
#include "engine/CTextureManager.h"
#include "engine/CBankFileAsyncData.h"
#include "engine/CBankFileAsyncEntry.h"
#include "engine/GFSizeof.h"
#include "engine/C3DExtentsI.h"
extern long TextureLoadingPriority;   // 0x01396C64: Ego_d.pdb ?TextureLoadingPriority@@3JA (a long) + the Anniversary PDB global name
inline unsigned long CGraphicInfoFrame::GetIndex() const {return GraphicInfo->Index;}
inline bool CGraphicInfoFrame::IsVolumeTexture() const {return GraphicInfo->IsVolumeTexture();}
inline unsigned long CGraphicFrame::Sizeof() const {return GFPSizeof(FrameData);}
void CGraphicInfoFrame::CFrameData::BeginLoading(unsigned long mipmap_level, unsigned long get_flags)
{
    const CGraphicDataBank::CGraphicHeader& header = GraphicInfoFrame->GraphicInfo->PeekGraphicHeader();
    if (mipmap_level > (unsigned long)(header.MipmapLevels - 1))
        mipmap_level = header.MipmapLevels - 1;
    long max_level = GraphicInfoFrame->SmallMipmapLevel;
    if (mipmap_level > (unsigned long)max_level)
        mipmap_level = max_level;
    if (ManagedTexture.IsValid()) {
        if ((unsigned long)CurrentMipmapLevel <= mipmap_level) {
            GraphicInfoFrame->TouchFrame(mipmap_level);
            return;
        }
        if (max_level > CurrentMipmapLevel - 1)
            max_level = CurrentMipmapLevel - 1;
    }
    if (IsLoading()) {
        if (mipmap_level >= (unsigned long)LoadingMipmapLevel)
            return;
        if (max_level > LoadingMipmapLevel - 1)
            max_level = LoadingMipmapLevel - 1;
    }
    CTextureManager& texture_manager = GetBank()->GetTextureManager();
    CPixelFormat pixel_format = GetBank()->GetPixelFormat(GraphicInfoFrame->GetTextureType());
    ManagedTexture.Lock();
    LoadingManagedTexture.Lock();
    bool allocate_success = false;
    CManagedTexture managed_texture;
    C3DExtentsI dimensions(header.Width >> mipmap_level, header.Height >> mipmap_level, header.Depth >> mipmap_level);
    if (GraphicInfoFrame->IsVolumeTexture()) {
        pixel_format = CPixelFormat(D3DFMT_A8R8G8B8);
        allocate_success = texture_manager.AllocateVolumeTexture(managed_texture, dimensions.X, dimensions.Y, dimensions.Z, header.MipmapLevels - mipmap_level, pixel_format, this);
    } else {
        for (; (long)mipmap_level <= max_level; ++mipmap_level, dimensions.X /= 2, dimensions.Y /= 2) {
            if (texture_manager.AllocateTexture(managed_texture, dimensions.X, dimensions.Y, header.MipmapLevels - mipmap_level, pixel_format, this)) {
                allocate_success = true;
                break;
            }
        }
    }
    ManagedTexture.Unlock();
    LoadingManagedTexture.Unlock();
    if (!allocate_success)
        return;
    if (IsLoading()) {
        if (!FileData->Abort()) {
            if (!(get_flags & 0x10))
                return;
            FileData->WaitUntilFinished();
        }
        FileData.ReleaseData();
    }
    CCountedPointer<CTexture> texture2 = managed_texture.GetTexture();
    LoadingManagedTexture = managed_texture;
    managed_texture.Release();
    LoadingMipmapLevel = (signed char)mipmap_level;
    CCountedPointer<CTexture> texture = LoadingManagedTexture.GetTexture();
    CCountedPointer<CBankFileAsyncEntry> async_entry = GetBank()->GetAsyncEntry(GraphicInfoFrame->GetIndex());
    unsigned long total_size = GetBank()->GetEntryDataSize(GraphicInfoFrame->GetIndex());
    unsigned long data_offset = 0;
    if (header.FrameCount > 1 && !header.IsFlatAnimatedGraphic && GetBank()->GetEntryDataType(GraphicInfoFrame->GetIndex()) == 1) {
        total_size /= header.FrameCount;
        data_offset = GraphicInfoFrame->FrameNum * total_size;
    }
    unsigned long mipmap_bytesize = header.FrameDataSize;
    long data_size = total_size;
    for (unsigned long c_level = 0; c_level < mipmap_level; ++c_level) {
        unsigned long current_size = mipmap_bytesize;
        if (c_level < 1 && header.CompressedMipmapSizes[c_level] > 0)
            current_size = header.CompressedMipmapSizes[c_level];
        data_size -= current_size;
        data_offset += current_size;
        mipmap_bytesize >>= 2;
    }
    FileData = async_entry->BeginReadDataAutoAlloc(data_offset, data_size, (EThreadedFilePriority)TextureLoadingPriority);
    SetResourceSize(Sizeof());
    RemoveFromResourceList();
    GetBank()->AddToLoadingQueue(*this);
}