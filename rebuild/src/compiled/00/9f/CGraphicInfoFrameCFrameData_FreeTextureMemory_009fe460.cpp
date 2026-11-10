// 009fe460 CGraphicInfoFrame::CFrameData::FreeTextureMemory
// Leaf: ?FreeTextureMemory@CFrameData@CGraphicInfoFrame@@QAE_N_N@Z
// Flags: none
#define FABLE_RETAIL_CGRAPHICINFOFRAME_FREE
#define FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS
#define FABLE_RETAIL_CCOUNTEDPOINTER
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CARRAY
#include "engine/CGraphicInfo.h"
#include "engine/CTextureManager.h"
#include "engine/GFSizeof.h"
// Original shared helper chain, FableWin01d2df40/03127100/03128920/02d96750.
inline CGraphicDataBank* CGraphicInfoFrame::GetBank() const {return GraphicInfo->Bank;}
inline bool CGraphicInfoFrame::IsFlatSequence() const {return GraphicInfo->Header.IsFlatAnimatedGraphic;}
inline void CGraphicInfoFrame::ReduceMipmapLevel(CGraphicFrame& frame,CManagedTexture& texture,unsigned long levels) const {
 GetBank()->TextureManager->ReduceMipmapLevel(frame,texture,levels);
}
inline unsigned long CGraphicFrame::Sizeof() const {return GFPSizeof(FrameData);}
bool CGraphicInfoFrame::CFrameData::FreeTextureMemory(bool allow_unload)
{
 if(IsLoaded()) {
  if(ManagedTexture.GetUnusedFrames()>=GetBank()->GetTexturePointersValidForFrames()) {
   unsigned long unused_frames=GetUnusedFrames();
   if(unused_frames<=GetBank()->GetReduceSizeFrameDelay())return false;
   unsigned long skip_levels=1;
   unsigned long new_level=CurrentMipmapLevel+skip_levels;
   long max_level=GraphicInfoFrame->SmallMipmapLevel;
   if(!GraphicInfoFrame->IsFlatSequence() && new_level<=max_level) {
    GraphicInfoFrame->ReduceMipmapLevel(Frame,ManagedTexture,skip_levels);
    CurrentMipmapLevel=(signed char)new_level;
    SetResourceSize(Sizeof());
    return true;
   } else if(allow_unload) {
    ResetTexture();
    GraphicInfoFrame->ReleaseData();
    return true;
   }
  }
 }
 SetResourceSize(Sizeof());
 return false;
}