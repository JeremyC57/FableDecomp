// CMinimapData::PopulateMinimapThemeList(const CWorldMap& worldMap, long mapNumber) -- fill the minimap theme grid of one map with the minimap theme of the strongest engine theme under each cell, then mark the cells under each overlapping building's physics mesh as theme 8
// Grade: DIFFER(1308v1340) -- 447 diff lines: frame layout only (retail frame 0x120 vs ours 0x110: retail keeps separate slots for the convert arguments TL [esp+0B4h] / BR [esp+9Ch], results [esp+0A4h] / [esp+0ACh], from [esp+90h], to [esp+0BCh], the cell temporary [esp+0C8h], the mesh [esp+0D4h] and the intersection [esp+114h] where VC7.1 packs ours) and the first loop's register roles (retail spills `this` from ebp to [esp+74h] and keeps worldMap in ebx); the call sequence agrees call for call
// Next idea: sibling scopes ({ } per cell iteration and per building) or by-value C2DCoordI/C3DVector temporaries named per site so VC7.1 stops sharing their slots; the sibling CRegionMinimap::PopulateRegionMinimap 0x0066CF00 (chunk 802 differ) has the same frame residue
// Leaf: ?PopulateMinimapThemeList@CMinimapData@@QAEXABVCWorldMap@@J@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00642AA0, 1340 bytes incl. the 8-case switch jump table; thiscall `ret 8`; one caller 0x006430BC
//   (CMinimapData 0x00643090); an unlisted function starts at 0x00642FE0.
// Identity (proven): Ego_r.pdb 0x00686400 / FableWin.pdb 0x0218B6C0 ?PopulateMinimapThemeList@CMinimapData@@QAEXABVCWorldMap@@J@Z; its
//   caller is the CMinimapData builder 0x00643090 and every callee below takes (worldMap, mapNumber) as that signature says.
// Callees: CWorldMap::ConvertLocalCoordsToWorldMapCoords 0x004FC4A0 / ConvertWorldMapCoordsToLocalCoords 0x004FC3C0 /
//   GetStrongestEngineThemeAt 0x0063ADF0 / GetGroundSizeZAt 0x0063A300 / PeekMap (vtable +0x4C), CMinimapData::PeekDefinitionManager (inline) -> CMainGameComponent::PeekDefinitionManager
//   0x00415FBC, CDefinitionManager::GetPDefFromGlobalIndex 0x009AD9E0 (the inline GetDef<CEngineThemeDef>), CThingSearchTools::
//   GetAllThings<CThingBuilding, vector<CThingBuilding*>, CThingFilter_OverlappingArea> 0x006437D0, CTCPhysicsBase::GetBoundingInfo
//   0x006AF640 / GetPhysicsMeshObject 0x006B0850, CPhysicsMeshObject::GetPointIntersectionFromTo 0x00A12450 / ~CPhysicsMeshObject
//   0x00A127D0, __ftol2 0x00BFEA70, free 0x00BFEA14; 100.0f 0x01230A08.
// Layout: CEngineThemeDef::MinimapTheme +0x90 (def schema retail offset 144), CMinimapData retail 0x3C bytes (CMinimapData.h overlay).
// Shape: CEngineThemeDef EMinimapThemeType 1..7 -> minimap theme 1,6,4,5,2,7,3 (else 0) as an 8-case switch; building cells 8.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGBUILDING
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CPHYSICSMESHOBJECT_EDGES
#define FABLE_RETAIL_CPHYSICSMESHOBJECT_CENTRE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_WORLD
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLDMAP
#define FABLE_RETAIL_CWORLDMAP_STRONGESTTHEME
#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#define FABLE_RETAIL_C2DBOXI
#define FABLE_RETAIL_C2DBOXF
#define FABLE_RETAIL_C2DBOXF_CLIPTO
#define FABLE_RETAIL_CWORLDMAP_COORDCONVERT
#define FABLE_RETAIL_CENGINETHEMEDEF
#define FABLE_RETAIL_CMINIMAPDATA_PEEKDEFS
#define FABLE_RETAIL_CMAP_WORLDEXTENTS
#define FABLE_RETAIL_C2DBOXI_CTORS
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/C3DVector.h"
#include "engine/C2DBoxF.h"
#include "engine/CThing.h"
#include "engine/CThingBuilding.h"
#include "engine/CTCBase.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CPhysicsMeshObject.h"
#include "engine/CMeshIntersectionData.h"
#include "engine/CDefClassBase.h"
#include "engine/CDefinitionManager.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CMainGameComponent.h"
#include "engine/CWorld.h"
#include "engine/CWorldMap.h"
#include "engine/CThingSearchTools.h"
#include "engine/CMap.h"
#include "engine/CEngineThemeDef.h"
#include "engine/CThingFilter_OverlappingArea.h"
#include "engine/CMinimapData.h"


