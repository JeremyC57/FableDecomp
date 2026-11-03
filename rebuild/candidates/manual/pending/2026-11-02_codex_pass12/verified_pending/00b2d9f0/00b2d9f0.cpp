// CEngineSurfaceCompositionManager::UpdateDiskCache(const CArray<CEngineTextureMorph>&,bool,float,const std::set<long>*) -- generate pending composite texture entries within the optional time budget.
// Leaf: ?UpdateDiskCache@CEngineSurfaceCompositionManager@@QAEXABV?$CArray@VCEngineTextureMorph@@@@_NMPBV?$set@JU?$less@J@std@@V?$allocator@J@2@@std@@@Z
#define FABLE_RETAIL_VENDOR_VECTOR_ORDINARY
#define FABLE_RETAIL_IVREFCOUNT_PRIVATE
#define FABLE_RETAIL_CRESOURCE_PRIVATE_MEMBERS
#define FABLE_RETAIL_TEXTURE_CACHE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CGRAPHICFRAME
#define FABLE_STLPORT_CONSTRUCTORS
#include "engine/CEngineSurfaceCompositionManager.h"
#include "engine/CGraphicInfo.h"
#include "engine/NEngineInternal.h"
double __fastcall GFGetTime();
void CEngineSurfaceCompositionManager::UpdateDiskCache(const CArray<CEngineTextureMorph>& all_morphs,bool use_timeout,float timeout,const std::set<long>* limit_to_textures)
{
 double start_t=GFGetTime();
 long update_count=0;
 bool finish_later=false;
 CGraphicDataBank& graphic_bank=*NEngineInternal::GPResourceManager->GetGraphicBank(MAIN_GRAPHIC_BANK_NO);
 unsigned long old_texture_resize_delay=graphic_bank.GetReduceSizeFrameDelay();
 unsigned long old_texture_unload_delay=graphic_bank.GetUnloadFrameDelay();
 graphic_bank.SetReduceSizeFrameDelay(1);
 graphic_bank.SetUnloadFrameDelay(1);
 NEngineInternal::GPEngine->DeallocateShadowBuffer();
 // Fidelity: open_flags and filter_key materialize integer temporaries; these local names are reconstruction choices.
 unsigned long open_flags=0;
 if(GeneratedMorphs.empty())open_flags=1;
 IntermediateBankFile.OpenFileReadWrite(IntermediateDiskCacheFileName,open_flags,0x1000);
 IntermediateBankFile.BeginUpdateEntries();
 for(long c_morph=0;c_morph<all_morphs.size();++c_morph) {
  CCompositeSurfaceCache surface_cache(*this);
  surface_cache.SetCurrentMorph(all_morphs[c_morph]);
  unsigned long graphic_index=surface_cache.GetCurrentMorph().OriginalTextureIndex;
  const long filter_key=static_cast<long>(graphic_index);
  if(!limit_to_textures || limit_to_textures->find(filter_key)!=limit_to_textures->end()) {
  if(surface_cache.IsMultipleTextures()&&graphic_index) {
   const CGraphicInfo* graphic_info=NEngineInternal::GPResourceManager->GetGraphicBank(MAIN_GRAPHIC_BANK_NO)->PeekGraphic(graphic_index);
   if(UpdateDiskCacheTexture(surface_cache,0,graphic_info->IsBump())) {
    RequiresCopy=true;
    ++update_count;
   }
   double elapsed=GFGetTime()-start_t;
   if(use_timeout&&elapsed>timeout) {finish_later=true;break;}
  }
  }
 }
 IntermediateBankFile.EndUpdateEntries();
 IntermediateBankFile.Close();
 NEngineInternal::GPEngine->ReallocateShadowBuffer();
 graphic_bank.SetReduceSizeFrameDelay(old_texture_resize_delay);
 graphic_bank.SetUnloadFrameDelay(old_texture_unload_delay);
 if(update_count) {double delta_t=GFGetTime()-start_t;}
 if(RequiresCopy&&!finish_later) {
  CopyIntermediateBank();
  RequiresCopy=false;
  PreloadCompletedBank();
 }
}
