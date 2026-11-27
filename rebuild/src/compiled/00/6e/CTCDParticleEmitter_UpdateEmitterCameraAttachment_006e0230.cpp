// 006e0230 CTCDParticleEmitter::UpdateEmitterCameraAttachment
// Leaf: ?UpdateEmitterCameraAttachment@CTCDParticleEmitter@@AAEX_N@Z
// Flags: none
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_EMITTER_CAMERA_ATTACHMENT_PASS29
#define FABLE_RETAIL_CTCBOASTINGCROWDCONTROL
#define FABLE_RETAIL_CTCDPARTICLEEMITTER_FULL
#define FABLE_RETAIL_CPARTICLEEMITTER
#define FABLE_RETAIL_CPARTICLEEMITTER_STARTFRAME_PASS23
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCBASE_COMPONENT
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CRIGHTHANDEDSET
#define FABLE_RETAIL_CQUATERNION_PARTICLE_UPDATE
#define FABLE_RETAIL_CCAMERA_UP_DIRECTION
#define FABLE_RETAIL_CAIGAMECAMERABASE
#define FABLE_RETAIL_C3DPOSITIONINTERPOLATOR
#define FABLE_RETAIL_C3DPOSITIONINTERPOLATOR_BASE
#define FABLE_RETAIL_C3DPOSITIONINTERPOLATOR_IMPLICIT_DTOR
#define FABLE_RETAIL_C3DORIENTATIONINTERPOLATOR_IMPLICIT_DTOR
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_CVECTORMAP_INSERT
#define FABLE_RETAIL_STLP_SORT_FASTCALL
#include "engine/ETCInterfaceType.h"
#include "engine/CParticleEmitter.h"
#include "engine/CTCDParticleEmitter.h"
#include "engine/CCamera.h"
#include "engine/CAIGameCameraBase.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CThing.h"
#include "engine/CWorld.h"
void CTCDParticleEmitter::UpdateEmitterCameraAttachment(bool reset)
{
    if (PEmitter != 0 || WaitingForParticleToLoad) {
        CCamera current_camera = GetWorld().GetGameCamera()->GetRenderCamera(1.0f);
        C3DVector new_pos = current_camera.PeekWorldPos() + current_camera.PeekViewDirection() * 4.0f;
        CRightHandedSet new_rhs;
        if (AttachmentFlags & AF_USE_THING_ORIENTATION) {
            new_rhs.Init(current_camera.PeekUpDirection(), current_camera.PeekViewDirection());
        } else if (PEmitter != 0) {
            new_rhs = PEmitter->GetCurrentOrientation();
        } else {
            new_rhs = PCreationParameters->RHSet;
        }

        if (ResetEmitterPosition || ResetEmitterOrientation) {
            if (AttachmentFlags & AF_USE_OFFSET)
                new_pos.Z += FloatOffset;
            if (ResetEmitterPosition) {
                GetParentThing().GetPhysicsTC()->SetPosition(new_pos);
                if (PEmitter != 0)
                    PEmitter->SetPosition(C3DPositionInterpolator(reset ? new_pos : PEmitter->PeekEndPos(), new_pos));
                ResetEmitterPosition = false;
            }
            if (ResetEmitterOrientation) {
                if (PEmitter != 0)
                    PEmitter->SetOrientation(C3DOrientationInterpolator(reset ? new_rhs : PEmitter->PeekEndOrientation(), new_rhs));
                else
                    PCreationParameters->RHSet = new_rhs;
                ResetEmitterOrientation = false;
            }
        }
    }
}