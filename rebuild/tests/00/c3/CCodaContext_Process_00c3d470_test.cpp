#include <cstdio>
#include <cstdlib>
#include <vector>
#ifdef _MSC_VER
typedef __int64 CREFTIME;
#else
typedef long long CREFTIME;
#endif
static int cases=0;
#define CHECK(x) do{if(!(x)){std::fprintf(stderr,"case%d line%d: %s\n",cases,__LINE__,#x);std::exit(1);}}while(0)
static std::vector<CREFTIME> trace;
static int mutationMode=0, callbackChanges=0, clockReads=0, frameIndex=0;
static unsigned int expectedCommit=0;
static CREFTIME clockValue=0, initialClock=0, clockOrigin=0;
class CCodaObject {public:CCodaObject* next;int id;CCodaObject():next(0),id(0){}virtual ~CCodaObject(){}CCodaObject*GetNext()const{return next;}};
class CCodaObjectList {public:CCodaObject*objects;CCodaObjectList():objects(0){}CCodaObject*GetObjects()const{return objects;}};
class CCodaContext;
static CCodaContext* context=0;
class CCodaListener:public CCodaObject {public:static unsigned int m_ffCommit;static unsigned int GetCommit(){return m_ffCommit;}static void ClearCommit(){m_ffCommit=0;}void Commit(){trace.push_back(100+id);m_ffCommit|=1u<<id;if(mutationMode==1)next=0;}};
unsigned int CCodaListener::m_ffCommit=0;
class CCodaEnvironment:public CCodaObject {public:static unsigned int m_ffSourceCommit;static unsigned int GetSourceCommit(){return m_ffSourceCommit;}static void ClearSourceCommit(){m_ffSourceCommit=0;}void Commit(){trace.push_back(400+id);}void Process(){trace.push_back(500+id);if(mutationMode==1)next=0;}};
unsigned int CCodaEnvironment::m_ffSourceCommit=0;
class CCoda3DSource;
class CCodaSource:public CCodaObject {public:unsigned int status;bool is3d;CCodaSource():status(0),is3d(false){}virtual bool Is3D()const{trace.push_back(200+id);return is3d;}CCoda3DSource*Get3D()const;bool IsPlaying()const{return (status&0x217)!=0;}void Process(CREFTIME time){CHECK(time==clockValue-clockOrigin);trace.push_back(300+id);trace.push_back(time);if(mutationMode==1)next=0;if(mutationMode==2)status=0;}};
class CCoda3DSource:public CCodaSource {public:void Commit(const CCodaObjectList&,const CCodaObjectList&,unsigned int);};
inline CCoda3DSource*CCodaSource::Get3D()const{return Is3D()?static_cast<CCoda3DSource*>(const_cast<CCodaSource*>(this)):0;}
class CSalPlatform {public:int frames;CSalPlatform():frames(0){}bool ReadyToProcess(){trace.push_back(600);return frameIndex<frames;}CREFTIME GetReferenceTime(){++clockReads;clockValue+=17;trace.push_back(700);return clockValue-clockOrigin;}void ProcessXtc(){trace.push_back(800);}int ProcessOutput();};
class CCodaContext {public:CSalPlatform*m_pSal;CCodaObjectList m_2DSources,m_3DSources,m_Listeners,m_Environments;unsigned int m_ffUpdate;void Process();};
void CCoda3DSource::Commit(const CCodaObjectList&a,const CCodaObjectList&b,unsigned int changes){CHECK(&a==&context->m_Listeners&&&b==&context->m_Environments&&changes==expectedCommit);trace.push_back(250+id);if(mutationMode==1)next=0;}
int CSalPlatform::ProcessOutput(){trace.push_back(900);++frameIndex;if(callbackChanges){CCodaListener::m_ffCommit=0x4000;CCodaEnvironment::m_ffSourceCommit=0x8000;context->m_ffUpdate=0x80001000u;}return -13;}

void CCodaContext::Process()
{
 bool bCommit=(m_ffUpdate>>31)&1;
 unsigned int ffCommit;
 if(bCommit){
  m_ffUpdate&=0x7fffffff;
  for(CCodaObject* pObject=m_Listeners.GetObjects();pObject;){
   CCodaListener* pListener=static_cast<CCodaListener*>(pObject);
   CCodaObject* pNext=pObject->GetNext();
   pListener->Commit();
   pObject=pNext;
  }
  ffCommit=CCodaListener::GetCommit()|m_ffUpdate|CCodaEnvironment::GetSourceCommit();
 }else ffCommit=0;
 bool bProcessSources=true;
 while(m_pSal->ReadyToProcess()){
  if(bProcessSources){
   CREFTIME time=m_pSal->GetReferenceTime();
   bProcessSources=false;
   for(CCodaObject* pObject=m_3DSources.GetObjects();pObject;){
    CCodaSource* pSource=static_cast<CCodaSource*>(pObject);
    CCodaObject* pNext=pObject->GetNext();
    if(bCommit)pSource->Get3D()->Commit(m_Listeners,m_Environments,ffCommit);
    if(pSource->IsPlaying()){pSource->Process(time);bProcessSources=true;}
    pObject=pNext;
   }
   m_pSal->ProcessXtc();
   for(CCodaObject* pObject=m_2DSources.GetObjects();pObject;){
    CCodaSource* pSource=static_cast<CCodaSource*>(pObject);
    CCodaObject* pNext=pObject->GetNext();
    if(pSource->IsPlaying()){pSource->Process(time);bProcessSources=true;}
    pObject=pNext;
   }
  }
  for(CCodaObject* pObject=m_Environments.GetObjects();pObject;){
   CCodaEnvironment* pEnvironment=static_cast<CCodaEnvironment*>(pObject);
   CCodaObject* pNext=pObject->GetNext();
   if(bCommit)pEnvironment->Commit();
   pEnvironment->Process();
   pObject=pNext;
  }
  m_pSal->ProcessOutput();
  CCodaListener::ClearCommit();
  CCodaEnvironment::ClearSourceCommit();
  m_ffUpdate=0;
 }
}

