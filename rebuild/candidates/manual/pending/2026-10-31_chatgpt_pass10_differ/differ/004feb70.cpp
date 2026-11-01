// 004feb70 CWorldMap::HeightAreaChanged
// Leaf: ?HeightAreaChanged@CWorldMap@@UAEXPBVC2DBoxI@@@Z
// Flags:
// Grade: ungraded pass9 true virtual interfaces/shared fields/nested vector model.
#include <stdlib.h>
#include "fable_string.h"
#define CWorld CWorldFields
#include "engine/CWorld.h"
#undef CWorld
#define CWorldMap CWorldMapFields
#include "engine/CWorldMap.h"
#undef CWorldMap
#define CPlayer CPlayerFields
#include "engine/CPlayer.h"
#undef CPlayer
class CPlayer;
class C3DVector{public:float X,Y,Z;};
class C2DBoxF;
class C2DCoordI{public:long X,Y;C2DCoordI(){}C2DCoordI(long x,long y):X(x),Y(y){}C2DCoordI(const C2DCoordI&o):X(o.X),Y(o.Y){}};
class C2DBoxI{public:long TLX,TLY,BRX,BRY;public:
 C2DBoxI(){}
 C2DBoxF ToC2DBoxF()const;
 C2DBoxI(const C2DCoordI&a,const C2DCoordI&b):TLX(a.X),TLY(a.Y),BRX(b.X),BRY(b.Y){}
 C2DBoxI(const C2DBoxI&b){Initialise(b);}
 void Initialise(const C2DBoxI&b){TLX=b.TLX;TLY=b.TLY;BRX=b.BRX;BRY=b.BRY;}
 long GetWidth()const{return BRX-TLX;}long GetHeight()const{return BRY-TLY;}
 void ExtendToContain(const C2DCoordI&);void ExtendBordersBy(long);
};class C2DBoxF{public:float TLX,TLY,BRX,BRY;public:
 C2DBoxF(float a,float b,float c,float d){Initialise(a,b,c,d);}
 C2DBoxF(const C2DBoxF&b){Initialise(b);}
 void Initialise(float a,float b,float c,float d){TLX=a;TLY=b;BRX=c;BRY=d;}
 void Initialise(const C2DBoxF&b){TLX=b.TLX;TLY=b.TLY;BRX=b.BRX;BRY=b.BRY;}
};
inline C2DBoxF C2DBoxI::ToC2DBoxF()const{return C2DBoxF((float)TLX,(float)TLY,(float)BRX,(float)BRY);}
class CDrawMapInfo;class CAFile;class CASuspendableProcess;class CMap;class CCharString;
class CBaseObjectPointer;class CThing;class CMapInfo;class CRegion;
class CRegionConnectionGraph;class CWADBank;class CLevelLoader;class CMainGameComponent;
namespace NThingMap{class CThingMapManager;}
namespace boost{template<class T>class scoped_ptr{T*ptr;};}
class CBaseIntelligentPointer{public:virtual~CBaseIntelligentPointer();private:CBaseObjectPointer*PData;};
template<class T>class CIntelligentPointer:public CBaseIntelligentPointer{};
template<class T>class CCountedPointer{public:T*Data;CCPPointerInfo*Info;CCountedPointer():Data(0),Info(0){}};
namespace std{

