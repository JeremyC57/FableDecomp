#include <stdio.h>
#include <stdlib.h>
#define FABLE_RETAIL_STLP_VECTOR
// 00637fc0 NThingMap::CThingMapManager::PlaceThing
// Leaf: ?PlaceThing@CThingMapManager@NThingMap@@QAEXAAVCTCMapwho@@ABVC3DVector@@@Z
// Flags:
#define FABLE_RETAIL_CTHINGMAPMANAGER_PLACE_PASS27
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTCMAPWHO
#define FABLE_RETAIL_CTCMAPWHO_PASS22_SHARED_NODE
#define FABLE_RETAIL_CTCMAPWHO_SETINMAP
#define FABLE_RETAIL_MAP_VALIDATE
#define FABLE_RETAIL_CTHINGMAPMANAGER
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_TO2D
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_ASSERT_DEAD_ARM
#define FABLE_RETAIL_CTHINGMAP_LAYERINFO
#define FABLE_RETAIL_C2DCOORDI_TOFLOAT
#define FABLE_RETAIL_C2DCOORDI_NOTEQUAL
#define FABLE_RETAIL_C2DCOORDI_ORIGINAL_COMPARE_PASS27
#include "fable_assert.h"
#include "engine/CThing.h"
#include "engine/CTCMapwho.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CThingMapManager.h"

inline const C2DBoxI& NThingMap::CThingMap::GetArea() const { return Area; }

namespace NThingMap {

void CThingMapManager::PlaceThing(CTCMapwho& mapwho,const C3DVector& pos)
{
    CThing& thing = mapwho.GetParentThing();
    if (!thing.IsLocal()) {
        FABLE_RETAIL_ASSERT(!mapwho.IsInMap());
        CTCPhysicsBase* ptc_physics = thing.GetPhysicsTC();
        float radius = ptc_physics->GetRadius();
        long mapwho_level = GetThingMapwhoLevel(radius);
        CMapCellPosInfoGroup mapwho_pos;
        GetThingLayerCellPositions(radius,pos.ToC2DVector(),mapwho_level,mapwho_pos);
        GetCellsForThingLayerCellPositions(mapwho_pos);
        if (mapwho_pos.TLPos.PCell)
            AddMapwhoNodeToCell(*mapwho_pos.TLPos.PCell,mapwho.GetTLNode());
        if (mapwho_pos.TRPos.PCell)
            AddMapwhoNodeToCell(*mapwho_pos.TRPos.PCell,mapwho.GetTRNode());
        if (mapwho_pos.BRPos.PCell)
            AddMapwhoNodeToCell(*mapwho_pos.BRPos.PCell,mapwho.GetBRNode());
        if (mapwho_pos.BLPos.PCell)
            AddMapwhoNodeToCell(*mapwho_pos.BLPos.PCell,mapwho.GetBLNode());
        mapwho.SetInMap(true);
        ++NoThings;
    }
}

void CThingMapManager::GetThingLayerCellPositions(float radius, const C2DVector& in_pos, long level, CMapCellPosInfoGroup& cells) const
{
    long scale_shift = LayerInfo[level].ScaleShift;
    cells.Layer = level;
    C2DCoordI tlpos((long)(in_pos.X-radius),(long)(in_pos.Y-radius));
    C2DCoordI brpos((long)(radius+in_pos.X),(long)(radius+in_pos.Y));
    C2DCoordI blpos(tlpos.X,brpos.Y);
    C2DCoordI trpos(brpos.X,tlpos.Y);
    InitPos(tlpos,cells.TLPos,level);
    InitPos(trpos,cells.TRPos,level);
    InitPos(blpos,cells.BLPos,level);
    InitPos(brpos,cells.BRPos,level);
}

void CThingMapManager::GetCellsForThingLayerCellPositions(CMapCellPosInfoGroup& cells)
{
    cells.TLPos.PCell = 0;
    if (cells.TLPos.PMapInfo)
        cells.TLPos.PCell = &GetCell(cells.TLPos.PMapInfo,cells.Layer,cells.TLPos.LocalPos);
    cells.TRPos.PCell = 0;
    if (cells.TRPos.PMapInfo && (cells.TRPos.PMapInfo != cells.TLPos.PMapInfo || cells.TRPos.LocalPos != cells.TLPos.LocalPos))
        cells.TRPos.PCell = &GetCell(cells.TRPos.PMapInfo,cells.Layer,cells.TRPos.LocalPos);
    cells.BLPos.PCell = 0;
    if (cells.BLPos.PMapInfo && (cells.BLPos.PMapInfo != cells.TLPos.PMapInfo || cells.BLPos.LocalPos != cells.TLPos.LocalPos))
        cells.BLPos.PCell = &GetCell(cells.BLPos.PMapInfo,cells.Layer,cells.BLPos.LocalPos);
    cells.BRPos.PCell = 0;
    if (cells.BRPos.PMapInfo && (cells.BRPos.PMapInfo != cells.BLPos.PMapInfo || cells.BRPos.LocalPos != cells.BLPos.LocalPos) && (cells.BRPos.PMapInfo != cells.TRPos.PMapInfo || cells.BRPos.LocalPos != cells.TRPos.LocalPos))
        cells.BRPos.PCell = &GetCell(cells.BRPos.PMapInfo,cells.Layer,cells.BRPos.LocalPos);
}
inline CThingMapCell& CThingMapManager::GetCell(const CThingMapInfo* pmap, long layer, const C2DCoordI& pos)
{
    FABLE_RETAIL_ASSERT(pmap->RefCount);
    return pmap->PThingMap->GetCell(static_cast<unsigned char>(layer), pos.X, pos.Y);
}

inline const CThingMapManager::CThingMapInfo* CThingMapManager::PeekMapInfoAt(const C2DCoordI& pos) const
{
    return &ThingMaps[pos.X / 128 + pos.Y / 128 * Dimensions.X];
}

void CThingMapManager::InitPos(const C2DCoordI& pos, CMapCellPosInfo& pos_info, long layer) const
{
    long scale_shift = LayerInfo[layer].ScaleShift;
    pos_info.PMapInfo = PeekMapInfoAt(pos);
    if (scale_shift == -1) {
        if (pos_info.PMapInfo && pos_info.PMapInfo->RefCount > 0) {
            pos_info.LocalPos.X = 0;
            pos_info.LocalPos.Y = 0;
        } else {
            pos_info.PMapInfo = 0;
        }
    } else if (pos_info.PMapInfo && pos_info.PMapInfo->RefCount > 0) {
        const C2DBoxI& map_area = pos_info.PMapInfo->PThingMap->GetArea();
        pos_info.LocalPos.X = (pos.X - map_area.TLX) >> scale_shift;
        pos_info.LocalPos.Y = (pos.Y - map_area.TLY) >> scale_shift;
        FABLE_RETAIL_ASSERT(pos_info.LocalPos.X >= 0 && pos_info.LocalPos.X < pos_info.PMapInfo->PThingMap->GetLayerExtents(layer).X);
        FABLE_RETAIL_ASSERT(pos_info.LocalPos.Y >= 0 && pos_info.LocalPos.Y < pos_info.PMapInfo->PThingMap->GetLayerExtents(layer).Y);
    } else {
        pos_info.PMapInfo = 0;
    }
}

}


