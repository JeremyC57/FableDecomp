// CThingSearchTools::ForEachInRoughArea(const C2DBoxI& area, const CFilter_Bind3<SolidToThing, PhysicsFlagsSolidToThing, OverlappingCircle>& filter, CThingFindHeightZBelowAt& func) -- runs the height-below functor on every physical thing linked into a mapwho cell overlapping the area (each thing once per search) that is not the source thing, collides with things like the source physics, and whose footprint circle overlaps the filter circle; true when any call returned true
// Grade: DIFFER(725v725) -- register order only: the level-width load is [ebx+eax+4] in retail (offset as SIB base), [eax+ebx+4] in ours, and at the node- and level-loop exits ours reloads the edi spill first (retail: edx, esi, edi, ebp and esi, edi)
// Leaf: ??$ForEachInRoughArea@
// Retail: Fable.exe (TLC, VC7.1) @ 0x0066A1E0, 725 bytes (incl. padding), `__thiscall`, `ret 0xc`; one caller 0x00666E8B.
// Callees: GetMapsInArea 0x0049C6F0, GetCellExtents 0x0049C700, CTCPhysicsBase virtual GetCollidesWithThings (+0xe4) /
//   GetCollidesWithThingsOneWay (+0xe8), CTCPhysicsBase::GetMapwhoPosition 0x006AFCB0 (C3DVector by hidden pointer),
//   (anonymous)::CThingFindHeightZBelowAt::operator() 0x00668AB0 (landed), memmove 0x00BFEAE6 (the scratch clear()).
// Shape: the landed ForEachInRoughArea 0x00518130 template with the filter and the functor by reference and the three
//   Bind3 operators of the landed 0x00669040 (same TU) inlined; the circle's squared distance is one expression here
//   (retail squares dy first, unlike the two-statement out-of-line copy).
#include <string.h>
struct C2DBoxI {
    long X0, Y0, X1, Y1;
    C2DBoxI() {}
    C2DBoxI(long x0, long y0, long x1, long y1) : X0(x0), Y0(y0), X1(x1), Y1(y1) {}
};
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
};
#define FILL10(n) virtual void v##n##0(); virtual void v##n##1(); virtual void v##n##2(); virtual void v##n##3(); \
    virtual void v##n##4(); virtual void v##n##5(); virtual void v##n##6(); virtual void v##n##7(); \
    virtual void v##n##8(); virtual void v##n##9();
