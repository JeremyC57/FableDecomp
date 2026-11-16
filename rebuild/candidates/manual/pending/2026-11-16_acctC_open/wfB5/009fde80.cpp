// CGraphicInfoFrame::CFrameData::BeginLoading(unsigned long mipmap_level, unsigned long get_flags) -- start loading this frame's texture at
// Grade: DIFFER(1265v1265) -- every instruction and register equals retail (rx diff: only the 2 relocation-masked lines); the ONLY residue is
//   the frame: retail 0x34 with depth's home in the TOP slot (created after every 8-byte item), ours 0x30 with depth's home made from the
//   packer's size-sorted list (the read temp joins it); all other frame offsets follow from that slot.
// the finest affordable mip level: clamp the level to the graphic's mip count and small-mip level; a loaded level at least that fine is
// only touched; otherwise allocate a texture (volume, or the finest level from the request down to the limit that fits), abort any
// read in flight, make the new texture the loading one and queue an async read of the level's bytes with the bank's resource bank.
// Retail: Fable.exe (TLC, VC7.1) @ 0x009FDE80, 1265 bytes, thiscall `ret 8`; Ego_r.pdb / FableWin.pdb ?BeginLoading@CFrameData@CGraphicInfoFrame@@QAEXKK@Z.
// Leaf: ?BeginLoading@CFrameData@CGraphicInfoFrame@@QAEXKK@Z
// Statement structure EVIDENCED by the Anniversary 360 Test build's PPC code + line table (probe/anniv/bl.txt, lib_graphic_data_bank.cpp
//   3064..3205; third source) and by the retail bytes:
//   - `if (!success) return;` early return (PPC 3136 test, 3138 a line holding only the branch to the exit);
//   - c_mipmap loop: `read_size = size` copy (PPC 3182, `mr r6,r30`), body `skip = level_size; if (c < 1 && sizes[c] > 0) skip = sizes[c];
//     read_size -= skip; offset += skip; level_size >>= 2;` (PPC 3185..3192; retail sub ebp / add edi / shr edx order);
//   - allocation loop: inline `header.MipmapLevels - mipmap_level` (no named levels: retail computes it in ecx), for-increment
//     `++mipmap_level, width /= 2, height /= 2` (retail's increment block loads mipmap_level before the halving; an empty increment or a
//     while gives 1233 bytes). The PPC lines put width / height at 3127 / 3128 as body lines: the 360 file was edited, the x86 bytes win;
//     byte-identical alternatives: increment `width /= 2, height /= 2` + `++mipmap_level;` last in the body, or `height /= 2` +
//     `++mipmap_level; width /= 2;`.
//   - unsigned c_mipmap (retail jbe 0x009FE200 / jae 0x009FE215); the volume call's inline `header.MipmapLevels - mipmap_level`.
// Names: parameter get_flags and locals async_entry, managed_texture, pixel_format, texture2 (managed_texture.GetTexture() copy),
//   texture (LoadingManagedTexture.GetTexture() copy) from the Anniversary 360 Test PDB (third source; PPC frame offsets 0x70 / 0x68).
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
void CGraphicInfoFrame::CFrameData::BeginLoading(unsigned long mipmap_level, unsigned long get_flags)
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
    CPixelFormat pixel_format = bank->GetPixelFormat(GraphicInfoFrame->GetTextureType());
    ManagedTexture.Lock();
    LoadingManagedTexture.Lock();
    bool success = false;
    CManagedTexture managed_texture;
    long width = header.Width >> mipmap_level;
    long height = header.Height >> mipmap_level;
    long depth = header.Depth >> mipmap_level;
    if (GraphicInfoFrame->IsVolumeTexture()) {
        pixel_format = CPixelFormat(D3DFMT_A8R8G8B8);
        success = texture_manager.AllocateVolumeTexture(managed_texture, width, height, depth, header.MipmapLevels - mipmap_level, pixel_format, this);
    } else {
        for (; (long)mipmap_level <= max_mipmap_level; ++mipmap_level, width /= 2, height /= 2) {
            if (texture_manager.AllocateTexture(managed_texture, width, height, header.MipmapLevels - mipmap_level, pixel_format, this)) {
                success = true;
                break;
            }
        }
    }
    ManagedTexture.Unlock();
    LoadingManagedTexture.Unlock();
    if (!success)
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
    CCountedPointer<CBankFileAsyncEntry> async_entry = GetBank()->CreateEntry(GraphicInfoFrame->GetIndex());
    unsigned long size = GetBank()->GetEntryDataSize(GraphicInfoFrame->GetIndex());
    unsigned long offset = 0;
    if (header.FrameCount > 1 && !header.IsFlatAnimatedGraphic && GetBank()->GetEntryDataType(GraphicInfoFrame->GetIndex()) == 1) {
        size /= header.FrameCount;
        offset = GraphicInfoFrame->FrameNum * size;
    }
    unsigned long level_size = header.FrameDataSize;
    unsigned long read_size = size;
    for (unsigned long c_mipmap = 0; c_mipmap < mipmap_level; ++c_mipmap) {
        unsigned long skip_size = level_size;
        if (c_mipmap < 1 && header.CompressedMipmapSizes[c_mipmap] > 0)
            skip_size = header.CompressedMipmapSizes[c_mipmap];
        read_size -= skip_size;
        offset += skip_size;
        level_size >>= 2;
    }
    FileData = async_entry->BeginReadDataAutoAlloc(offset, read_size, TextureLoadingPriority);
    SetResourceSize(Sizeof());
    RemoveFromResourceList();
    GetBank()->AddToLoadingQueue(*this);
}