CBaseClass::CBaseClass() {}
CBaseClass::~CBaseClass() {}
CBaseClassNonCopyable::CBaseClassNonCopyable() {}
CBaseClassNonCopyable::~CBaseClassNonCopyable() {}

bool CThing::IsLocal() const { abort(); return false; }
bool __fastcall GFAssertFailed(const char* expression, const char*, const char*, unsigned long)
{
    fputs(expression, stderr);
    abort();
    return false;
}

namespace NThingMap {
unsigned char CThingMapManager::GetThingMapwhoLevel(float) const { abort(); return 0; }

const CLayerInfo LayerInfo[] = {
    {2, 1, CRGBColour(), CRGBColour()},
    {4, 2, CRGBColour(), CRGBColour()},
    {8, 3, CRGBColour(), CRGBColour()},
    {32, 5, CRGBColour(), CRGBColour()},
    {64, 6, CRGBColour(), CRGBColour()},
    {-1, -1, CRGBColour(), CRGBColour()}
};

CMapwhoLevelInfo::CMapwhoLevelInfo() : Size(0), Extents(0, 0), ScaleShift(0), NoThings(0) {}
CMapwhoLevelInfo::CMapwhoLevelInfo(const CMapwhoLevelInfo& other)
    : Size(other.Size), Extents(other.Extents), ScaleShift(other.ScaleShift), NoThings(other.NoThings) {}
CThingMapCell::CThingMapCell() : PFirstNode(0) {}

CThingMap::CThingMap(CThingMapManager& manager, const C2DBoxI& area)
    : Manager(manager), Area(area), NoThings(0) {}
CThingMap::~CThingMap() {}
const C2DExtentsI& CThingMap::GetLayerExtents(long layer) const { return MapwhoLevelInfo[layer].Extents; }

class TestMap : public CThingMap {
public:
    TestMap(CThingMapManager& manager, long x, long y)
        : CThingMap(manager, C2DBoxI(C2DCoordI(x + 64, y + 64), 64))
    {
        MapwhoLevelInfo.resize(6);
        Cells.resize(6);
        for (unsigned i = 0; i < 6; ++i) {
            long width = LayerInfo[i].ScaleShift < 0 ? 1 : 128 >> LayerInfo[i].ScaleShift;
            MapwhoLevelInfo[i].Extents = C2DExtentsI(width, width);
            Cells[i].resize(width * width);
        }
    }
};

class TestManager : public CThingMapManager {
public:
    TestManager()
    {
        Dimensions = C2DExtentsI(2, 2);
        ThingMaps.resize(4);
    }
    void SetMap(unsigned i, TestMap& map, long references)
    {
        ThingMaps[i].PThingMap = &map;
        ThingMaps[i].RefCount = references;
    }
    void Locate(const C2DVector& pos, float radius, long level, CMapCellPosInfoGroup& cells)
    {
        GetThingLayerCellPositions(radius, pos, level, cells);
        GetCellsForThingLayerCellPositions(cells);
    }
};
}

