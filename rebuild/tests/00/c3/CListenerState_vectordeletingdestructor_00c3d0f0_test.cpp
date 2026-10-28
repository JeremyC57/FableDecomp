#include <stdio.h>
#include <new>
#ifndef _MSC_VER
#define __forceinline inline
#endif
void FreeBlock_00c3c370(void*);
class CUnprovenLibraryObject_012B5868{public:virtual ~CUnprovenLibraryObject_012B5868();};
class CPooledNode:public CUnprovenLibraryObject_012B5868{public:CPooledNode* Prev;CPooledNode* Next;CPooledNode();virtual ~CPooledNode();};
class CUnprovenFloat3_00C40970{public:float X,Y,Z;};
class CListenerState:public CPooledNode{public:CUnprovenFloat3_00C40970 Vector0C,Vector18;int Integer24,Integer28,Integer2C;float Float30,Float34,Float38;int Integer3C;float Scalar40;int Integer44;float Scalar48,Scalar4C,Scalar50,Scalar54,Scalar58,Scalar5C,Scalar60,Scalar64;int Integer68,Integer6C;unsigned int UnprovenWord70;int Integer74;unsigned long DirtyFlags;void* EffectHandle;CListenerState();virtual ~CListenerState();void SetVector0C(const CUnprovenFloat3_00C40970*);void SetVector18(const CUnprovenFloat3_00C40970*);void SetInteger44(int);void SetInteger6C(int);static __forceinline void operator delete(void* p){FreeBlock_00c3c370(p);}static __forceinline void operator delete[](void* p){FreeBlock_00c3c370(p);}};
#ifdef _MSC_VER
typedef char NodeSize[sizeof(CPooledNode)==12?1:-1];typedef char ParameterSize[sizeof(CListenerState)==128?1:-1];
#endif
CListenerState* ForceArray_00c3d0f0(unsigned int n){return new CListenerState[n];}
CUnprovenLibraryObject_012B5868::~CUnprovenLibraryObject_012B5868(){}CPooledNode::CPooledNode():Prev(0),Next(0){}CPooledNode::~CPooledNode(){}static int destroyed,freed;CListenerState::CListenerState(){}CListenerState::~CListenerState(){++destroyed;}static bool arrayAllocation;static void* expectedFreeAddress;static int wrongFree;
void FreeBlock_00c3c370(void* p){++freed;if(p!=expectedFreeAddress)++wrongFree;
#ifdef _MSC_VER
::operator delete(p);
#else
if(arrayAllocation)::operator delete[](p);else ::operator delete(p);
#endif
}
int main(){for(unsigned int n=1;n<4;++n){destroyed=freed=0;CListenerState* p=ForceArray_00c3d0f0(n);arrayAllocation=true;expectedFreeAddress=(char*)p-sizeof(size_t);delete[]p;if(destroyed!=(int)n||freed!=1||wrongFree)goto fail;CListenerState* q=new CListenerState;arrayAllocation=false;expectedFreeAddress=q;delete q;if(destroyed!=(int)n+1||freed!=2||wrongFree)goto fail;}puts("00c3d0f0_TEST PASS");return 0;fail:puts("00c3d0f0_TEST FAIL");return 1;}