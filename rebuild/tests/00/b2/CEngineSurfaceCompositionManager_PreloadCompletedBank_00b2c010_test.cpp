#include <stdio.h>
#include <stdlib.h>
#include <string.h>
static unsigned trial;static unsigned long seed=0x1234567;
static unsigned long rnd(){seed=seed*1664525UL+1013904223UL;return seed^(seed>>15);}
#define CHECK(x) do{if(!(x)){printf("case %u line %d\n",trial,__LINE__);exit(1);}}while(0)
template<class T>struct CCountedPointer {T* data;T& operator*() const{return *data;}};
namespace std {
 template<class T>struct list {typedef T* iterator; T elems[16];int count; T*begin(){return elems;}T*end(){return elems+count;}};
 template<class T>struct set {typedef T* iterator;T elems[16];int count;set():count(0){}
 void insert(T value){int i=0;while(i<count&&elems[i]<value)++i;if(i<count&&elems[i]==value)return;CHECK(count<16);for(int k=count;k>i;--k)elems[k]=elems[k-1];elems[i]=value;++count;}
 T*begin(){return elems;}T*end(){return elems+count;}};
}
struct CEngineTextureMorph{unsigned long index;};
struct CCompositeSurfaceCache{unsigned long LastUsedFrame;CEngineTextureMorph morph;CEngineTextureMorph&GetCurrentMorph(){return morph;}};
static CCompositeSurfaceCache surfaces[16];
static int liveFrames,created,clockCalls,bankTicks,processCalls,iteration;static double times[24],lastStart,maxTime;
static double GFGetTime(){CHECK(clockCalls<24);return times[clockCalls++];}
struct CGraphicFrame{CGraphicFrame(){++liveFrames;++created;}~CGraphicFrame(){--liveFrames;}};
struct CGraphicInfoFrame{unsigned index,readyAfter,queries;bool GetFrame(CGraphicFrame&,unsigned long level,unsigned long flags)const{
 CHECK(liveFrames==1&&!level&&!flags);return (unsigned)iteration>=readyAfter;}
};static CGraphicInfoFrame frames[12];static unsigned order[180],num;
struct CGraphicDataBank{const CGraphicInfoFrame* PeekGraphicInfoFrame(unsigned long index,unsigned long number){
 CHECK(index&&index<12&&!number&&num<180);order[num++]=index;++frames[index].queries;return &frames[index];}
 void FrameEnd(){CHECK(!liveFrames&&processCalls==bankTicks);++bankTicks;}
 void ProcessLoadingQueue(bool b,double t){CHECK(b&&bankTicks==processCalls+1);CHECK(t==lastStart+maxTime);++iteration;++processCalls;}
};
struct CEngineSurfaceCompositionManager{std::list<CCountedPointer<CCompositeSurfaceCache> >CachedSurfaces;
 CGraphicDataBank CompletedBankFile;unsigned long CurrentFrame;float MaxPreloadTime;int lookups;
 unsigned long FindIndexInCompletedBank(const CEngineTextureMorph& m){++lookups;return m.index;}
 void PreloadCompletedBank();};
