// CThingSearchTools::GetAllThingsInRoughArea<const CThing, std::vector<const CThing*>, CThingFilter_HasPhysicsMesh>(...) -- finds physics mesh candidates in the rough area.
// Leaf: ??$GetAllThingsInRoughArea@$$CBVCThing@@V?$vector@PBVCThing@@V?$allocator@PBVCThing@@@std@@@std@@VCThingFilter_HasPhysicsMesh@@@CThingSearchTools@@QBEXABVC2DBoxI@@ABVCThingFilter_HasPhysicsMesh@@AAV?$vector@PBVCThing@@V?$allocator@PBVCThing@@@std@@@std@@@Z
// Flags: default
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_STATTYPEFLAGS
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_CLIPAREA
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ORIGINAL_ROUGHAREA
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ROUGHAREA_EGOD
#define FABLE_RETAIL_CTHINGMAP_PEEKCELL_INLINE
#define FABLE_RETAIL_C2DBOXI
#define FABLE_RETAIL_C2DBOXI_COPY
#define FABLE_RETAIL_C2DBOXI_GETWIDTH
#define FABLE_RETAIL_C2DBOXI_GETHEIGHT
#define FABLE_RETAIL_CTCMAPWHO
#define FABLE_RETAIL_CTCMAPWHO_STATTYPE
#define FABLE_RETAIL_CTCMAPWHO_SEARCHID
#define FABLE_RETAIL_STLP_VECTOR_OVERFLOW_OUTOFLINE
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_STLP_VECTOR_C
#define FABLE_RETAIL_CCHARSTRING
#define protected public
#include "engine/CThing.h"
#undef protected
#include "engine/NThingMap_CThingMap.h"
#include "engine/CTCMapwho.h"
#include "engine/CThingSearchTools.h"
#include "engine/CThingFilter_HasPhysicsMesh.h"

template void CThingSearchTools::GetAllThingsInRoughArea<const CThing, std::vector<const CThing*>, CThingFilter_HasPhysicsMesh>(
    const C2DBoxI&, const CThingFilter_HasPhysicsMesh&, std::vector<const CThing*>&) const;

#include "engine/CWorldMap.h"
#include <stdio.h>

CBaseClass::CBaseClass() {}
CBaseClass::~CBaseClass() {}
CBaseObject::CBaseObject() : IntelligentPointer(0) {}
CBaseObject::~CBaseObject() {}
void CBaseObject::Validate() const {}
void CClass::Save(CCharString&) {}
CCharString::CCharString() : PStringData(0) {}
CCharString::~CCharString() {}
CBaseClassNonCopyable::CBaseClassNonCopyable() {}
CBaseClassNonCopyable::~CBaseClassNonCopyable() {}
CTCBase::CTCBase(CThing& thing) : ParentThing(&thing) {}
CTCBase::~CTCBase() {}
void CTCBase::OnSerialise(CPersistContext&, bool) {}
void CTCBase::OnRemove() {}
void CTCBase::ConstructFromParams(const CTCParamsBase&) {}
void CTCBase::OnCreate() {}
void CTCBase::InitialActivate() {}
void CTCBase::OnInitialActivate() {}
void CTCBase::RenderUpdate(float) {}
bool CTCBase::HasFrameUpdate() const { return false; }
void CTCBase::FrameUpdate() {}
void CTCBase::BulletTimeFrameUpdate() {}
void CTCBase::DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo&) {}
void CTCBase::Copy(const CTCBase&) {}
void CTCBase::OnDie() {}
void CTCBase::OnKill() {}
void CTCBase::OnLevelUnload() {}
void CTCBase::OnSetInLimbo(bool) {}
bool CTCBase::Validate(CCharString&) const { return true; }
bool CTCBase::IsFreeForDeletion() const { return true; }
bool CTCBase::IsSerialiseCreatable() const { return false; }
bool CTCBase::IsSerialisableForHeroState() const { return false; }
bool CTCBase::IsComponentSerialisable() const { return false; }
bool CTCBase::IsDriver() const { return false; }
bool CTCBase::IsCopyable() const { return false; }
void CTCBase::GlobalUpdate(bool) {}
void CTCBase::HandleMessage(const NTCMessages::CMessageBase&) {}
CTCMapwho::CTCMapwho(CThing& thing) : CTCBase(thing), SearchID(0), InMapwho(false) {}
CTCMapwho::~CTCMapwho() {}
void CTCMapwho::OnDie() {}
ETCInterfaceType CTCMapwho::GetInterfaceType() const { return TCI_MAPWHO; }
CCharString CTCMapwho::GetClassTypeName() const { return CCharString(); }

