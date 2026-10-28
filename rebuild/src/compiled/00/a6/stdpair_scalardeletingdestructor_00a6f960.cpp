// std::pair::scalar_deleting_destructor(unsigned int flags) -- Destroy original map records and release counted ownership.
// Leaf: ??_G?$pair@JVC3DMeshVolume@@@std@@QAEPAXI@Z
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
void DestroyPair(Element* p){delete p;}