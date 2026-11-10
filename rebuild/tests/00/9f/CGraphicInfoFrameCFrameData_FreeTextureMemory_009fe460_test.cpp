// Exact selected target and helper bodies copied; observable collaborators exercise resource lifecycle branches.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define CHECK(c) do{if(!(c)){printf("FAIL %d case %d\n",__LINE__,cases);abort();}}while(0)
static int cases,unusedReads,setCalls,reduceCalls,resetCalls,releaseCalls,frameSizeReads,loadingSizeReads;
static char events[32];static int eventCount;
static void Event(char ch){events[eventCount++]=ch;events[eventCount]=0;}
class CResource {public:unsigned long Unused,ResourceSize;unsigned long GetUnusedFrames()const{++unusedReads;return Unused;}void SetResourceSize(unsigned long n){++setCalls;Event('S');ResourceSize=n;}};
class CGraphicFrameData {public:unsigned long Bytes;unsigned long Sizeof()const{++frameSizeReads;return Bytes;}};
class CGraphicFrame {public:CGraphicFrameData* FrameData;bool IsInitialised()const{return FrameData!=0;}unsigned long Sizeof()const;};
template<class T>inline bool GFPIsNull(T* p){return p==0;}
template<class T>inline unsigned long GFSizeof(T& t){return t.Sizeof();}
template<class T>inline unsigned long GFPSizeof(const T& p){return !GFPIsNull(p)?GFSizeof(*p)+sizeof(p):sizeof(p);}
class CManagedTexture {public:CResource* Data;unsigned long Bytes;unsigned long GetUnusedFrames(){return Data->GetUnusedFrames();}bool IsValid()const{return Data!=0;}unsigned long Sizeof()const{++loadingSizeReads;return Bytes;}};
class CTextureManager {public:bool Result;CGraphicFrame* ExpectedFrame;CManagedTexture* ExpectedTexture;bool ReduceMipmapLevel(CGraphicFrame& f,CManagedTexture& t,unsigned long n){CHECK(&f==ExpectedFrame&&&t==ExpectedTexture&&n==1);++reduceCalls;Event('D');return Result;}};
class CGraphicDataBank {public:unsigned long ReduceSizeFrameDelay,TexturePointersValidForFrames;CTextureManager* TextureManager;unsigned long GetReduceSizeFrameDelay()const{return ReduceSizeFrameDelay;}unsigned long GetTexturePointersValidForFrames()const{return TexturePointersValidForFrames;}};
class CGraphicInfo {public:struct CGraphicHeader{bool IsFlatAnimatedGraphic;}Header;CGraphicDataBank* Bank;};
class CGraphicInfoFrame {public:class CFrameData;CGraphicInfo* GraphicInfo;signed char SmallMipmapLevel;CGraphicDataBank* GetBank()const;bool IsFlatSequence()const;void ReduceMipmapLevel(CGraphicFrame&,CManagedTexture&,unsigned long)const;void ReleaseData()const{++releaseCalls;Event('L');}};
class CGraphicInfoFrame::CFrameData:public CResource {public:
 CGraphicFrame Frame;CManagedTexture ManagedTexture,LoadingManagedTexture;signed char CurrentMipmapLevel;const CGraphicInfoFrame* GraphicInfoFrame;
 bool IsLoaded(){return Frame.IsInitialised();}CGraphicDataBank* GetBank()const{return GraphicInfoFrame->GetBank();}
 unsigned long Sizeof()const{unsigned long size=sizeof(*this);size+=Frame.Sizeof();if(LoadingManagedTexture.IsValid())size+=LoadingManagedTexture.Sizeof();return size;}
 void ResetTexture(){++resetCalls;Event('R');Frame.FrameData=0;}
 bool FreeTextureMemory(bool allow_unload);
};
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

int main(){
 CGraphicInfoFrame outer;CGraphicInfo info;CGraphicDataBank bank;CTextureManager manager;CResource resource;
 CGraphicFrameData data;CGraphicInfoFrame::CFrameData frame;
 frame.GraphicInfoFrame=&outer;outer.GraphicInfo=&info;info.Bank=&bank;bank.TextureManager=&manager;
 manager.ExpectedFrame=&frame.Frame;manager.ExpectedTexture=&frame.ManagedTexture;data.Bytes=128;
 unsigned long unused[]={4,5,6};signed char current[]={-1,0,2,3};signed char maximum[]={0,3};
 for(int loaded=0;loaded<2;++loaded)for(int tex=0;tex<3;++tex)for(int age=0;age<3;++age)for(int c=0;c<4;++c)for(int m=0;m<2;++m)for(int flat=0;flat<2;++flat)for(int allow=0;allow<2;++allow)for(int loading=0;loading<2;++loading)for(int succeeds=0;succeeds<2;++succeeds){
  ++cases;unusedReads=setCalls=reduceCalls=resetCalls=releaseCalls=frameSizeReads=loadingSizeReads=eventCount=0;events[0]=0;
  frame.Frame.FrameData=loaded?&data:0;frame.ManagedTexture.Data=&resource;resource.Unused=unused[tex];frame.Unused=unused[age];frame.ResourceSize=999;
  bank.ReduceSizeFrameDelay=bank.TexturePointersValidForFrames=5;
  frame.CurrentMipmapLevel=current[c];outer.SmallMipmapLevel=maximum[m];info.Header.IsFlatAnimatedGraphic=flat!=0;
  frame.LoadingManagedTexture.Data=loading?&resource:0;frame.LoadingManagedTexture.Bytes=64;manager.Result=succeeds!=0;
  int early=loaded&&tex>=1&&age<=1;
  int eligible=loaded&&tex>=1&&age==2;
  int reduce=eligible&&!flat&&(int(current[c])+1<=int(maximum[m]));
  int unload=eligible&&!reduce&&allow;
  int update=!early&&!unload;
  bool result=frame.FreeTextureMemory(allow!=0);
  CHECK(result==(reduce||unload));CHECK(reduceCalls==reduce&&resetCalls==unload&&releaseCalls==unload&&setCalls==update);
  CHECK(unusedReads==(loaded?(tex>=1?2:1):0));
  CHECK(frame.CurrentMipmapLevel==(reduce?current[c]+1:current[c]));CHECK(frame.Frame.FrameData==((loaded&&!unload)?&data:0));
  CHECK(frame.ResourceSize==(update?sizeof(frame)+sizeof(CGraphicFrameData*)+(loaded?128:0)+(loading?64:0):999));
  CHECK(frameSizeReads==(update&&loaded)&&loadingSizeReads==(update&&loading));
  CHECK(strcmp(events,reduce?"DS":unload?"RL":update?"S":"")==0);
  CHECK(resource.Unused==unused[tex]&&frame.Unused==unused[age]&&info.Bank==&bank);
 }
 CHECK(cases==2304);puts("009fe460_TEST PASS");return 0;
}