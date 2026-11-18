// CThingPhysical::DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo&) -- for a drawable thing (slot
// Grade: DIFFER(865v865) with 23 strict instruction hunks.
// Leaf: ?DrawPreparePrimitivesForRendering@CThingPhysical@@UAE_NABVCEnginePrimitivePrepareInfo@@@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x006A64D0, `ret 4`.

#define FABLE_RETAIL_CTHINGPHYSICAL_DEBUGDRAW_PASS24
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_ORIGINAL_VTABLE
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGPHYSICAL_ORIGINAL_VTABLE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CCOUNTEDPOINTER
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CTHING_MOREACCESS
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHING_CTCGRAPHICAPPEARANCE
#define FABLE_RETAIL_CTHING_PEEKCOMPONENT
#define FABLE_RETAIL_CTHING_GETWORLD
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_STATIC_FOCAL
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_MAINFONT
#define FABLE_RETAIL_CTCCREATUREGENERATOR
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CIVCOUNTEDPOINTEEBASE
#define FABLE_RETAIL_CFONTBANK_HEIGHT
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_CIENGINE
#define FABLE_RETAIL_CIENGINE_RENDER
#define FABLE_RETAIL_CFONTBANK
#include "fable_string.h"
#include "engine/CThingPhysical.h"
#include "engine/CThingDrawInformation.h"
#include "engine/CThingPhysicalDebugInfo.h"
#include "engine/NGraphicAppearance_CTCGraphicAppearance.h"
#include "engine/CWorld.h"
#include "engine/CMainGameComponent.h"
#include "engine/CTCCreatureGenerator.h"
#include "engine/CFontBank.h"
#include "engine/CIEngine.h"
#include "engine/CEnginePrimitivePrepareInfo.h"
#include "engine/CEnginePrimitiveText.h"
#include "engine/EEngineInterpolationSet.h"

bool FABLE_FASTCALL GFWorld3DDistanceSmallerThan(const C3DVector& a, const C3DVector& b, float distance);
extern bool g_DrawThingDebugState;

bool CThingPhysical::DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo& info)
{
    if (!DrawIsDrawable() || IsHidden() || !CThing::DrawPreparePrimitivesForRendering(info) || info.Edit2DEngine)
        return false;
    if (DrawIsDisplayingInformation() && GFWorld3DDistanceSmallerThan(GetWorld().GetGameLODCentre(), GetPos(), 48.0f)) {
        CThingDrawInformation thing_info;
        C3DVector next_pos;
        if (GetTCGraphicAppearance()->GetStaticFocalPosIfLoaded(next_pos)) {
            DrawGetThingDisplayInformation(&thing_info);
            if (thing_info.StringOn) {
                CCharString str(thing_info.DrawString);
                info.Engine->AddPrimitive(CEnginePrimitiveText(GetUniqueID(), next_pos, 1.0f,
                    str.ToWideString(), PeekComponent().PeekMainFont(), CRGBColour(0, 255, 0, 255), false, ENGINE_INTERPOLATION_SET_0));
                next_pos.Z += PeekComponent().PeekDebugFont().GetFontHeight() * 0.2f;
            }
        }
    }
    CTCCreatureGenerator* pTCCreatureGen;
    if (GetTC(TCI_CREATURE_GENERATOR, &pTCCreatureGen))
        pTCCreatureGen->DrawDebugInfo(info);
    if (g_DrawThingDebugState) {
        CThingPhysicalDebugInfo physics_info;
        if (DrawGetPhysicsDebugInfo(physics_info)) {
            C3DVector next_pos = GetTCGraphicAppearance()->GetFocalPos(1.0f);
            next_pos.Z += 1.5f;
            CCharString str;
            str += physics_info.Moving ? "M " : "";
            str += physics_info.OnGround ? "g" : "";
            str += physics_info.OnMesh ? "m" : "";
            str += physics_info.Collidable ? "c" : "";
            str += physics_info.Interactable ? "i" : "";
            str += physics_info.Immoveable ? "r" : "";
            info.Engine->AddPrimitive(CEnginePrimitiveText(GetUniqueID(), next_pos, 1.0f,
                str.ToWideString(), PeekComponent().PeekMainFont(), CRGBColour(0, 255, 0, 255), false, ENGINE_INTERPOLATION_SET_0));
            next_pos.Z += PeekComponent().PeekDebugFont().GetFontHeight() * 0.2f;
        }
    }
    return true;
}
