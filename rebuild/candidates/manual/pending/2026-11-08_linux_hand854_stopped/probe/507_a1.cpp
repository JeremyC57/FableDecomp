// CWorldMap::BuildStaticMapGenerationVisibilityInfo(CStaticMapGenerationVisibiilityInfo& info) -- describe every sea region (its name and all its own and visible maps) for static-map generation, and with map optimisations on give each map the set of maps visible from it (its region's maps and visible maps, for maps loaded on player proximity)
// Leaf: ?BuildStaticMapGenerationVisibilityInfo@CWorldMap@@QAEXAAVCStaticMapGenerationVisibiilityInfo@@@Z
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CWORLDMAP
#define FABLE_RETAIL_CWORLDMAP_STATICMAPVISIBILITY
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER_GLOBALDEF
#define FABLE_RETAIL_CREGION
#define FABLE_RETAIL_CREGION_PREGIONDEF
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_SET
#include "stlport/fable_containers.h"
#include "stlport/fable_map.h"
#include "fable_string.h"
#include "engine/CArray.h"
#include "engine/CDefPointer.h"
#include "engine/CMapInfo.h"
#include "engine/CRegion.h"
#include "engine/CWorldMap.h"
#include "engine/CMainGameComponent.h"
#include "engine/CGameDefinitionManager.h"

extern bool g_bBuildRetailStaticMaps;                // 0x013B864A
extern bool g_bEnableMapOptimisations;               // 0x013B864B

class CRegionDef : public CDefPointeeBase {          // Ego_r.pdb CRegionDef::HasSea (+0xB0 in Ego_r, retail +0xA4)
public:
    unsigned char _pad[0xa4 - sizeof(CDefPointeeBase)];
    bool HasSea;                                     // +0xa4
};
class CGlobalDef {                                   // engine/_quarantine/CGlobalDef.h: +0x58
public:
    unsigned char _pad[0x58];
    float StaticMapGenerationEstimatedMaxCameraHeightAboveLandscape;   // +0x58
};
class CStaticMapGenerationVisibiilityInfo {          // Ego_r.pdb (Maps +0, SeaBodies, EstimatedMaxCameraHeightAboveLandscape)
public:
    struct CMapDesc {
        std::set<long> VisibleMaps;
    };
    struct CSeaBodyDesc {
        CCharString Identifier;
        CArray<long> Maps;
    };
    CArray<CMapDesc> Maps;                           // +0x00
    CArray<CSeaBodyDesc> SeaBodies;                  // +0x0c
    float EstimatedMaxCameraHeightAboveLandscape;    // +0x18
};

void CWorldMap::BuildStaticMapGenerationVisibilityInfo(CStaticMapGenerationVisibiilityInfo& info)
{
    info.SeaBodies.clear();
    for (std::vector<CRegion>::iterator region = Regions.begin(); region != Regions.end(); ++region) {
        CDefPointer<const CRegionDef> def = region->GetPRegionDef();
        if (!def.IsNull() && def->HasSea) {
            CStaticMapGenerationVisibiilityInfo::CSeaBodyDesc desc;
            desc.Identifier = region->GetName();
            for (std::vector<long>::const_iterator map = region->GetMaps().begin(); map != region->GetMaps().end(); ++map)
                desc.Maps.push_back(*map);
            std::vector<long> visMaps = region->GetVisMaps();
            for (std::vector<long>::iterator visMap = visMaps.begin(); visMap != visMaps.end(); ++visMap)
                desc.Maps.push_back(*visMap);
            info.SeaBodies.push_back(desc);
        }
    }
    if (g_bBuildRetailStaticMaps)
        g_bEnableMapOptimisations = true;
    if (g_bEnableMapOptimisations) {
        info.EstimatedMaxCameraHeightAboveLandscape = Component.PeekDefinitionManager().PeekGlobalDef().StaticMapGenerationEstimatedMaxCameraHeightAboveLandscape;
        info.Maps.clear();
        info.Maps.resize(Maps.size());
        for (std::vector<CRegion>::iterator region = Regions.begin(); region != Regions.end(); ++region) {
            std::vector<long> visMaps = region->GetVisMaps();
            for (std::vector<long>::const_iterator map = region->GetMaps().begin(); map != region->GetMaps().end(); ++map) {
                long mapNumber = *map;
                std::set<long> visible;
                if (Maps[mapNumber].LoadedOnPlayerProximity) {
                    for (std::vector<long>::const_iterator other = region->GetMaps().begin(); other != region->GetMaps().end(); ++other)
                        visible.insert(*other);
                    for (unsigned int i = 0; i < visMaps.size(); ++i)
                        visible.insert(visMaps[i]);
                }
                info.Maps[mapNumber].VisibleMaps = visible;
            }
        }
    } else {
        info.Maps.clear();
    }
}
