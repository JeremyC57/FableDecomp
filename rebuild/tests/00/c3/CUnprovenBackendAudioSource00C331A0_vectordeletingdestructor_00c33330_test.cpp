#include <stdio.h>
#include <new>
#ifndef _MSC_VER
#define __forceinline inline
#endif
void FreeBlock_00c3c370(void*);
class CUnprovenLibraryObject_012B5868{public:virtual ~CUnprovenLibraryObject_012B5868();};
class CPooledNode:public CUnprovenLibraryObject_012B5868{public:CPooledNode* Prev;CPooledNode* Next;CPooledNode();virtual ~CPooledNode();};
class CUnprovenCountedPooledNode_00C3C590:public CPooledNode{public:unsigned int RefCount;CUnprovenCountedPooledNode_00C3C590();virtual ~CUnprovenCountedPooledNode_00C3C590();virtual void Cleanup();};
struct CUnprovenAudioDescriptor28{unsigned int Format,Channels,Rate,Bytes,Unproven10,Unproven14;void* Data;};
class CUnprovenAudioSource:public CUnprovenCountedPooledNode_00C3C590{public:CUnprovenAudioDescriptor28 Descriptor;unsigned int Flags;CUnprovenAudioSource();virtual ~CUnprovenAudioSource();virtual void Cleanup();virtual int Load(const CUnprovenAudioDescriptor28*);virtual int Read(void*);};
struct CUnprovenAudioBackendToken16{unsigned int Handle,Unproven04,Flags,Unproven0C;};
class CUnprovenBackendAudioSource_00C331A0:public CUnprovenAudioSource{public:CUnprovenAudioBackendToken16 BackendToken;CUnprovenBackendAudioSource_00C331A0();virtual ~CUnprovenBackendAudioSource_00C331A0();virtual void Cleanup();virtual int Load(const CUnprovenAudioDescriptor28*);static __forceinline void operator delete(void* p){FreeBlock_00c3c370(p);}static __forceinline void operator delete[](void* p){FreeBlock_00c3c370(p);}};
#ifdef _MSC_VER
typedef char ExactDescriptor[sizeof(CUnprovenAudioDescriptor28)==28?1:-1];typedef char ExactSource[sizeof(CUnprovenAudioSource)==48?1:-1];typedef char ExactBackendSource[sizeof(CUnprovenBackendAudioSource_00C331A0)==64?1:-1];
#endif
CUnprovenBackendAudioSource_00C331A0* ForceArray_00c33330(unsigned int n){return new CUnprovenBackendAudioSource_00C331A0[n];}
CUnprovenLibraryObject_012B5868::~CUnprovenLibraryObject_012B5868(){}CPooledNode::CPooledNode():Prev(0),Next(0){}CPooledNode::~CPooledNode(){}static int destroyed,freed;CUnprovenCountedPooledNode_00C3C590::CUnprovenCountedPooledNode_00C3C590():RefCount(0){}CUnprovenCountedPooledNode_00C3C590::~CUnprovenCountedPooledNode_00C3C590(){}void CUnprovenCountedPooledNode_00C3C590::Cleanup(){}CUnprovenAudioSource::CUnprovenAudioSource(){}CUnprovenAudioSource::~CUnprovenAudioSource(){}void CUnprovenAudioSource::Cleanup(){}int CUnprovenAudioSource::Load(const CUnprovenAudioDescriptor28*){return 0;}int CUnprovenAudioSource::Read(void*){return 0;}CUnprovenBackendAudioSource_00C331A0::CUnprovenBackendAudioSource_00C331A0(){}CUnprovenBackendAudioSource_00C331A0::~CUnprovenBackendAudioSource_00C331A0(){++destroyed;}void CUnprovenBackendAudioSource_00C331A0::Cleanup(){}int CUnprovenBackendAudioSource_00C331A0::Load(const CUnprovenAudioDescriptor28*){return 0;}static bool arrayAllocation;static void* expectedFreeAddress;static int wrongFree;
void FreeBlock_00c3c370(void* p){++freed;if(p!=expectedFreeAddress)++wrongFree;
#ifdef _MSC_VER
::operator delete(p);
#else
if(arrayAllocation)::operator delete[](p);else ::operator delete(p);
#endif
}
int main(){for(unsigned int n=1;n<4;++n){destroyed=freed=0;CUnprovenBackendAudioSource_00C331A0* p=ForceArray_00c33330(n);arrayAllocation=true;expectedFreeAddress=(char*)p-sizeof(size_t);delete[]p;if(destroyed!=(int)n||freed!=1||wrongFree)goto fail;CUnprovenBackendAudioSource_00C331A0* q=new CUnprovenBackendAudioSource_00C331A0;arrayAllocation=false;expectedFreeAddress=q;delete q;if(destroyed!=(int)n+1||freed!=2||wrongFree)goto fail;}puts("00c33330_TEST PASS");return 0;fail:puts("00c33330_TEST FAIL");return 1;}