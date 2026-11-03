// Self-contained C++03 behavior fixture. The selected target body is pasted below.
// Observable dependency implementations check loop filtering, timers, cache state and cleanup.
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <map>
#include <set>
#ifndef _MSC_VER
#define __fastcall
#endif
static void Check(bool x){if(!x)abort();}
static std::vector<int> Events;
static unsigned ClockCalls;
double __fastcall GFGetTime(){Events.push_back(10);return (double)ClockCalls++;}
template<class T> class CArray:public std::vector<T>{};
class CCompositeBlendDesc {};
class CEngineTextureMorph {public:unsigned long OriginalTextureIndex;std::vector<CCompositeBlendDesc> Blends;CEngineTextureMorph():OriginalTextureIndex(0){} };
class CWideString {};
class CBankFile {public:
 void OpenFileReadWrite(const CWideString&,unsigned long create,unsigned long page){Check(page==0x1000);Events.push_back(20+(int)create);}
 void BeginUpdateEntries(){Events.push_back(30);}
 void EndUpdateEntries(){Events.push_back(31);}
 void Close(){Events.push_back(32);}
};
class CGraphicInfo {public:bool Bump;CGraphicInfo():Bump(false){}bool IsBump()const{return Bump;}};
class CGraphicDataBank {public:
 unsigned long UnloadFrameDelay,ReduceSizeFrameDelay;CGraphicInfo Graphics[8];
 CGraphicDataBank():UnloadFrameDelay(7),ReduceSizeFrameDelay(9){for(unsigned i=0;i<8;++i)Graphics[i].Bump=(i%2)!=0;}
 unsigned long GetUnloadFrameDelay()const{return UnloadFrameDelay;}
 unsigned long GetReduceSizeFrameDelay()const{return ReduceSizeFrameDelay;}
 void SetUnloadFrameDelay(unsigned long x){UnloadFrameDelay=x;Events.push_back(100+(int)x);}
 void SetReduceSizeFrameDelay(unsigned long x){ReduceSizeFrameDelay=x;Events.push_back(200+(int)x);}
 const CGraphicInfo* PeekGraphic(unsigned long index)const{Check(index<8);return &Graphics[index];}
};
enum EGraphicBankNo { MAIN_GRAPHIC_BANK_NO=0 };
class CEngineResourceManager {public:CGraphicDataBank Bank;CGraphicDataBank* GetGraphicBank(EGraphicBankNo n){Check(n==MAIN_GRAPHIC_BANK_NO);return &Bank;}};
class CEngine {public:bool DeallocateShadowBuffer(){Events.push_back(40);return true;}void ReallocateShadowBuffer(){Events.push_back(41);}};
namespace NEngineInternal {CEngineResourceManager* GPResourceManager;CEngine* GPEngine;}
class CCompositeSurfaceCache;
class CEngineSurfaceCompositionManager {public:
 CBankFile IntermediateBankFile;CWideString IntermediateDiskCacheFileName;
 std::map<int,int> GeneratedMorphs;bool RequiresCopy;unsigned SuccessMask,Copied,Preloaded,Constructed,Destroyed;
 std::vector<unsigned long> Updated;
 CEngineSurfaceCompositionManager():RequiresCopy(false),SuccessMask(0),Copied(0),Preloaded(0),Constructed(0),Destroyed(0){}
 bool UpdateDiskCacheTexture(CCompositeSurfaceCache&,unsigned long,bool);
 void CopyIntermediateBank(){++Copied;Events.push_back(50);}
 void PreloadCompletedBank(){++Preloaded;Events.push_back(51);}
 void UpdateDiskCache(const CArray<CEngineTextureMorph>&,bool,float,const std::set<long>*);
};
class CCompositeSurfaceCache {public:
 CEngineSurfaceCompositionManager& Owner;CEngineTextureMorph CurrentMorph;
 explicit CCompositeSurfaceCache(CEngineSurfaceCompositionManager& x):Owner(x){++Owner.Constructed;Events.push_back(60);}
 ~CCompositeSurfaceCache(){++Owner.Destroyed;Events.push_back(61);}
 void SetCurrentMorph(const CEngineTextureMorph& x){CurrentMorph=x;}
 CEngineTextureMorph& GetCurrentMorph(){return CurrentMorph;}
 bool IsMultipleTextures()const{return CurrentMorph.Blends.size()>1;}
};
bool CEngineSurfaceCompositionManager::UpdateDiskCacheTexture(CCompositeSurfaceCache& cache,unsigned long mipmap,bool is_bump){
 Check(mipmap==0);unsigned long index=cache.GetCurrentMorph().OriginalTextureIndex;
 Check(is_bump==((index%2)!=0));Updated.push_back(index);Events.push_back(300+(int)index);
 Check(NEngineInternal::GPResourceManager->Bank.UnloadFrameDelay==1&&NEngineInternal::GPResourceManager->Bank.ReduceSizeFrameDelay==1);
 return (SuccessMask&(1u<<index))!=0;
}
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

