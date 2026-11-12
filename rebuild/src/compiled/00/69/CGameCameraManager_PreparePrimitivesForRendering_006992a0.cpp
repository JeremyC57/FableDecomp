// CGameCameraManager::PreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo& info) const -- the camera manager's debug drawing:
//   modes 1/3 let the current camera mode draw itself; modes 2/3 put a 0.25 pyramid on every thing carrying TCI_DRIVER_CAMERA_POINTS
//   (and let its shape manager component, TCI_SHAPE_MANAGER, draw), then draw a 0.05-wide line from the camera position to each debug point.
// Leaf: ?PreparePrimitivesForRendering@CGameCameraManager@@UBEXABVCEnginePrimitivePrepareInfo@@@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x006992A0, 506 bytes, `ret 4`; slot 2 of the CGameCameraManager vtable 0x0125C754. Name: Ego_r.pdb
//   0x006C0240 + FableWin.pdb 0x02202DA0 (same mangled name).
// Callees: malloc 0x00BFEA0E / free 0x00BFEA14 (inline STLport list<CThing*> ctor/dtor);
//   CThingSearchTools::GetAllThings<CThing, list<CThing*>, CThingFilter_HasTC> 0x004ACD70 (template body visible, called out of line;
//   it calls CThingFilter_HasTC::operator() 0x006A80A0 out of line); CThing::GetPos 0x004C73D0; NCameraModes::DrawPyramid 0x008764E0
//   (__fastcall); CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound 0x0040F020 (inside the expanded CThing::GetTC);
//   virtual: NCameraModes::CBase slot 5 PreparePrimitivesForRendering, CTCBase slot 11 DrawPreparePrimitivesForRendering,
//   CIEngine slot 20 AddPrimitive.
// Layout: World +0x74 (CWorld::PThingSearchTools_ptr +0x20), CurrentMode +0x98 (CBase::GoTo.Position +0x54, inline GetPosition),
//   DrawDebugGraphics +0x154, DebugPoints +0x15C (list<pair<C3DVector,CRGBColour> >); info.Engine +0x34.
// Shape: the GetAllThings body is visible to the compiler (so the list does not escape and is homed in the dead `info` argument slot)
//   while the filter's operator() is not (so the filter temp's store stays before the call setup); each debug point is copied into
//   a local pair (End.Z and the colour pass through the 16-byte temp); the CEnginePrimitiveLine constructor is expanded in place.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CGAMECAMERAMANAGER
#define FABLE_RETAIL_CGAMECAMERAMANAGER_ANGLES
#define FABLE_RETAIL_CGAMECAMERAMANAGER_DEBUGPOINTS
#define FABLE_RETAIL_NCAMERAMODES_CBASE_CYLINDER
#define FABLE_RETAIL_NCAMERAMODES_CBASE_GETPOSITION_INLINE
#define FABLE_RETAIL_CENGINEPRIMITIVEPREPAREINFO
#define FABLE_RETAIL_CTHINGGAMEOBJECT_ONHIT
#define FABLE_RETAIL_CENGINEPRIMITIVE_ID_CTOR
#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ALLTHINGSBODY
#define FABLE_RETAIL_CTHINGFILTER_HASTC_OUTOFLINE
#define FABLE_RETAIL_CIENGINE
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CWorld.h"
#include "engine/CThingSearchTools.h"
#include "engine/CThingFilter_HasTC.h"
#include "engine/CEnginePrimitivePrepareInfo.h"
#include "engine/CEnginePrimitiveLine.h"
#include "engine/CIEngine.h"
#include "engine/NCameraModes_CBase.h"
#include "engine/CGameCameraManager.h"

namespace NCameraModes {
void __fastcall DrawPyramid(const CEnginePrimitivePrepareInfo& info, const C3DVector& pos, float size);   // 0x008764E0
}

void CGameCameraManager::PreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo& info) const
{
    if (DrawDebugGraphics == 0)
        return;
    if (DrawDebugGraphics == 1 || DrawDebugGraphics == 3)
        CurrentMode->PreparePrimitivesForRendering(info);
    if (DrawDebugGraphics == 2 || DrawDebugGraphics == 3) {
        std::list<CThing*> things;
        World->PeekThingSearchTools().GetAllThings<CThing>(CThingFilter_HasTC(TCI_DRIVER_CAMERA_POINTS), things);
        for (std::list<CThing*>::iterator it = things.begin(); it != things.end(); ++it) {
            CThing* thing = *it;
            NCameraModes::DrawPyramid(info, thing->GetPos(), 0.25f);
            CTCBase* shapes;
            if (thing->GetTC(TCI_SHAPE_MANAGER, &shapes))
                shapes->DrawPreparePrimitivesForRendering(info);
        }
        C3DVector pos = CurrentMode->GetPosition();
        for (std::list<std::pair<C3DVector, CRGBColour> >::const_iterator p = DebugPoints.begin(); p != DebugPoints.end(); ++p) {
            std::pair<C3DVector, CRGBColour> point = *p;
            info.Engine.AddPrimitive(CEnginePrimitiveLine(0, pos, point.first, point.second, 0.05f, ENGINE_PRIMITIVE_BLEND_SOLID, 0, true,
                                                          ENGINE_INTERPOLATION_SET_0, ENGINE_PRIMITIVE_BLEND_OP_ADD));
        }
    }
}