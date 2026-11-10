#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
class CCodaObjectBase {public:virtual ~CCodaObjectBase() {}};
#define private public
#ifndef FABLE_CCODACONTEXT_H
#define FABLE_CCODACONTEXT_H

// Layout: SDK 2.10 CodeView member names; native offsets are 12 bytes below the SDK debug donor.
// The SDK release also retains eight more base bytes than this native fork; see VENDOR_COMPILER_EVIDENCE.txt.
// Native 00c3d030 confirms all accessed offsets, and list stride 0x14.
typedef struct _CRESOURCEDESC {                         // CODA.h (SDK 2.10 ProgrammingReference: CODA_Create's parameter)
    int n2DSources;
    int n3DSources;
    int nListeners;
    int nEnvironments;
} CRESOURCEDESC;
class CSalPlatform;
class CCoda2DSource;
class CCoda3DSource;
class CCodaListener;
class CCodaEnvironment;
class CCodaEAX;
class CCodaObject;
class CCodaObjectList : public CCodaObjectBase { public: // SDK 2.10 CodeView, 20 bytes
    int GetNumFree() const { return m_nFree; }
private:
    CCodaObject* m_pObjects;                             // +0x04
    CCodaObject* m_pFree;                                // +0x08
    int m_nFree;                                         // +0x0c
    unsigned int m_nObjects;                             // +0x10
};
class CCodaObject : public CCodaObjectBase { };          // release layout: vptr only (debug adds m_DebugTest/links)
class CCodaContext : public CCodaObject { public:
    CRESOURCEDESC* GetResourceCounts(CRESOURCEDESC* pResourceDesc) const;   // descriptive name: absent from SDK 2.10
private:
    CSalPlatform* m_pSal;                                // +0x04
    CCoda2DSource* m_2DSourceMem;                                 // +0x08
    CCoda3DSource* m_3DSourceMem;                                 // +0x0c
    CCodaListener* m_ListenerMem;                                 // +0x10
    CCodaEnvironment* m_EnvironmentMem;                              // +0x14
    CCodaEAX* m_EAX;                                         // +0x18
    CCodaObjectList m_2DSources;                         // +0x1c (m_nFree +0x28)
    CCodaObjectList m_3DSources;                         // +0x30
    CCodaObjectList m_Listeners;                         // +0x44
    CCodaObjectList m_Environments;                      // +0x58
    int m_nMax2DSources;                                 // +0x6c
    int m_nMax3DSources;                                 // +0x70
    int m_nMaxListeners;                                 // +0x74
    int m_nMaxEnvironments;                              // +0x78
    unsigned char UnmodelledTail[0x2c]; // SDK release size 0xa8; no tail member used here.
};
#endif

#undef private
CRESOURCEDESC* CCodaContext::GetResourceCounts(CRESOURCEDESC* pResourceDesc) const
{
    int count = m_nMax2DSources;
    if (m_pSal) {
        if (count > 0) pResourceDesc->n2DSources = m_2DSources.GetNumFree();
        else pResourceDesc->n2DSources = count;
        count = m_nMax3DSources;
        if (count > 0) pResourceDesc->n3DSources = m_3DSources.GetNumFree();
        else pResourceDesc->n3DSources = count;
        count = m_nMaxListeners;
        if (count > 0) pResourceDesc->nListeners = m_Listeners.GetNumFree();
        else pResourceDesc->nListeners = count;
        count = m_nMaxEnvironments;
        if (count > 0) pResourceDesc->nEnvironments = m_Environments.GetNumFree();
        else pResourceDesc->nEnvironments = count;
    } else {
        pResourceDesc->n2DSources = count;
        pResourceDesc->n3DSources = m_nMax3DSources;
        pResourceDesc->nListeners = m_nMaxListeners;
        pResourceDesc->nEnvironments = m_nMaxEnvironments;
    }
    return pResourceDesc;
}


