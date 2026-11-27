#define FABLE_RETAIL_C3DVECTOR_NORMALISE_PAREN_SQUARES_ZYX
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_ROTATE_ANGLES
#define FABLE_RETAIL_C3DVECTOR_SUBTRACT_ASSIGN
// Leaf: ?CameraDoConversation@CGameScriptInterface@@UBEXABVCScriptThing@@0W4ECameraOp@@_N@Z
// Flags: /O2 /Oy
#define FABLE_RETAIL_CONVERSATION_CAMERA_PASS29
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CAIGAMECAMERABASE
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_C3DVECTOR_NORMALISE
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CCountedPointer.h"
#include "engine/CWorld.h"
#include "engine/CAIGameCameraBase.h"
#include "engine/CGameScriptInterface.h"
#include "engine/CScriptThing.h"

#include "engine/NGraphicAppearance_CTCGraphicAppearance.h"
#include "engine/CThingCreatureBase.h"

void CGameScriptInterface::CameraDoConversation(const CScriptThing& script_speaker, const CScriptThing& script_speakee, ECameraOp camera_op, bool wait) const
{
    if (!script_speaker.IsAlive())
        return;
    if (!script_speakee.IsAlive())
        return;
    if (!(script_speaker.GetPThing()->GetTypeFlags() & 8))
        return;
    if (!(script_speakee.GetPThing()->GetTypeFlags() & 8))
        return;
    CThingCreatureBase* tspeaker = static_cast<CThingCreatureBase*>(script_speaker.GetPThing());
    CThingCreatureBase* tspeakee = static_cast<CThingCreatureBase*>(script_speakee.GetPThing());
    float message_length_secs = 6.0f;
    switch (camera_op) {
    case CAM_OP_SPEAKER_LEFT_ZOOM_ON_SPEAKEE: {
        C3DVector start_track_pos = tspeaker->GetTCGraphicAppearance()->GetFocalPos(1.0f);
        C3DVector view_pos = tspeakee->GetTCGraphicAppearance()->GetFocalPos(1.0f);
        C3DVector facing_vect = tspeaker->GetPhysicsTC()->GetFacingVector();
        facing_vect.Z = 0.0f;
        facing_vect.Normalise();
        start_track_pos -= facing_vect * 0.5f;
        facing_vect.Rotate(0.75f, 0.0f);
        start_track_pos += facing_vect * 0.5f;
        C3DVector end_track_pos = start_track_pos + (view_pos - start_track_pos) * 0.5f;
        World->GetGameCamera()->ScriptMoveToPosAndLookAtPos(start_track_pos, view_pos, 0.0f);
        World->GetGameCamera()->ScriptStart();
        World->GetGameCamera()->ScriptMoveToPosAndLookAtPos(end_track_pos, view_pos, message_length_secs);
        World->GetGameCamera()->ScriptSetRotationSpeed(0.0f);
        World->GetGameCamera()->ScriptStart();
        break;
    }
    case CAM_OP_SPEAKER_RIGHT_ZOOM_ON_SPEAKEE: {
        C3DVector start_track_pos = tspeaker->GetTCGraphicAppearance()->GetFocalPos(1.0f);
        C3DVector view_pos = tspeakee->GetTCGraphicAppearance()->GetFocalPos(1.0f);
        C3DVector facing_vect = tspeaker->GetPhysicsTC()->GetFacingVector();
        facing_vect.Z = 0.0f;
        facing_vect.Normalise();
        start_track_pos -= facing_vect * 0.5f;
        facing_vect.Rotate(0.25f, 0.0f);
        start_track_pos += facing_vect * 0.5f;
        C3DVector end_track_pos = start_track_pos + (view_pos - start_track_pos) * 0.5f;
        World->GetGameCamera()->ScriptMoveToPosAndLookAtPos(start_track_pos, view_pos, 0.0f);
        World->GetGameCamera()->ScriptStart();
        World->GetGameCamera()->ScriptMoveToPosAndLookAtPos(end_track_pos, view_pos, message_length_secs);
        World->GetGameCamera()->ScriptSetRotationSpeed(0.0f);
        World->GetGameCamera()->ScriptStart();
        break;
    }
    case CAM_OP_SPEAKEE_LEFT_ZOOM_ON_SPEAKER: {
        C3DVector start_track_pos = tspeakee->GetTCGraphicAppearance()->GetFocalPos(1.0f);
        C3DVector view_pos = tspeaker->GetTCGraphicAppearance()->GetFocalPos(1.0f);
        C3DVector facing_vect = tspeakee->GetPhysicsTC()->GetFacingVector();
        facing_vect.Z = 0.0f;
        facing_vect.Normalise();
        start_track_pos -= facing_vect * 0.5f;
        facing_vect.Rotate(0.75f, 0.0f);
        start_track_pos += facing_vect * 0.5f;
        C3DVector end_track_pos = start_track_pos + (view_pos - start_track_pos) * 0.5f;
        World->GetGameCamera()->ScriptMoveToPosAndLookAtPos(start_track_pos, view_pos, 0.0f);
        World->GetGameCamera()->ScriptStart();
        World->GetGameCamera()->ScriptMoveToPosAndLookAtPos(end_track_pos, view_pos, message_length_secs);
        World->GetGameCamera()->ScriptSetRotationSpeed(0.0f);
        World->GetGameCamera()->ScriptStart();
        break;
    }
    case CAM_OP_SPEAKEE_RIGHT_ZOOM_ON_SPEAKER: {
        C3DVector start_track_pos = tspeakee->GetTCGraphicAppearance()->GetFocalPos(1.0f);
        C3DVector view_pos = tspeaker->GetTCGraphicAppearance()->GetFocalPos(1.0f);
        C3DVector facing_vect = tspeakee->GetPhysicsTC()->GetFacingVector();
        facing_vect.Z = 0.0f;
        facing_vect.Normalise();
        start_track_pos -= facing_vect * 0.5f;
        facing_vect.Rotate(0.25f, 0.0f);
        start_track_pos += facing_vect * 0.5f;
        C3DVector end_track_pos = start_track_pos + (view_pos - start_track_pos) * 0.5f;
        World->GetGameCamera()->ScriptMoveToPosAndLookAtPos(start_track_pos, view_pos, 0.0f);
        World->GetGameCamera()->ScriptStart();
        World->GetGameCamera()->ScriptMoveToPosAndLookAtPos(end_track_pos, view_pos, message_length_secs);
        World->GetGameCamera()->ScriptSetRotationSpeed(0.0f);
        World->GetGameCamera()->ScriptStart();
        break;
    }
    case CAM_OP_ZOOM_ON_SPEAKER_FACE: {
        C3DVector start_track_pos = tspeaker->GetTCGraphicAppearance()->GetFocalPos(1.0f);
        C3DVector view_pos = tspeaker->GetTCGraphicAppearance()->GetFocalPos(1.0f);
        C3DVector facing_vect = tspeaker->GetPhysicsTC()->GetFacingVector();
        facing_vect.Z = 0.0f;
        facing_vect.Normalise();
        start_track_pos += facing_vect * 0.75f;
        C3DVector end_track_pos = start_track_pos + (view_pos - start_track_pos) * 0.5f;
        World->GetGameCamera()->ScriptMoveToPosAndLookAtPos(start_track_pos, view_pos, 0.0f);
        World->GetGameCamera()->ScriptStart();
        World->GetGameCamera()->ScriptMoveToPosAndLookAtPos(end_track_pos, view_pos, message_length_secs);
        World->GetGameCamera()->ScriptSetRotationSpeed(0.0f);
        World->GetGameCamera()->ScriptStart();
        break;
    }
    case CAM_OP_ZOOM_ON_SPEAKEE_FACE: {
        C3DVector start_track_pos = tspeakee->GetTCGraphicAppearance()->GetFocalPos(1.0f);
        C3DVector view_pos = tspeakee->GetTCGraphicAppearance()->GetFocalPos(1.0f);
        C3DVector facing_vect = tspeakee->GetPhysicsTC()->GetFacingVector();
        facing_vect.Z = 0.0f;
        facing_vect.Normalise();
        start_track_pos += facing_vect * 0.75f;
        C3DVector end_track_pos = start_track_pos + (view_pos - start_track_pos) * 0.5f;
        World->GetGameCamera()->ScriptMoveToPosAndLookAtPos(start_track_pos, view_pos, 0.0f);
        World->GetGameCamera()->ScriptStart();
        World->GetGameCamera()->ScriptMoveToPosAndLookAtPos(end_track_pos, view_pos, message_length_secs);
        World->GetGameCamera()->ScriptSetRotationSpeed(0.0f);
        World->GetGameCamera()->ScriptStart();
        break;
    }
    }
}