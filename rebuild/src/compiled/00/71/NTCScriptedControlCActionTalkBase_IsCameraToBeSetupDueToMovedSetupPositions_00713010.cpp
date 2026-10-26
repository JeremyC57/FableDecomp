// NTCScriptedControl::CActionTalkBase::IsCameraToBeSetupDueToMovedSetupPositions(const CSubtitledMessage::CCameraTag&) -- compare the current looking-from/looking-at positions against the camera-tag setup positions; retail length 0x12e bytes, helper @ 0x00712340 and tolerance globals @ 0x012654e8/0x012654ec.
#include <math.h>

class CThing;
class C3DVector { public: float x, y, z; };
namespace NTCScriptedControl {
struct CSubtitledMessage { struct CCameraTag; };

struct CActionTalkBase {
    unsigned char _pad_0x00[0x74];
    C3DVector ThingLookingFromPos; // PDB +0x78; native +0x74 (four-byte smaller CArray)
    C3DVector ThingLookingAtPos;   // PDB +0x84; native +0x80
    bool ThingLookingPosSet;       // native +0x8c
    bool ParentIsDemonDoor;        // native +0x8d
    bool GettingUp;                // native +0x8e
    bool IsCameraToBeSetupDueToMovedSetupPositions(const CSubtitledMessage::CCameraTag&) const;
    void GetCameraSetupPositions(const CSubtitledMessage::CCameraTag&, const CThing*&, C3DVector&,
                                 const CThing*&, C3DVector&) const; // 0x00712340
};
}

extern const float CameraSetupVerticalTolerance;   // 0x012654e8, 0.25f
extern const float CameraSetupHorizontalTolerance; // 0x012654ec, 0.8f

bool NTCScriptedControl::CActionTalkBase::IsCameraToBeSetupDueToMovedSetupPositions(
    const CSubtitledMessage::CCameraTag& tag) const
{
    if (ParentIsDemonDoor || !ThingLookingPosSet || !GettingUp)
        return false;

    const CThing* cameraThing = 0;
    C3DVector cameraPosition = { 0.0f, 0.0f, 0.0f };
    const CThing* targetThing = 0;
    C3DVector targetPosition = { 0.0f, 0.0f, 0.0f };
    GetCameraSetupPositions(tag, cameraThing, cameraPosition, targetThing, targetPosition);

    C3DVector targetOffset;
    targetOffset.x = ThingLookingAtPos.x - targetPosition.x;
    targetOffset.y = ThingLookingAtPos.y - targetPosition.y;
    targetOffset.z = ThingLookingAtPos.z - targetPosition.z;
    C3DVector cameraOffset;
    cameraOffset.x = ThingLookingFromPos.x - cameraPosition.x;
    cameraOffset.y = ThingLookingFromPos.y - cameraPosition.y;
    cameraOffset.z = ThingLookingFromPos.z - cameraPosition.z;

    if (fabsf(cameraOffset.z) > CameraSetupVerticalTolerance ||
        fabsf(targetOffset.z) > CameraSetupVerticalTolerance)
        return true;
    if (sqrtf(targetOffset.y * targetOffset.y + targetOffset.x * targetOffset.x) > CameraSetupHorizontalTolerance)
        return true;
    if (!(sqrtf(cameraOffset.y * cameraOffset.y + cameraOffset.x * cameraOffset.x) > CameraSetupHorizontalTolerance))
        return false;
    return true;
}