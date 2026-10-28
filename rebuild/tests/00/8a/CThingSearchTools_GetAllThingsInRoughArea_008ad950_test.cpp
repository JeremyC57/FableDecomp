#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#ifndef _MSC_VER
#include <new>
#define __forceinline inline
#define __fastcall
#define __declspec(x)
typedef unsigned long RetailUID;typedef char FixtureRetailUID64[sizeof(RetailUID)==8?1:-1];
#else
#include <stddef.h>
inline void* operator new(unsigned int,void*p){return p;}
typedef unsigned __int64 RetailUID;
#endif
namespace std{struct __true_type{__true_type(){}};}using std::__true_type;
namespace std {
template <class _Tp> inline const _Tp& (max)(const _Tp& __a, const _Tp& __b) { return __a < __b ? __b : __a; }
inline void* __copy_trivial(const void* __first, const void* __last, void* __result)
{
    return (__last == __first) ? __result
                               : ((char*)memmove(__result, __first, ((const char*)__last - (const char*)__first)))
                                     + ((const char*)__last - (const char*)__first);
}
template <class _OutputIter, class _Size, class _Tp>
inline _OutputIter fill_n(_OutputIter __first, _Size __n, const _Tp& __value)
{
    for (; __n > 0; --__n, ++__first)
        *__first = __value;
    return __first;
}
template <class _Tp> class allocator {
public:
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};
template <class _Value,class _Tp, class _Alloc> class _STLP_alloc_proxy : public _Alloc {
public:
    _Value _M_data;
};
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;                                          
    _Tp* _M_finish;                                         
    _STLP_alloc_proxy<_Tp*,_Tp, _Alloc> _M_end_of_storage;      
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp* pointer;
    typedef unsigned int size_type;
    pointer begin() { return this->_M_start; } pointer end() { return this->_M_finish; }
    void clear() { erase(this->_M_start,this->_M_finish); }
    pointer erase(pointer first,pointer last) { this->_M_finish=(pointer)__copy_trivial(last,this->_M_finish,first); return first; }
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
    void push_back(const _Tp& __x)
    {
        if (this->_M_finish != this->_M_end_of_storage._M_data) {
            new (this->_M_finish) _Tp(__x);
            ++this->_M_finish;
        }
        else
            _M_insert_overflow(this->_M_finish, __x, __true_type(), 1UL, true);
    }
public:
    void _M_clear()
    {
        this->_M_end_of_storage.deallocate(this->_M_start, this->_M_end_of_storage._M_data - this->_M_start);
    }
    void _M_set(pointer __s, pointer __f, pointer __e)
    {
        this->_M_start = __s;
        this->_M_finish = __f;
        this->_M_end_of_storage._M_data = __e;
    }
    void _M_insert_overflow(pointer __position, const _Tp& __x, const __true_type&, size_type __fill_len,
                            bool __atend = false);   
};
#pragma auto_inline(off)
template <class _Tp, class _Alloc> __declspec(noinline)
void vector<_Tp, _Alloc>::_M_insert_overflow(pointer __position, const _Tp& __x, const __true_type&, size_type __fill_len,
                                             bool __atend)
{
    const size_type __old_size = size();
    const size_type __len = __old_size + (max)(__old_size, __fill_len);

    pointer __new_start = this->_M_end_of_storage.allocate(__len);
    pointer __new_finish = (pointer)__copy_trivial(this->_M_start, __position, __new_start);
    
    __new_finish = fill_n(__new_finish, __fill_len, __x);
    if (!__atend)
        
        __new_finish = (pointer)__copy_trivial(__position, this->_M_finish, __new_finish);
    _M_clear();
    _M_set(__new_start, __new_finish, __new_start + __len);
}
#pragma auto_inline(on)
}