// Representation-overlap diagnostics exercise implementation behavior only.
// Reinterpreting context subobjects as a descriptor is not a standard C++
// object-lifetime guarantee; the 8750 ordinary object cases are separate.
static int ReadWord(const unsigned char* bytes,int offset) {
 int value;memcpy(&value,bytes+offset,4);return value;
}
static void WriteWord(unsigned char* bytes,int offset,int value) {
 memcpy(bytes+offset,&value,4);
}
static unsigned CheckOverlaps() {
 assert(sizeof(int)==4 && CHAR_BIT==8);
 unsigned cases=0;int values[5]={-11,-1,0,1,19};
 for(int active=0;active!=2;++active)
 for(int a=0;a!=5;++a)for(int b=0;b!=5;++b)for(int c=0;c!=5;++c)for(int e=0;e!=5;++e)
 for(int pattern=0;pattern!=7;++pattern)
 for(int window=0;window!=9;++window) {
  CCodaContext context;
  context.m_pSal=active?reinterpret_cast<CSalPlatform*>(&context):0;
  context.m_2DSourceMem=0;context.m_3DSourceMem=0;context.m_ListenerMem=0;
  context.m_EnvironmentMem=0;context.m_EAX=0;
  CCodaObjectList* lists[4]={&context.m_2DSources,&context.m_3DSources,&context.m_Listeners,&context.m_Environments};
  int counts[4]={pattern-3,10-pattern,pattern*7,-pattern};
  for(int k=0;k!=4;++k){lists[k]->m_pObjects=0;lists[k]->m_pFree=0;lists[k]->m_nFree=counts[k];lists[k]->m_nObjects=0;}
  context.m_nMax2DSources=values[a];context.m_nMax3DSources=values[b];
  context.m_nMaxListeners=values[c];context.m_nMaxEnvironments=values[e];
  memset(context.UnmodelledTail,0x5a,sizeof(context.UnmodelledTail));
  unsigned char* bytes=reinterpret_cast<unsigned char*>(&context);
  int maxima[4]={int(reinterpret_cast<unsigned char*>(&context.m_nMax2DSources)-bytes),int(reinterpret_cast<unsigned char*>(&context.m_nMax3DSources)-bytes),int(reinterpret_cast<unsigned char*>(&context.m_nMaxListeners)-bytes),int(reinterpret_cast<unsigned char*>(&context.m_nMaxEnvironments)-bytes)};
  int freeOffsets[4];for(int k=0;k!=4;++k)freeOffsets[k]=int(reinterpret_cast<unsigned char*>(&lists[k]->m_nFree)-bytes);
  int windows[9]={maxima[1],maxima[2],maxima[3],freeOffsets[1],freeOffsets[2],freeOffsets[2]-4,freeOffsets[3],freeOffsets[3]-4,freeOffsets[3]-8};
  int outputOffset=windows[window];assert(outputOffset>=0 && outputOffset+16<=int(sizeof(context)));
  unsigned char expected[sizeof(CCodaContext)];memcpy(expected,bytes,sizeof(context));
  int count=ReadWord(expected,maxima[0]);
  for(int k=0;k!=4;++k){
   if(k)count=ReadWord(expected,maxima[k]);
   if(active && count>0)count=ReadWord(expected,freeOffsets[k]);
   WriteWord(expected,outputOffset+4*k,count);
  }
  CRESOURCEDESC* output=reinterpret_cast<CRESOURCEDESC*>(bytes+outputOffset);
  assert(context.GetResourceCounts(output)==output);
  assert(memcmp(expected,bytes,sizeof(context))==0);
  ++cases;
 }
 assert(cases==78750);return cases;
}

int main() {
 int values[5]={-11,-1,0,1,19};unsigned cases=0;
 for(int active=0;active!=2;++active)
 for(int a=0;a!=5;++a)for(int b=0;b!=5;++b)for(int c=0;c!=5;++c)for(int e=0;e!=5;++e)
 for(int pattern=0;pattern!=7;++pattern) {
  CCodaContext context;context.m_pSal=active?reinterpret_cast<CSalPlatform*>(&context):0;
  context.m_nMax2DSources=values[a];context.m_nMax3DSources=values[b];context.m_nMaxListeners=values[c];context.m_nMaxEnvironments=values[e];
  int maxima[4]={values[a],values[b],values[c],values[e]};int freeCounts[4]={pattern-3,10-pattern,pattern*7,0-pattern};
  context.m_2DSources.m_nFree=freeCounts[0];context.m_3DSources.m_nFree=freeCounts[1];context.m_Listeners.m_nFree=freeCounts[2];context.m_Environments.m_nFree=freeCounts[3];
  struct Guarded {int before;CRESOURCEDESC desc;int after;} output;output.before=123;output.after=456;
  CRESOURCEDESC* result=context.GetResourceCounts(&output.desc);assert(result==&output.desc);
  int actual[4]={output.desc.n2DSources,output.desc.n3DSources,output.desc.nListeners,output.desc.nEnvironments};
  for(int i=0;i!=4;++i)assert(actual[i]==(active&&maxima[i]>0?freeCounts[i]:maxima[i]));
  assert(output.before==123&&output.after==456);
  assert(context.m_nMax2DSources==maxima[0]&&context.m_nMax3DSources==maxima[1]&&context.m_nMaxListeners==maxima[2]&&context.m_nMaxEnvironments==maxima[3]);
  ++cases;
 }
 assert(cases==8750);
#ifndef FABLE_TEST_DEFINED_ONLY
 assert(CheckOverlaps()==78750);
#endif
 puts("00c3d030_TEST PASS");return 0;
}