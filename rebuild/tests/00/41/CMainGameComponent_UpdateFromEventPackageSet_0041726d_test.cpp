#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <algorithm>
#ifdef _MSC_VER
namespace std { template<class T> inline const T& __fastcall max(const T& a,const T& b) { return a<b?b:a; } }
#endif
static void check(bool v) { if(!v) { puts("FAIL");exit(1); } }
static std::string trace;
static long worlds[8],reported[8];static bool updates[8];static unsigned long checksums[8],seeds[8];static int nw,nr;
class CGameEventPackage { public: long TimeStamp;long GetTimeStamp() const { return TimeStamp; } };
class CGameEventPackageSet { public: unsigned long n;CGameEventPackage packages[8]; mutable unsigned long count_queries;
 unsigned long GetNoPackages() const { ++count_queries;return n; }
 const CGameEventPackage* GetPackage(unsigned long i) const { check(i<n);return packages+i; }
};
class CWorld { public: void Update(const CGameEventPackage& p,bool up) { trace+='W';worlds[nw]=p.TimeStamp;updates[nw++]=up; } };
class CDisplayEngine { public: void ConfirmFeedbackGameEventPackage(const CGameEventPackage*) { trace+='F'; } void WorldUpdate() {trace+='D';} };
class CNetworkClient { public: void Update(long frame,unsigned long checksum,unsigned long seed) { trace+='N';reported[nr]=frame;checksums[nr]=checksum;seeds[nr++]=seed; } };
namespace boost { template<class T> class scoped_ptr { T* ptr;public:scoped_ptr(T* p):ptr(p){} T* operator->() const{return ptr;} }; }
class CMainGameComponent { public:
 long CurrentServerFrame,InputServerFrame;unsigned long WorldSeed;bool AllowRender;
 boost::scoped_ptr<CWorld> PWorld;boost::scoped_ptr<CDisplayEngine> PDisplayEngine;CNetworkClient Client;
 CMainGameComponent(CWorld* w,CDisplayEngine* d):PWorld(w),PDisplayEngine(d){}
 void ProcessEventPackage(const CGameEventPackage*) { trace+='P'; }
 unsigned long PeekWorldChecksum()const { return (unsigned long)InputServerFrame*17u+(unsigned long)CurrentServerFrame*3u; }
 unsigned long PeekWorldSeed()const { return WorldSeed; }
 long GetCurrentServerFrame()const { return CurrentServerFrame; }
 void UpdateFromEventPackageSet(const CGameEventPackageSet*);
};
void CMainGameComponent::UpdateFromEventPackageSet(const CGameEventPackageSet* event_package_set) {
    for(long c1=0;c1<event_package_set->GetNoPackages();++c1) {
        const CGameEventPackage* event_package=event_package_set->GetPackage(c1);
        if(event_package->GetTimeStamp()>InputServerFrame) {
            ProcessEventPackage(event_package);
            bool do_update=false;
            if(InputServerFrame==CurrentServerFrame) {
                do_update=true;
                AllowRender=true;
            }
            PWorld->Update(*event_package,do_update);
            PDisplayEngine->ConfirmFeedbackGameEventPackage(event_package);
            InputServerFrame=event_package->GetTimeStamp();
            CurrentServerFrame=std::max(event_package->GetTimeStamp(),CurrentServerFrame);
            Client.Update(GetCurrentServerFrame(),PeekWorldChecksum(),PeekWorldSeed());
            PDisplayEngine->WorldUpdate();
        }
    }
}
int main() {
 const long values[]={-2,0,1,3,7};
 for(int initial=0;initial!=5;++initial)for(int current=0;current!=5;++current)for(int permit=0;permit!=2;++permit)for(int seq=0;seq!=625;++seq) {
  CWorld w;CDisplayEngine d;CMainGameComponent x(&w,&d);CGameEventPackageSet set;set.n=4;set.count_queries=0;
  x.InputServerFrame=values[initial];x.CurrentServerFrame=values[current];x.AllowRender=permit!=0;x.WorldSeed=0x9a7b3c2du;
  int code=seq;for(int j=0;j!=4;++j){set.packages[j].TimeStamp=values[code%5];code/=5;}
  trace="";nw=nr=0;x.UpdateFromEventPackageSet(&set);
  long input=values[initial],server=values[current];bool allow=permit!=0;int n=0;std::string want;
  for(int j=0;j!=4;++j) {long stamp=set.packages[j].TimeStamp;if(stamp>input){bool update=input==server;if(update)allow=true;check(worlds[n]==stamp&&updates[n]==update);input=stamp;if(server<stamp)server=stamp;check(reported[n]==server);check(checksums[n]==(unsigned long)input*17u+(unsigned long)server*3u);check(seeds[n]==x.WorldSeed);++n;want+="PWFND";}}
  check(trace==want&&nw==n&&nr==n);check(x.InputServerFrame==input&&x.CurrentServerFrame==server&&x.AllowRender==allow);check(set.count_queries==5);
 }
 CWorld w;CDisplayEngine d;CMainGameComponent x(&w,&d);CGameEventPackageSet empty;empty.n=0;empty.count_queries=0;x.InputServerFrame=9;x.CurrentServerFrame=11;x.AllowRender=false;trace="";nw=nr=0;x.UpdateFromEventPackageSet(&empty);check(trace==""&&nw==0&&nr==0&&empty.count_queries==1&&x.InputServerFrame==9&&x.CurrentServerFrame==11&&!x.AllowRender);
 puts("0041726d_TEST PASS");return 0;
}