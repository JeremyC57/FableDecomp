#define MIN(a,b) ((a) < (b) ? (a) : (b))
#define MAX(a,b) ((a) > (b) ? (a) : (b))
#define FABLE_RETAIL_STLP_FILL_NOINLINE
#define FABLE_RETAIL_STLP_UNINITIALIZED_FILL_N_NOINLINE
#define FABLE_RETAIL_STLP_RANDOM_ACCESS_COPY_BACKWARD
#define FABLE_RETAIL_THEME_FILL_HELPERS_PASS30
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_STLP_VECTOR_C
#define FABLE_RETAIL_CTHINGFILTER_OVERLAPPINGSPHERE_OUTOFLINE
#define FABLE_RETAIL_MAP_VALIDATE
#define FABLE_RETAIL_CTCTARGETINGPLAYER_UPDATE_LIST
#define FABLE_RETAIL_CTCMAPWHO
#define FABLE_RETAIL_CTCMAPWHO_SEARCHID
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ORIGINAL_ROUGHAREA
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ROUGHAREA_THISEND
#define FABLE_RETAIL_CTHINGMAP_PEEKCELL_INLINE
#define FABLE_RETAIL_C2DBOXI_CTORS
#define FABLE_RETAIL_C2DBOXI_GETWIDTH
#define FABLE_RETAIL_C2DBOXI_GETHEIGHT
// CTCVolumeContainmentTracker::CalculateBlendedLightingChannel at 0x00785160.
// Leaf: ?CalculateBlendedLightingChannel@CTCVolumeContainmentTracker@@AAEXAAVCEnginePrimitiveThemeChannelDesc@@@Z
// Flags: /O2 /Oy
// Retail-asm-helper: GFFloatToLongFloor, GFFloatToLongCeil
// Retail-asm: 0x007851D7 and 0x00785211 fistp floor, 0x0078524B and 0x00785285 fistp ceil.
// Inline-budget: the same-TU rough-area and STLport fill-insert bodies preserve their call sites and the original frame.
#define FABLE_RETAIL_CTCVOLUMECONTAINMENTTRACKER_UPDATE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHING_GETTC_ORIGINAL_CHAIN
#define FABLE_RETAIL_CTHING_COMPONENT_QUERY_PASS27
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#define FABLE_RETAIL_CTHING_STATTYPEFLAGS
#define FABLE_RETAIL_C2DBOXI
#define FABLE_RETAIL_CTHINGBUILDING_TYPEFLAGS
#define FABLE_RETAIL_TC_VOLUME_CONTAINMENT_TRACKER_DEF
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_MOVEMENT
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_LIGHTINGCHANNEL
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CPHYSICSMESHOBJECT
#define FABLE_RETAIL_CPHYSICSMESHOBJECT_CONTAINMENTVOLUME
#define FABLE_RETAIL_MAP_CACHED_VOLUMES_PASS26
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CARRAY_ORIGINAL_USER_SPECIAL_MEMBERS_PASS22
#define FABLE_RETAIL_CARRAY_PUSHBACK
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_STLP_VECTOR_ORIGINAL_RESIZE_INSERT_CHAIN_PASS22

#include "engine/CThingBuilding.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CPhysicsMeshObject.h"
#include "engine/CTCVolumeContainmentTracker.h"
#include "engine/CTCVolumeContainmentTrackerDef.h"
#include "engine/CWorld.h"
#include "engine/C2DBoxI.h"
#include "engine/CTCMapwho.h"
#include "engine/NThingMap_CThingMap.h"
#include "engine/CThingSearchTools.h"
#include "engine/CThingFilter_OverlappingSphere.h"
#include "engine/NGraphicAppearance_CTCGraphicAppearance.h"
#include "engine/C3DVector.h"
#include "engine/C2DBoxI.h"
#include "engine/CArray.h"
#include "engine/GFFloatToLongFloor.h"
#include "engine/GFFloatToLongCeil.h"

extern const CCharString BUILDING_BOUNDRY;

