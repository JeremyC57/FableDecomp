// NCameraModes::CLookAtFront::CLookAtFrontInit::Alloc
// Leaf: ?Alloc@CLookAtFrontInit@CLookAtFront@NCameraModes@@UBEPAVCBase@3@AAVCGameCameraManager@@ABV?$CDefPointer@$$CBVCCameraModeDef@@@@@Z
// Flags:
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_NCAMERAMODES_CBASE_CYLINDER
#define FABLE_RETAIL_NCAMERAMODES_CBASE_INLINE_ACCESSORS
#define FABLE_RETAIL_CCOUNTEDPOINTER
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_PACKED

#define FABLE_RETAIL_NCAMERAMODES_LOOKATFRONT_PASS27
#define FABLE_RETAIL_CGAMECAMERAMANAGER_LOOKAROUND
#define FABLE_RETAIL_CGAMECAMERAMANAGER_GETOBSERVEDTHING
#define FABLE_RETAIL_CCAMERAMODEDEF_CYLINDER
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CAIGAMECAMERABASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_PASS22_STATICFOCAL
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_C3DVECTOR_ORIGINAL_COMPONENT_CTOR
#define FABLE_RETAIL_C3DVECTOR_ORIGINAL_COPY_FORMAL
#define FABLE_RETAIL_CINTELLIGENTPOINTER_GET_ORIGINAL_CHAIN
#define FABLE_RETAIL_CDEFPOINTER_ORIGINAL_DIRECT_DEREF_PASS26
#include "rebuild_abi.h"
#include "fable_assert.h"
#include "engine/ThingAlive.h"
#include "engine/NCameraModes_CLookAtFront.h"
#include "engine/CCameraModeDef.h"
#include "engine/CGameCameraManager.h"
#include "engine/CThing.h"
#include "engine/CThingGameObject.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/NGraphicAppearance_CTCGraphicAppearance.h"

NCameraModes::CBase* NCameraModes::CLookAtFront::CLookAtFrontInit::Alloc(
        CGameCameraManager& cam_man, const CDefPointer<const CCameraModeDef>& cam_mode_def) const
{
    const CThingGameObject* pobs = cam_man.GetObservedThing();
    FABLE_RETAIL_ASSERT(IS_THING_ALIVE(pobs));

    C3DVector facing = pobs->GetPhysicsTC()->GetFacingVector();
    C3DVector start_pos = pobs->GetPos() + facing * cam_mode_def->StringLengthXY +
                         C3DVector(0.0f, 0.0f, cam_mode_def->HeightOffsetZ);
    C3DVector look_at = pobs->GetTCGraphicAppearance()->GetStaticFocalPos();

    return new CLookAtFront(cam_man, cam_mode_def, start_pos, look_at, true);
}

inline NCameraModes::CLookAtFront::CLookAtFront(CGameCameraManager& cam_man,
        const CDefPointer<const CCameraModeDef>& cam_mode_def,
        const C3DVector& start_pos, const C3DVector& look_at, bool allow_look_around)
    : CSimplePointBase(cam_man, cam_mode_def, start_pos, look_at, allow_look_around)
{}

// Same-TU constructor at 00873A10. Its visible body keeps the temporary vector slots reusable.
NCameraModes::CSimplePointBase::CSimplePointBase(CGameCameraManager& cam_man,
        const CDefPointer<const CCameraModeDef>& cam_mode_def,
        const C3DVector& start_pos, const C3DVector& look_at, bool allow_look_around)
    : CBase(cam_man, cam_mode_def), Position(start_pos), LookAtPos(look_at),
      AllowLookAround(allow_look_around)
{}