int main(){unsigned cases=0;
 for(unsigned count=0;count<=6;++count)for(unsigned filter_mode=0;filter_mode<3;++filter_mode)for(unsigned timed=0;timed<2;++timed)for(unsigned budget=0;budget<3;++budget)for(unsigned success=0;success<2;++success)for(unsigned prior=0;prior<2;++prior){
  CEngineResourceManager resources;CEngine engine;NEngineInternal::GPResourceManager=&resources;NEngineInternal::GPEngine=&engine;
  CEngineSurfaceCompositionManager manager;manager.SuccessMask=success?0xaa:0;manager.RequiresCopy=prior!=0;
  if(prior)manager.GeneratedMorphs[1]=1;
  CArray<CEngineTextureMorph> morphs;std::set<long> filter;
  filter.insert(1);filter.insert(3);filter.insert(5);
  if(filter_mode==2)filter.clear();
  for(unsigned i=0;i<count;++i){CEngineTextureMorph m;m.OriginalTextureIndex=i;m.Blends.resize((i%3)+1);morphs.push_back(m);}
  Events.clear();ClockCalls=0;manager.UpdateDiskCache(morphs,timed!=0,(float)budget,filter_mode?&filter:0);
  std::vector<unsigned long> expected;unsigned visited=0,updated=0,successful=0;bool interrupted=false;
  for(unsigned i=0;i<count;++i){++visited;if((filter_mode==2)||(filter_mode==1&&(i%2)==0))continue;if(i==0||(i%3)==0)continue;expected.push_back(i);++updated;if(success&&(i%2))++successful;if(timed&&updated>budget){interrupted=true;break;}}
  Check(manager.Updated==expected&&manager.Constructed==visited&&manager.Destroyed==visited);
  Check(resources.Bank.UnloadFrameDelay==7&&resources.Bank.ReduceSizeFrameDelay==9);
  Check(ClockCalls==1+updated+(successful?1:0));
  bool needed=prior||successful;bool copied=needed&&!interrupted;
  Check(manager.Copied==(copied?1u:0u)&&manager.Preloaded==manager.Copied&&manager.RequiresCopy==(needed&&!copied));
  Check(Events.size()>=9&&Events[0]==10&&Events[1]==201&&Events[2]==101&&Events[3]==40&&Events[4]==(prior?20:21)&&Events[5]==30);
  unsigned cleanup=6;while(cleanup<Events.size()&&Events[cleanup]!=31)++cleanup;
  Check(cleanup+5<=Events.size()&&Events[cleanup+1]==32&&Events[cleanup+2]==41&&Events[cleanup+3]==209&&Events[cleanup+4]==107);
  ++cases;
 }
 Check(cases==504);puts("00b2d9f0_TEST PASS");return 0;
}
