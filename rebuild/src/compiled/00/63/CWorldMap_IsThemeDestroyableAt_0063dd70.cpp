// CWorldMap::IsThemeDestroyableAt(const C3DVector& pos) const -- on a loaded map, look up the strongest engine theme at the position and report whether its def names a destruction theme
// Leaf: ?IsThemeDestroyableAt@CWorldMap@@QBE_NABVC3DVector@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0063DD70. Name: Ego_r.pdb 0x006EAC20.
// Callees: GetMapNumberAt (virtual slot 16) / IsMapLoaded (virtual slot 10), GetStrongestEngineThemeAt(const C3DVector&, long&) const
//   0x0063ADF0, PeekDefinitionManager 0x004FB310, CDefinitionManager::GetDef<CEngineThemeDef>(long, CDefPointer<const CEngineThemeDef>*)
//   const 0x0063F3A0 (FableWin.pdb ??$GetDef@VCEngineThemeDef@@@CDefinitionManager@@QBE_NJPAV?$CDefPointer@$$CBVCEngineThemeDef@@@@@Z),
//   the def's release (vtable slot 1).
// Layout: CEngineThemeDef (Ego_r.pdb; only engine/_quarantine has a header) DestructionThemeDef +0x98, modelled on the shared
//   CDefClassBase as the landed 0x0066CF00 does.
// NF41 (2026-11-16): was a local CDefPointeeBase / CDefPointer / CDefinitionManager / CWorldMap model with a stand-in GetDef(long,
//   CDefPointer&) body; now the include chain of the landed 0x0066CF00 (engine/CWorldMap.h + the NF41 opt-in
//   FABLE_RETAIL_CWORLDMAP_ISTHEMEDESTROYABLE, engine/CDefinitionManager.h GETDEF_GLOBAL_BODY).
#define FABLE_RETAIL_CWORLDMAP_ISTHEMEDESTROYABLE
#define FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFPOINTER_SETDYNAMICCAST_REREAD
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGBUILDING
#define FABLE_RETAIL_CTHINGBUILDING_TYPEFLAGS
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
#define FABLE_RETAIL_C2DBOXI
#define FABLE_RETAIL_C2DBOXF
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

class CEngineThemeDef : public CDefClassBase {                       // Ego_r.pdb CEngineThemeDef (no shared header: engine/_quarantine only)
public:
    unsigned char _pad_0x25[0x98 - 0x25];
    long DestructionThemeDef;                                        // +0x98
};

bool CWorldMap::IsThemeDestroyableAt(const C3DVector& pos) const
{
    long map = GetMapNumberAt(pos);
    if (map != 0 && IsMapLoaded(map)) {
        long layer;
        long theme = GetStrongestEngineThemeAt(pos, layer);
        CDefPointer<const CEngineThemeDef> def;
        PeekDefinitionManager().GetDef(theme, &def);
        return def->DestructionThemeDef != 0;
    }
    return false;
}
