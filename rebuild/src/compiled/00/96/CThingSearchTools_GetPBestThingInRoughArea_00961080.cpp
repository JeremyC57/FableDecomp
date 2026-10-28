// CThingSearchTools::GetPBestThingInRoughArea(const C2DBoxI& area, const filter_type& filter, const comparator_type& compare, thing_type** out) const -- choose the preferred matching thing across mapwho cells once per search stamp.
// Leaf: ??$GetPBestThingInRoughArea@VCThingFilter_IsOpinionSourceWithFreshDeed@@VCThingCompare_Nearest@@$$CBVCThing@@@CThingSearchTools@@QBE_NABVC2DBoxI@@ABVCThingFilter_IsOpinionSourceWithFreshDeed@@ABVCThingCompare_Nearest@@PAPBVCThing@@@Z
// Identity: proven; hand951G070 exact mapping; own DeedReactionRoot::IsRunnable0x960B41/filter0x960FC0/comparator<constCThing>0x95FDA0 and exact constCThing PDB instance.
// Layout: the generic search model carries only this class type-mask tag; these three instantiations search CThing/const CThing. No CreatureBase object, field, sizeof, constructor or method is used, so its generated flattened full header layout is not asserted or accessed.
// Layout: the generic search model carries only this class type-mask tag; these three instantiations search CThing/const CThing. No AICreature object, field, sizeof, constructor or method is used. FreshDeed invokes an external filter and keeps its tracked-creature member opaque; no generated full AICreature layout is asserted or accessed.
// Layout: engine/CTCMapwho.h flattens CTCBase and splits all node members into pointer fragments. The local minimal view reads inherited ParentThing+4 (named Thing in this view) and SearchID+0x4C, agreeing with the header; linked-node traversal uses only the first two real node pointer fields.
// Layout: shared SearchTools header retains AreaMaps evidence; local mutable actual12-byte vector and real NThingMap::CThingMap namespace/key reconcile donor16. CThing/CTCMapwho partial named views are documented in N035; no instruction transplant. FreshDeed PCreature is an unused opaque8-byte member view because only its external filter call is performed.
#define CThingSearchTools CThingSearchTools_HeaderLayout
#include "engine/CThingSearchTools.h"
#undef CThingSearchTools
#include <string.h>
#include <stdlib.h>
#ifndef _MSC_VER
#include <new>
#define __forceinline inline
#define __declspec(x)
#endif
class C2DBoxI { public:
    long X0, Y0, X1, Y1;
    C2DBoxI() {}
    C2DBoxI(long x0, long y0, long x1, long y1) : X0(x0), Y0(y0), X1(x1), Y1(y1) {}
};

class CThing {                                        
public:
    enum { TypeMask = 0 };
    unsigned char _pad00[0x60];
    class CTCPhysicsBase* PhysicsTC;                      
    unsigned char _pad64[0x6c - 0x64];
    unsigned long ThingTypeFlags;                          
};

template <class T> inline bool IsThingOfType(const CThing* thing)
{
    unsigned long mask = T::TypeMask;
    return (thing->ThingTypeFlags & mask) == mask;
}
#ifdef _MSC_VER
inline void* operator new(unsigned int, void* where) { return where; }
#endif
namespace std { struct __true_type { __true_type() {} }; }
using std::__true_type;
class CThingCreatureBase:public CThing { public: enum { TypeMask=0xF }; };
class CThingAICreature:public CThingCreatureBase { public: enum { TypeMask=0x1F }; };
class C3DVector { public: float X,Y,Z; }; class CThingCompare_Nearest { public: C3DVector Pos; template<class T> T* operator()(T*,T*) const; };
class CThingFilter_IsOpinionSourceWithFreshDeed { public: unsigned char PCreature[8]; long LastVDeedID;
#ifdef _MSC_VER
unsigned __int64 LastSourceUID;
#else
unsigned long long LastSourceUID;
#endif
 bool operator()(const CThing&) const; };
