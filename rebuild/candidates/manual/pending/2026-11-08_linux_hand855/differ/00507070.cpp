// CWorldMap::BuildStaticMapGenerationVisibilityInfo(CStaticMapGenerationVisibiilityInfo& info) -- describe every sea region (its name and all its own and visible maps) for static-map generation, and with map optimisations on give each map the set of maps visible from it (its region's maps and visible maps, for maps loaded on player proximity)
// Grade: DIFFER(1628v1432) -- 403 diff lines: per-site inlining (retail calls vector<long>'s copy constructor 0x00514160 for both CSeaBodyDesc copies inside the inlined SeaBodies.push_back but expands it for the visMaps copy; ours expands all three, and a declared-only copy constructor calls all three: 1298 bytes, md 609) and the register/slot choices that follow; otherwise the call sequence agrees call for call
// Next idea: c2's static block frequency (docs/pipeline/INLINE_DEPTH_REVISIT.md): find a loop/guard shape that makes the push_back copies colder than the visMaps copy (a range-constructor visMaps probe aligned every call but moved `this` to esi: md 659)
// Leaf: ?BuildStaticMapGenerationVisibilityInfo@CWorldMap@@QAEXAAVCStaticMapGenerationVisibiilityInfo@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00507070, 1432 bytes, thiscall `ret 4`, 8-aligned ebp frame; no caller or reference in retail
//   (dead in the shipped game: static-map generation is a tools path).
// Identity (proven): Ego_r.pdb 0x004DAF10 / FableWin.pdb 0x01C91A40 ?BuildStaticMapGenerationVisibilityInfo@CWorldMap@@QAEXAAVCStaticMapGenerationVisibiilityInfo@@@Z;
//   0x00507070 lies in the CWorldMap block (CWorldMap::CWorldMap 0x005066E0 before it, DeleteAllRegions 0x00507610 after it), walks
//   CWorldMap::Regions (+0x2C, 0x58-byte CRegion) and Maps (+0x20, 72-byte CMapInfo), and writes the PDB's CStaticMapGenerationVisibiilityInfo
//   (Maps +0, SeaBodies +0x0C, EstimatedMaxCameraHeightAboveLandscape +0x18 with STLport's 12-byte vectors; Ego_r +0/+0x10/+0x20).
//   Catalogue placeholders RebuildStringPairTableFromTree / LText_LoadAndProcessEntries are wrong. The two globals are descriptive.
// Callees: SeaBodies.clear() = __copy<CSeaBodyDesc*> 0x00517200 + __destroy_aux<CSeaBodyDesc*> 0x00519EB0 (__fastcall, see the
//   FABLE_RETAIL_STLP_FASTCALL_DESTROY_AUX overlay opt-in), CCharString() 0x0099E4B0 / operator= 0x0099EFB0 / copy 0x0099EC30 / ~ 0x0099EAE0,
//   vector<long>::_M_insert_overflow 0x00440990 (both push loops), vector<long>(const vector&) 0x00514160, __uninitialized_copy<CSeaBodyDesc*>
//   0x00518C00 (__fastcall), malloc 0x00BFEA0E / memmove 0x00BFEAE6 / free 0x00BFEA14, CMainGameComponent::PeekDefinitionManager 0x00415FBC
//   (global def +0xD0, StaticMapGenerationEstimatedMaxCameraHeightAboveLandscape +0x58), set<long> operator= 0x00512CE0,
//   __destroy_aux<CMapDesc*> 0x0051AC70, vector<CMapDesc>::resize 0x0051D240, _Rb_tree<long>::insert_unique 0x00512DA0 / _M_erase 0x004A9CB0.
// Data: g_bBuildRetailStaticMaps 0x013B864A, g_bEnableMapOptimisations 0x013B864B (descriptive names).
// Header debt: CRegionDef (HasSea, retail +0xA4), CGlobalDef (+0x58) and CStaticMapGenerationVisibiilityInfo are local models here; no
//   engine/ header exists for them (CGlobalDef.h is quarantined).
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
#define FABLE_STLP_OUTOFLINE_UNINIT_COPY
#define FABLE_RETAIL_STLP_FASTCALL_DESTROY_AUX
#include "stlport/fable_containers.h"
#include "stlport/fable_map.h"
template <> void std::vector<long>::_M_insert_overflow(long* position, const long& x, const __true_type&, unsigned int fillLength, bool atEnd);   // 0x00440990
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
