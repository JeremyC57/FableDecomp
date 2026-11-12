#ifndef FABLE_ENGINE_CGRAPHICINFO_H
#define FABLE_ENGINE_CGRAPHICINFO_H
#ifdef FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS
#include "engine/CGraphicDataBank.h"
#include "engine/CGraphicInfoFrame.h"
class CGraphicInfo {
public:
    const CGraphicInfoFrame* PeekInfoFrame(unsigned long index) const {return Frames+index;}
#ifdef FABLE_RETAIL_CGRAPHICINFO_GETFRAME3
    // Opt-in (chunk 2048, RenderSunFlare 0x00B64550): Ego_r.pdb / FableWin.pdb ?GetFrame@CGraphicInfo@@QBE_NAAVCGraphicFrame@@KKK@Z 0x009FF8D0.
    bool GetFrame(CGraphicFrame& frame, unsigned long frame_index, unsigned long mipmap_level, unsigned long flags) const;
#endif
#ifdef FABLE_RETAIL_CGRAPHICINFO_GETVOLUMETEXTURE
    // Retail opt-in (chunk 2008, row 0x00B54890): Ego_r.pdb 0x00B5D250 ?GetVolumeTexture@CGraphicInfo@@QBE_NPAPAVCVolumeTexture@@K@Z, out of line
    // in retail at 0x009FFB20 (`mov ecx,[ecx+28h] ; jmp 0x009FF710`, the Frames forwarder to CGraphicInfoFrame::GetVolumeTexture). Declaration only.
    bool GetVolumeTexture(class CVolumeTexture** texture, unsigned long level) const;   // 0x009FFB20
#endif
#ifdef FABLE_RETAIL_CGRAPHICINFO_CONSTRUCT
    CGraphicInfo(unsigned long index,CGraphicDataBank* bank,const CGraphicDataBank::CGraphicHeader& header);
    bool IsBump() const {return Bank->GetEntryDataType(Index)==2;}
private:
#endif
#ifdef FABLE_RETAIL_CGRAPHICINFOFRAME_FREE
private:
    friend class CGraphicInfoFrame;
#elif defined(FABLE_RETAIL_CGRAPHICINFOFRAME_GETBANK_DIRECT)
    friend class CGraphicInfoFrame;
#endif
    CGraphicDataBank* GetBank() const {return Bank;}
private:
#if defined(FABLE_RETAIL_CGRAPHICINFO_CONSTRUCT) || defined(FABLE_RETAIL_CGRAPHICINFOFRAME_FREE)
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
#ifdef FABLE_RETAIL_CMANAGEDTEXTURE_TOUCHTEXTURE_GET
// Nested opt-in (chunk 2048): FableWin.pdb ?TouchTexture@CManagedTexture@@QAEXXZ is 17 instructions / 44 bytes = the pointer test
// through CIVCountedPointer::Get (needs FABLE_RETAIL_CIVCOUNTEDPOINTER_GET) (FableWin.pdb debug size reproduced by plain VC7.1 /Od of this text: /opt/fable_lane/tools/fwsize.py); `!Data.IsNull()` is 18 / 47.
FABLE_FORCEINLINE void CManagedTexture::TouchTexture()
{
    if (Data.Get())
        Data->TouchResource();
}
#else
FABLE_FORCEINLINE void CManagedTexture::TouchTexture()
{
    if (!Data.IsNull())
        Data->TouchResource();
}
#endif
#ifdef FABLE_RETAIL_CGRAPHICINFOFRAME_BEGINLOADING
inline bool CGraphicInfoFrame::IsFrameDataLoaded() const
{
    if (Data.IsNull())
        return false;
    return Data->IsLoaded();
}
#endif
#ifdef FABLE_RETAIL_CGRAPHICINFOFRAME_TOUCHFRAME
// Opt-in (chunk 2048): FableWin.pdb lists ?TouchFrame@CGraphicInfoFrame@@QBEXK@Z (69 debug instructions = the touch block) and
// ?GetBank@CGraphicInfoFrame@@ABEPAVCGraphicDataBank@@XZ (10 = a forwarding read) in GetFrame's COMDAT run: GetFrame calls them.
#ifdef FABLE_RETAIL_CGRAPHICINFOFRAME_GETBANK_DIRECT
// Nested opt-in (chunk 2048): FableWin.pdb ?GetBank@CGraphicInfoFrame@@ABEPAVCGraphicDataBank@@XZ is 10 instructions / 20 bytes = the
// GraphicInfo's Bank read directly (FableWin.pdb debug size reproduced by plain VC7.1 /Od of this text: /opt/fable_lane/tools/fwsize.py); through CGraphicInfo::GetBank (no FableWin copy) it is 10 / 22.
inline CGraphicDataBank* CGraphicInfoFrame::GetBank() const { return GraphicInfo->Bank; }
#else
inline CGraphicDataBank* CGraphicInfoFrame::GetBank() const { return GraphicInfo->GetBank(); }
#endif
FABLE_FORCEINLINE void CGraphicInfoFrame::TouchFrame(unsigned long mipmap_level) const
{
    if (Data->TouchedFrame != GetBank()->GetCurrentFrame() || mipmap_level <= (unsigned long)Data->TouchedMipmapLevel) {
        Data->TouchedFrame = GetBank()->GetCurrentFrame();
        Data->ManagedTexture.TouchTexture();
        if (mipmap_level <= (unsigned long)Data->CurrentMipmapLevel)
            Data->TouchResource();
        if ((unsigned long)Data->TouchedMipmapLevel > mipmap_level)
            Data->TouchedMipmapLevel = (signed char)mipmap_level;
    }
}
#endif
// GetFrame: the control flow of the landed out-of-line copy 0x0041C260 (uncalled COMDAT) and of the landed CGraphicInfo::GetFrame 0x009FF8D0
// that expands it.
FABLE_FORCEINLINE bool CGraphicInfoFrame::GetFrame(CGraphicFrame& frame, unsigned long mipmap_level, unsigned long flags) const
{
    if (!Data.IsNull()) {
        if (Data->Frame.IsInitialised()) {
#ifdef FABLE_RETAIL_CGRAPHICINFOFRAME_TOUCHFRAME
            if (!(flags & 8))
                TouchFrame(mipmap_level);
#else
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
#endif
            if (mipmap_level < (unsigned long)Data->CurrentMipmapLevel) {
                Data->BeginLoading(mipmap_level, flags);
#ifdef FABLE_RETAIL_CFRAMEDATA_ISLOADING
                if (!(flags & 16) || !Data->IsLoading()) {
#else
                if (!(flags & 16) || !Data->FileData.Get()) {
#endif
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
}
#endif
#endif
#endif