class CTCPhysicsBase {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    FILL10(1) FILL10(2) FILL10(3) FILL10(4)
    virtual void v50(); virtual void v51(); virtual void v52(); virtual void v53(); virtual void v54();
    virtual void v55(); virtual void v56();
    virtual bool GetCollidesWithThings() const;         // slot 57 (+0xe4)
    virtual bool GetCollidesWithThingsOneWay() const;   // slot 58 (+0xe8)
    unsigned char _pad04[0x34 - 0x04];
    float         Radius;                               // +0x34
    C3DVector GetMapwhoPosition() const;                // 0x006AFCB0
};
struct CThing {
    unsigned char   _pad00[0x60];
    CTCPhysicsBase* PhysicsTC;                             // +0x60
    unsigned char   _pad64[0x6c - 0x64];
    unsigned long   ThingTypeFlags;                        // +0x6c
};
struct CThingPhysical : CThing {};
class CTCSolidToThing;
class CThingFilter_SolidToThing {
public:
    CThing*          SourceThing;               // +0x00
    CTCSolidToThing* PTCSolidToThing;           // +0x04
    __forceinline bool operator()(const CThing& thing) const
    {
        return &thing != SourceThing;
    }
};
class CThingFilter_PhysicsFlagsSolidToThing {
public:
    CThing*         SourceThing;                // +0x00
    CTCPhysicsBase* SourceThingPhysicsTC;       // +0x04
    __forceinline bool operator()(const CThing& thing) const
    {
        CTCPhysicsBase* physics = thing.PhysicsTC;
        if (!SourceThingPhysicsTC || !physics)
            return false;
        if (!(SourceThingPhysicsTC->GetCollidesWithThings() || SourceThingPhysicsTC->GetCollidesWithThingsOneWay()))
            return false;
        return physics->GetCollidesWithThings();
    }
};
class CThingFilter_OverlappingCircle {
public:
    C3DVector SpherePos;                        // +0x00
    float     Radius;                           // +0x0c
    __forceinline bool Overlaps(const C3DVector& pos, const CTCPhysicsBase* physics) const
    {
        float dx = pos.X - SpherePos.X;
        float dy = pos.Y - SpherePos.Y;
        float r = physics->Radius + Radius;
        if (dx * dx + dy * dy < r * r)
            return true;
        return false;
    }
    __forceinline bool operator()(const CThing& thing) const
    {
        CTCPhysicsBase* physics = thing.PhysicsTC;
        if (physics)
            return Overlaps(physics->GetMapwhoPosition(), physics);
        return false;
    }
};
template <class A, class B, class C> class CFilter_Bind3 {
public:
    const A* F1;                                // +0x00
    const B* F2;                                // +0x04
    const C* F3;                                // +0x08
    template <class T> __forceinline bool operator()(const T& thing) const
    {
        return (*F1)(thing) && (*F2)(thing) && (*F3)(thing);
    }
};
namespace {
struct CThingFindHeightZBelowAt {
    C3DVector* From;                            // +0x00
    C3DVector* To;                              // +0x04
    float      Height;                          // +0x08
    bool operator()(CThing* thing);             // 0x00668AB0
};
}
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
struct CMapwhoLevelInfo { long SingleCell; long Width; long Height; long _pad[2]; };
struct CCellVector { CThingMapNode** _M_start; CThingMapNode** _M_finish; CThingMapNode** _M_end_of_storage; };
struct CThingMap {
    void*             _vfptr;
    CMapwhoLevelInfo* LevelInfo;                           // +0x04
    CMapwhoLevelInfo* LevelInfoEnd;
    CMapwhoLevelInfo* LevelInfoCap;
    void*             _pad10;
    CCellVector*      Cells;                               // +0x14
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
struct CThingSearchTools {
    void*      _pad00;
    void*      Manager;                                    // +0x04
    void*      _pad08;
    CMapVector TempMaps;                                   // +0x0c
    void GetMapsInArea(const C2DBoxI& area, CMapVector* maps);              // 0x0049C6F0
    C2DBoxI GetCellExtents(CThingMap* map, const C2DBoxI& area);            // 0x0049C700
    template<class T, class F, class G> bool ForEachInRoughArea(const C2DBoxI& area, const F& filter, G& func);
};
template<class T, class F, class G> bool CThingSearchTools::ForEachInRoughArea(const C2DBoxI& area, const F& filter, G& func)
{
    bool result = false;
    GetMapsInArea(area, &TempMaps);
    ++g_SearchStamps.Searches;
    g_SearchStamps.Found = 0;
    ++g_SearchStamps.Stamp;
    for (CThingMap** it = TempMaps._M_start; it != TempMaps._M_finish; ++it) {
        CThingMap* map = *it;
        C2DBoxI extents = GetCellExtents(map, area);
        for (long level = 0; level < g_NumMapwhoLevels; ++level) {
            long shift = g_MapwhoLevels[level].Shift;
            C2DBoxI box(extents.X0 >> shift, extents.Y0 >> shift, ((extents.X1 - 1) >> shift) + 1,
                        ((extents.Y1 - 1) >> shift) + 1);
            if (box.X0 == box.X1 || box.Y0 == box.Y1)
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
                            if ((thing->ThingTypeFlags & 3) == 3 && filter(*(T*)thing) && func((T*)thing))
                                result = true;
                        }
                    }
                }
            }
            }
        }
    }
    TempMaps.clear();
    return result;
}
template bool CThingSearchTools::ForEachInRoughArea<CThingPhysical, CFilter_Bind3<CThingFilter_SolidToThing,
    CThingFilter_PhysicsFlagsSolidToThing, CThingFilter_OverlappingCircle>, CThingFindHeightZBelowAt>(const C2DBoxI& area,
    const CFilter_Bind3<CThingFilter_SolidToThing, CThingFilter_PhysicsFlagsSolidToThing, CThingFilter_OverlappingCircle>& filter,
    CThingFindHeightZBelowAt& func);
