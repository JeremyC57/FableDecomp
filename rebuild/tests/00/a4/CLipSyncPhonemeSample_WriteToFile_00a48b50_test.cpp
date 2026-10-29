#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#ifndef _MSC_VER
#define __cdecl
#define __fastcall
#define __stdcall
#define __declspec(x)
#define __forceinline inline
#define __int64 __attribute__((__mode__(__DI__))) long
#endif
#define PASS9_TEST
// 00a48b50 CLipSyncPhonemeSample::WriteToFile
// Leaf: ?WriteToFile@CLipSyncPhonemeSample@@QBEXAAVCDataOutputStream@@@Z
// Flags:
#include <stddef.h>
#include "fable_string.h"
#define CDataOutputStream CDataOutputStreamFields
#include "engine/CDataOutputStream.h"
#undef CDataOutputStream
class CBaseClass{public:virtual~CBaseClass();};
class CBaseClassNonCopyable:public CBaseClass{protected:virtual~CBaseClassNonCopyable();};
class CDataStream:public CBaseClassNonCopyable{public:virtual void SetPosition(unsigned long)=0;virtual unsigned long GetPosition()const=0;virtual void Close()=0;};
class CDataOutputStream:public CDataStream{public:CCharString NewLine;virtual void Write(const void*,long)=0;void WriteULONG(unsigned long);};
// Layout: the callee's original state is unread here; its native 12-byte frequency array plus four floats occupy28 bytes.
class CSpeechFrame{unsigned char unread[28];public:void WriteToFile(CDataOutputStream&)const;};
class CCPPointerInfo;
// Layout: only the original pointer fields and proven const dereference API are exposed. No lifetime operation is performed by this row.
template<class T>class CCountedPointer{public:T*Data;CCPPointerInfo*Info;T*operator->()const{return Data;}};
class CLipSyncPhonemeSample{public:CCharString Symbol;unsigned long ID;CCountedPointer<CSpeechFrame>SampleFrame;void WriteToFile(CDataOutputStream&)const;};
void CLipSyncPhonemeSample::WriteToFile(CDataOutputStream&stream)const{stream.WriteULONG(ID);SampleFrame->WriteToFile(stream);}
FABLE_STATIC_ASSERT(sizeof(CDataOutputStream)==sizeof(CDataOutputStreamFields));
FABLE_STATIC_ASSERT(sizeof(CLipSyncPhonemeSample)==16);

class CCPPointerInfo{public:unsigned long RefCount;void(__fastcall*DeleteFunc)(void*);void*Data;};
static CLipSyncPhonemeSample*testSample;static CDataOutputStream*testStream;static CSpeechFrame*testFrame;static bool testMutate;static unsigned long testID;static int testError,testPhase,testIDs,testFrames;
CBaseClass::~CBaseClass(){}CBaseClassNonCopyable::~CBaseClassNonCopyable(){}CCharString::CCharString(){memset(this,0,sizeof *this);}CCharString::~CCharString(){}
void CDataOutputStream::WriteULONG(unsigned long id){if(this!=testStream||id!=testID||testPhase)testError=1;++testIDs;testPhase=1;if(testMutate){testSample->ID=id+1;testSample->SampleFrame.Data=testFrame;}}
void CSpeechFrame::WriteToFile(CDataOutputStream&stream)const{if(this!=testFrame||&stream!=testStream||testPhase!=1)testError=2;++testFrames;testPhase=2;}
struct TestStream:CDataOutputStream{virtual void SetPosition(unsigned long){testError=3;}virtual unsigned long GetPosition()const{return 0;}virtual void Close(){testError=3;}virtual void Write(const void*,long){testError=3;}};
int main(){const unsigned long ids[]={0,1,17,0x7fffffffUL,0x80000000UL,0xffffffffUL,0x12345678UL},refs[]={1,2,17,0xffffffffUL};CSpeechFrame frames[3];TestStream streams[2];int cases=0;
 for(int id=0;id<7;++id)for(int frame=0;frame<3;++frame)for(int stream=0;stream<2;++stream)for(int mutate=0;mutate<2;++mutate)for(int ref=0;ref<4;++ref){CLipSyncPhonemeSample sample;CCPPointerInfo info;info.RefCount=refs[ref];info.DeleteFunc=0;info.Data=&frames[frame];sample.ID=ids[id];sample.SampleFrame.Data=&frames[frame];sample.SampleFrame.Info=&info;unsigned char symbol[4];memcpy(symbol,&sample.Symbol,4);testSample=&sample;testStream=&streams[stream];testFrame=&frames[(frame+(mutate?1:0))%3];testMutate=mutate!=0;testID=ids[id];testError=testPhase=testIDs=testFrames=0;const CLipSyncPhonemeSample&input=sample;input.WriteToFile(streams[stream]);
 if(testError||testPhase!=2||testIDs!=1||testFrames!=1||sample.ID!=testID+(mutate?1UL:0UL)||sample.SampleFrame.Data!=testFrame||sample.SampleFrame.Info!=&info||info.RefCount!=refs[ref]||info.DeleteFunc||info.Data!=&frames[frame]||memcmp(symbol,&sample.Symbol,4))return 1;++cases;}
 if(cases!=336)return 2;puts("00a48b50_TEST PASS");return 0;}