static void Check(bool condition)
{
    if (!condition)
        abort();
}

static int RunActualCellLookupCases()
{
    using namespace NThingMap;
    TestManager manager;
    TestMap topLeft(manager, 0, 0);
    TestMap topRight(manager, 128, 0);
    TestMap bottomLeft(manager, 0, 128);
    TestMap bottomRight(manager, 128, 128);
    TestMap* maps[] = {&topLeft, &topRight, &bottomLeft, &bottomRight};
    CMapCellPosInfoGroup group;
    CMapCellPosInfo* corners[] = {&group.TLPos, &group.TRPos, &group.BLPos, &group.BRPos};
    long localX[] = {125, 1, 125, 1};
    long localY[] = {125, 125, 1, 1};
    for (unsigned active = 0; active < 16; ++active) {
        for (unsigned i = 0; i < 4; ++i)
            manager.SetMap(i, *maps[i], active & (1 << i) ? 1 : -1);
        for (unsigned level = 0; level < 6; ++level) {
            manager.Locate(C2DVector(127, 127), 2, level, group);
            Check(group.Layer == level);
            for (unsigned i = 0; i < 4; ++i) {
                bool present = (active & (1 << i)) != 0;
                Check((corners[i]->PMapInfo != 0) == present);
                if (present) {
                    long shift = LayerInfo[level].ScaleShift;
                    unsigned x = shift < 0 ? 0 : localX[i] >> shift;
                    unsigned y = shift < 0 ? 0 : localY[i] >> shift;
                    Check(corners[i]->LocalPos.X == x && corners[i]->LocalPos.Y == y);
                    Check(corners[i]->PCell == &maps[i]->GetCell(level, x, y));
                } else {
                    Check(corners[i]->PCell == 0);
                }
            }
        }
    }
    manager.Locate(C2DVector(10, 10), 0, 0, group);
    Check(group.TLPos.PCell == &topLeft.GetCell(0, 5, 5));
    Check(!group.TRPos.PCell && !group.BLPos.PCell && !group.BRPos.PCell);
    manager.Locate(C2DVector(11, 10), 1, 0, group);
    Check(group.TLPos.PCell == &topLeft.GetCell(0, 5, 4));
    Check(group.TRPos.PCell == &topLeft.GetCell(0, 6, 4));
    Check(group.BLPos.PCell == &topLeft.GetCell(0, 5, 5));
    Check(group.BRPos.PCell == &topLeft.GetCell(0, 6, 5));

    return 0;
}