static void connect(CCodaObjectList&list,CCodaObject**objects,int n){list.objects=n?objects[0]:0;for(int k=0;k<n;++k)objects[k]->next=k+1<n?objects[k+1]:0;}
int main(){
 const unsigned int statuses[3]={0,0x200,0x800};
 for(int dirty=0;dirty<2;++dirty)for(int frames=0;frames<4;++frames)for(int listeners=0;listeners<3;++listeners)for(int three=0;three<3;++three)for(int two=0;two<3;++two)for(int envs=0;envs<3;++envs)for(int active=0;active<3;++active)for(int mutation=0;mutation<3;++mutation)for(int callback=0;callback<2;++callback){
  ++cases;trace.clear();CSalPlatform sal;sal.frames=frames;CCodaContext owner;owner.m_pSal=&sal;owner.m_ffUpdate=0x20u|(dirty?0x80000000u:0);context=&owner;mutationMode=mutation;callbackChanges=callback;clockReads=0;frameIndex=0;initialClock=((CREFTIME)1<<32)+41;clockOrigin=((CREFTIME)1<<32)-19;clockValue=initialClock;
  CCodaListener l[2];CCodaEnvironment e[2];CCoda3DSource s3[2];CCodaSource s2[2];CCodaObject*pl[2],*pe[2],*p3[2],*p2[2];
  for(int k=0;k<2;++k){l[k].id=k;e[k].id=k;s3[k].id=k;s2[k].id=10+k;s3[k].is3d=true;s3[k].status=statuses[active];s2[k].status=k==0?statuses[(active+1)%3]:statuses[active];pl[k]=&l[k];pe[k]=&e[k];p3[k]=&s3[k];p2[k]=&s2[k];}
  connect(owner.m_Listeners,pl,listeners);connect(owner.m_Environments,pe,envs);connect(owner.m_3DSources,p3,three);connect(owner.m_2DSources,p2,two);
  CCodaListener::m_ffCommit=0x40;CCodaEnvironment::m_ffSourceCommit=0x80;
  expectedCommit=dirty?(0xe0u|((1u<<listeners)-1)):0;
  std::vector<CREFTIME>wanted;unsigned int expectedListener=0x40;
  if(dirty)for(int k=0;k<listeners;++k){wanted.push_back(100+k);expectedListener|=1u<<k;}
  bool runSources=true;unsigned int st3[2]={s3[0].status,s3[1].status},st2[2]={s2[0].status,s2[1].status};int expectedReads=0;CREFTIME expectedTime=initialClock-clockOrigin;bool linked3=true,linked2=true,linkedEnv=true;
  for(int f=0;f<frames;++f){
   wanted.push_back(600);
   if(runSources){
    wanted.push_back(700);++expectedReads;expectedTime+=17;runSources=false;
    int n3=linked3?three:(three?1:0);
    for(int k=0;k<n3;++k){if(dirty){wanted.push_back(200+k);wanted.push_back(250+k);if(mutation==1)linked3=false;}if(st3[k]&0x217){wanted.push_back(300+k);wanted.push_back(expectedTime);runSources=true;if(mutation==1)linked3=false;if(mutation==2)st3[k]=0;}}
    wanted.push_back(800);int n2=linked2?two:(two?1:0);
    for(int k=0;k<n2;++k)if(st2[k]&0x217){wanted.push_back(310+k);wanted.push_back(expectedTime);runSources=true;if(mutation==1&&k==0)linked2=false;if(mutation==2)st2[k]=0;}
   }
   int ne=linkedEnv?envs:(envs?1:0);for(int k=0;k<ne;++k){if(dirty)wanted.push_back(400+k);wanted.push_back(500+k);if(mutation==1)linkedEnv=false;}
   wanted.push_back(900);
  }
  wanted.push_back(600);
  owner.Process();CHECK(trace==wanted);CHECK(clockReads==expectedReads&&frameIndex==frames);CHECK(CCodaListener::m_ffCommit==(frames?0:expectedListener));CHECK(CCodaEnvironment::m_ffSourceCommit==(frames?0:0x80));CHECK(owner.m_ffUpdate==(frames?0:0x20));
 }
 CHECK(cases==11664);std::puts("00c3d470_TEST PASS");return 0;
}