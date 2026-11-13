#ifndef FABLE_ENGINE_CGRAPHICINFO_H
#define FABLE_ENGINE_CGRAPHICINFO_H
#ifdef FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS
#include "engine/CGraphicDataBank.h"
#include "engine/CGraphicInfoFrame.h"
#ifdef FABLE_RETAIL_CGRAPHICINFO_LOADFRAME
#include "engine/CCountedPointer.h"
class CManagedTexture;class CTexture;
#endif
class CGraphicInfo {
public:
    const CGraphicInfoFrame* PeekInfoFrame(unsigned long index) const {return Frames+index;}
#ifdef FABLE_RETAIL_CGRAPHICINFO_TEXTURETYPE
    // Retail opt-in (wf8, row 0x00BA4A40): FableWin.pdb ?GetTextureType@CGraphicInfo@@QBE?AW4ETextureType@CGraphicDataBank@@XZ
    // (inline in retail: +0x1C read in place, 0x00BA4B1C). No layout change.
    CGraphicDataBank::ETextureType GetTextureType() const {return TextureType;}
#endif
#ifdef FABLE_RETAIL_CGRAPHICINFO_GETVOLUMETEXTURE
    // Retail opt-in (chunk 2008, row 0x00B54890): Ego_r.pdb 0x00B5D250 ?GetVolumeTexture@CGraphicInfo@@QBE_NPAPAVCVolumeTexture@@K@Z, out of line
    // in retail at 0x009FFB20 (`mov ecx,[ecx+28h] ; jmp 0x009FF710`, the Frames forwarder to CGraphicInfoFrame::GetVolumeTexture). Declaration only.
    bool GetVolumeTexture(class CVolumeTexture** texture, unsigned long level) const;   // 0x009FFB20
#endif
#ifdef FABLE_RETAIL_CGRAPHICINFO_GETFRAME
    // Retail opt-in (chunk 2045, row 0x00BB8EF0): Ego_r.pdb / FableWin.pdb ?GetFrame@CGraphicInfo@@QBE_NAAVCGraphicFrame@@KKK@Z, out of line in
    // retail at 0x009FF8D0 (the frame index, mipmap level and flags; `ret 10h`). Declaration only.
    bool GetFrame(CGraphicFrame& frame, unsigned long frame_index, unsigned long mipmap_level, unsigned long flags) const;   // 0x009FF8D0
#endif
#ifdef FABLE_RETAIL_CGRAPHICINFO_FRAMEFACTORS
    // Opt-in (row 0x00BBF4A0): FableWin.pdb ?GetFrameWidthFactor@CGraphicInfo@@QBEMXZ / ?GetFrameHeightFactor@CGraphicInfo@@QBEMXZ, inline in
    // retail (0x00BBFF58..0x00BBFF8D: fild FrameWidth (+6) / fidiv Width (+0), fild FrameHeight (+8) / fidiv Height (+2)). Needs the typed
    // Header (FABLE_RETAIL_CGRAPHICINFO_SPRITE).
    float GetFrameWidthFactor() const { return (float)Header.FrameWidth / Header.Width; }
    float GetFrameHeightFactor() const { return (float)Header.FrameHeight / Header.Height; }
#endif
#ifdef FABLE_RETAIL_CGRAPHICINFO_CONSTRUCT
    CGraphicInfo(unsigned long index,CGraphicDataBank* bank,const CGraphicDataBank::CGraphicHeader& header);
    bool IsBump() const {return Bank->GetEntryDataType(Index)==2;}
private:
#endif
#ifdef FABLE_RETAIL_CFRAMEDATA_UPLOADFRAME
    // Retail opt-in (wf12, row 0x009FF220): FableWin.pdb ?IsBump@CGraphicInfo@@QBE_NXZ and ?IsVolumeTexture@CGraphicInfo@@QBE_NXZ,
    // inline in retail (GetEntryDataType 0x009CC210 ==2 / ==4). No layout change.
    // (IsBump is also declared by FABLE_RETAIL_CGRAPHICINFO_CONSTRUCT above, which ends in `private:`; both opt-ins may be combined.)
public:
#ifndef FABLE_RETAIL_CGRAPHICINFO_CONSTRUCT
    bool IsBump() const {return Bank->GetEntryDataType(Index)==2;}
#endif
    bool IsVolumeTexture() const {return Bank->GetEntryDataType(Index)==4;}
#ifdef FABLE_RETAIL_CGRAPHICINFO_CONSTRUCT
private:
#endif
#endif
#if defined(FABLE_RETAIL_CGRAPHICINFOFRAME_FREE) || defined(FABLE_RETAIL_CGRAPHICINFO_SPRITE)
private:
    friend class CGraphicInfoFrame;
#endif
    CGraphicDataBank* GetBank() const {return Bank;}
#ifdef FABLE_RETAIL_CGRAPHICINFO_LOADFRAME
    // Retail opt-in (chunk 2004, row 0x009FED30 CGraphicDataBank::LoadGraphicFrame): Ego_r.pdb
    // ?InitialiseFramesAsSurfaceSections@CGraphicInfo@@AAEXAAVCManagedTexture@@CAAV?$CCountedPointer@VCTexture@@@@_N2@Z (private; the
    // `signed char` second parameter is the PDB's; out of line in retail at 0x009FE780, called by the bank with ecx = the graphic info,
    // 0x009FF190) -- the bank reads the info's header in place (Width +0x00 .. FrameDataSize +0x14, CompressedMipmapSizes +0x18), so
    // it is a friend (the original access grant is not in the PDB name sets). Declaration only, no layout change.
    friend class CGraphicDataBank;
    void InitialiseFramesAsSurfaceSections(CManagedTexture& managed_texture,signed char mipmap_level,CCountedPointer<CTexture>& texture,
                                           bool alpha,bool boolean_alpha);   // 0x009FE780
#endif
private:
#if defined(FABLE_RETAIL_CGRAPHICINFO_CONSTRUCT) || defined(FABLE_RETAIL_CGRAPHICINFOFRAME_FREE) || defined(FABLE_RETAIL_CGRAPHICINFO_SPRITE)
    CGraphicDataBank::CGraphicHeader Header;
#else
    unsigned char Header[28]; // complete CGraphicDataBank::CGraphicHeader storage
#endif
    CGraphicDataBank::ETextureType TextureType;
    CGraphicDataBank* Bank;
    long Index;
    CGraphicInfoFrame* Frames;
};
FABLE_STATIC_ASSERT(sizeof(CGraphicInfo)==44);
#ifdef FABLE_RETAIL_CGRAPHICINFO_TEXTURETYPE
// FableWin.pdb ?GetTextureType@CGraphicInfoFrame@@QBE?AW4ETextureType@CGraphicDataBank@@XZ (inline: GraphicInfo +0xC, then +0x1C).
inline CGraphicDataBank::ETextureType CGraphicInfoFrame::GetTextureType() const {return GraphicInfo->GetTextureType();}
#endif
#ifdef FABLE_RETAIL_CGRAPHICINFOFRAME_GETFRAME
// Retail opt-in (chunk 2031, rows 0x00B54E30 CEngineWeatherRenderer::RenderSnow / 0x00B65A20 CEngineSkyRenderer::RenderOuterSky): the inline
// frame fetch both rows expand (0x00B54ECE..0x00B54FF0 with mipmap level 0 and flags 0). Bodies modelled on the landed out-of-line sibling
// CGraphicInfoFrame::GetTexture 0x00B66C10 (same tests in the same order) with the frame shared instead of its texture:
// - FableWin.pdb ?TouchTexture@CManagedTexture@@QAEXXZ: the managed texture's resource touched (CResource::TouchResource);
// - the touch block (FableWin.pdb ?TouchFrame@CGraphicInfoFrame@@QBEXK@Z's body, written in place as the landed 0x0041C260 has it): once per
//   bank frame (or for a finer level) the touched frame / level updated, the managed texture and (when the level is loaded) the frame data touched;
// - FableWin.pdb ?GetFrame@CGraphicInfoFrame@@QBE_NAAVCGraphicFrame@@KK@Z: a loaded frame is touched (unless flags & 8), a finer level is
//   requested (BeginLoading 0x009FDE80), the frame shared; an unloaded one starts loading (a CFrameData 0x009FE380 is created first unless
//   flags & 4) and is fetched only by a forced load (flags & 1, InternalGetFrameForcedLoading 0x009FF6B0).
// Needs FABLE_RETAIL_CRESOURCE_TOUCHRESOURCE, FABLE_RETAIL_CRESOURCELIST, FABLE_RETAIL_CGRAPHICFRAME + _GETTEXTURE (CGraphicFrame::operator=).
// TouchTexture FABLE_FORCEINLINE (owner 2026-11-01 rule): retail's only out-of-line copy 0x0041BB70 (`mov esi,[ecx+4] ; test esi,esi`, then the
// TouchResource body) has no caller; plain `inline` leaves it called in GetFrame (c1 56, score 133 at f 0.094; threshold 74).
FABLE_FORCEINLINE void CManagedTexture::TouchTexture()
{
    if (!Data.IsNull())
        Data->TouchResource();
}
// GetFrame: the control flow of the landed out-of-line copy 0x0041C260 (uncalled COMDAT) and of the landed CGraphicInfo::GetFrame 0x009FF8D0
// that expands it.
FABLE_FORCEINLINE bool CGraphicInfoFrame::GetFrame(CGraphicFrame& frame, unsigned long mipmap_level, unsigned long flags) const
{
    if (!Data.IsNull()) {
        if (Data->Frame.IsInitialised()) {
            if (!(flags & 8)) {
                if (Data->TouchedFrame != GraphicInfo->GetBank()->GetCurrentFrame() || mipmap_level <= (unsigned long)Data->TouchedMipmapLevel) {
                    Data->TouchedFrame = GraphicInfo->GetBank()->GetCurrentFrame();
                    Data->ManagedTexture.TouchTexture();
                    if (mipmap_level <= (unsigned long)Data->CurrentMipmapLevel)
                        Data->TouchResource();
                    if ((unsigned long)Data->TouchedMipmapLevel > mipmap_level)
                        Data->TouchedMipmapLevel = (signed char)mipmap_level;
                }
            }
            if (mipmap_level < (unsigned long)Data->CurrentMipmapLevel) {
                Data->BeginLoading(mipmap_level, flags);
                if (!(flags & 16) || !Data->FileData.Get()) {
                    frame = Data->Frame;
                    return true;
                }
            } else {
                frame = Data->Frame;
                return true;
            }
        }
        Data->BeginLoading(mipmap_level, flags);
    } else {
        if (flags & 4)
            return false;
        Data.Reset(new CFrameData(this));
        Data->BeginLoading(mipmap_level, flags);
    }
    if (!(flags & 1) || !InternalGetFrameForcedLoading(flags))
        return false;
    frame = Data->Frame;
    return true;
#endif
#ifdef FABLE_RETAIL_CGRAPHICDATABANK_PEEKGRAPHIC
// Retail opt-in (chunk 2045, row 0x00BB8EF0): FableWin.pdb ?PeekGraphic@CGraphicDataBank@@QBEPBVCGraphicInfo@@K@Z (donor 0x01D2D7A0), inline in
// retail: 0x00BB8F49..0x00BB8F81 reads GraphicList (+0x1E0 begin / +0x1E4 end), divides by sizeof(CGraphicInfo) 0x2C and returns the element
// address for an index below the count, else 0. Defined here because the element type must be complete.
inline const CGraphicInfo* CGraphicDataBank::PeekGraphic(unsigned long index) const
{
    return index < GraphicList.size() ? &GraphicList[index] : 0;
}
#endif
#endif
#endif
