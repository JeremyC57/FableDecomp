// CGraphicInfoFrame::CFrameData::UploadFrame(bool force) -- finish an async texture read: a volume texture goes through
// CGraphicDataBank::LoadVolumeGraphic, anything else through LoadGraphicFrame into a local frame; on success the loading texture
// (and, unless the graphic is a flat animated sequence, the frame) becomes current with its mip level. Then the file data is
// dropped, the resource size is recomputed and the frame data is re-registered with the bank's CResourceBank.
// Retail: Fable.exe @ 0x009FF220, 540 bytes, __fastcall `ret 4`; Ego_r.pdb / FableWin.pdb ?UploadFrame@CFrameData@CGraphicInfoFrame@@QAE_N_N@Z.
// Callees: CBankFile::GetEntryDataType 0x009CC210 (IsVolumeTexture ==4, IsBump ==2), CGraphicDataBank::LoadVolumeGraphic 0x009FD9B0,
//          CGraphicDataBank::LoadGraphicFrame 0x009FED30, CManagedTexture::operator= 0x00A69B20, CManagedTexture::Release 0x00A69AD0,
//          CGraphicFrameData::Uninitialise 0x009FCBA0, CGraphicFrameData::Sizeof 0x009FCBF0, CManagedTexture::Sizeof 0x00A69E60,
//          CResource::SetResourceSize 0x009FBF20, CResourceBank::AddResource 0x009FC380, operator delete 0x00BFE9BC.
// Shape: the FableWin.pdb inline helpers IsVolumeTexture / UploadToVolumeTexture / UploadToTexture (Ego_r out of line 0x00B580C0 /
// 0x00B5B6A0, just before UploadFrame) expand here; the header argument is passed as GraphicInfo->Header directly (read through the member
// again after the IsBump call, as retail reloads it). CGraphicFrame's operator= / destructor are the header's FABLE_FORCEINLINE opt-in.
// Leaf: ?UploadFrame@CFrameData@CGraphicInfoFrame@@QAE_N_N@Z
// Flags: none
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
#define FABLE_RETAIL_CFRAMEDATA_UPLOADFRAME
#include "engine/CGraphicInfo.h"
#include "engine/GFSizeof.h"
// Original shared helper chain, FableWin01d2df40/03127100/02d96750 (?GetBank@CGraphicInfoFrame@@ABEPAVCGraphicDataBank@@XZ,
// ?IsFlatSequence@CGraphicInfoFrame@@QBE_NXZ, ?Sizeof@CGraphicFrame@@QBEKXZ): bodies copied from landed row 0x009FE460 FreeTextureMemory.
// FableWin03127060/031273d0/03127370 = ?IsVolumeTexture / ?UploadToVolumeTexture / ?UploadToTexture@CGraphicInfoFrame (inline).
inline CGraphicDataBank* CGraphicInfoFrame::GetBank() const {return GraphicInfo->Bank;}
inline bool CGraphicInfoFrame::IsFlatSequence() const {return GraphicInfo->Header.IsFlatAnimatedGraphic;}
inline unsigned long CGraphicFrame::Sizeof() const {return GFPSizeof(FrameData);}
inline bool CGraphicInfoFrame::IsVolumeTexture() const {return GraphicInfo->IsVolumeTexture();}
inline bool CGraphicInfoFrame::UploadToVolumeTexture(CManagedTexture& texture,CBankFileAsyncData& data,unsigned long level) const {
 return GetBank()->LoadVolumeGraphic(texture,data,GraphicInfo->Header,true,level,GraphicInfo->IsBump());
}
inline bool CGraphicInfoFrame::UploadToTexture(CGraphicFrame& frame,CManagedTexture& texture,CBankFileAsyncData& data,unsigned long level,bool force) const {
 return GetBank()->LoadGraphicFrame(frame,texture,data,GraphicInfo,true,level,GetTextureType(),force);
}
bool CGraphicInfoFrame::CFrameData::UploadFrame(bool force)
{
 bool result=false;
 if(GraphicInfoFrame->IsVolumeTexture()) {
  if(GraphicInfoFrame->UploadToVolumeTexture(LoadingManagedTexture,*FileData,LoadingMipmapLevel)) {
   ManagedTexture=LoadingManagedTexture;
   LoadingManagedTexture.Release();
   CurrentMipmapLevel=LoadingMipmapLevel;
   result=true;
  }
 } else {
  CGraphicFrame frame;
  if(GraphicInfoFrame->UploadToTexture(frame,LoadingManagedTexture,*FileData,LoadingMipmapLevel,force)) {
   if(!GraphicInfoFrame->IsFlatSequence()) {
    Frame=frame;
    ManagedTexture=LoadingManagedTexture;
    LoadingManagedTexture.Release();
    CurrentMipmapLevel=LoadingMipmapLevel;
   }
   result=true;
  }
 }
 FileData.ReleaseData();
 SetResourceSize(Sizeof());
 GetBank()->AddResource(*this);
 return result;
}