void CTCVolumeContainmentTracker::CalculateBlendedLightingChannel(CEnginePrimitiveThemeChannelDesc& desc)
{
    C3DVector parent_pos = GetParentThing().GetPos();
    float light_sphere_radius = PDef->LightingChannelSphereModelRadius;
    C3DVector sphere_center = parent_pos + C3DVector(0.0f, 0.0f, PDef->LightingChannelSphereModelZOffset);
    std::list<CThingBuilding*> buildings;
    C2DBoxI search_area;
    search_area.TLX = GFFloatToLongFloor(sphere_center.X - light_sphere_radius);
    search_area.TLY = GFFloatToLongFloor(sphere_center.Y - light_sphere_radius);
    search_area.BRX = GFFloatToLongCeil(sphere_center.X + light_sphere_radius);
    search_area.BRY = GFFloatToLongCeil(sphere_center.Y + light_sphere_radius);
    GetWorld().PeekThingSearchTools().GetAllThingsInRoughArea<CThingBuilding>(
        search_area, CThingFilter_OverlappingSphere(sphere_center, light_sphere_radius), buildings);
    CArray<std::pair<long,float> > themes;
    float total_internal_theme_weight = 0.0f;
    for (std::list<CThingBuilding*>::iterator itr = buildings.begin(); itr != buildings.end(); ++itr) {
        NGraphicAppearance::CTCGraphicAppearance* graphic_app;
        if ((*itr)->GetTC(&graphic_app)) {
            CTCPhysicsBase* ptc_physics = (*itr)->GetPhysicsTC();
            if (ptc_physics && ptc_physics->HasPhysicsMeshObject()) {
                CPhysicsMeshObject physics_mesh_object = ptc_physics->GetPhysicsMeshObject();
                float intersection_amount = physics_mesh_object.PeekRoughProportionOfSphereContainedInVolume(
                    BUILDING_BOUNDRY, sphere_center, light_sphere_radius);
                if (intersection_amount > 1.0f / 255.0f) {
                    bool found_match = false;
                    for (CArray<std::pair<long,float> >::iterator theme_itr = themes.begin();
                         theme_itr != themes.end(); theme_itr++) {
                        if (theme_itr->first == graphic_app->PeekLightingChannel().ChannelIndex0) {
                            theme_itr->second += intersection_amount;
                            found_match = true;
                            break;
                        }
                    }
                    if (!found_match)
                        themes.push_back(std::pair<long,float>(graphic_app->PeekLightingChannel().ChannelIndex0, intersection_amount));
                }
                total_internal_theme_weight += intersection_amount;
            }
        }
    }
    float total_weight = total_internal_theme_weight;
    if (total_internal_theme_weight < 1.0f - 1.0f / 255.0f) {
        themes.push_back(std::pair<long,float>(0, 1.0f - total_internal_theme_weight));
        total_weight = 1.0f;
    }
    if (themes.size() > 2) {
        long ordered_themes[3] = {-1, -1, -1};
        for (long ord = 0; ord < 3; ++ord) {
            for (long n = 0; (unsigned long)n < themes.size(); ++n) {
                if (ordered_themes[ord] < 0 || themes[n].second > themes[ordered_themes[ord]].second) {
                    bool already_found = false;
                    for (long o = 0; o < ord; ++o) {
                        if (ordered_themes[o] == n) {
                            already_found = true;
                            break;
                        }
                    }
                    if (!already_found)
                        ordered_themes[ord] = n;
                }
            }
        }
        long theme0 = themes[ordered_themes[0]].first;
        float weight0 = themes[ordered_themes[0]].second;
        float theme1 = (float)themes[ordered_themes[1]].first;
        float weight1 = themes[ordered_themes[1]].second - themes[ordered_themes[2]].second;
        total_weight = weight0 + weight1;
        themes.resize(2);
        themes[0] = std::pair<long,float>(theme0, weight0);
        themes[1] = std::pair<long,float>((long)theme1, weight1);
    }
    if (themes.size() == 1) {
        desc.ChannelIndex0 = (unsigned char)themes[0].first;
        desc.ChannelBlendFactor = 0;
    } else {
        desc.ChannelIndex0 = (unsigned char)themes[0].first;
        desc.ChannelIndex1 = (unsigned char)themes[1].first;
        desc.ChannelBlendFactor = (unsigned char)MAX(0.0f, MIN(255.0f, themes[1].second * 255.0f / total_weight));
    }
}

#undef MAX
#undef MIN