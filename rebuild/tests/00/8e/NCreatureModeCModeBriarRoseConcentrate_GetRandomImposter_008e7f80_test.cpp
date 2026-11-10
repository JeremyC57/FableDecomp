#include <cstdio>
#include <vector>
#include <functional>
#include <algorithm>
static int gWorlds,gSeeds,gRotates,gGets,gPredicates,gModeQueries,gBadMode;
class CTCCreatureModeManager{public:bool Active;bool IsModeActive(unsigned long mode)const{++gModeQueries;gBadMode|=mode!=0x3f;return Active;}};
class CThing{public:bool Dead;CTCCreatureModeManager Mode;bool IsDead()const{return Dead;}bool GetTC(CTCCreatureModeManager** out){*out=&Mode;return true;}};
template<class T>class CIntelligentPointer{public:T* PItem;CIntelligentPointer(T* p=0):PItem(p){}T* Get()const{++gGets;return PItem;}T* operator->()const{return Get();}};
template<class T>bool operator!=(const CIntelligentPointer<T>& p,int){return p.Get()!=0;}
template<class T>bool IS_THING_ALIVE(const T& thing){if(thing!=0){if(!thing->IsDead())return true;}return false;}
class CWorld{public:unsigned int Seed;unsigned int* GetWorldSeed()const{++gSeeds;return &const_cast<CWorld*>(this)->Seed;}};
static unsigned int GFROR13(unsigned int value){++gRotates;return (value>>13)|(value<<19);}
static unsigned int GFRandom(unsigned int range,unsigned int* seed){*seed*=0x24a1;*seed+=0x24df;*seed=GFROR13(*seed);if(range)return *seed%range;return 0;}
class CIsImposterHit:public std::unary_function<const CIntelligentPointer<CThing>,bool>{public:bool operator()(const CIntelligentPointer<CThing>& pthing)const{++gPredicates;CTCCreatureModeManager* ptc_cmm;if(IS_THING_ALIVE(pthing)&&pthing->GetTC(&ptc_cmm)&&ptc_cmm->IsModeActive(0x3f))return false;return true;}};
namespace NCreatureMode {class CModeBriarRoseConcentrate{public:std::vector<CIntelligentPointer<CThing> > Imposters;CWorld* World;CWorld& GetWorld()const{++gWorlds;return *World;}CThing* GetRandomImposter()const;};}
CThing* NCreatureMode::CModeBriarRoseConcentrate::GetRandomImposter()const
{
 long num_not_hit_imposters=std::count_if(Imposters.begin(),Imposters.end(),std::not1(CIsImposterHit()));
 if(num_not_hit_imposters>0){
  long random_i=GFRandom(num_not_hit_imposters,GetWorld().GetWorldSeed());
  for(long c1=0;c1!=Imposters.size();++c1){
   if(IS_THING_ALIVE(Imposters[c1])){
    if(random_i--==0)return Imposters[c1].Get();
   }
  }
 }
 return 0;
}

int main(){int ok=sizeof(unsigned int)==4,cases=0;unsigned int seeds[8]={0,1,2,255,65535,0x12345678u,0x80000000u,0xffffffffu};int patterns=1;for(int n=0;n<=5;++n){if(n)patterns*=4;for(int pattern=0;pattern<patterns;++pattern)for(int si=0;si<8;++si){CThing things[5];int states[5];int mask=pattern,notHit=0,expectedGets=0,alive=0;NCreatureMode::CModeBriarRoseConcentrate mode;CWorld world;world.Seed=seeds[si];mode.World=&world;for(int i=0;i<n;++i){states[i]=mask%4;mask/=4;things[i].Dead=states[i]==1;things[i].Mode.Active=states[i]==3;mode.Imposters.push_back(CIntelligentPointer<CThing>(states[i]?&things[i]:0));if(states[i]==3)++notHit;if(states[i]>=2)++alive;expectedGets+=states[i]>=2?3:(states[i]?2:1);}
unsigned int expectedSeed=seeds[si];CThing* expected=0;if(notHit){expectedSeed=expectedSeed*0x24a1u+0x24dfu;expectedSeed=(expectedSeed>>13)|(expectedSeed<<19);int rank=expectedSeed%(unsigned int)notHit;for(int i=0;i<n;++i){expectedGets+=states[i]?2:1;if(states[i]>=2&&rank--==0){expected=&things[i];++expectedGets;break;}}}
gWorlds=gSeeds=gRotates=gGets=gPredicates=gModeQueries=gBadMode=0;CThing* result=mode.GetRandomImposter();ok &= result==expected&&world.Seed==expectedSeed&&gWorlds==(notHit?1:0)&&gSeeds==gWorlds&&gRotates==gWorlds&&gPredicates==n&&gModeQueries==alive&&!gBadMode&&gGets==expectedGets;++cases;}}
ok &= cases==10920;std::printf(ok?"008e7f80_TEST PASS\n":"008e7f80_TEST FAIL\n");return ok?0:1;}