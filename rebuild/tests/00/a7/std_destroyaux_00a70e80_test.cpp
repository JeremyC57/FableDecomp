#include <stdio.h>
#include <stdlib.h>
// std::__destroy_aux(first,last,tag) -- Destroy original map records and release counted ownership.
// Leaf: ??$__destroy_aux@PAU?$pair@JVC3DMeshVolume@@@std@@@std@@YIXPAU?$pair@JVC3DMeshVolume@@@0@0ABU__false_type@0@@Z
// Flags: /Gr
// Identity: Own CPhysicsMesh destructor00a6d550 uses NavigationContainmentVolumes, original CVectorMap<long,C3DMeshVolume>; separate string-keyed map has two string destructors. Deleting helper unreferenced; pair identity probable.
// Layout: real pair16 has scalar key0 and original twelve-byte second at4; C3DMeshVolume Name0/ConvexVolume4, CCompositeSurfaceInfo CBaseClass vptr0/Cache4. PDB fields and own helper calls agree; no active record headers.
#include <new>
#ifndef _MSC_VER
#define __fastcall
#define __forceinline inline
#endif
class CCPPointerInfo{public:unsigned long RefCount;void (__fastcall* DeleteFunc)(void*);void* Data;};
template<class T>class CCountedPointer{public:T* Data;CCPPointerInfo* Info;CCountedPointer():Data(0),Info(0){}__forceinline void ReleaseInfo(){if(Info){--Info->RefCount;if(Info->RefCount==0){Info->DeleteFunc(Info->Data);::operator delete(Info);}}}__forceinline ~CCountedPointer(){ReleaseInfo();Data=0;Info=0;}};
class CCharStringData;class CCharString{public:CCharStringData* PStringData;CCharString():PStringData(0){}~CCharString();};class CContainmentVolume;
class C3DMeshVolume{public:CCharString Name;CCountedPointer<CContainmentVolume> ConvexVolume;};
class CBaseClass{public:virtual ~CBaseClass();};class CCompositeSurfaceCache;
class CCompositeSurfaceInfo:public CBaseClass{public:CCountedPointer<CCompositeSurfaceCache> Cache;};
namespace std{template<class A,class B>struct pair{A first;B second;};struct __false_type{__false_type(){}};template<class Iter>void __fastcall __destroy_aux(Iter first,Iter last,const __false_type&){for(;first!=last;++first)first->~pair();}}
typedef std::pair<long,C3DMeshVolume> Element;
template void __fastcall std::__destroy_aux<Element*>(Element*,Element*,const std::__false_type&);

static int callbacks,finalizers,errors;static CCPPointerInfo* sharedInfo;static void* token;
static void __fastcall DestroyData(void* p){if(p!=token)++errors;++callbacks;}
CCharString::~CCharString(){++finalizers;}
CBaseClass::~CBaseClass(){++finalizers;}
static bool run(){int object;token=&object;CCPPointerInfo* block=new CCPPointerInfo;block->RefCount=2;block->DeleteFunc=&DestroyData;block->Data=token;sharedInfo=block;
Element* records=(Element*)malloc(3*sizeof(Element));for(int i=0;i<3;++i)new(records+i)Element;records[0].second.ConvexVolume.Info=block;records[2].second.ConvexVolume.Info=block;std::__destroy_aux(records,records,std::__false_type());bool ok=callbacks==0&&finalizers==0&&block->RefCount==2;std::__destroy_aux(records,records+3,std::__false_type());ok=ok&&callbacks==1&&finalizers==3;free(records);return ok;}
int main(){bool ok=run()&&!errors;puts(ok?"00a70e80_TEST PASS":"00a70e80_TEST FAIL");return ok?0:1;}