namespace std{template<class A,class B>struct pair{A first;B second;};struct _List_node_base{_List_node_base*_M_next;_List_node_base*_M_prev;};template<class T>struct _List_node:public _List_node_base{T _M_data;};template<class T,class A>class _List_base{public:typedef _List_node<T> _Node;_STLP_alloc_proxy<_Node*,_Node,allocator<_Node> > _M_node;};template<class T,class A=allocator<T> >class list:public _List_base<T,A>{};struct _List_iterator_base{_List_node_base*_M_node;};template<class T>struct _Nonconst_traits{};template<class T,class Traits>struct _List_iterator:public _List_iterator_base{};template<unsigned int N>class bitset{public:unsigned long _M_w[(N+31)/32];};}
template<class T>class CArray:public std::vector<T>{};template<class K,class V>class CKeyPairCompareLess{};template<class K,class V,class C=CKeyPairCompareLess<K,V> >class CVectorMap{public:CArray<std::pair<K,V> > Data;C Compare;bool Dirty;};
class CCharStringData;class CCharString{public:CCharStringData*PStringData;CCharString();CCharString(const CCharString&);~CCharString();};class CBaseClass{public:CBaseClass();virtual ~CBaseClass();};class CBaseClassNonCopyable:public CBaseClass{public:CBaseClassNonCopyable();virtual ~CBaseClassNonCopyable();private:CBaseClassNonCopyable(const CBaseClassNonCopyable&);CBaseClassNonCopyable&operator=(const CBaseClassNonCopyable&);};struct CBaseObjectPointer;class CBaseObject:public CBaseClass{public:CBaseObjectPointer*IntelligentPointer;public:CBaseObject();virtual ~CBaseObject();};class CClass:public CBaseObject{public:std::_List_iterator<CClass*,std::_Nonconst_traits<CClass*> >ClassListPos,TypeListPos;unsigned char Type;};class CTCBase;enum ETCInterfaceType{TC_INTERFACE_DescriptiveZero=0};class CTCDriverBase;class CTCPhysicsBase;class CTCGraphicAppearance;
class CThing:public CClass{public:enum{TypeMask=0};RetailUID UniqueID;std::bitset<274>UsingComponents;CVectorMap<ETCInterfaceType,CTCBase*>Components;std::list<CTCBase*>UpdateComponentsList;CCharString SerialisationSectionName;CTCDriverBase*PTCDriver;CTCPhysicsBase*PhysicsTC;CTCGraphicAppearance*PTCGraphicAppearance;long WorldFrameCreatedAt;unsigned long ThingTypeFlags;};class CThingPhysical:public CThing{};class CThingGameObject:public CThingPhysical{};class CThingCreatureBase:public CThingGameObject{public:enum{TypeMask=0xf};};class CThingAICreature:public CThingCreatureBase{public:enum{TypeMask=0x1f};};template<class T>inline bool IsThingOfType(const CThing*p){unsigned long mask=T::TypeMask;return(p->ThingTypeFlags&mask)==mask;}
class CTCBase:public CBaseClassNonCopyable{public:CThing*ParentThing;bool ToBeRemoved,IsInGlobalUpdateTCs;CTCBase(CThing&);virtual ~CTCBase();};class CTCMapwho;class CWorldMap;class CThingManager;
class C2DBoxI{public:long TLX,TLY,BRX,BRY;C2DBoxI(){}C2DBoxI(long a,long b,long c,long d):TLX(a),TLY(b),BRX(c),BRY(d){}};class C2DExtentsI{public:long XExtent,YExtent;};
namespace NThingMap{class CThingMapCell;class CThingMapManager;class CMapwhoLevelInfo{public:long Size;C2DExtentsI Extents;long ScaleShift,NoThings;};class CMapwhoNode{public:CTCMapwho*PMapwho;CMapwhoNode*PNext,*PPrevious;CThingMapCell*PCell;};class CThingMapCell{public:CMapwhoNode*PFirstNode;};class CThingMap:public CBaseClassNonCopyable{public:std::vector<CMapwhoLevelInfo>MapwhoLevelInfo;CThingMapManager*Manager;std::vector<std::vector<CThingMapCell> >Cells;C2DBoxI Area;long NoThings;CThingMap(CThingMapManager&,const C2DBoxI&);};}
class CTCMapwho:public CTCBase{public:NThingMap::CMapwhoNode TLNode,TRNode,BLNode,BRNode;unsigned long SearchID;unsigned char InMapwho:1;CTCMapwho(CThing&);};using namespace NThingMap;
struct SMapwhoLevel{long Level,Shift;long _pad[2];};extern SMapwhoLevel g_MapwhoLevels[];extern long g_NumMapwhoLevels;struct SSearchStamps{unsigned long Searches,Found,Stamp;};extern SSearchStamps g_SearchStamps;
class CThingFilter_HasScriptName{public:CCharString Name;bool operator()(const CThing&)const;};
typedef std::vector<CThingMap*> CMapVector;
class CThingSearchTools { public:
    CWorldMap* WorldMap;                                   
    NThingMap::CThingMapManager* PThingMapManager;                           
    CThingManager* ThingManager;                               
    mutable CMapVector AreaMaps;                                   
    public:
    void GetAllThingMapsInArea(const C2DBoxI& area,CMapVector& maps) const;              
    C2DBoxI ClipAndConvertWorldMapAreaToLocalArea(const CThingMap& map,const C2DBoxI& area) const;            
    public:
    template <class TThing, class TContainer, class TFilter>
    void GetAllThingsInRoughArea(const C2DBoxI& area, const TFilter& filter, TContainer& things) const;
};
template <class TThing, class TContainer, class TFilter>
void CThingSearchTools::GetAllThingsInRoughArea(const C2DBoxI& area, const TFilter& filter, TContainer& things) const
{
    CMapVector* maps = &AreaMaps;
    GetAllThingMapsInArea(area,*maps);
    ++g_SearchStamps.Searches;
    g_SearchStamps.Found = 0;
    ++g_SearchStamps.Stamp;
    for (CThingMap** it = maps->begin(); it != AreaMaps.end(); ++it) {
        CThingMap* map = *it;
        C2DBoxI extents = ClipAndConvertWorldMapAreaToLocalArea(*map,area);
        for (long level = 0; level < g_NumMapwhoLevels; ++level) {
            long shift = g_MapwhoLevels[level].Shift;
            C2DBoxI box(extents.TLX >> shift, extents.TLY >> shift, ((extents.BRX - 1) >> shift) + 1,
                        ((extents.BRY - 1) >> shift) + 1);
            long width = box.BRX - box.TLX;
            if (width == 0 || box.BRY - box.TLY == 0)
                continue;
            long y = box.TLY;
            if (y < box.BRY) {
                
                
                for (; y < box.BRY; ++y) {
                    CThingMapCell* cell=&map->Cells.begin()[(unsigned char)level].begin()[y*(*(const long*)((unsigned char)level*sizeof(CMapwhoLevelInfo)+(size_t)map->MapwhoLevelInfo.begin()+offsetof(CMapwhoLevelInfo,Extents)+offsetof(C2DExtentsI,XExtent)))+box.TLX];
                    for (long x = box.TLX; x < box.BRX; ++x, ++cell) {
                        for (CMapwhoNode* node = cell->PFirstNode; node; node = node->PNext) {
                            CTCMapwho* entry = node->PMapwho;
                            if (entry->SearchID < g_SearchStamps.Stamp) {
                                entry->SearchID = g_SearchStamps.Stamp;
                                CThing* thing = entry->ParentThing;
                                if (IsThingOfType<TThing>(thing)) {
                                TThing* found=static_cast<TThing*>(thing);
                                if (filter(*found)) {
                                things.push_back(found);
                            }
                            }
                            }
                        }
                    }
                }
            }
        }
    }
    maps->clear();
}

