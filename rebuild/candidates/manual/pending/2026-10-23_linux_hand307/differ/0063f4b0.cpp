// Grade: DIFFER(769v769) -- with the 007FCEF0 indexing recipe applied (no change in length), the only residue is one swapped spill-reload pair at the map-loop exit (edi before esi; retail esi first); same lane gap as 00518130
// CThingSearchTools::ForEachInRoughArea<CThingPhysical, CFilter_Bind2<CThingFilter_HasNavigationInformation,
// CThingFilter_OverlappingBoundingBox>, CThingFindNavigationLayerAt>(const C2DBoxI& area, const filter&, functor&) -- runs
// the navigation-layer finder over every physical thing in the overlapping mapwho cells that has navigation information
// and whose physics mesh overlaps the filter's box (each thing once per search); true when any thing was visited
// Retail: Fable.exe (TLC, VC7.1) @ 0x0063F4B0, `ret 0xc`.
// Callees: as 0x00518130 (GetMapsInArea forwarder 0x0049C6F0, GetCellExtents 0x0049C700, memmove 0x00BFEAE6), plus the
//   thing's virtual +0xdc (has navigation information), the physics component's virtual +0xa8 (has mesh object) and +0x174
//   (physics mesh), CTCPhysicsBase::GetPhysicsMeshObject 0x006B0850, CPhysicsMeshObject::OverlapsBox 0x00A12890 /
//   GetNavigationLevelAt 0x00A11D10 / ~CPhysicsMeshObject 0x00A127D0. Filters and finder inline (the landed
//   CThingFindNavigationLayerAt::operator() 0x0063EFB0 body).
#include <string.h>
struct C2DBoxI {
    long X0, Y0, X1, Y1;
    C2DBoxI() {}
    C2DBoxI(long x0, long y0, long x1, long y1) : X0(x0), Y0(y0), X1(x1), Y1(y1) {}
};
struct C3DVector { float X, Y, Z; };
struct CAABox;
struct CPhysicsMeshObject {
    unsigned char _body[0x40];
    ~CPhysicsMeshObject();                                 // 0x00A127D0
    bool OverlapsBox(const CAABox& box);                   // 0x00A12890
    long GetNavigationLevelAt(const C3DVector& pos);       // 0x00A11D10
};
struct CTCPhysicsBase {
#define VF(n) virtual void v##n();
#define VF10(p) VF(p##0) VF(p##1) VF(p##2) VF(p##3) VF(p##4) VF(p##5) VF(p##6) VF(p##7) VF(p##8) VF(p##9)
    VF10(0) VF10(1) VF10(2) VF10(3) VF(40) VF(41)
    virtual bool HasPhysicsMeshObject();                   // slot 42 (+0xa8)
    VF(43) VF(44) VF(45) VF(46) VF(47) VF(48) VF(49) VF10(5) VF10(6) VF10(7) VF10(8) VF(90) VF(91) VF(92)
    virtual void* GetPhysicsMesh();                        // slot 93 (+0x174)
    CPhysicsMeshObject GetPhysicsMeshObject();             // 0x006B0850
};
struct CThing {
    enum { TypeMask = 0 };
    VF10(0) VF10(1) VF10(2) VF10(3) VF10(4) VF(50) VF(51) VF(52) VF(53) VF(54)
    virtual bool HasNavigationInformation();               // slot 55 (+0xdc)
    unsigned char   _pad04[0x60 - 0x04];
    CTCPhysicsBase* PhysicsTC;                             // +0x60
    unsigned char   _pad64[0x6c - 0x64];
    unsigned long   ThingTypeFlags;                        // +0x6c
};
struct CThingPhysical : CThing { enum { TypeMask = 3 }; };
struct CThingFilter_HasNavigationInformation {
    bool operator()(CThing* thing) const
    {
        CTCPhysicsBase* physics = thing->PhysicsTC;
        if (physics && physics->HasPhysicsMeshObject())
            return thing->HasNavigationInformation();
        return thing->HasNavigationInformation();
    }
};
struct CThingFilter_OverlappingBoundingBox {
    const CAABox* Box;
    bool operator()(CThing* thing) const
    {
        const CAABox* box = Box;
        CTCPhysicsBase* physics = thing->PhysicsTC;
        if (physics->GetPhysicsMesh()) {
            CPhysicsMeshObject mesh = physics->GetPhysicsMeshObject();
            return mesh.OverlapsBox(*box);
        }
        return false;
    }
};
template<class F1, class F2> struct CFilter_Bind2 {
    F1 First;
    F2 Second;
    bool operator()(CThing* thing) const { return First(thing) && Second(thing); }
};
struct CThingFindNavigationLayerAt {
    C3DVector Pos;                                         // +0x00
    long      Layer;                                       // +0x0c
    bool operator()(CThing* thing)
    {
        CTCPhysicsBase* physics = thing->PhysicsTC;
        if (physics && physics->HasPhysicsMeshObject()) {
            CPhysicsMeshObject mesh = physics->GetPhysicsMeshObject();
            long level = mesh.GetNavigationLevelAt(Pos);
            if (level > Layer)
                Layer = level;
        }
        return true;
    }
};
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
    CMapVector* maps = &TempMaps;
    GetMapsInArea(area, maps);
    ++g_SearchStamps.Searches;
    g_SearchStamps.Found = 0;
    ++g_SearchStamps.Stamp;
    for (CThingMap** it = maps->_M_start; it != TempMaps._M_finish; ++it) {
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
                            if ((thing->ThingTypeFlags & T::TypeMask) == T::TypeMask && filter(thing) && func(thing))
                                result = true;
                        }
                    }
                }
            }
            }
        }
    }
    maps->clear();
    return result;
}
template bool CThingSearchTools::ForEachInRoughArea<CThingPhysical,
    CFilter_Bind2<CThingFilter_HasNavigationInformation, CThingFilter_OverlappingBoundingBox>, CThingFindNavigationLayerAt>(
    const C2DBoxI& area, const CFilter_Bind2<CThingFilter_HasNavigationInformation, CThingFilter_OverlappingBoundingBox>& filter,
    CThingFindNavigationLayerAt& func);
