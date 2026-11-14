// CGraphicInfoFrame::CFrameData::BeginLoading(unsigned long mipmap_level, unsigned long flags) -- start loading this frame's texture at
// Grade: DIFFER(1269v1265) -- c2 keeps the allocation loop's mipmap_level (and depth) in ebx and spills the header reference; retail keeps header in ebx for the whole function and mipmap_level in its parameter home (frame +4, size/offset registers swapped)
// the finest affordable mip level: clamp the level to the graphic's mip count and small-mip level; a loaded level at least that fine is
// only touched; otherwise allocate a texture (volume, or the finest level from the request down to the limit that fits), abort any
// read in flight, make the new texture the loading one and queue an async read of the level's bytes with the bank's resource bank.
// Retail: Fable.exe (TLC, VC7.1) @ 0x009FDE80, 1265 bytes, thiscall `ret 8`; Ego_r.pdb / FableWin.pdb ?BeginLoading@CFrameData@CGraphicInfoFrame@@QAEXKK@Z.
// Leaf: ?BeginLoading@CFrameData@CGraphicInfoFrame@@QAEXKK@Z
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
#define FABLE_RETAIL_CBANKFILEASYNC_CREATEENTRY
#define FABLE_RETAIL_CBANKFILE_ENTRYDATASIZE
#define FABLE_RETAIL_CBANKFILEASYNCDATA_WAITUNTILFINISHED
#define FABLE_RETAIL_CGRAPHICINFO_PEEKGRAPHICHEADER
#define FABLE_RETAIL_CGRAPHICINFOFRAME_GETINDEX
#include "engine/CGraphicInfo.h"
#include "engine/CTextureManager.h"
#include "engine/CBankFileAsyncData.h"
#include "engine/CBankFileAsyncEntry.h"
#include "engine/GFSizeof.h"
extern EThreadedFilePriority TextureLoadingPriority;   // 0x01396C64 (name: Anniversary PDB global; the only reference is this row)
inline CGraphicDataBank* CGraphicInfoFrame::GetBank() const {return GraphicInfo->Bank;}
inline bool CGraphicInfoFrame::IsFlatSequence() const {return GraphicInfo->Header.IsFlatAnimatedGraphic;}
inline unsigned long CGraphicInfoFrame::GetIndex() const {return GraphicInfo->Index;}
inline bool CGraphicInfoFrame::IsVolumeTexture() const {return GraphicInfo->IsVolumeTexture();}
inline unsigned long CGraphicFrame::Sizeof() const {return GFPSizeof(FrameData);}
FABLE_FORCEINLINE void CManagedTexture::TouchTexture()
{
    if (!Data.IsNull())
        Data->TouchResource();
}
FABLE_FORCEINLINE void CGraphicInfoFrame::TouchFrame(unsigned long mipmap_level) const
{
    if (!Data.IsNull()) {
        if (Data->TouchedFrame != GetBank()->GetCurrentFrame() || mipmap_level <= (unsigned long)Data->TouchedMipmapLevel) {
            Data->TouchedFrame = GetBank()->GetCurrentFrame();
            Data->ManagedTexture.TouchTexture();
            if (mipmap_level <= (unsigned long)Data->CurrentMipmapLevel)
                Data->TouchResource();
            if ((unsigned long)Data->TouchedMipmapLevel > mipmap_level)
                Data->TouchedMipmapLevel = (signed char)mipmap_level;
        }
    }
}
void CGraphicInfoFrame::CFrameData::BeginLoading(unsigned long mipmap_level, unsigned long flags)
{
    const CGraphicDataBank::CGraphicHeader& header = GraphicInfoFrame->GraphicInfo->PeekGraphicHeader();
    if (mipmap_level > (unsigned long)(header.MipmapLevels - 1))
        mipmap_level = header.MipmapLevels - 1;
    long max_mipmap_level = GraphicInfoFrame->SmallMipmapLevel;
    if (mipmap_level > (unsigned long)max_mipmap_level)
        mipmap_level = max_mipmap_level;
    if (ManagedTexture.IsValid()) {
        if ((unsigned long)CurrentMipmapLevel <= mipmap_level) {
            GraphicInfoFrame->TouchFrame(mipmap_level);
            return;
        }
        if (max_mipmap_level > CurrentMipmapLevel - 1)
            max_mipmap_level = CurrentMipmapLevel - 1;
    }
    if (IsLoading()) {
        if (mipmap_level >= (unsigned long)LoadingMipmapLevel)
            return;
        if (max_mipmap_level > LoadingMipmapLevel - 1)
            max_mipmap_level = LoadingMipmapLevel - 1;
    }
    CGraphicDataBank* bank = GetBank();
    CTextureManager& texture_manager = bank->GetTextureManager();
    CPixelFormat format = bank->GetPixelFormat(GraphicInfoFrame->GetTextureType());
    ManagedTexture.Lock();
    LoadingManagedTexture.Lock();
    bool success = false;
    CManagedTexture texture;
    long width = header.Width >> mipmap_level;
    long height = header.Height >> mipmap_level;
    long depth = header.Depth >> mipmap_level;
    if (GraphicInfoFrame->IsVolumeTexture()) {
        format = CPixelFormat(D3DFMT_A8R8G8B8);
        success = texture_manager.AllocateVolumeTexture(texture, width, height, depth, header.MipmapLevels - mipmap_level, format, this);
    } else {
        for (; (long)mipmap_level <= max_mipmap_level; ++mipmap_level) {
            if (texture_manager.AllocateTexture(texture, width, height, header.MipmapLevels - mipmap_level, format, this)) {
                success = true;
                break;
            }
            width /= 2;
            height /= 2;
        }
    }
    ManagedTexture.Unlock();
    LoadingManagedTexture.Unlock();
    if (success) {
        if (IsLoading()) {
            if (!FileData->Abort()) {
                if (!(flags & 0x10))
                    return;
                FileData->WaitUntilFinished();
            }
            FileData.ReleaseData();
        }
        CCountedPointer<CTexture> new_texture = texture.GetTexture();
        LoadingManagedTexture = texture;
        texture.Release();
        LoadingMipmapLevel = (signed char)mipmap_level;
        CCountedPointer<CTexture> loading_texture = LoadingManagedTexture.GetTexture();
        CCountedPointer<CBankFileAsyncEntry> entry = GetBank()->CreateEntry(GraphicInfoFrame->GetIndex());
        unsigned long size = GetBank()->GetEntryDataSize(GraphicInfoFrame->GetIndex());
        unsigned long offset = 0;
        if (header.FrameCount > 1 && !header.IsFlatAnimatedGraphic && GetBank()->GetEntryDataType(GraphicInfoFrame->GetIndex()) == 1) {
            size /= header.FrameCount;
            offset = GraphicInfoFrame->FrameNum * size;
        }
        unsigned long level_size = header.FrameDataSize;
        for (long c_mipmap = 0; c_mipmap < (long)mipmap_level; ++c_mipmap) {
            unsigned long skip_size = level_size;
            if (c_mipmap < 1 && header.CompressedMipmapSizes[c_mipmap] > 0)
                skip_size = header.CompressedMipmapSizes[c_mipmap];
            offset += skip_size;
            size -= skip_size;
            level_size >>= 2;
        }
        FileData = entry->BeginReadDataAutoAlloc(offset, size, TextureLoadingPriority);
        SetResourceSize(Sizeof());
        RemoveFromResourceList();
        GetBank()->AddToLoadingQueue(*this);
    }
}
