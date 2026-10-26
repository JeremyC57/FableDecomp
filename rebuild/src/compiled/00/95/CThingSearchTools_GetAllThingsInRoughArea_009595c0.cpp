// CThingSearchTools::GetAllThingsInRoughArea(const C2DBoxI& area, const TFilter& filter, TContainer& things) -- append every thing of type TThing in the mapwho cells under the area that passes the filter to the result vector, each thing visited once per search stamp; instance <const CThingDeadCreature, std::vector<const CThingDeadCreature*>, CThingFilter_None>
// Leaf: GetAllThingsInRoughArea
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009595C0, length 573 bytes (`ret 0xc`).
// Identity (sweep3): Ego_r/FableWin name this instance ??$GetAllThingsInRoughArea@$$CBVCThingDeadCreature@@V?$vector@PBVCThingDeadCreature@@...VCThingFilter_None@@@CThingSearchTools@@QBEX... (the only DeadCreature vector in the name sets is vector<const CThingDeadCreature*>).
//   The landed name FindThingsInRoughArea has no PDB match: CThingSearchTools' rough-area collector of things is
//   GetAllThingsInRoughArea (ForEachInRoughArea applies a functor, GetAllTCsInRoughArea / GetAllTCDsInRoughArea collect components).
//   The catalog label "GetPBestThingInRoughArea<CThingFilter_IsPotentialCustomer,CThingCompare_Nearest,CThingCreatureBase>" was propagated across byte-identical instances.
// Callees: GetMapsInArea 0x0049C6F0 (forwarder), GetCellExtents 0x0049C700 (C2DBoxI by value), memmove 0x00BFEAE6 (scratch-list clear), vector<T*>::_M_insert_overflow 0x00959260 (push_back growth).
// Globals: search stamps 0x013B89A8 (+0 searches, +4 found, +8 stamp), mapwho level count 0x013BA848, level shift table 0x0137DADC (16-byte stride).
// Layout: engine/CThingSearchTools.h gives AreaMaps (+0x0c) the PDB's 16-byte vector; the retail STLport vector is 12 bytes, so the
//   search tools, the thing map (+0x04 level infos, +0x14 cell vectors) and the cell entries are modelled locally.
// Shape: the type test copies the mask into a local (`(flags & mask) == mask`); inline filters are written out in the TU.
//   The y loop is `long y = box.Y0; if (y < box.Y1) { offset; cellsOffset; for (; y < box.Y1; ++y) ... }` with the two user
//   byte-offset variables declared in that order: VC7.1 then ranks them as the SIB base over its own hoisted temps,
//   giving retail's `[hoisted + LevelInfo + 4]` Width load and cell-row load (the family recipe from 007FCEF0).
//   The overflow is a same-TU stand-in under auto_inline(off) so the __true_type tag and the cell pointer take retail's argument slots.

