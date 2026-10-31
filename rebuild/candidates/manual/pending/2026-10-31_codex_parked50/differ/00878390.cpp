// 00878390 NCameraModes::CTools::GetThingsOfType<CThingBuilding>
// Codex parked-50 best attempt: DIFFER(161v160); residue: 161/160 bytes; corrected original box/filter model still differs in integer coordinate computation and filter-pointer/frame scheduling.
// Flags:
// Leaf: ??$GetThingsOfType@VCThingBuilding@@@CTools@NCameraModes@@QBEXAAV?$list@PBVCThingBuilding@@V?$allocator@PBVCThingBuilding@@@std@@@std@@JABVC3DVector@@M@Z
#include "engine/CBaseClassNonCopyable.h"
#define CWorld CWorldFields
#include "engine/CWorld.h"
#undef CWorld
#define CGameCameraManager CGameCameraManagerFields
#include "engine/CGameCameraManager.h"
#undef CGameCameraManager
#define CAIGameCameraBase CAIGameCameraBaseFields
#include "engine/CAIGameCameraBase.h"
#undef CAIGameCameraBase
#define CThingSearchTools CThingSearchToolsFields
#include "engine/CThingSearchTools.h"
#undef CThingSearchTools
#pragma pack(push,1)
class C3DVector{public:float X,Y,Z;C3DVector(const C3DVector&v):X(v.X),Y(v.Y),Z(v.Z){}};
#pragma pack(pop)
class C2DCoordI{public:long X,Y;C2DCoordI(long x,long y):X(x),Y(y){}};
class C2DBoxI{public:long TLX,TLY,BRX,BRY;C2DBoxI(const C2DCoordI&a,const C2DCoordI&b):TLX(a.X),TLY(a.Y),BRX(b.X),BRY(b.Y){}};
class CThing;class CThingBuilding;
namespace std {
template<class T>class allocator{};
template<class P,class T,class A>class _STLP_alloc_proxy:public A{public:P _M_data;};
struct _List_node_base{_List_node_base*_M_next;_List_node_base*_M_prev;};
template<class T>struct _List_node:public _List_node_base{T _M_data;};
template<class T,class A>class _List_base{protected:_STLP_alloc_proxy<_List_node<T>*,_List_node<T>,allocator<_List_node<T> > >_M_node;};
template<class T,class A=allocator<T> >class list:public _List_base<T,A>{};
template<class A,class R>struct unary_function{typedef A argument_type;typedef R result_type;};
}
class CThingFilter_IsOfType:public std::unary_function<CThing,bool>{unsigned long Type;public:CThingFilter_IsOfType(unsigned long t):Type(t){}};
class CThingFilter_OverlappingSphere:public std::unary_function<CThing,bool>{C3DVector SpherePos;float Radius;public:CThingFilter_OverlappingSphere(const C3DVector&p,float r):SpherePos(p),Radius(r){}};
template<class A,class B>class CFilter_Bind2{protected:const A&F1;const B&F2;public:CFilter_Bind2(const A&a,const B&b):F1(a),F2(b){}};
template<class A,class B>inline CFilter_Bind2<A,B>__fastcall GFFilter_Bind2(const A&a,const B&b){return CFilter_Bind2<A,B>(a,b);}
class CThingSearchTools{
 unsigned char unreadSearch[sizeof(CThingSearchToolsFields)];
public:template<class T,class C,class F>void GetAllThingsInRoughArea(const C2DBoxI&,const F&,C&)const;
};
namespace boost{template<class T>class scoped_ptr{T*px;public:T&operator*()const{return *px;}};}
class CProceduralTexture;class CIDrawEnvironment;class CIDrawWorldMap;
class CIDrawWorld{public:virtual const CProceduralTexture*DrawGetProceduralTexture()const=0;virtual const CIDrawEnvironment*DrawGetEnvironment()const=0;virtual const CIDrawWorldMap*DrawGetWorldMap()const=0;virtual~CIDrawWorld();};
// Layout: native world has two four-byte bases, protected search pointer at20.
class CWorld:public CBaseClassNonCopyable,public CIDrawWorld{
 unsigned char unreadWorldPrefix[0x20-8];
protected:boost::scoped_ptr<CThingSearchTools>PThingSearchTools;
private:unsigned char unreadWorldTail[sizeof(CWorldFields)-0x24];
public:const CThingSearchTools&PeekThingSearchTools()const{return *PThingSearchTools;}
};
// Only the original destructor is needed in this unused virtual-surface projection.
class CAIGameCameraBase{public:virtual~CAIGameCameraBase();};
// Layout: native manager reference World74 after the two camera records.
class CGameCameraManager:public CAIGameCameraBase{
 unsigned char unreadCameras[0x74-4];const CWorld&World;
 unsigned char unreadManager[sizeof(CGameCameraManagerFields)-0x78];
public:const CWorld&GetWorld()const{return World;}
};
// Analysis context only: read both IN objects and every referenced filter byte;
// write only through OUT. Never deploy this as the retail search helper.
template<>__declspec(noinline)void CThingSearchTools::GetAllThingsInRoughArea<CThingBuilding,std::list<const CThingBuilding*>,CFilter_Bind2<CThingFilter_IsOfType,CThingFilter_OverlappingSphere> >(const C2DBoxI&box,const CFilter_Bind2<CThingFilter_IsOfType,CThingFilter_OverlappingSphere>&filter,std::list<const CThingBuilding*>&out)const{
 unsigned int any=0;const unsigned char*b=reinterpret_cast<const unsigned char*>(&box);
 for(unsigned int i=0;i<sizeof(box);++i)any|=b[i];
 const void*const*refs=reinterpret_cast<const void*const*>(&filter);
 const unsigned char*f1=static_cast<const unsigned char*>(refs[0]);const unsigned char*f2=static_cast<const unsigned char*>(refs[1]);
 for(unsigned int j=0;j<sizeof(CThingFilter_IsOfType);++j)any|=f1[j];
 for(unsigned int k=0;k<sizeof(CThingFilter_OverlappingSphere);++k)any|=f2[k];
 if(any==0)reinterpret_cast<unsigned char*>(&out)[0]=0;
}
namespace NCameraModes {
// Layout: original manager const reference0; remaining native tools32 unread.
class CTools{const CGameCameraManager&CameraManager;unsigned char unreadTools[28];
public:template<class T>void GetThingsOfType(std::list<const T*>&,long,const C3DVector&,float)const;
};
template<class T>void CTools::GetThingsOfType(std::list<const T*>&out,long type,const C3DVector&pos,float radius)const{
 long x=(long)pos.X;long y=(long)pos.Y;long r=(long)radius;
 C2DBoxI box(C2DCoordI(x-r,y-r),C2DCoordI(x,y));box.BRX+=r;box.BRY+=r;
 CameraManager.GetWorld().PeekThingSearchTools().GetAllThingsInRoughArea<T>(box,GFFilter_Bind2(CThingFilter_IsOfType(type),CThingFilter_OverlappingSphere(pos,radius)),out);
}
template void CTools::GetThingsOfType<CThingBuilding>(std::list<const CThingBuilding*>&,long,const C3DVector&,float)const;
}