namespace NThingMap {
CMapwhoNode::CMapwhoNode():PMapwho(0),PNext(0),PPrevious(0),PCell(0) {}
}
namespace PlaceFixture {
using NThingMap::CMapwhoNode;
using NThingMap::CThingMapCell;
void check(bool b) { if (!b) exit(1); }
struct CTCPhysicsBase { float radius; unsigned reads; float GetRadius(){++reads;return radius;} };
struct CThing {
    bool local; CTCPhysicsBase phys; unsigned probes;
    bool IsLocal(){return local;} CTCPhysicsBase* GetPhysicsTC(){++probes;return &phys;}
};
struct CTCMapwho {
    CThing thing; bool in; unsigned events[4], count; CMapwhoNode nodes[4];
    CThing& GetParentThing(){return thing;} bool IsInMap() const{return in;}
    CMapwhoNode& node(unsigned i){check(count<4);events[count++]=i;return nodes[i];}
    CMapwhoNode& GetTLNode(){return node(0);} CMapwhoNode& GetTRNode(){return node(1);}
    CMapwhoNode& GetBRNode(){return node(2);} CMapwhoNode& GetBLNode(){return node(3);}
    void SetInMap(bool b){in=b;}
};
struct CellPos {CThingMapCell* PCell;};
struct CMapCellPosInfoGroup {CellPos TLPos,TRPos,BLPos,BRPos;};
struct CThingMapManager {
    long NoThings; unsigned mask, phase; CThingMapCell cells[4]; CMapwhoNode old[4]; bool shared;
    long GetThingMapwhoLevel(float r){check(phase++==0 && r==1.25f);return 3;}
    void GetThingLayerCellPositions(float r,const C2DVector& pos,long level,CMapCellPosInfoGroup&){
        check(phase++==1 && r==1.25f && pos.X==2.5f && pos.Y==-3.5f && level==3);
    }
    void GetCellsForThingLayerCellPositions(CMapCellPosInfoGroup& g){
        check(phase++==2);CellPos* corners[4]={&g.TLPos,&g.TRPos,&g.BRPos,&g.BLPos};
        for(unsigned i=0;i<4;++i)corners[i]->PCell=(mask&(1u<<i))?&cells[shared?0:i]:0;
    }
    void AddMapwhoNodeToCell(CThingMapCell& cell,CMapwhoNode& node) {
        CMapwhoNode* pfirst_node = cell.PFirstNode;
        node.PPrevious = 0;
        node.PNext = pfirst_node;
        if (pfirst_node) pfirst_node->PPrevious = &node;
        cell.PFirstNode = &node;
        node.PCell = &cell;
    }
    void PlaceThing(CTCMapwho&,const C3DVector&);
};
void CThingMapManager::PlaceThing(CTCMapwho& mapwho,const C3DVector& pos)
{
    CThing& thing = mapwho.GetParentThing();
    if (!thing.IsLocal()) {
        FABLE_RETAIL_ASSERT(!mapwho.IsInMap());
        CTCPhysicsBase* ptc_physics = thing.GetPhysicsTC();
        float radius = ptc_physics->GetRadius();
        long mapwho_level = GetThingMapwhoLevel(radius);
        CMapCellPosInfoGroup mapwho_pos;
        GetThingLayerCellPositions(radius,pos.ToC2DVector(),mapwho_level,mapwho_pos);
        GetCellsForThingLayerCellPositions(mapwho_pos);
        if (mapwho_pos.TLPos.PCell)
            AddMapwhoNodeToCell(*mapwho_pos.TLPos.PCell,mapwho.GetTLNode());
        if (mapwho_pos.TRPos.PCell)
            AddMapwhoNodeToCell(*mapwho_pos.TRPos.PCell,mapwho.GetTRNode());
        if (mapwho_pos.BRPos.PCell)
            AddMapwhoNodeToCell(*mapwho_pos.BRPos.PCell,mapwho.GetBRNode());
        if (mapwho_pos.BLPos.PCell)
            AddMapwhoNodeToCell(*mapwho_pos.BLPos.PCell,mapwho.GetBLNode());
        mapwho.SetInMap(true);
        ++NoThings;
    }
}
void execute(unsigned mask,unsigned oldMask,bool local,bool shared){
    CThingMapManager m; CTCMapwho component;
    component.in=false;component.count=0;component.thing.local=local;component.thing.probes=0;
    component.thing.phys.radius=1.25f;component.thing.phys.reads=0;
    m.NoThings=7;m.mask=mask;m.phase=0;m.shared=shared;
    for(unsigned i=0;i<4;++i)if(oldMask&(1u<<i)){m.old[i].PCell=&m.cells[i];m.cells[i].PFirstNode=&m.old[i];}
    m.PlaceThing(component,C3DVector(2.5f,-3.5f,11.0f));
    check(m.NoThings==7+(!local) && component.in==!local && m.phase==(local?0u:3u));
    check(component.thing.probes==unsigned(!local) && component.thing.phys.reads==unsigned(!local));
    unsigned n=0;
    for(unsigned i=0;i<4;++i){
        if(!local&&(mask&(1u<<i))){check(component.events[n++]==i);check(component.nodes[i].PCell==&m.cells[shared?0:i]);}
        else check(!component.nodes[i].PCell && !component.nodes[i].PPrevious && !component.nodes[i].PNext);
    }
    check(n==component.count);
    for(unsigned i=0;i<4;++i){
        CMapwhoNode* prev=0;CMapwhoNode* walk=m.cells[i].PFirstNode;
        for(int j=3;j>=0;--j)if(!local&&(mask&(1u<<j))&&(shared?i==0:i==unsigned(j))){
            check(walk==&component.nodes[j] && walk->PPrevious==prev);prev=walk;walk=walk->PNext;
        }
        if(oldMask&(1u<<i)){check(walk==&m.old[i] && walk->PPrevious==prev);walk=walk->PNext;}
        check(walk==0);
    }
}
}
int main(){
    if (RunActualCellLookupCases() != 0) return 1;
    for(unsigned mask=0;mask<16;++mask)for(unsigned before=0;before<16;++before)
        for(unsigned local=0;local<2;++local)for(unsigned shared=0;shared<2;++shared)
            PlaceFixture::execute(mask,before,local!=0,shared!=0);
    puts("00637fc0_TEST PASS");return 0;
}