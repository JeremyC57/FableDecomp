// Self-contained dependency fixture; selected function and original recovery body are copied below.
// Tests exercise lifetime, retry order, cache replacement and loading; no actual file or allocator service.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <map>
#ifdef _MSC_VER
#define FABLE_FASTCALL __fastcall
#else
#define FABLE_FASTCALL
#endif
static void check(bool v,int line){if(!v){fprintf(stderr,"check %d\n",line);exit(2);}}
#define CHECK(x) check((x),__LINE__)
class C3DAnimation2;class C3DAnimationInfo;class CMovableResourceMemoryPool;
struct TestContext {
 int shared_success,instance_success,shared_calls,instance_calls,waits,loads,adds,removes,begins,shrinks,defrags,progress,file_deaths,animation_deaths;
 unsigned long datatype;bool fit;int shared_expected;CMovableResourceMemoryPool* pool;C3DAnimation2* shared;char events[256];int event_n;
 TestContext(){memset(this,0,sizeof(*this));}
 void Event(char c){events[event_n++]=c;events[event_n]=0;CHECK(event_n<255);}
};
static TestContext* ctx;
class CCPPointerInfo {
public:unsigned long RefCount;void(FABLE_FASTCALL*DeleteFunc)(void*);void* Data;
 CCPPointerInfo(unsigned long r,void(FABLE_FASTCALL*d)(void*),void*p):RefCount(r),DeleteFunc(d),Data(p){}
};
template<class T>class CCountedPointer {
public:T* Data;CCPPointerInfo* Info;
 CCountedPointer():Data(0),Info(0){}
 CCountedPointer(T*data):Data(data){if(data)Info=new CCPPointerInfo(1,DeleteData,(void*)Data);else Info=0;}
 CCountedPointer(const CCountedPointer&other):Data(other.Data),Info(other.Info){if(Info)++Info->RefCount;}
 ~CCountedPointer(){if(Info){--Info->RefCount;if(Info->RefCount==0){Info->DeleteFunc(Info->Data);delete Info;}}Data=0;Info=0;}
 CCountedPointer&operator=(const CCountedPointer&other){ShareData(other.Data,other.Info);return *this;}
 T*operator->()const{return Data;}T&operator*()const{return *Data;}T*get()const{return Data;}bool IsNull()const{return Data==0;}
 void ReleaseData(){if(Info){--Info->RefCount;if(Info->RefCount==0){Info->DeleteFunc(Info->Data);delete Info;}}Info=0;Data=0;}
 void ShareData(T*data,CCPPointerInfo*info){if(Info!=info){ReleaseData();Data=data;Info=info;if(info)++info->RefCount;}}
 void Reset(T*data){ReleaseData();Data=data;if(data)Info=new CCPPointerInfo(1,DeleteData,(void*)Data);}
 static void FABLE_FASTCALL DeleteData(void*data){delete static_cast<T*>(data);}
};
class C3DAnimFile{public:int id;C3DAnimFile():id(123){}~C3DAnimFile(){ctx->Event('F');++ctx->file_deaths;}};
class C3DMesh2{public:unsigned long MeshID;unsigned long GetMeshID()const{return MeshID;}};
enum EAnimationType{ANIMTYPE_BASE=0,ANIMTYPE_BASE_RELATIVE=4,ANIMTYPE_COMBINATION_PARTIAL=3,ANIMTYPE_COMBINATION_DELTA=5};
class CHandleBasedMemoryPool{public:void Defragment(){++ctx->defrags;ctx->Event('D');}};
class CMovableResourceMemoryPool:public CHandleBasedMemoryPool{};
class C3DAnimation2 {
public:int id;CMovableResourceMemoryPool*pool;
 C3DAnimation2():id(99),pool(0){}
 ~C3DAnimation2(){++ctx->animation_deaths;}
 void SetMemoryPool(CMovableResourceMemoryPool*p){CHECK(p==ctx->pool);pool=p;ctx->Event('P');}
 bool InitialiseSharedAnimation(const C3DAnimFile&f,EAnimationType t){CHECK(f.id==123&&pool==ctx->pool&&t==ctx->shared_expected);ctx->Event('S');return ctx->shared_calls++==ctx->shared_success;}
 bool InitialiseAnimationInstance(C3DAnimation2&a,const C3DMesh2&m,bool fit,float*match){CHECK(&a==ctx->shared&&pool==ctx->pool&&fit==ctx->fit);CHECK(m.MeshID!=0);CHECK(match);*match=0.5f;ctx->Event('I');return ctx->instance_calls++==ctx->instance_success;}
};
class CBankFileAsyncData {
public:bool finished;unsigned long Length;int payload;
 CBankFileAsyncData(bool f):finished(f),Length(4),payload(543){}
 ~CBankFileAsyncData(){ctx->Event('X');}
 bool IsFinished()const{return finished;}
 bool WaitUntilFinished(){CHECK(!finished);++ctx->waits;ctx->Event('W');finished=true;return true;}
 void*GetData(){CHECK(finished);return &payload;}
 unsigned long GetLength()const{return Length;}
};
class C3DAnimFileLoader{public:CCountedPointer<C3DAnimFile> LoadAnimFileCompressed(const void*d,unsigned long n,CHandleBasedMemoryPool&p){CHECK(*(const int*)d==543&&n==4&&&p==ctx->pool);++ctx->loads;ctx->Event('L');CCountedPointer<C3DAnimFile> f(new C3DAnimFile);return f;}};
class C3DAnimFileXLoader:public C3DAnimFileLoader{};
class CResource {public:void RemoveFromResourceList(){++ctx->removes;ctx->Event('R');}};
class CResourceList{public:void ShrinkResources(unsigned long n){CHECK(n==0);++ctx->shrinks;ctx->Event('H');}};
class CResourceBank{public:CResourceList ResourceList;CResourceList&GetResourceList(){return ResourceList;}void AddResource(CResource&){++ctx->adds;ctx->Event('A');}};
class CMeshDataBank {
public:CMovableResourceMemoryPool*AnimationMemoryPool;C3DAnimFileXLoader*AnimLoader;CResourceBank AnimationResourceBank;unsigned long index;
 CMovableResourceMemoryPool*GetAnimationMemoryPool(){return AnimationMemoryPool;}
 CResourceBank&GetAnimationResourceBank(){return AnimationResourceBank;}
 unsigned long GetEntryDataType(unsigned long n)const{CHECK(n==index);return ctx->datatype;}
 EAnimationType GetAnimationType(unsigned long datatype)const {
  EAnimationType animtype=ANIMTYPE_BASE;datatype&=15;
  if(datatype==10)animtype=ANIMTYPE_BASE_RELATIVE;
  else if(datatype==7||datatype==8)animtype=ANIMTYPE_COMBINATION_DELTA;
  else if(datatype==9)animtype=ANIMTYPE_COMBINATION_PARTIAL;
  return animtype;
 }
};
class CMeshDataBankEntry{public:CMeshDataBank*Bank;unsigned long Index;CMeshDataBank*GetBank()const{return Bank;}unsigned long GetDataType()const{return GetBank()->GetEntryDataType(Index);}};
template<class T>class CIVCountedPointer{public:T*Object;CIVCountedPointer():Object(0){}bool IsNull()const{return Object==0;}T*operator->()const{return Object;}};
class C3DAnimationInfo:public CMeshDataBankEntry {
public:
 class CAnimationData:public CResource {
 public:std::map<unsigned long,CCountedPointer<C3DAnimation2> > Animations;CCountedPointer<C3DAnimation2>SharedAnimation;CCountedPointer<CBankFileAsyncData>FileData;const C3DAnimationInfo*AnimInfo;
  void BuildSharedAnimation();
 };
 mutable CIVCountedPointer<CAnimationData>Data;
 C3DAnimationInfo(){Bank=0;Index=0;}
 ~C3DAnimationInfo(){delete Data.Object;}
 bool IsLoading()const{return !Data.IsNull()&&!Data->FileData.IsNull();}
 bool IsLoaded()const{return !Data.IsNull()&&!Data->SharedAnimation.IsNull();}
 void BeginLoading(bool b)const{CHECK(b);++ctx->begins;ctx->Event('B');if(Data.IsNull()){Data.Object=new CAnimationData;Data->AnimInfo=this;}Data->FileData.Reset(new CBankFileAsyncData(true));}
 void BuildSharedAnimation()const{if(!IsLoading())BeginLoading(true);Data->RemoveFromResourceList();Data->BuildSharedAnimation();ctx->shared=Data->SharedAnimation.Data;}
 C3DAnimFileXLoader&GetAnimLoader()const{return *GetBank()->AnimLoader;}
 bool RecoverAnimationMemory(unsigned long)const;
 CCountedPointer<C3DAnimation2>BuildAnimationInstance(const C3DMesh2&)const;
};
class CCharString{public:const char*text;CCharString(const char*p,long=-1):text(p){}};
namespace NProgressDisplay{void FABLE_FASTCALL DisplayProgress(const CCharString&s,float f,bool a,bool b){CHECK(strcmp(s.text,"Asset Wait: Animation")==0&&f==-1.0f&&!a&&!b);++ctx->progress;ctx->Event('G');}}
inline bool C3DAnimationInfo::RecoverAnimationMemory(unsigned long step)const {
 switch(step){
 case 0:{CMeshDataBank* pbank=GetBank();pbank->GetAnimationResourceBank().GetResourceList().ShrinkResources(0);return true;}
 case 1:{CMeshDataBank* pbank=GetBank();CMovableResourceMemoryPool* panimation_pool=pbank->GetAnimationMemoryPool();panimation_pool->Defragment();return true;}
 }
 return false;
}
void C3DAnimationInfo::CAnimationData::BuildSharedAnimation() {
 if(!FileData->IsFinished()) {
  NProgressDisplay::DisplayProgress("Asset Wait: Animation",-1.0f,false,false);
  FileData->WaitUntilFinished();
 }
 CMeshDataBank* bank=AnimInfo->GetBank();
 CCountedPointer<C3DAnimFile> anim_file=AnimInfo->GetAnimLoader().LoadAnimFileCompressed(FileData->GetData(),FileData->GetLength(),*bank->GetAnimationMemoryPool());
 SharedAnimation.Reset(new C3DAnimation2);
 SharedAnimation->SetMemoryPool(bank->GetAnimationMemoryPool());
 unsigned long datatype=AnimInfo->GetDataType();
 EAnimationType animtype=bank->GetAnimationType(datatype);
 for(long c_step=0;;++c_step) {
  if(SharedAnimation->InitialiseSharedAnimation(*anim_file,animtype))break;
  if(!AnimInfo->RecoverAnimationMemory(c_step))break;
 }
 FileData.Reset(0);
 AnimInfo->GetBank()->GetAnimationResourceBank().AddResource(*this);
}
CCountedPointer<C3DAnimation2> C3DAnimationInfo::BuildAnimationInstance(const C3DMesh2& mesh)const {
 if(!IsLoaded())BuildSharedAnimation();
 unsigned long datatype=GetDataType();
 bool fit_to_mesh=(datatype&0x80)==0x80;
 CCountedPointer<C3DAnimation2> animation;
 float match;
 animation.Reset(new C3DAnimation2);
 animation->SetMemoryPool(GetBank()->GetAnimationMemoryPool());
 for(long c_step=0;;++c_step) {
  if(animation->InitialiseAnimationInstance(*Data->SharedAnimation,mesh,fit_to_mesh,&match))break;
  if(!RecoverAnimationMemory(c_step))break;
 }
 Data->Animations[mesh.GetMeshID()]=animation;
 return animation;
}

