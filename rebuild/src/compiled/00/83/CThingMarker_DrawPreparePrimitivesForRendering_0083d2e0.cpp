// Flags: (none)
// Leaf: ?DrawPreparePrimitivesForRendering@CThingMarker@@UAE_NABVCEnginePrimitivePrepareInfo@@@Z
#define FABLE_RETAIL_CTHINGMARKER_DRAW_PASS29
#define FABLE_RETAIL_CTHINGPHYSICAL_DEBUGDRAW_PASS24
#define FABLE_RETAIL_CTHING_GETTC_DONOR_NAMES_PASS23
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CRIGHTHANDEDSET
#define FABLE_RETAIL_C3DPOSITIONINTERPOLATOR
#define FABLE_RETAIL_C3DPOSITIONINTERPOLATOR_BASE
#define FABLE_RETAIL_C3DPOSITIONINTERPOLATOR_IMPLICIT_DTOR
#define FABLE_RETAIL_C3DORIENTATIONINTERPOLATOR_IMPLICIT_DTOR
#define FABLE_RETAIL_CENGINEPRIMITIVESTATICMESH_PASS22
#define FABLE_RETAIL_CENGINEPRIMITIVE_ID_CTOR
#define FABLE_RETAIL_CENGINEPRIMITIVESTATICMESH_INLINE_CTOR
#define FABLE_RETAIL_CENGINEPRIMITIVESTATICMESH_CTOR_CALLED_PASS26
#define FABLE_RETAIL_CENGINEPRIMITIVEPREPAREINFO
#define FABLE_RETAIL_CENGINEPRIMITIVE_PASS22
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_EDITOR
#define FABLE_RETAIL_CIENGINE
#define FABLE_RETAIL_CIENGINE_RENDER
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTCPHYSICSBASE_INTERPOLATED_POSITION
#define FABLE_RETAIL_CTCPHYSICSBASE_LASTPOSITION
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_MOREACCESS
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFPOINTER_RELEASE_SELFDELETING_POINTEE
#define FABLE_RETAIL_CIVCOUNTEDPOINTEEBASE_RELEASE_DELETES
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY
#define FABLE_RETAIL_CDEFPOINTER_SETDYNAMICCAST_REREAD   // NF346: GetDef<T> copies = retail (the re-reading SetDynamicCast, owner 0x0066C340; as 0x0066B370)
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CRESOURCE
#define FABLE_RETAIL_CTHINGBASEDEF
#define FABLE_RETAIL_CTHINGMARKER_DEFINITION
#define FABLE_RETAIL_CAPPEARANCEDEF_COMPLETE
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_BODY
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_CAPPEARANCEDEF_SIZE_BODY   // NF357: GetSubDef<CAppearanceDef> pick-any copy = size-mode owner 0x0042B0A2 (scaffolding, not retail)
#define FABLE_RETAIL_CENGINEMESHEFFECT
class CEnginePrimitivePrepareInfo;
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CThingBaseDef.h"
#include "engine/CAppearanceDef.h"
#include "engine/CThingMarker.h"

#include "engine/CEnginePrimitivePrepareInfo.h"
#include "engine/C3DPositionInterpolator.h"
#include "engine/C3DOrientationInterpolator.h"
#include "engine/EEngineInterpolationSet.h"
#include "engine/CEnginePrimitiveStaticMesh.h"
#include "engine/CIEngine.h"
#include "engine/CMainGameComponent.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/NGraphicAppearance_CTCGraphicAppearance.h"

bool CThingMarker::DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo& info)
{
    if (CThingPhysical::DrawPreparePrimitivesForRendering(info)) {
    if (PeekComponent().IsEditorActive()) {
        CEngineGraphic def_graphic = GetCurrentGraphic();
        info.Engine.AddPrimitive(CEnginePrimitiveStaticMesh(GetUniqueID(),def_graphic.BankIndex,
            GetPhysicsTC()->GetPosition(info.Interpolate.Current.WFInterpolate),
            GetPhysicsTC()->GetRHSet(info.Interpolate.Current.WFInterpolate),
            Scale * def_graphic.RenderSizeX,true,255,CRGBColour(255,255,255,255),0,ENGINE_INTERPOLATION_SET_DEFAULT,CEnginePrimitiveThemeChannelDesc(),true));
    }
    NGraphicAppearance::CTCGraphicAppearance* ptc_appearance = GetTCGraphicAppearance();
    if (ptc_appearance) {
        C3DPositionInterpolator interpolated_pos(GetPhysicsTC()->GetLastPosition(),GetPhysicsTC()->GetPosition());
        C3DOrientationInterpolator interpolated_rhs(GetPhysicsTC()->GetLastRHSet(),GetPhysicsTC()->GetRHSet());
        ptc_appearance->Draw(info,interpolated_pos,interpolated_rhs);
    }
    return true;
    }
    return false;
}

CEngineGraphic CThingMarker::GetCurrentGraphic() const
{
    CDefPointer<const CAppearanceDef> appearance;
    if (PBaseDef->GetSubDef(&appearance) && appearance->Graphic.BankIndex > 0)
        return appearance->Graphic;
    return PeekDefinition()->Graphic;
}