template void CThingSearchTools::GetAllThingsInRoughArea<CThingAICreature,std::vector<CThingAICreature*>,CThingFilter_HasScriptName>(const C2DBoxI&,const CThingFilter_HasScriptName&,std::vector<CThingAICreature*>&) const;

#ifdef _MSC_VER
typedef char NativeClassSize[sizeof(CClass)==20?1:-1];typedef char NativeMapSize[sizeof(NThingMap::CThingMap)==52?1:-1];typedef char NativeMapwhoSize[sizeof(CTCMapwho)==84?1:-1];typedef char NativeLevelInfoSize[sizeof(CMapwhoLevelInfo)==20?1:-1];
#endif
CBaseClass::CBaseClass(){}CBaseClass::~CBaseClass(){}CBaseClassNonCopyable::CBaseClassNonCopyable(){}CBaseClassNonCopyable::~CBaseClassNonCopyable(){}CBaseObject::CBaseObject(){IntelligentPointer=0;}CBaseObject::~CBaseObject(){}CCharString::CCharString():PStringData(0){}CCharString::CCharString(const CCharString&x):PStringData(x.PStringData){}CCharString::~CCharString(){}CTCBase::CTCBase(CThing&t):ParentThing(&t),ToBeRemoved(false),IsInGlobalUpdateTCs(false){}CTCBase::~CTCBase(){}CTCMapwho::CTCMapwho(CThing&t):CTCBase(t),SearchID(0),InMapwho(false){}
namespace NThingMap{class CThingMapManager{};CThingMap::CThingMap(CThingMapManager&m,const C2DBoxI&a):Manager(&m),Area(a),NoThings(0){}}
SMapwhoLevel g_MapwhoLevels[1]; long g_NumMapwhoLevels=1; SSearchStamps g_SearchStamps={0,0,0};
static CThingAICreature objects[4]; static union EntryStorage{double align;unsigned char bytes[sizeof(CTCMapwho)];}slots[4];static CTCMapwho*entries[4]; static CMapwhoNode nodes[5]; static CThingMapCell cells[4]; static CMapwhoLevelInfo levelInfo[1]; static std::vector<CThingMapCell> cellVectors[1]; static NThingMap::CThingMapManager fixtureManager;static CThingMap map(fixtureManager,C2DBoxI(0,0,2,2)); static int filterCalls=0,bad=0;
void CThingSearchTools::GetAllThingMapsInArea(const C2DBoxI&,CMapVector& maps) const { if(maps._M_start[0]!=&map) bad=1; maps._M_finish=maps._M_start+1; }
C2DBoxI CThingSearchTools::ClipAndConvertWorldMapAreaToLocalArea(const CThingMap& source,const C2DBoxI& area) const { if(&source!=&map) bad=1; return area; }
bool CThingFilter_HasScriptName::operator()(const CThing& thing) const { ++filterCalls; return &thing!=&objects[2]; }
int main() { g_MapwhoLevels[0].Shift=0; levelInfo[0].Extents.XExtent=levelInfo[0].Extents.YExtent=2; cellVectors[0]._M_start=cells; cellVectors[0]._M_finish=cells+4; cellVectors[0]._M_end_of_storage._M_data=cells+4; map.MapwhoLevelInfo._M_start=levelInfo;map.MapwhoLevelInfo._M_finish=levelInfo+1;map.MapwhoLevelInfo._M_end_of_storage._M_data=levelInfo+1;map.Cells._M_start=cellVectors;map.Cells._M_finish=cellVectors+1;map.Cells._M_end_of_storage._M_data=cellVectors+1;
 for(int i=0;i<4;++i) { objects[i].ThingTypeFlags=CThingAICreature::TypeMask; entries[i]=new((void*)slots[i].bytes)CTCMapwho(objects[i]);nodes[i].PMapwho=entries[i];nodes[i].PNext=0;nodes[i].PPrevious=0;nodes[i].PCell=0; } objects[1].ThingTypeFlags=0; nodes[4].PMapwho=entries[0]; nodes[4].PNext=0; nodes[0].PNext=&nodes[1]; nodes[2].PNext=&nodes[3]; cells[0].PFirstNode=&nodes[0]; cells[1].PFirstNode=&nodes[4]; cells[2].PFirstNode=&nodes[2]; cells[3].PFirstNode=0;
 CThingSearchTools tools; CThingMap* scratch[1]; scratch[0]=&map; tools.AreaMaps._M_start=tools.AreaMaps._M_finish=scratch; tools.AreaMaps._M_end_of_storage._M_data=scratch+1;
 std::vector<CThingAICreature*> out; out._M_start=out._M_finish=out._M_end_of_storage._M_data=0; CThingFilter_HasScriptName filter;
 for(int run=0;run<2;++run) { tools.GetAllThingsInRoughArea<CThingAICreature,std::vector<CThingAICreature*>,CThingFilter_HasScriptName>(C2DBoxI(0,0,2,2),filter,out); if(bad || out._M_finish-out._M_start!=2*(run+1) || filterCalls!=3*(run+1) || out._M_start[2*run]!=&objects[0] || out._M_start[2*run+1]!=&objects[3] || tools.AreaMaps._M_finish!=scratch || g_SearchStamps.Searches!=unsigned(run+1) || g_SearchStamps.Stamp!=unsigned(run+1) || g_SearchStamps.Found) goto fail; }
 free(out._M_start); puts("008ad950_TEST PASS"); return 0; fail: puts("008ad950_TEST FAIL"); return 1; }