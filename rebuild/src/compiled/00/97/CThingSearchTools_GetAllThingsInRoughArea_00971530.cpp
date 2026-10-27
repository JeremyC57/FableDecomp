// CThingSearchTools::GetAllThingsInRoughArea(const C2DBoxI& area, const CFilter_Bind2& filter, std::vector& result) -- collect the CThing things in the rough area that pass CThingFilter_CanBeUsedAsCover and CThingFilter_MakesLineWithPosInDirectionOfLine
// Leaf: ??$GetAllThingsInRoughArea@VCThing@@V?$vector@PAVCThing@@V?$allocator@PAVCThing@@@std@@@std@@V?$CFilter_Bind2@VCThingFilter_CanBeUsedAsCover@@VCThingFilter_MakesLineWithPosInDirectionOfLine@@@@@CThingSearchTools@@QBEXABVC2DBoxI@@ABV?$CFilter_Bind2@VCThingFilter_CanBeUsedAsCover@@VCThingFilter_MakesLineWithPosInDirectionOfLine@@@@AAV?$vector@PAVCThing@@V?$allocator@PAVCThing@@@std@@@std@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00971530, 602 bytes, thiscall, `ret 0xc`.
// Identity (proven): GetAllThingsInRoughArea<CThing, std::vector<CThing*>, CFilter_Bind2<CThingFilter_CanBeUsedAsCover, CThingFilter_MakesLineWithPosInDirectionOfLine> >: the template arguments are the filter callees in its own bytes (CThingFilter_CanBeUsedAsCover 0x006A7AB0 then CThingFilter_MakesLineWithPosInDirectionOfLine 0x006A7AD0, each `QBE_NABVCThing@@@Z`, and the vector growth callee 0x00436AC0 for CThing*); its caller is CAIStateGroup_CreatureUseCoverBase::GetCoverThingList 0x00970DF0 (call at 0x00970F14); the same instantiation is in the FableWin.pdb symbol set (ghidra_out/_fablewin_api.tsv) carries ??$GetAllThingsInRoughArea@VCThing@@.
// Callees: CThingSearchTools::GetAllThingMapsInArea 0x0049C6F0 and ClipAndConvertWorldMapAreaToLocalArea 0x0049C700
//   (Ego_r.pdb spellings, protected const); the two filter operator()s; std::vector<CThing*>::_M_insert_overflow 0x00436AC0;
//   memmove 0x00BFEAE6 (AreaMaps.clear()).
// Shape (family L060): twin of the landed GetAllThingsInRoughArea 0x00746560 (Bind3 over const CThing, 619 bytes): the
//   search stamp bump, the inlined protected GetMapwhoArea (layer shift from the layer table at 0x0137DAD8 + 4, stride 0x10),
//   the per-layer row offsets hoisted under the first row test, IsThingOfType<CThing> (mask 0, folds to `xor dl,dl; jne`),
//   the filter chain, the inline STLport push_back (null-tested placement copy, out-of-line overflow with __true_type).
// Layout: CThingSearchTools as engine/CThingSearchTools.h (0x18, retail STLport vector AreaMaps at +0xc), modelled locally
//   because the generated header carries no member functions and the member template must be declared in the class.
//   CThing has no shared header: only the retail type-flag word +0x6c (PDB +0x7c) is touched.
// Names: the layer table and the three search-stamp globals keep their landed descriptive spellings (variable names
//   unrecovered); the overflow member is STLport 4.6 protected (IAE) with the __true_type tag (pointer elements).
#include "engine/CTCMapwho.h"
#include "engine/CTCBase.h"
#include <stddef.h>
#include <new>
extern "C" void* __cdecl memmove(void*, const void*, size_t);   // 0x00BFEAE6
extern "C" void* __cdecl malloc(unsigned int);
extern "C" void __cdecl free(void*);
namespace std {
struct __true_type { __true_type() {} };
template <class _Tp> class allocator { };
__forceinline void* __copy_trivial(const void* __first, const void* __last, void* __result)
{
    return __last == __first ? __result
        : (char*)memmove(__result, __first, (const char*)__last - (const char*)__first) + ((const char*)__last - (const char*)__first);
}
template <class _T1, class _T2> __forceinline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _Tp, class _Alloc = allocator<_Tp> >
class vector {
public:
    typedef _Tp* iterator;
    typedef _Tp* pointer;
    typedef unsigned int size_type;
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
protected:
    void _M_insert_overflow(pointer __position, const _Tp& __x, const __true_type&, size_type __fill_len, bool __atend = false);
public:
    __forceinline iterator begin() const { return _M_start; }
    __forceinline iterator end() const { return _M_finish; }
    __forceinline _Tp& operator[](size_type __n) const { return _M_start[__n]; }
    __forceinline void push_back(const _Tp& __x)
    {
        if (this->_M_finish != this->_M_end_of_storage) {
            _Construct(this->_M_finish, __x);
            ++this->_M_finish;
        }
        else
            _M_insert_overflow(this->_M_finish, __x, __true_type(), 1UL, true);
    }
    __forceinline void clear()
    {
        this->_M_finish = (pointer)__copy_trivial(this->_M_finish, this->_M_finish, this->_M_start);
    }
};
}
class CThing;
// Same-TU body (not graded), an explicit specialization defined above its caller: the growth callee only READS its value
// through the const reference (a member-template body is instantiated at the end of the TU and gives the caller nothing).
#pragma auto_inline(off)
template <>
void std::vector<CThing*>::_M_insert_overflow(CThing** __position, CThing* const& __x, const std::__true_type&, unsigned int __fill_len, bool __atend)
{
    unsigned int __size = _M_finish - _M_start;
    unsigned int __len = __size + (__size < __fill_len ? __fill_len : __size);
    CThing** __new_start = __len ? (CThing**)malloc(__len * sizeof(CThing*)) : 0;
    CThing** __new_finish = __new_start;
    for (CThing** __p = _M_start; __p != __position; ++__p, ++__new_finish) new (__new_finish) CThing*(*__p);
    for (unsigned int __i = 0; __i < __fill_len; ++__i, ++__new_finish) new (__new_finish) CThing*(__x);
    if (!__atend) for (CThing** __p = __position; __p != _M_finish; ++__p, ++__new_finish) new (__new_finish) CThing*(*__p);
    if (_M_start) free(_M_start);
    _M_start = __new_start; _M_finish = __new_finish; _M_end_of_storage = __new_start + __len;
}
#pragma auto_inline(on)
class CThing { public:
    unsigned char _pad_0x00[0x6c];
    unsigned long ThingTypeFlags;                                  // +0x6c retail (PDB +0x7c)
    enum { ThingTypeMask = 0 };
};
template <class T> __forceinline bool IsThingOfType(const CThing* thing)
{
    unsigned long mask = T::ThingTypeMask;
    return (thing->ThingTypeFlags & mask) == mask;
}
class C3DVector { public: float X, Y, Z; };
class C2DBoxI { public:
    long TLX, TLY, BRX, BRY;
    C2DBoxI() {}
    C2DBoxI(const C2DBoxI& v) : TLX(v.TLX), TLY(v.TLY), BRX(v.BRX), BRY(v.BRY) {}
    __forceinline long Width() const { return BRX - TLX; }
    __forceinline long Height() const { return BRY - TLY; }
};
struct CMapwhoNode { CTCMapwho* PMapwho; CMapwhoNode* PNext; CMapwhoNode* PPrevious; CThingMapCell* PCell; };
struct CThingMapCell { CMapwhoNode* PFirstNode; };
struct CThingMapManager;
class CThingManager;
class CWorldMap;
namespace NThingMap {
class C2DExtentsI { public: long X, Y; };
class CMapwhoLevelInfo { public: long Size; C2DExtentsI Extents; long ScaleShift; long NoThings; };
class CThingMap { public:
    long _padding_;                                                // +0x00
    std::vector<CMapwhoLevelInfo> MapwhoLevelInfo;                 // +0x04
    CThingMapManager* Manager;                                     // +0x10
    std::vector<std::vector<CThingMapCell> > Cells;                // +0x14
    C2DBoxI Area;                                                  // +0x20
    long NoThings;                                                 // +0x30
};
extern long g_NoMapwhoLevels;                                      // 0x013BA848
}
// The table's real element type is NThingMap::CLayerInfo (Ego_r.pdb, 16 bytes); the landed spelling below keeps a global struct.
struct CLayerInfo { long Size; long ScaleShift; unsigned long UsedColour, UnusedColour; };
extern CLayerInfo LayerInfo_0137dad8[];                            // 0x0137DAD8, landed descriptive spelling
extern long SearchCount_013b89a8;                                  // 0x013B89A8
extern long SearchWork_013b89ac;                                   // 0x013B89AC
extern unsigned long SearchID_013b89b0;                            // 0x013B89B0
class CThingFilter_CanBeUsedAsCover { public:
    bool operator()(const CThing&) const;                          // 0x006A7AB0
};
class CThingFilter_MakesLineWithPosInDirectionOfLine { public:
    C3DVector Pos;                                                 // +0x00
    C3DVector Line;                                                // +0x0c
    bool operator()(const CThing&) const;                          // 0x006A7AD0
};
class CThingFilter_MakesLineWithPosInDirectionOfLine;
template <class TFilter1, class TFilter2> class CFilter_Bind2 { public:
    TFilter1* F1;                                                        // +0x00
    TFilter2* F2;                                                      // +0x04
    template <class T> __forceinline bool operator()(const T& p) const { return (*this->F1)(p) && (*this->F2)(p); }
};
class CThingSearchTools {
public:
    template <class T, class TContainer, class TFilter>
    void GetAllThingsInRoughArea(const C2DBoxI& area, const TFilter& filter, TContainer& result) const;
protected:
    void GetAllThingMapsInArea(const C2DBoxI& area, std::vector<NThingMap::CThingMap*>& maps) const;        // 0x0049C6F0
    C2DBoxI ClipAndConvertWorldMapAreaToLocalArea(const NThingMap::CThingMap& map, const C2DBoxI& area) const; // 0x0049C700
    __forceinline C2DBoxI GetMapwhoArea(const NThingMap::CThingMap&, long layer, const C2DBoxI& box) const
    {
        long shift = LayerInfo_0137dad8[layer].ScaleShift;
        C2DBoxI out;
        out.TLX = box.TLX >> shift; out.TLY = box.TLY >> shift;
        out.BRX = ((box.BRX - 1) >> shift) + 1; out.BRY = ((box.BRY - 1) >> shift) + 1;
        return out;
    }
public:
    CWorldMap* WorldMap;                                           // +0x00
    CThingMapManager* PThingMapManager;                            // +0x04
    CThingManager* ThingManager;                                   // +0x08
    mutable std::vector<NThingMap::CThingMap*> AreaMaps;           // +0x0c
};
template <class T, class TContainer, class TFilter>
void CThingSearchTools::GetAllThingsInRoughArea(const C2DBoxI& area, const TFilter& filter, TContainer& result) const
{
    GetAllThingMapsInArea(area, AreaMaps);
    ++SearchCount_013b89a8; SearchWork_013b89ac = 0; ++SearchID_013b89b0;
    for (std::vector<NThingMap::CThingMap*>::iterator m = AreaMaps.begin(); m != AreaMaps.end(); ++m) {
        NThingMap::CThingMap* map = *m;
        C2DBoxI bounds = ClipAndConvertWorldMapAreaToLocalArea(*map, area);
        for (long layer = 0; layer < NThingMap::g_NoMapwhoLevels; ++layer) {
            C2DBoxI scaled = GetMapwhoArea(*map, layer, bounds);
            if (scaled.Width() != 0 && scaled.Height() != 0) {
                long y = scaled.TLY;
                if (y < scaled.BRY) {
                    long offset = (unsigned char)layer * sizeof(NThingMap::CMapwhoLevelInfo);
                    long cellsOffset = (unsigned char)layer * sizeof(std::vector<CThingMapCell>);
                    for (; y < scaled.BRY; ++y) {
                        long rowWidth = ((NThingMap::CMapwhoLevelInfo*)((char*)map->MapwhoLevelInfo._M_start + offset))->Extents.X;
                        std::vector<CThingMapCell>& rowCells = *(std::vector<CThingMapCell>*)((char*)map->Cells._M_start + cellsOffset);
                        CThingMapCell* cell = &rowCells[y * rowWidth + scaled.TLX];
                        for (long x = scaled.TLX; x < scaled.BRX; ++x, ++cell) {
                            for (CMapwhoNode* node = cell->PFirstNode; node; node = node->PNext) {
                                CTCMapwho* who = node->PMapwho;
                                if (who->SearchID < SearchID_013b89b0) {
                                    who->SearchID = SearchID_013b89b0;
                                    CThing* thing = ((CTCBase*)who)->ParentThing;
                                    if (IsThingOfType<CThing>(thing)) {
                                        CThing* typed = thing;
                                        if (filter(*typed)) result.push_back(typed);
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    AreaMaps.clear();
}
template void CThingSearchTools::GetAllThingsInRoughArea<CThing, std::vector<CThing*>, CFilter_Bind2<CThingFilter_CanBeUsedAsCover, CThingFilter_MakesLineWithPosInDirectionOfLine> >(
    const C2DBoxI&, const CFilter_Bind2<CThingFilter_CanBeUsedAsCover, CThingFilter_MakesLineWithPosInDirectionOfLine>&, std::vector<CThing*>&) const;