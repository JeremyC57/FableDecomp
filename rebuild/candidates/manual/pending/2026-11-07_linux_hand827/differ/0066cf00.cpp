// CRegionMinimap::PopulateRegionMinimap(const CWorldMap& worldMap) -- fill every minimap cell with the region-minimap theme of the strongest engine theme under it (lit or not by the theme's lighting flag), then mark the cells under each building's physics mesh as theme 8
// Grade: DIFFER(3236v3248) -- 659 difflines (669 for chunk 802's parked source): the opt-in reread template operator=<U> (hdr CDefPointer.h FABLE_RETAIL_CDEFPOINTER_ASSIGN_REREAD, retail 0x0066D03C `mov eax,[esp+40h]`) fixes the GetDef block shape; left: x (retail ebx) and the theme def pointer (retail ebp) swap registers so every use differs, the frame is 8 bytes larger (0x1ec v 0x1e4) and the slot layout differs; GetBoundingBoxCentre 0x00A11AE0 has no relocation binding
// Next idea: the 8-byte frame gap first (the two GetTGAPositionFromWorldCoordinates temporaries: a sibling block or shared named local so they overlay), which may also move the x / def register choice; then the corner loop's strength-reduction base (retail ecx = begin+0xC)
// Leaf: ?PopulateRegionMinimap@CRegionMinimap@@QAEXABVCWorldMap@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0066CF00, 3248 bytes, thiscall `ret 4`; switch table 0x0066DB90 (8 cases). Ego_r.pdb
//   ?PopulateRegionMinimap@CRegionMinimap@@QAEXABVCWorldMap@@@Z (donor 0x006A1B80); caller CreateRegionMinimap 0x0066DC50.
// Callees: CWorldMap::GetStrongestEngineThemeAt(const C3DVector&, long&) const 0x0063ADF0, CMainGameComponent::PeekDefinitionManager
//   0x00415FBC, CDefinitionManager::GetPDefFromGlobalIndex(long) const 0x009AD9E0, CWorldMap::IsPosInMap(const C3DVector&) const
//   0x004FC600, CRegionMinimap::IlluminateRegionMinimapTheme 0x0066C900, CThingSearchTools::GetAllThings<CThingBuilding,
//   vector<CThingBuilding*>, CThingFilter_None> 0x0066E5A0, CTCPhysicsBase::GetPhysicsMeshInfo (slot 93, +0x174) /
//   GetPhysicsMeshObject 0x006B0850 / GetBoundingInfo 0x006AF640, CPhysicsMeshObject::GetBoundingBoxXEdge 0x00A11C20 / YEdge
//   0x00A11C90 / Centre 0x00A11AE0 / GetPointIntersectionFromTo 0x00A12450 / ~CPhysicsMeshObject 0x00A127D0,
//   vector<C3DVector>::_M_insert_overflow 0x0066E370, C2DBoxF::ClipTo 0x0066C560, CWorldMap::GetGroundSizeZAt 0x0063A300,
//   __ftol2 0x00BFEA70, free 0x00BFEA14.
// Data: the RegionMinimapLightingMap table 0x0137E9D8 ({eTheme, useLighting}, 8 bytes; Ego_r.pdb struct RegionMinimapLightingMap,
//   the array's own name is in neither PDB name list: descriptive), 0.0f 0x0122DEDC, 100.0f 0x01230A08.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFPOINTER_ASSIGN_REREAD
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
#define FABLE_RETAIL_CREGIONMINIMAP
#define FABLE_RETAIL_CREGIONMINIMAP_POPULATE
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
#include "engine/CThingFilter_None.h"
#include "engine/CRegionMinimap.h"

enum EMinimapThemeType { };

class CEngineThemeDef : public CDefClassBase {                       // Ego_r.pdb CEngineThemeDef (168 bytes); retail +0x90 = PDB +0x94
public:
    unsigned char _pad_0x25[0x90 - 0x25];
    EMinimapThemeType MinimapTheme;                                  // +0x90
};

struct RegionMinimapLightingMap { CRegionMinimap::RegionMinimapTheme eTheme; bool useLighting; };
extern RegionMinimapLightingMap g_RegionMinimapLightingMap[];       // 0x0137E9D8 (descriptive name)

template <class T> inline bool CDefinitionManager::GetDef(long index, CDefPointer<const T>* out) const
{
    if (index > 0) {
        CDefPointer<const CDefClassBase> def = GetPDefFromGlobalIndex(index);
        if (def.Get()) {
            *out = def;
            return true;
        }
    }
    return false;
}

