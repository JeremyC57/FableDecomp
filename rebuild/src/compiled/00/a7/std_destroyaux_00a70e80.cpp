// std::__destroy_aux(first,last,tag) -- Destroy original map records and release counted ownership.
// Leaf: ??$__destroy_aux@PAU?$pair@JVC3DMeshVolume@@@std@@@std@@YIXPAU?$pair@JVC3DMeshVolume@@@0@0ABU__false_type@0@@Z
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Flags: /Gr
// Identity: Own CPhysicsMesh destructor00a6d550 uses NavigationContainmentVolumes, original CVectorMap<long,C3DMeshVolume>; separate string-keyed map has two string destructors. Deleting helper unreferenced; pair identity probable.
// Layout: real pair16 has scalar key0 and original twelve-byte second at4; C3DMeshVolume Name0/ConvexVolume4, CCompositeSurfaceInfo CBaseClass vptr0/Cache4. PDB fields and own helper calls agree; no active record headers.
#include <new>
#ifndef _MSC_VER
#define __fastcall
#define __forceinline inline
#endif
#include "engine/CCountedPointer.h"
class CCharStringData;class CCharString{public:CCharStringData* PStringData;CCharString():PStringData(0){}~CCharString();};class CContainmentVolume;
class C3DMeshVolume{public:CCharString Name;CCountedPointer<CContainmentVolume> ConvexVolume;};
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0 (slot 0), implicit copy
class CCompositeSurfaceCache;
class CCompositeSurfaceInfo:public CBaseClass{public:CCountedPointer<CCompositeSurfaceCache> Cache;};
namespace std{template<class A,class B>struct pair{A first;B second;};struct __false_type{__false_type(){}};template<class Iter>void __fastcall __destroy_aux(Iter first,Iter last,const __false_type&){for(;first!=last;++first)first->~pair();}}
typedef std::pair<long,C3DMeshVolume> Element;
template void __fastcall std::__destroy_aux<Element*>(Element*,Element*,const std::__false_type&);