class CTCMapwho { public:
    unsigned char _pad00[4];
    CThing*       Thing;                                   
    unsigned char _pad08[0x4c - 0x08];
    unsigned long SearchID;                             
};
namespace NThingMap {
class CMapwhoNode { public:
    CTCMapwho* Entry;
    CMapwhoNode*  Next;
};

struct MapwhoLevelInfoView { long SingleCell; long Width; long Height; long _pad[2]; };
struct CellVectorView { CMapwhoNode** _M_start; CMapwhoNode** _M_finish; CMapwhoNode** _M_end_of_storage; };
class CThingMap { public:
    void*             _vfptr;
    MapwhoLevelInfoView* LevelInfo;                           
    MapwhoLevelInfoView* LevelInfoEnd;
    MapwhoLevelInfoView* LevelInfoCap;
    void*             _pad10;
    CellVectorView*      Cells;                               

    CMapwhoNode** GetCellPtr(long cellsOffset, long offset, long x, long y) const
    {
        return &(*(const CellVectorView*)((unsigned long)Cells + cellsOffset))._M_start[y * *(const long*)((unsigned long)offset + (unsigned long)LevelInfo + sizeof(long)) + x];
    }
};
}
using namespace NThingMap;
struct SMapwhoLevel { long Level,Shift; long _pad[2]; };
extern SMapwhoLevel g_MapwhoLevels[];                      
extern long g_NumMapwhoLevels;                             
struct SSearchStamps { unsigned long Searches; unsigned long Found; unsigned long Stamp; };
extern SSearchStamps g_SearchStamps;                       

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
template <class _Value, class _Alloc> class _STLP_alloc_proxy : public _Alloc {
public:
    _Value _M_data;
};
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;                                          
    _Tp* _M_finish;                                         
    _STLP_alloc_proxy<_Tp*, _Alloc> _M_end_of_storage;      
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

typedef std::vector<CThingMap*> CMapVector;
class CThingSearchTools { public:
    void*      WorldMap;                                   
    void*      PThingMapManager;                           
    void*      ThingManager;                               
    mutable CMapVector AreaMaps;                                   
    protected:
    void GetAllThingMapsInArea(const C2DBoxI& area,CMapVector& maps) const;              
    C2DBoxI ClipAndConvertWorldMapAreaToLocalArea(const CThingMap& map,const C2DBoxI& area) const;            
    public:
    template<class F,class C,class T> bool GetPBestThingInRoughArea(const C2DBoxI& area,const F& filter,const C& compare,T** out) const;
};
template<class F,class C,class T> bool CThingSearchTools::GetPBestThingInRoughArea(const C2DBoxI& area,const F& filter,const C& compare,T** out) const
{
    T* best=0;
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
            C2DBoxI box(extents.X0 >> shift, extents.Y0 >> shift, ((extents.X1 - 1) >> shift) + 1,
                        ((extents.Y1 - 1) >> shift) + 1);
            long width = box.X1 - box.X0;
            if (width == 0 || box.Y1 - box.Y0 == 0)
                continue;
            long y = box.Y0;
            if (y < box.Y1) {
                long offset = (unsigned char)level * (long)sizeof(MapwhoLevelInfoView);
                long cellsOffset = (unsigned char)level * (long)sizeof(CellVectorView);
                for (; y < box.Y1; ++y) {
                    CMapwhoNode** cell = map->GetCellPtr(cellsOffset, offset, box.X0, y);
                    for (long x = box.X0; x < box.X1; ++x, ++cell) {
                        for (CMapwhoNode* node = *cell; node; node = node->Next) {
                            CTCMapwho* entry = node->Entry;
                            if (entry->SearchID < g_SearchStamps.Stamp) {
                                entry->SearchID = g_SearchStamps.Stamp;
                                CThing* thing = entry->Thing;
                                if(IsThingOfType<T>(thing) && filter(*thing)) best=compare(best,static_cast<T*>(thing));
                            }
                        }
                    }
                }
            }
        }
    }
    maps->clear();
    *out=best;
    return best!=0;
}

template bool CThingSearchTools::GetPBestThingInRoughArea<CThingFilter_IsOpinionSourceWithFreshDeed,CThingCompare_Nearest,const CThing>(const C2DBoxI&,const CThingFilter_IsOpinionSourceWithFreshDeed&,const CThingCompare_Nearest&,const CThing**) const;