long NThingMap::MaxLayers = 2;
const NThingMap::CLayerInfo NThingMap::LayerInfo[] = {
    { 1, 0, CRGBColour(), CRGBColour() },
    { 2, 1, CRGBColour(), CRGBColour() }
};
NThingMap::CThingMap::~CThingMap() {}
unsigned long CThingSearchTools::NoLookups = 0;
unsigned long CThingSearchTools::NoThingsScanned = 0;
unsigned long CThingSearchTools::SearchID = 0;
CThingSearchTools::CThingSearchTools(CThingManager& things, CWorldMap& world)
    : WorldMap(world), PThingMapManager(0), ThingManager(things) {}
CThingSearchTools::~CThingSearchTools() {}

static NThingMap::CThingMap* active_map;
static const CThing* accepted_thing;
static unsigned long filter_calls;
static bool leaked_maps;

void CThingSearchTools::GetAllThingMapsInArea(const C2DBoxI&, std::vector<NThingMap::CThingMap*>& maps) const
{
    if (!maps.empty())
        leaked_maps = true;
    if (active_map)
        maps.push_back(active_map);
}

C2DBoxI CThingSearchTools::ClipAndConvertWorldMapAreaToLocalArea(
    const NThingMap::CThingMap&, const C2DBoxI& area) const
{
    return area;
}

bool CThingFilter_HasPhysicsMesh::operator()(const CThing& thing) const
{
    ++filter_calls;
    return &thing == accepted_thing;
}

int main()
{
    CWorldMap world;
    // The search holds this engine-manager reference. The map-provider stub
    // supplies the maps directly and never dereferences the opaque handle.
    static unsigned __int64 manager_storage[32];
    CThingManager& manager = *reinterpret_cast<CThingManager*>(manager_storage);
    CThingSearchTools search(manager, world);
    NThingMap::CThingMap map;
    CThing accepted, rejected, prior;
    accepted.ThingTypeFlags = 0;
    rejected.ThingTypeFlags = 1;
    CTCMapwho accepted_mapwho(accepted), rejected_mapwho(rejected);
    active_map = &map;
    accepted_thing = &accepted;
    map.MapwhoLevelInfo.resize(2);
    map.MapwhoLevelInfo[0].Extents.X = 2;
    map.MapwhoLevelInfo[1].Extents.X = 1;
    map.Cells.resize(2);
    map.Cells[0].resize(4);
    map.Cells[1].resize(1);
    NThingMap::CMapwhoNode first, second, duplicate;
    first.PMapwho = &accepted_mapwho;
    first.PNext = &second;
    second.PMapwho = &rejected_mapwho;
    second.PNext = 0;
    duplicate.PMapwho = &accepted_mapwho;
    duplicate.PNext = 0;
    map.Cells[0][0].PFirstNode = &first;
    map.Cells[0][3].PFirstNode = &duplicate;
    map.Cells[1][0].PFirstNode = &duplicate;

    C2DBoxI area;
    area.TLX = area.TLY = 0;
    area.BRX = area.BRY = 2;
    CThingFilter_HasPhysicsMesh filter;
    std::vector<const CThing*> things;
    things.push_back(&prior);
    CThingSearchTools::NoThingsScanned = 5;
    search.GetAllThingsInRoughArea<const CThing>(area, filter, things);
    if (things.size() != 2 || things[0] != &prior || things[1] != &accepted
        || filter_calls != 2 || CThingSearchTools::NoThingsScanned != 0
        || accepted_mapwho.GetSearchID() != 1 || rejected_mapwho.GetSearchID() != 1)
        return 1;

    filter_calls = 0;
    area.BRX = 0;
    search.GetAllThingsInRoughArea<const CThing>(area, filter, things);
    if (things.size() != 2 || filter_calls != 0 || accepted_mapwho.GetSearchID() != 1)
        return 2;

    active_map = 0;
    area.BRX = 2;
    search.GetAllThingsInRoughArea<const CThing>(area, filter, things);
    if (things.size() != 2 || filter_calls != 0 || leaked_maps
        || CThingSearchTools::SearchID != 3 || CThingSearchTools::NoLookups != 3)
        return 3;
    puts("0049c490_TEST PASS");
    return 0;
}