static int expected_type(unsigned long n){static const int values[16]={0,0,0,0,0,0,0,5,5,3,4,0,0,0,0,0};return values[n&15];}
static int tries(int success){return success<3?success+1:3;}
static const char* retry_events(int success,char kind){if(kind=='S'){static const char*e[]={"S","SHS","SHSDS","SHSDS"};return e[success];}static const char*e[]={"I","IHI","IHIDI","IHIDI"};return e[success];}
int main(){unsigned count=0;
 for(int finished=0;finished<2;++finished)for(int share=0;share<3;++share)for(int success=0;success<4;++success)for(unsigned long type=0;type<32;++type){
  TestContext t;ctx=&t;CMovableResourceMemoryPool pool;C3DAnimFileXLoader loader;CMeshDataBank bank;bank.AnimationMemoryPool=&pool;bank.AnimLoader=&loader;bank.index=0x12345678UL;t.pool=&pool;t.datatype=type;t.shared_expected=expected_type(type);t.shared_success=success;
  {C3DAnimationInfo owner;owner.Bank=&bank;owner.Index=bank.index;owner.Data.Object=new C3DAnimationInfo::CAnimationData;owner.Data->AnimInfo=&owner;owner.Data->FileData.Reset(new CBankFileAsyncData(finished!=0));
   CCountedPointer<CBankFileAsyncData> retained;
   if(share)retained=owner.Data->FileData;
   if(share==2)owner.Data->SharedAnimation.Reset(new C3DAnimation2);
   t.event_n=0;t.events[0]=0;
   owner.Data->BuildSharedAnimation();
   CHECK(owner.Data->FileData.IsNull()&&owner.Data->FileData.Info==0);
   CHECK(owner.Data->SharedAnimation.Data&&owner.Data->SharedAnimation.Info->RefCount==1);
   CHECK(t.loads==1&&t.adds==1&&t.shared_calls==tries(success)&&t.waits==!finished&&t.progress==!finished&&t.file_deaths==1);
   CHECK(t.shrinks==(success>=1)&&t.defrags==(success>=2));
   CHECK(t.animation_deaths==(share==2));
   char expected[128];strcpy(expected,finished?"LP":"GWLP");strcat(expected,retry_events(success,'S'));strcat(expected,share?"AF":"XAF");CHECK(strcmp(t.events,expected)==0);
   if(share)CHECK(retained.Info->RefCount==1&&retained->finished);
  }
  CHECK(t.animation_deaths==(share==2?2:1));++count;
 }
 for(int state=0;state<4;++state)for(int prior=0;prior<3;++prior)for(int success=0;success<4;++success)for(unsigned long type=0;type<32;++type){
  TestContext t;ctx=&t;CMovableResourceMemoryPool pool;C3DAnimFileXLoader loader;CMeshDataBank bank;bank.AnimationMemoryPool=&pool;bank.AnimLoader=&loader;bank.index=3;t.pool=&pool;t.datatype=type|(type&1?0x80:0);t.fit=(type&1)!=0;t.shared_expected=expected_type(type);t.shared_success=0;t.instance_success=success;
  {C3DAnimationInfo owner;owner.Bank=&bank;owner.Index=bank.index;C3DMesh2 mesh;mesh.MeshID=0x80000000UL+type+1;CCountedPointer<C3DAnimation2> old;
   if(state){owner.Data.Object=new C3DAnimationInfo::CAnimationData;owner.Data->AnimInfo=&owner;if(state==2)owner.Data->FileData.Reset(new CBankFileAsyncData(true));if(state==3)owner.Data->SharedAnimation.Reset(new C3DAnimation2);}
   if(prior&&state){owner.Data->Animations[mesh.MeshID].Reset(new C3DAnimation2);if(prior==2)old=owner.Data->Animations[mesh.MeshID];owner.Data->Animations[mesh.MeshID+1000].Reset(new C3DAnimation2);}
   if(state==3)t.shared=owner.Data->SharedAnimation.Data;
   t.event_n=0;t.events[0]=0;
   {CCountedPointer<C3DAnimation2> result=owner.BuildAnimationInstance(mesh);
    CHECK(result.Data&&result.Data->pool==&pool&&result.Info->RefCount==2);
    CHECK(owner.Data->Animations[mesh.MeshID].Data==result.Data);
    CHECK(t.instance_calls==tries(success)&&t.begins==(state<2)&&t.removes==(state!=3)&&t.loads==(state!=3));
    CHECK(t.shrinks==(success>=1)&&t.defrags==(success>=2));
    CHECK(t.shared_calls==(state!=3)&&t.file_deaths==(state!=3));
    CHECK(t.animation_deaths==(prior==1&&state));
    if(prior&&state)CHECK(owner.Data->Animations.size()==2);else CHECK(owner.Data->Animations.size()==1);
    if(prior==2&&state)CHECK(old.Info->RefCount==1&&old.Data!=result.Data);
    char expected[128];strcpy(expected,state==3?"":state==2?"RLPSXAF":"BRLPSXAF");strcat(expected,"P");strcat(expected,retry_events(success,'I'));CHECK(strcmp(t.events,expected)==0);
   }
   CHECK(owner.Data->Animations[mesh.MeshID].Info->RefCount==1);
  }
  CHECK(t.animation_deaths==(prior&&state?4:2));++count;
 }
 CHECK(count==2304);
 puts("00a24d70_TEST PASS");return 0;
}