 struct __true_type{};struct __false_type{};
 struct input_iterator_tag{};struct forward_iterator_tag:public input_iterator_tag{};struct bidirectional_iterator_tag:public forward_iterator_tag{};struct random_access_iterator_tag:public bidirectional_iterator_tag{};
 template<class I>struct iterator_traits;
 template<class T>struct iterator_traits<T*>{typedef T value_type;typedef int difference_type;typedef random_access_iterator_tag iterator_category;};
 template<class T>struct __type_traits{typedef __false_type has_trivial_assignment_operator;typedef __false_type has_trivial_destructor;};
 template<>struct __type_traits<unsigned long>{typedef __true_type has_trivial_assignment_operator;typedef __true_type has_trivial_destructor;};
 template<class I,class O,class D>inline O __copy(I first,I last,O result,const random_access_iterator_tag&,D*){for(D n=last-first;n>0;--n){*result=*first;++first;++result;}return result;}
 template<class I,class O>inline O __copy_ptrs(I first,I last,O result,const __false_type&){return __copy(first,last,result,typename iterator_traits<I>::iterator_category(),(typename iterator_traits<I>::difference_type*)0);}
 template<class T>inline void __destroy_aux(T*p,const __false_type&){p->~T();}
 template<class T>inline void __destroy_aux(T*,const __true_type&){}
 template<class T>inline void _Destroy(T*p){__destroy_aux(p,typename __type_traits<T>::has_trivial_destructor());}
 template<class I>inline void __destroy_aux(I first,I last,const __false_type&){for(;first!=last;++first)_Destroy(&*first);}
 template<class I>inline void __destroy_aux(I,I,const __true_type&){}
 template<class I,class T>inline void __destroy(I first,I last,T*){__destroy_aux(first,last,typename __type_traits<T>::has_trivial_destructor());}
 template<class I>inline void _Destroy(I first,I last){__destroy(first,last,(typename iterator_traits<I>::value_type*)0);}
 template<class T>class allocator{public:
 allocator(){}allocator(const allocator&){}template<class U>allocator(const allocator<U>&){}~allocator(){}
 T*allocate(unsigned int n,const void* =0){return n?(T*)malloc(n*sizeof(T)):0;}
 void deallocate(T*p,unsigned int){if(p)free(p);}
 };
 template<class A,class R>struct unary_function{typedef A argument_type;typedef R result_type;};
 template<class A,class B,class R>struct binary_function{typedef A first_argument_type;typedef B second_argument_type;typedef R result_type;};
 template<class T>struct less:public binary_function<T,T,bool>{bool operator()(const T&a,const T&b)const{return a<b;}};
 template<class T>struct _Identity:public unary_function<T,T>{const T&operator()(const T&v)const{return v;}};
 template<class A,class B>struct pair{typedef A first_type;A first;B second;};
 template<class P>struct _Select1st:public unary_function<P,typename P::first_type>{const typename P::first_type&operator()(const P&p)const{return p.first;}};
 template<class K,class C=less<K>,class A=allocator<K> >class set;
 template<class V,class T,class A>class _STLP_alloc_proxy:public A{public:V _M_data;_STLP_alloc_proxy(const A&a,V p):A(a),_M_data(p){}};
 template<class T,class A>class _Vector_base{public:
 _Vector_base(const A&a):_M_start(0),_M_finish(0),_M_end_of_storage(a,0){}
 ~_Vector_base(){if(_M_start)_M_end_of_storage.deallocate(_M_start,_M_end_of_storage._M_data-_M_start);}
 protected:T*_M_start;T*_M_finish;_STLP_alloc_proxy<T*,T,A>_M_end_of_storage;
 };
 template<class T,class A=allocator<T> >class vector:public _Vector_base<T,A>{public:
  explicit vector(const A&a=A()):_Vector_base<T,A>(a){}
  ~vector(){_Destroy(this->_M_start,this->_M_finish);}
  typedef unsigned int size_type;
  vector(size_type,const T&,const A& =A());void swap(vector&);
typedef T*iterator;typedef T*pointer;
  typedef typename __type_traits<T>::has_trivial_assignment_operator _TrivialAss;
  iterator begin(){return this->_M_start;}iterator end(){return this->_M_finish;}
  size_type size()const{return size_type(this->_M_finish-this->_M_start);}
  T&operator[](size_type n){return *(begin()+n);}
  iterator erase(iterator first,iterator last){pointer i=__copy_ptrs(last,this->_M_finish,first,_TrivialAss());_Destroy(i,this->_M_finish);this->_M_finish=i;return first;}
  const T*begin()const{return this->_M_start;}
  const T&operator[](size_type n)const{return *(begin()+n);}
 };
 struct _List_node_base{_List_node_base*_M_next,*_M_prev;};
 template<class T>struct _List_node:public _List_node_base{T _M_data;};
 template<class T,class A>class _List_base{protected:_STLP_alloc_proxy<_List_node<T>*,_List_node<T>,allocator<_List_node<T> > >_M_node;};
 template<class T,class A=allocator<T> >class list:public _List_base<T,A>{public:explicit list(const A& =A());~list();};