void CEngineSurfaceCompositionManager::PreloadCompletedBank()
{
    long most_recent = 0x7fffffff;
    std::set<unsigned long> preload_indices;
    if (false) {} else for (std::list<CCountedPointer<CCompositeSurfaceCache> >::iterator itr = CachedSurfaces.begin(); itr != CachedSurfaces.end(); ++itr) {
        CCompositeSurfaceCache& surface = **itr;
        unsigned long index = FindIndexInCompletedBank(surface.GetCurrentMorph());
        if (index && surface.LastUsedFrame) {
            long frames_since_used = CurrentFrame - surface.LastUsedFrame;
            most_recent = most_recent < frames_since_used ? most_recent : frames_since_used;
        }
    }
    if (false) {} else for (std::list<CCountedPointer<CCompositeSurfaceCache> >::iterator itr = CachedSurfaces.begin(); itr != CachedSurfaces.end(); ++itr) {
        CCompositeSurfaceCache& surface = **itr;
        unsigned long index = FindIndexInCompletedBank(surface.GetCurrentMorph());
        if (index && surface.LastUsedFrame) {
            long frames_since_used = CurrentFrame - surface.LastUsedFrame;
            if (frames_since_used <= most_recent + 100)
                preload_indices.insert(index);
        }
    }
    double start_t = GFGetTime();
    bool loading = true;
    long preload_count = 0;
    while (loading) {
        loading = false;
        preload_count = 0;
        if (false) {} else for (std::set<unsigned long>::iterator itr = preload_indices.begin(); itr != preload_indices.end(); ++itr) {
            CGraphicFrame frame;
            const CGraphicInfoFrame* p_graphic_info_frame = CompletedBankFile.PeekGraphicInfoFrame(*itr,0);
            if (p_graphic_info_frame->GetFrame(frame,0,0))
                ++preload_count;
            else
                loading = true;
        }
        double delta_t = GFGetTime() - start_t;
        if (delta_t >= MaxPreloadTime)
            break;
        CompletedBankFile.FrameEnd();
        CompletedBankFile.ProcessLoadingQueue(true, MaxPreloadTime + start_t);
    }
    if (preload_count) {
        double delta_t = GFGetTime() - start_t;
    }
}
int main(){
 for(trial=0;trial<16384;++trial){
  CEngineSurfaceCompositionManager m;m.CachedSurfaces.count=rnd()%15;m.CurrentFrame=3000;m.lookups=0;
  m.MaxPreloadTime=(float)((int)(rnd()%21)-1)*.25f;maxTime=m.MaxPreloadTime;
  lastStart=2000.25;for(int k=0;k<24;++k)times[k]=lastStart+(k*.25);
  for(unsigned k=0;k<12;++k){frames[k].index=k;frames[k].queries=0;frames[k].readyAfter=rnd()%7;}
  int cutoff=0x7fffffff;
  for(int k=0;k<m.CachedSurfaces.count;++k){unsigned age=rnd()%400;surfaces[k].morph.index=rnd()%12;
   surfaces[k].LastUsedFrame=(rnd()%7)?m.CurrentFrame-age:0;m.CachedSurfaces.elems[k].data=surfaces+k;
   if(surfaces[k].morph.index&&surfaces[k].LastUsedFrame&&(int)age<cutoff)cutoff=age;}
  bool selected[12]={false};int require=0,unique=0;
  for(int k=0;k<m.CachedSurfaces.count;++k){unsigned ix=surfaces[k].morph.index;
   if(ix&&surfaces[k].LastUsedFrame&&(long)(m.CurrentFrame-surfaces[k].LastUsedFrame)<=cutoff+100&&!selected[ix]){
    selected[ix]=true;++unique;if((int)frames[ix].readyAfter>require)require=frames[ix].readyAfter;}}
  int limit=0;while((limit+1)*.25<maxTime)++limit;
  int finalIteration=require<limit?require:limit;
  int expectedTicks=(finalIteration+1)*.25<maxTime?finalIteration+1:finalIteration;
  bool finalSuccess=false;for(int ix=1;ix<12;++ix)if(selected[ix]&&(int)frames[ix].readyAfter<=finalIteration)finalSuccess=true;
  liveFrames=created=clockCalls=bankTicks=processCalls=iteration=0;num=0;
  m.PreloadCompletedBank();
  CHECK(m.lookups==2*m.CachedSurfaces.count&&bankTicks==expectedTicks&&processCalls==bankTicks);
  CHECK(clockCalls==2+finalIteration+(finalSuccess?1:0)&&!liveFrames&&created==unique*(finalIteration+1));
  unsigned pos=0;for(int step=0;step<=finalIteration;++step)for(int ix=1;ix<12;++ix)if(selected[ix])CHECK(order[pos++]==(unsigned)ix);
  CHECK(num==pos);for(int ix=1;ix<12;++ix)CHECK(frames[ix].queries==(unsigned)(selected[ix]?finalIteration+1:0));
 }
 puts("00b2c010_TEST PASS");return 0;
}