void CRegionMinimap::PopulateRegionMinimap(const CWorldMap& worldMap)
{
    C2DCoordI topLeft;
    C2DExtentsI extents;
    GetRegionMapExtents(topLeft, extents);
    long size = GetRegionMinimapSize();
    long strength = 0;
    CDefPointer<const CEngineThemeDef> themeDef;
    C2DCoordI position;
    for (position.Y = 0; position.Y < size; ++position.Y) {
        for (position.X = 0; position.X < size; ++position.X) {
            C3DVector world;
            GetWorldCoordinatesFromTGAPosition(world, position);
            long theme = worldMap.GetStrongestEngineThemeAt(world, strength);
            PeekDefinitionManager().GetDef(theme, &themeDef);
            RegionMinimapTheme minimapTheme = (RegionMinimapTheme)0;
            if (worldMap.IsPosInMap(world)) {
                switch (themeDef->MinimapTheme) {
                case 1: minimapTheme = (RegionMinimapTheme)1; break;
                case 2: minimapTheme = (RegionMinimapTheme)6; break;
                case 3: minimapTheme = (RegionMinimapTheme)4; break;
                case 4: minimapTheme = (RegionMinimapTheme)5; break;
                case 5: minimapTheme = (RegionMinimapTheme)2; break;
                case 6: minimapTheme = (RegionMinimapTheme)7; break;
                case 7: minimapTheme = (RegionMinimapTheme)3; break;
                case 8: minimapTheme = (RegionMinimapTheme)8; break;
                default: minimapTheme = (RegionMinimapTheme)0; break;
                }
            }
            SetRegionMinimapThemeValue(position, minimapTheme);
            if (g_RegionMinimapLightingMap[minimapTheme].useLighting)
                SetRegionMinimapThemeLighting(position, IlluminateRegionMinimapTheme(worldMap, world));
            else
                SetRegionMinimapThemeLighting(position, 0.0f);
        }
    }
    C2DBoxF regionBox((float)topLeft.X, (float)topLeft.Y, (float)(topLeft.X + extents.X), (float)(topLeft.Y + extents.Y));
    std::vector<CThingBuilding*> buildings;
    CMainGameComponent::Get()->PeekWorld().PeekThingSearchTools().GetAllThings<CThingBuilding, std::vector<CThingBuilding*>, CThingFilter_None>(CThingFilter_None(), buildings);
    C3DVector boundsCentre(0.0f, 0.0f, 0.0f);
    float boundsRadius = 0.0f;
    for (std::vector<CThingBuilding*>::iterator it = buildings.begin(); it != buildings.end(); ++it) {
        CThingBuilding* building = *it;
        if (building->GetPhysicsTC() && building->GetPhysicsTC()->GetPhysicsMeshInfo()) {
            CPhysicsMeshObject mesh = building->GetPhysicsTC()->GetPhysicsMeshObject();
            C3DVector xEdge = mesh.GetBoundingBoxXEdge();
            C3DVector yEdge = mesh.GetBoundingBoxYEdge();
            C3DVector centre = mesh.GetBoundingBoxCentre();
            std::vector<C3DVector> corners;
            C2DBoxF bounds(-1.0f, -1.0f, -1.0f, -1.0f);
            corners.push_back(centre + xEdge + yEdge);
            corners.push_back(centre - xEdge + yEdge);
            corners.push_back(centre + xEdge - yEdge);
            corners.push_back(centre - xEdge - yEdge);
            for (unsigned long i = 0; i < corners.size(); ++i) {
                const C3DVector& corner = corners[i];
                if (corner.X < bounds.TLX || bounds.TLX < 0.0f)
                    bounds.TLX = corner.X;
                if (corner.Y < bounds.TLY || bounds.TLY < 0.0f)
                    bounds.TLY = corner.Y;
                if (corner.X > bounds.BRX || bounds.BRX < 0.0f)
                    bounds.BRX = corner.X;
                if (corner.Y > bounds.BRY || bounds.BRY < 0.0f)
                    bounds.BRY = corner.Y;
            }
            building->GetPhysicsTC()->GetBoundingInfo(boundsCentre, boundsRadius);
            C2DBoxF buildingBox(boundsCentre.X - boundsRadius, boundsCentre.Y - boundsRadius,
                                boundsCentre.X + boundsRadius, boundsCentre.Y + boundsRadius);
            buildingBox.ClipTo(regionBox);
            C2DCoordI tgaTopLeft;
            C2DCoordI tgaBottomRight;
            GetTGAPositionFromWorldCoordinates(C3DVector(buildingBox.TLX, buildingBox.TLY, 0.0f), tgaTopLeft);
            GetTGAPositionFromWorldCoordinates(C3DVector(buildingBox.BRX, buildingBox.BRY, 0.0f), tgaBottomRight);
            if (tgaTopLeft.X > 0)
                --tgaTopLeft.X;
            if (tgaTopLeft.Y > 0)
                --tgaTopLeft.Y;
            if (tgaBottomRight.X < size)
                ++tgaBottomRight.X;
            if (tgaBottomRight.Y < size)
                ++tgaBottomRight.Y;
            for (position.Y = tgaTopLeft.Y; position.Y < tgaBottomRight.Y; ++position.Y) {
                for (position.X = tgaTopLeft.X; position.X < tgaBottomRight.X; ++position.X) {
                    C3DVector world;
                    GetWorldCoordinatesFromTGAPosition(world, position);
                    world.Z = worldMap.GetGroundSizeZAt(world);
                    CMeshIntersectionData intersection;
                    if (mesh.GetPointIntersectionFromTo(C3DVector(world.X, world.Y, world.Z + 100.0f), C3DVector(world), intersection))
                        SetRegionMinimapThemeValue(position, (RegionMinimapTheme)8);
                }
            }
        }
    }
}