template <class T> inline bool CDefinitionManager::GetDef(long index, CDefPointer<const T>* out) const
{
    if (index > 0) {
        CDefPointer<const CDefClassBase> def = GetPDefFromGlobalIndex(index);
        if (def.Get()) {
            if (out->Object != def.Object) {
                out->Release();
                out->Object = def.Object;
                if (out->Object)
                    out->Object->AddRef();
            }
            return true;
        }
    }
    return false;
}

void CMinimapData::PopulateMinimapThemeList(const CWorldMap& worldMap, long mapNumber)
{
    CDefPointer<const CEngineThemeDef> themeDef;
    long strength = 0;
    C3DVector world(0.0f, 0.0f, 0.0f);
    C3DVector local(0.0f, 0.0f, 0.0f);
    for (long y = 0; y < MinimapExtents.Y; ++y) {
        for (long x = 0; x < MinimapExtents.X; ++x) {
            local = C3DVector((float)x, (float)y, 0.0f);
            world = worldMap.ConvertLocalCoordsToWorldMapCoords(mapNumber, local);
            long theme = worldMap.GetStrongestEngineThemeAt(world, strength);
            PeekDefinitionManager().GetDef(theme, &themeDef);
            MinimapTheme minimapTheme;
            switch (themeDef->MinimapTheme) {
            case 1: minimapTheme = (MinimapTheme)1; break;
            case 2: minimapTheme = (MinimapTheme)6; break;
            case 3: minimapTheme = (MinimapTheme)4; break;
            case 4: minimapTheme = (MinimapTheme)5; break;
            case 5: minimapTheme = (MinimapTheme)2; break;
            case 6: minimapTheme = (MinimapTheme)7; break;
            case 7: minimapTheme = (MinimapTheme)3; break;
            default: minimapTheme = (MinimapTheme)0; break;
            }
            SetMinimapThemeValue(C2DCoordI(x, y), minimapTheme);
        }
    }
    std::vector<CThingBuilding*> buildings;
    C2DCoordI mapPos = worldMap.PeekMap(mapNumber).GetWorldPosition();
    C2DBoxI mapArea(mapPos.X, mapPos.Y, mapPos.X + worldMap.PeekMap(mapNumber).GetWorldSizeX(), mapPos.Y + worldMap.PeekMap(mapNumber).GetWorldSizeY());
    CMainGameComponent::Get()->PeekWorld().PeekThingSearchTools().GetAllThings<CThingBuilding, std::vector<CThingBuilding*>, CThingFilter_OverlappingArea>(CThingFilter_OverlappingArea(&mapArea), buildings);
    C3DVector boundsCentre(0.0f, 0.0f, 0.0f);
    float boundsRadius = 0.0f;
    for (std::vector<CThingBuilding*>::iterator it = buildings.begin(); it != buildings.end(); ++it) {
        CThingBuilding* building = *it;
        if (building->GetPhysicsTC()) {
            building->GetPhysicsTC()->GetBoundingInfo(boundsCentre, boundsRadius);
            long centreX = (long)boundsCentre.X;
            long centreY = (long)boundsCentre.Y;
            long radius = (long)boundsRadius;
            long left = centreX - radius;
            long top = centreY - radius;
            long right = centreX + radius;
            long bottom = centreY + radius;
            if (left < mapArea.TLX) left = mapArea.TLX; else if (left > mapArea.BRX) left = mapArea.BRX;
            if (top < mapArea.TLY) top = mapArea.TLY; else if (top > mapArea.BRY) top = mapArea.BRY;
            if (right < mapArea.TLX) right = mapArea.TLX; else if (right > mapArea.BRX) right = mapArea.BRX;
            if (bottom < mapArea.TLY) bottom = mapArea.TLY; else if (bottom > mapArea.BRY) bottom = mapArea.BRY;
            C2DCoordI topLeft(left, top);
            C2DCoordI bottomRight(right, bottom);
            C2DCoordI localTopLeft = worldMap.ConvertWorldMapCoordsToLocalCoords(mapNumber, topLeft);
            C2DCoordI localBottomRight = worldMap.ConvertWorldMapCoordsToLocalCoords(mapNumber, bottomRight);
            for (long cy = localTopLeft.Y; cy < localBottomRight.Y; ++cy) {
                for (long cx = localTopLeft.X; cx < localBottomRight.X; ++cx) {
                    C3DVector pos = worldMap.ConvertLocalCoordsToWorldMapCoords(mapNumber, C3DVector((float)cx, (float)cy, 0.0f));
                    pos.Z = worldMap.GetGroundSizeZAt(pos);
                    C3DVector to(pos);
                    C3DVector from(pos.X, pos.Y, pos.Z + 100.0f);
                    CPhysicsMeshObject mesh = building->GetPhysicsTC()->GetPhysicsMeshObject();
                    CMeshIntersectionData intersection;
                    if (mesh.GetPointIntersectionFromTo(from, to, intersection))
                        SetMinimapThemeValue(C2DCoordI(cx, cy), (MinimapTheme)8);
                }
            }
        }
    }
}
