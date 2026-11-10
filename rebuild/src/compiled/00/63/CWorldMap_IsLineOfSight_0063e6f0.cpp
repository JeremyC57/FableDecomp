// CWorldMap::IsLineOfSight(const CThing& from, const CThing& to, bool checkThings) const -- line of sight between the eye points of two things (creature head, graphic focal point, else physics position): both in the same nonzero map and the terrain line clear; optionally gathers the game objects in the bounding box
// Leaf: ?IsLineOfSight@CWorldMap@@QBE_NABVCThing@@0_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0063E6F0, 599 bytes, thiscall, `ret 0Ch`.
// Identity: Ego_r.pdb 0x006EB120 / FableWin.pdb 0x024A59C0 ?IsLineOfSight@CWorldMap@@QBE_NABVCThing@@0_N@Z; the C3DVector overload
//   (Ego_r 0x006E8890) is the 0x0063AEE0 callee.
// Callees: CThingCreatureBase::GetHeadPos(bool) const 0x00661B40; NGraphicAppearance::CTCGraphicAppearance::GetFocalPos(float)
//   const 0x004C1060; GetMapNumberAt(const C3DVector&) vtable +0x40; ConvertWorldMapCoordsToLocalCoords(long, const C3DVector&)
//   0x004FC2A0 (results unused); IsLineOfSight(const C3DVector&, const C3DVector&) 0x0063AEE0; CWorld::PeekThingManager() slot 4
//   (+0x10); CThingManager::PeekThingSearchTools() 0x0051E480; GetAllThingsInRoughArea<CThingGameObject, list, CFilter_Bind2<
//   CThingFilter_IsNot, CThingFilter_IsNot> > 0x0063FAE0; the list destructor 0x0063F360.
// Retail quirk: for the SECOND thing the focal-point and physics-position branches store into the FIRST position (a copy-paste
//   slip in the original source, kept).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_STATTYPEFLAGS
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGGAMEOBJECT_STATTYPEFLAGS
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGCREATUREBASE_HEADPOS
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ROUGHAREABODY
#define FABLE_RETAIL_CTCMAPWHO
#define FABLE_RETAIL_CTCMAPWHO_SEARCHID
#define FABLE_RETAIL_CCLASSFACTORY
#define FABLE_RETAIL_CTHINGMANAGER
#define FABLE_RETAIL_CTHINGMANAGER_ACCESSORS
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLDMAP
#define FABLE_RETAIL_CWORLDMAP_LINEOFSIGHT
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_LIST_DTOR_OUTOFLINE
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/C2DBoxI.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CThingCreatureBase.h"
#include "engine/NGraphicAppearance_CTCGraphicAppearance.h"
#include "engine/CWorld.h"
class CTCMapwho;                                                    // engine/CTCMapwho.h names it in NThingMap::CMapwhoNode first
#include "engine/CTCMapwho.h"
#include "engine/NThingMap_CThingMap.h"
#include "engine/CThingManager.h"
#include "engine/CThingSearchTools.h"
#include "engine/CFilter_Bind2.h"
#include "engine/CThingFilter_IsNot.h"
#include "engine/CWorldMap.h"
#include "stlport/fable_containers.h"

bool CWorldMap::IsLineOfSight(const CThing& from, const CThing& to, bool checkThings) const
{
    const CTCPhysicsBase* fromPhysics = from.GetPhysicsTC();
    const CTCPhysicsBase* toPhysics = to.GetPhysicsTC();
    C3DVector fromPos;
    C3DVector toPos;
    if (from.GetTypeFlags() & 8)
        fromPos = static_cast<const CThingCreatureBase&>(from).GetHeadPos(true);
    else if (from.GetTCGraphicAppearance())
        fromPos = from.GetTCGraphicAppearance()->GetFocalPos(1.0f);
    else
        fromPos = fromPhysics->GetPosition();
    if (to.GetTypeFlags() & 8)
        toPos = static_cast<const CThingCreatureBase&>(to).GetHeadPos(true);
    else if (to.GetTCGraphicAppearance())
        fromPos = to.GetTCGraphicAppearance()->GetFocalPos(1.0f);
    else
        fromPos = toPhysics->GetPosition();

    long fromMap = GetMapNumberAt(fromPos);
    long toMap = GetMapNumberAt(toPos);
    if (fromMap == 0 || fromMap != toMap)
        return false;
    C3DVector fromLocal = ConvertWorldMapCoordsToLocalCoords(fromMap, fromPos);
    C3DVector toLocal = ConvertWorldMapCoordsToLocalCoords(fromMap, toPos);
    if (!IsLineOfSight(fromPos, toPos))
        return false;
    if (checkThings)
    {
        const CThingManager& thingManager = World.PeekThingManager();
        C2DBoxI area;
        area.TLX = (long)(fromPos.X < toPos.X ? fromPos.X : toPos.X);
        area.TLY = (long)(fromPos.Y < toPos.Y ? fromPos.Y : toPos.Y);
        area.BRX = (long)(fromPos.X > toPos.X ? fromPos.X : toPos.X);
        area.BRY = (long)(fromPos.Y > toPos.Y ? fromPos.Y : toPos.Y);
        std::list<const CThingGameObject*> things;
        thingManager.PeekThingSearchTools().GetAllThingsInRoughArea<CThingGameObject>(area,
            CFilter_Bind2<CThingFilter_IsNot, CThingFilter_IsNot>(CThingFilter_IsNot(from), CThingFilter_IsNot(to)), things);
        for (std::list<const CThingGameObject*>::iterator it = things.begin(); it != things.end(); ++it)
        {
        }
    }
    return true;
}