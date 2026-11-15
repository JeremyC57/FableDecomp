#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#ifdef _MSC_VER
#define TESTCALL __stdcall
#else
#define TESTCALL
#endif
static void CheckAt(bool b,int line){if(!b){fprintf(stderr,"failed line %d\n",line);abort();}}
#define Check(x) CheckAt((x),__LINE__)
static int Trace[40],Count;
static void Log(int n){Check(Count<40);Trace[Count++]=n;}
class CCodaContext;class CCoda2DSource;
static CCodaContext* Active;
static bool AllocFails,CtorThrows,CreateOK;
static int Mutation;
static void* Allocated;
static unsigned int Requested;
class CCodaObject {public:virtual ~CCodaObject(){} };
class CCodaSource:public CCodaObject {
public:
 CCodaSource();
 virtual bool Is3D()const{Log(3);return true;}
 virtual bool Create(unsigned int);
 CCoda2DSource* Get2D()const;
};
class CCoda2DSource:public CCodaSource {
public:
 CCoda2DSource(){}
 virtual bool Is3D()const{Log(3);return false;}
 virtual bool Create(unsigned int n){return CCodaSource::Create(n);}
 static void* operator new(size_t n)throw(){Log(4);Check(n==sizeof(CCoda2DSource));if(AllocFails)return 0;Allocated=malloc(n);Check(Allocated!=0);return Allocated;}
 static void operator delete(void* p)throw(){Log(12);Check(p==Allocated);free(p);Allocated=0;}
};
CCoda2DSource* CCodaSource::Get2D()const{return Is3D()?0:static_cast<CCoda2DSource*>(const_cast<CCodaSource*>(this));}
class CCodaObjectList {
public:
 CCodaObject* Free;
 CCodaObject* Added;
 CCodaObject* GetFree()const{Log(2);return Free;}
 void RemoveFree(){Log(8);}
 void Add(CCodaObject* p){Log(p?9:10);Added=p;}
};
class CCodaContext {
public:
 int m_nMax2DSources;
 CCodaObjectList m_2DSources;
 void(TESTCALL *m_pfnErrorCallback)(int);
 void SetError(int);
 void ReleaseSource(CCodaSource* p){Check(p!=0);Log(7);if(Mutation==2)m_nMax2DSources=1;}
 CCoda2DSource* Create2DSource(unsigned int ffCaps);
};
int CODA_Error;
CCodaContext CODA;
void CCodaContext::SetError(int error){if(CODA_Error==0){CODA_Error=error;if(m_pfnErrorCallback)m_pfnErrorCallback(error);}}
CCodaSource::CCodaSource(){Log(5);
#if !defined(_MSC_VER) || defined(_CPPUNWIND)
 if(CtorThrows)throw 123;
#else
 Check(!CtorThrows);
#endif
}
bool CCodaSource::Create(unsigned int n){Check(n==Requested);Log(6);if(Mutation==1)Active->m_nMax2DSources=0;return CreateOK;}
static int SeenError;
static void TESTCALL ErrorCallback(int n){Check(CODA_Error==n);SeenError=n;Log(1);}
CCoda2DSource* CCodaContext::Create2DSource(unsigned int ffCaps)
{
 CCoda2DSource* pSource;
 if(m_nMax2DSources>0){
  CCodaObject* pObject=m_2DSources.GetFree();
  if(!pObject){CODA.SetError(-9);pSource=0;}
  else pSource=static_cast<CCodaSource*>(pObject)->Get2D();
 }else if(m_nMax2DSources==-1)pSource=new CCoda2DSource;
 else {CODA.SetError(-9);pSource=0;}
 if(!pSource)return pSource;
 if(!pSource->Create(ffCaps)){ReleaseSource(pSource);pSource=0;}
 if(m_nMax2DSources>0)m_2DSources.RemoveFree();
 m_2DSources.Add(pSource);
 return pSource;
}
int main(){
 CtorThrows=false;Count=0;CCoda2DSource pooled;CCodaSource plain;
 const int limits[]={-2,-1,0,1,17};unsigned long cases=0;
 for(int f=0;f<4;++f)
 for(int lim=0;lim<5;++lim)for(int pool=0;pool<3;++pool)for(int ok=0;ok<2;++ok)
 for(int af=0;af<2;++af)for(int err=0;err<2;++err)for(int cb=0;cb<2;++cb)for(int mut=0;mut<3;++mut){
  CCodaContext context;Active=&context;context.m_nMax2DSources=limits[lim];
  context.m_2DSources.Free=pool==0?0:pool==1?static_cast<CCodaObject*>(&pooled):static_cast<CCodaObject*>(&plain);
  context.m_2DSources.Added=&plain;context.m_pfnErrorCallback=0;
  CODA.m_pfnErrorCallback=cb?ErrorCallback:0;CODA_Error=err?17:0;SeenError=0;
  CreateOK=ok!=0;AllocFails=af!=0;Mutation=mut;Allocated=0;Requested=f;Count=0;
  CCoda2DSource* got=context.Create2DSource(f);
  int want[40],n=0,error=0;bool object=false,acquired=false;
  if(limits[lim]>0){
   want[n++]=2;if(pool==0)error=-9;else{want[n++]=3;object=pool==1;}
  }else if(limits[lim]==-1){want[n++]=4;if(!af){want[n++]=5;object=true;acquired=true;}}
  else error=-9;
  if(error&& !err&&cb)want[n++]=1;
  int finalLimit=limits[lim];
  if(object){
   want[n++]=6;if(mut==1)finalLimit=0;
   if(!ok){want[n++]=7;if(mut==2)finalLimit=1;}
   if(finalLimit>0)want[n++]=8;
   want[n++]=ok?9:10;
  }
  Check(Count==n);for(int i=0;i<n;++i)Check(Trace[i]==want[i]);
  Check(CODA_Error==(err?17:error));Check(SeenError==((error&&!err&&cb)?error:0));
  Check(context.m_nMax2DSources==finalLimit);
  CCoda2DSource* expected=object&&ok?(acquired?static_cast<CCoda2DSource*>(Allocated):&pooled):0;
  Check(got==expected);
  Check(context.m_2DSources.Added==(object?static_cast<CCodaObject*>(expected):static_cast<CCodaObject*>(&plain)));
  if(Allocated){void* memory=Allocated;static_cast<CCoda2DSource*>(memory)->~CCoda2DSource();free(memory);Allocated=0;}
  ++cases;
 }
#if !defined(_MSC_VER) || defined(_CPPUNWIND)
 // Constructor failure must invoke class cleanup exactly once and propagate the exception.
 CCodaContext context;Active=&context;context.m_nMax2DSources=-1;context.m_2DSources.Added=&plain;
 Requested=1;
 AllocFails=false;CtorThrows=true;Count=0;bool caught=false;
 try{context.Create2DSource(1);}catch(int code){Check(code==123);caught=true;}
 Check(caught&&Allocated==0&&Count==3&&Trace[0]==4&&Trace[1]==5&&Trace[2]==12);
 Check(context.m_2DSources.Added==&plain);
#endif
 Check(cases==2880);
 puts("01214fe0_TEST PASS");return 0;
}