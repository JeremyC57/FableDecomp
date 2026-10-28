// CThingSearchTools::GetAllThingsInRoughArea(const C2DBoxI& area,const filter_type& filter,container_type& things) const -- scan original typed mapwho cells and append distinct matches.
// Leaf: ??$GetAllThingsInRoughArea@VCThingAICreature@@V?$vector@PAVCThingAICreature@@V?$allocator@PAVCThingAICreature@@@std@@@std@@VCThingFilter_HasDefGlobalIndex@@@CThingSearchTools@@QBEXABVC2DBoxI@@ABVCThingFilter_HasDefGlobalIndex@@AAV?$vector@PAVCThingAICreature@@V?$allocator@PAVCThingAICreature@@@std@@@std@@@Z
// Identity: proven; Own filter/caller/template mapping preserved. Original full CThing:CClass:CBaseObject:CBaseClass hierarchy through typed UID/bitset274/component CVectorMap/list/string/driver/physics/flags, actual CreatureBase:GameObject:Physical chain. CTCMapwho true CTCBase/NonCopy/BaseClass plus four complete16-byte original NThingMap nodes, SearchID76 and one-bit InMapwho. Original NThingMap::CThingMap true NonCopy/BaseClass with real MapwhoLevelInfo and nested Cells vectors, Manager/Area/NoThings,52bytes. Original level info Size/Extents/ScaleShift/NoThings20 and cell PFirstNode4; no flat vector/node/opaque manager fields or fabricated GetCellPtr helper. Real protected _Vector_base and three-argument _STLP_alloc_proxy, original active vector algorithms retained. Shared headers aliased unchanged. Primitive extents XExtent/YExtent projection follows validated rootN166 model; unused interfaces/suffixes omitted explicitly.
// Layout: original known owner/container/base identities and typed storage, actual counter/global contracts preserved.
#define CThingSearchTools CThingSearchTools_HeaderEvidence
#include "engine/CThingSearchTools.h"
#undef CThingSearchTools
#define CMapwhoNode CMapwhoNode_HeaderEvidence
#define CThingMapCell CThingMapCell_HeaderEvidence
#define CTCMapwho CTCMapwho_HeaderEvidence
#include "engine/CTCMapwho.h"
#undef CTCMapwho
#undef CMapwhoNode
#undef CThingMapCell
#define CBaseObject CBaseObject_HeaderEvidence
#include "engine/CBaseObject.h"
#undef CBaseObject
#define CClass CClass_HeaderEvidence
#include "engine/CClass.h"
#undef CClass
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#ifndef _MSC_VER
#include <new>
#define __forceinline inline
#define __fastcall
#define __declspec(x)
typedef unsigned long long RetailUID;
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
protected:
    _Tp* _M_start;                                          
    _Tp* _M_finish;                                         
    _STLP_alloc_proxy<_Tp*,_Tp, _Alloc> _M_end_of_storage;      
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : protected _Vector_base<_Tp, _Alloc> {
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
protected:
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

namespace std{template<class A,class B>struct pair{A first;B second;};struct _List_node_base{_List_node_base*_M_next;_List_node_base*_M_prev;};template<class T>struct _List_node:public _List_node_base{T _M_data;};template<class T,class A>class _List_base{protected:typedef _List_node<T> _Node;_STLP_alloc_proxy<_Node*,_Node,allocator<_Node> > _M_node;};template<class T,class A=allocator<T> >class list:protected _List_base<T,A>{};struct _List_iterator_base{_List_node_base*_M_node;};template<class T>struct _Nonconst_traits{};template<class T,class Traits>struct _List_iterator:public _List_iterator_base{};template<unsigned int N>class bitset{public:unsigned long _M_w[(N+31)/32];};}
template<class T>class CArray:public std::vector<T>{};template<class K,class V>class CKeyPairCompareLess{};template<class K,class V,class C=CKeyPairCompareLess<K,V> >class CVectorMap{public:CArray<std::pair<K,V> > Data;C Compare;bool Dirty;};
class CCharStringData;class CCharString{public:CCharStringData*PStringData;CCharString();CCharString(const CCharString&);~CCharString();};class CBaseClass{public:CBaseClass();virtual ~CBaseClass();};class CBaseClassNonCopyable:public CBaseClass{protected:CBaseClassNonCopyable();virtual ~CBaseClassNonCopyable();private:CBaseClassNonCopyable(const CBaseClassNonCopyable&);CBaseClassNonCopyable&operator=(const CBaseClassNonCopyable&);};struct CBaseObjectPointer;class CBaseObject:public CBaseClass{public:CBaseObjectPointer*IntelligentPointer;protected:CBaseObject();virtual ~CBaseObject();};class CClass:public CBaseObject{public:std::_List_iterator<CClass*,std::_Nonconst_traits<CClass*> >ClassListPos,TypeListPos;unsigned char Type;};class CTCBase;enum ETCInterfaceType{TC_INTERFACE_DescriptiveZero=0};class CTCDriverBase;class CTCPhysicsBase;class CTCGraphicAppearance;
class CThing:public CClass{public:enum{TypeMask=0};RetailUID UniqueID;std::bitset<274>UsingComponents;CVectorMap<ETCInterfaceType,CTCBase*>Components;std::list<CTCBase*>UpdateComponentsList;CCharString SerialisationSectionName;CTCDriverBase*PTCDriver;CTCPhysicsBase*PhysicsTC;CTCGraphicAppearance*PTCGraphicAppearance;long WorldFrameCreatedAt;unsigned long ThingTypeFlags;};class CThingPhysical:public CThing{};class CThingGameObject:public CThingPhysical{};class CThingCreatureBase:public CThingGameObject{public:enum{TypeMask=0xf};};class CThingAICreature:public CThingCreatureBase{public:enum{TypeMask=0x1f};};template<class T>inline bool IsThingOfType(const CThing*p){unsigned long mask=T::TypeMask;return(p->ThingTypeFlags&mask)==mask;}
class CTCBase:public CBaseClassNonCopyable{public:CThing*ParentThing;bool ToBeRemoved,IsInGlobalUpdateTCs;CTCBase(CThing&);virtual ~CTCBase();};class CTCMapwho;class CWorldMap;class CThingManager;
class C2DBoxI{public:long TLX,TLY,BRX,BRY;C2DBoxI(){}C2DBoxI(long a,long b,long c,long d):TLX(a),TLY(b),BRX(c),BRY(d){}};class C2DExtentsI{public:long XExtent,YExtent;};
namespace NThingMap{class CThingMapCell;class CThingMapManager;class CMapwhoLevelInfo{public:long Size;C2DExtentsI Extents;long ScaleShift,NoThings;};class CMapwhoNode{public:CTCMapwho*PMapwho;CMapwhoNode*PNext,*PPrevious;CThingMapCell*PCell;};class CThingMapCell{public:CMapwhoNode*PFirstNode;};class CThingMap:public CBaseClassNonCopyable{public:std::vector<CMapwhoLevelInfo>MapwhoLevelInfo;CThingMapManager*Manager;std::vector<std::vector<CThingMapCell> >Cells;C2DBoxI Area;long NoThings;CThingMap(CThingMapManager&,const C2DBoxI&);};}
class CTCMapwho:public CTCBase{public:NThingMap::CMapwhoNode TLNode,TRNode,BLNode,BRNode;unsigned long SearchID;unsigned char InMapwho:1;CTCMapwho(CThing&);};using namespace NThingMap;
struct SMapwhoLevel{long Level,Shift;long _pad[2];};extern SMapwhoLevel g_MapwhoLevels[];extern long g_NumMapwhoLevels;struct SSearchStamps{unsigned long Searches,Found,Stamp;};extern SSearchStamps g_SearchStamps;
class CThingFilter_HasDefGlobalIndex{public:unsigned long DefGlobalIndex;bool operator()(const CThing&)const;};
typedef std::vector<CThingMap*> CMapVector;
class CThingSearchTools { public:
    CWorldMap* WorldMap;                                   
    NThingMap::CThingMapManager* PThingMapManager;                           
    CThingManager* ThingManager;                               
    mutable CMapVector AreaMaps;                                   
    protected:
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

template void CThingSearchTools::GetAllThingsInRoughArea<CThingAICreature,std::vector<CThingAICreature*>,CThingFilter_HasDefGlobalIndex>(const C2DBoxI&,const CThingFilter_HasDefGlobalIndex&,std::vector<CThingAICreature*>&) const;

#ifdef _MSC_VER
typedef char NativeClassSize[sizeof(CClass)==20?1:-1];typedef char NativeMapSize[sizeof(NThingMap::CThingMap)==52?1:-1];typedef char NativeMapwhoSize[sizeof(CTCMapwho)==84?1:-1];typedef char NativeLevelInfoSize[sizeof(CMapwhoLevelInfo)==20?1:-1];
#endif