 struct _Rb_tree_node_base{bool _M_color;_Rb_tree_node_base*_M_parent,*_M_left,*_M_right;};
 template<class T>struct _Rb_tree_node:public _Rb_tree_node_base{T _M_value_field;};
 template<class T,class A>struct _Rb_tree_base{protected:_STLP_alloc_proxy<_Rb_tree_node<T>*,_Rb_tree_node<T>,allocator<_Rb_tree_node<T> > >_M_header;};
 template<class K,class V,class Extract,class C,class A>class _Rb_tree:public _Rb_tree_base<V,A>{protected:unsigned int _M_node_count;C _M_key_compare;};
 template<class K,class T,class C=less<K>,class A=allocator<pair<const K,T> > >class map{_Rb_tree<K,pair<const K,T>,_Select1st<pair<const K,T> >,C,A>_M_t;};

}
template<class T>class CArray:public std::vector<T>{};
#ifdef _MSC_VER
typedef unsigned __int64 MapFileStamp;
#else
typedef unsigned int MapFileStamp __attribute__((mode(DI),aligned(8)));
#endif
class CMap{public:void HeightAreaChanged(const C2DBoxI*);};
class CMapInfo{public:
 C2DBoxI Area;CCountedPointer<CMap>PMap;CCharString LevelName,LevelScriptName;unsigned long MapUID;
 bool Used,Placed,Activated,Loading,LoadedOnPlayerProximity;CCharString SerialiseScript;MapFileStamp LevStamp,TngStamp;bool IsSea;
};
typedef char MapInfoLayout[sizeof(CMapInfo)==72?1:-1];
class CIEngine;class CEngineManager;
template<class T>class CTBaseSingleton{public:virtual~CTBaseSingleton();static T&__fastcall Get();};
class CEngineManager:public CTBaseSingleton<CEngineManager>{public:virtual~CEngineManager();
protected:std::list<CIEngine*>Engines;std::vector<C2DBoxI>ChangedAreas;
public:void AddChangedArea(const C2DBoxI&);
};
class CNavigatorManager{public:void UpdateNavigationMapArea(long,const C2DBoxF&);};
class CWorld:public CWorldFields{public:CNavigatorManager&GetNavigatorManager(){return *PNavigationManager_ptr;}};
#include "engine/CBaseClassNonCopyable.h"
class CIDrawWorldMap{public:
 virtual long DrawGetNoMaps()const=0;
 virtual CDrawMapInfo DrawGetMapInfo(long)const=0;
 virtual C2DBoxI DrawGetDimensions()const=0;
 virtual long DrawGetGranularity()const=0;
 virtual bool DrawIsPosInMap(const C3DVector&)const=0;
 virtual float DrawGetSizeZAt(const C3DVector&,float)const=0;
 virtual C3DVector DrawConvertWorldMapCoordsToLocalCoords(long,const C3DVector&)const=0;
 virtual long DrawGetMyPlayerMapNumber()const=0;
 virtual bool DrawFindLastUnblockedGroundPositionAndReportCollision(const C3DVector&,const C3DVector&,C3DVector&)const=0;
 virtual bool DrawFindLastUnblockedThemePositionAndReportCollision(const C3DVector&,const C3DVector&,C3DVector&)const=0;
 virtual long DrawGetFrame()const=0;
 virtual~CIDrawWorldMap();
};
class CWorldMap:public CBaseClassNonCopyable,public CIDrawWorldMap{public:
 virtual~CWorldMap();
 virtual void DeleteAllLevels();virtual void DeleteAllRegions();
 virtual void LoadFromFile(CAFile&,long);virtual void SaveToFile(CAFile&);virtual void RemoveLevel(long);
 virtual void LoadTopologyFromFile(long,CCountedPointer<CAFile>,CASuspendableProcess&);virtual void PostLoadTopology(long);
 virtual void ForceLoad(long);virtual void ForceUnload(long);virtual bool IsMapLoaded(long)const;virtual bool IsMapUsed(long)const;
 virtual long GetGranularity()const;virtual const C2DBoxI&GetDimensions()const;
 virtual CMap&GetMapAt(const C2DCoordI&);virtual const CMap&PeekMapAt(const C2DCoordI&)const;
 virtual long GetMapNumberAt(const C2DCoordI&)const;virtual long GetMapNumberAt(const C3DVector&)const;
 virtual CMap&GetMap(long);virtual const CMap&PeekMap(long)const;
 virtual void GetAllMapsInArea(const C2DBoxI&,std::vector<unsigned long>&)const;
 virtual void GetAllLoadedMapsInArea(const C2DBoxI&,std::vector<unsigned long>&)const;
 virtual void PostRegionLoad(long);virtual C2DCoordI GetMapPos(long)const;
 virtual bool IsMapChangeable(long)const;virtual bool IsPosChangeable(const C2DCoordI&)const;
 virtual std::set<CCharString>GetLoadableSections()const;virtual void Update();virtual void HeightAreaChanged(const C2DBoxI*);
protected:
 void UpdateDestroyedThemeAreas();void UpdateAllEdges();
 virtual void LoadScriptBrushesFromFile(long,CCountedPointer<CAFile>);virtual long AllocateNewMap();
public:
 virtual long DrawGetNoMaps()const;virtual CDrawMapInfo DrawGetMapInfo(long)const;virtual C2DBoxI DrawGetDimensions()const;
 virtual long DrawGetGranularity()const;virtual bool DrawIsPosInMap(const C3DVector&)const;
 virtual float DrawGetSizeZAt(const C3DVector&,float)const;virtual C3DVector DrawConvertWorldMapCoordsToLocalCoords(long,const C3DVector&)const;
 virtual long DrawGetMyPlayerMapNumber()const;
 virtual bool DrawFindLastUnblockedGroundPositionAndReportCollision(const C3DVector&,const C3DVector&,C3DVector&)const;
 virtual bool DrawFindLastUnblockedThemePositionAndReportCollision(const C3DVector&,const C3DVector&,C3DVector&)const;
 virtual long DrawGetFrame()const;
 C2DBoxI ClipAndConvertWorldMapAreaToLocalArea(long,const C2DBoxI&)const;
// Layout: original two polymorphic bases occupy0..7, World8, Component12,
 // scoped_ptr16, STLport map20, CArray<CMapInfo>32. Shared flattened header
 // imported above; only the used prefix is reconstructed here.
protected:CWorld&World;CMainGameComponent*Component;boost::scoped_ptr<NThingMap::CThingMapManager>PThingMapManager;
 std::map<CCharString,long>LoadedScriptedMapBrushes;CArray<CMapInfo>Maps;
};
void CWorldMap::HeightAreaChanged(const C2DBoxI*area){
 C2DBoxI dimensions=GetDimensions();std::vector<unsigned long>maps;
 if(!area)area=&dimensions;
 CTBaseSingleton<CEngineManager>::Get().AddChangedArea(*area);GetAllMapsInArea(*area,maps);
 for(unsigned long*it=maps.begin();it<maps.end();++it){long map=*it;C2DBoxI local=ClipAndConvertWorldMapAreaToLocalArea(map,*area);CMap*pmap=Maps[map].PMap.Data;if(pmap)pmap->HeightAreaChanged(&local);}
 UpdateAllEdges();
 for(unsigned long*it=maps.begin();it<maps.end();++it)World.GetNavigatorManager().UpdateNavigationMapArea(*it,area->ToC2DBoxF());
}