#include <string.h>
#include <stdlib.h>
struct C2DBoxI {
    long X0, Y0, X1, Y1;
    C2DBoxI() {}
    C2DBoxI(long x0, long y0, long x1, long y1) : X0(x0), Y0(y0), X1(x1), Y1(y1) {}
};
// Retail CThing: ThingTypeFlags at +0x6c (THING_TYPE_* inheritance bits); TypeMask selects the searched class.
class CThing {                                        // PDB def type: class
public:
    enum { TypeMask = 0 };
    unsigned char _pad00[0x60];
    struct CTCPhysicsBase* PhysicsTC;                      // +0x60
    unsigned char _pad64[0x6c - 0x64];
    unsigned long ThingTypeFlags;                          // +0x6c
};
// The type test: the mask is copied into a local, so the CThing (mask 0) test folds late and keeps retail's
// `xor dl,dl ; jne`, and a byte-sized mask compares through the low byte (`and edx,0Fh ; cmp dl,0Fh`).
template <class T> inline bool IsThingOfType(const CThing* thing)
{
    unsigned long mask = T::TypeMask;
    return (thing->ThingTypeFlags & mask) == mask;
}
inline void* operator new(unsigned int, void* where) { return where; }
namespace std { struct __true_type { __true_type() {} }; }
using std::__true_type;
class CThingDeadCreature : public CThing { public: enum { TypeMask = 0x87 }; };
class CThingFilter_None {
public:
    bool operator()(const CThing&) const { return true; }
};
// One mapwho cell list node: {entry, next}; the entry holds the thing (+4) and its search stamp (+0x4c).
struct CThingMapEntry {
    unsigned char _pad00[4];
    CThing*       Thing;                                   // +0x04
    unsigned char _pad08[0x4c - 0x08];
    unsigned long SearchStamp;                             // +0x4c
};
struct CThingMapNode {
    CThingMapEntry* Entry;
    CThingMapNode*  Next;
};
// A thing map: per-level grid infos (20 bytes, width at +4) at +0x04, per-level cell vectors at +0x14.
struct CMapwhoLevelInfo { long SingleCell; long Width; long Height; long _pad[2]; };
struct CCellVector { CThingMapNode** _M_start; CThingMapNode** _M_finish; CThingMapNode** _M_end_of_storage; };
struct CThingMap {
    void*             _vfptr;
    CMapwhoLevelInfo* LevelInfo;                           // +0x04
    CMapwhoLevelInfo* LevelInfoEnd;
    CMapwhoLevelInfo* LevelInfoCap;
    void*             _pad10;
    CCellVector*      Cells;                               // +0x14
    // offset = level * sizeof(CMapwhoLevelInfo), cellsOffset = level * sizeof(CCellVector): the caller's user variables
    // (Width first by offset, the cell row first by the Cells pointer) are what give retail's SIB base/index order.
    CThingMapNode** GetCellPtr(long cellsOffset, long offset, long x, long y) const
    {
        return &(*(const CCellVector*)((unsigned long)Cells + cellsOffset))._M_start[y * *(const long*)((unsigned long)offset + (unsigned long)LevelInfo + 4) + x];
    }
};
struct SMapwhoLevel { long Shift; long _pad[3]; };
extern SMapwhoLevel g_MapwhoLevels[];                      // 0x0137DADC
extern long g_NumMapwhoLevels;                             // 0x013BA848
struct SSearchStamps { unsigned long Searches; unsigned long Found; unsigned long Stamp; };
extern SSearchStamps g_SearchStamps;                       // 0x013B89A8
// STLport vector<CThingMap*> (the search tools' scratch map list); clear() is erase(begin, end) over memmove.
struct CMapVector {
    CThingMap** _M_start;
    CThingMap** _M_finish;
    CThingMap** _M_end_of_storage;
    static CThingMap** CopyDown(CThingMap** src, CThingMap** srcEnd, CThingMap** dest)
    {
        if (src == srcEnd)
            return dest;
        unsigned long count = (const char*)srcEnd - (const char*)src;
        return (CThingMap**)((char*)memmove(dest, src, count) + count);
    }
    void erase(CThingMap** first, CThingMap** last) { _M_finish = CopyDown(last, _M_finish, first); }
    void clear() { erase(_M_start, _M_finish); }
};
// The result: STLport 4.6 std::vector<const CThingDeadCreature*> (_Vector_base + _STLP_alloc_proxy); push_back's growth path is the
// protected POD `_M_insert_overflow`, defined in the class body in STLport and kept out of line by VC7.1 (0x00959260). Its
// verbatim body sits in this TU under auto_inline(off) + noinline, so VC7.1 sees that the __true_type tag is never read and
// homes the tag and the pushed value in the dead argument slot as retail does.
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
    _Tp* _M_start;                                          // +0x00
    _Tp* _M_finish;                                         // +0x04
    _STLP_alloc_proxy<_Tp*, _Alloc> _M_end_of_storage;      // +0x08
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp* pointer;
    typedef unsigned int size_type;
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
                            bool __atend = false);   // 0x00959260 for vector<const CThingDeadCreature*>
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
    // handle insertion
    __new_finish = fill_n(__new_finish, __fill_len, __x);
    if (!__atend)
        // copy remainder
        __new_finish = (pointer)__copy_trivial(__position, this->_M_finish, __new_finish);
    _M_clear();
    _M_set(__new_start, __new_finish, __new_start + __len);
}
#pragma auto_inline(on)
}
// CThingSearchTools: engine/CThingSearchTools.h names +0x0c "AreaMaps" (vector<NThingMap::CThingMap*>, 16 bytes in
// the PDB); retail's STLport vector is 12 bytes and only the scratch-vector role matters here, so a local model.
struct CThingSearchTools {
    void*      WorldMap;                                   // +0x00
    void*      PThingMapManager;                           // +0x04
    void*      ThingManager;                               // +0x08
    CMapVector AreaMaps;                                   // +0x0c
    void GetMapsInArea(const C2DBoxI& area, CMapVector* maps);              // 0x0049C6F0
    C2DBoxI GetCellExtents(CThingMap* map, const C2DBoxI& area);            // 0x0049C700
    template <class TThing, class TContainer, class TFilter>
    void GetAllThingsInRoughArea(const C2DBoxI& area, const TFilter& filter, TContainer& things);
};
template <class TThing, class TContainer, class TFilter>
void CThingSearchTools::GetAllThingsInRoughArea(const C2DBoxI& area, const TFilter& filter, TContainer& things)
{
    CMapVector* maps = &AreaMaps;
    GetMapsInArea(area, maps);
    ++g_SearchStamps.Searches;
    g_SearchStamps.Found = 0;
    ++g_SearchStamps.Stamp;
    for (CThingMap** it = maps->_M_start; it != AreaMaps._M_finish; ++it) {
        CThingMap* map = *it;
        C2DBoxI extents = GetCellExtents(map, area);
        for (long level = 0; level < g_NumMapwhoLevels; ++level) {
            long shift = g_MapwhoLevels[level].Shift;
            C2DBoxI box(extents.X0 >> shift, extents.Y0 >> shift, ((extents.X1 - 1) >> shift) + 1,
                        ((extents.Y1 - 1) >> shift) + 1);
            long width = box.X1 - box.X0;
            if (width == 0 || box.Y1 - box.Y0 == 0)
                continue;
            long y = box.Y0;
            if (y < box.Y1) {
                long offset = (unsigned char)level * (long)sizeof(CMapwhoLevelInfo);
                long cellsOffset = (unsigned char)level * (long)sizeof(CCellVector);
                for (; y < box.Y1; ++y) {
                    CThingMapNode** cell = map->GetCellPtr(cellsOffset, offset, box.X0, y);
                    for (long x = box.X0; x < box.X1; ++x, ++cell) {
                        for (CThingMapNode* node = *cell; node; node = node->Next) {
                            CThingMapEntry* entry = node->Entry;
                            if (entry->SearchStamp < g_SearchStamps.Stamp) {
                                entry->SearchStamp = g_SearchStamps.Stamp;
                                CThing* thing = entry->Thing;
                                if (IsThingOfType<TThing>(thing) && filter(*static_cast<TThing*>(thing))) {
                                TThing* found = static_cast<TThing*>(thing);
                                things.push_back(found);
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
template void CThingSearchTools::GetAllThingsInRoughArea<const CThingDeadCreature, std::vector<const CThingDeadCreature*>, CThingFilter_None >(const C2DBoxI& area, const CThingFilter_None& filter, std::vector<const CThingDeadCreature*>& things);
