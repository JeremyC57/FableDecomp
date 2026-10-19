// NTCScriptedControl::CActionTalkBase::IsCameraToBeSetupDueToMovedSetupPositions(const CSubtitledMessage::CCameraTag&) -- compare the current looking-from/looking-at positions against the camera-tag setup positions; retail length 0x12e bytes, helper @ 0x00712340 and tolerance globals @ 0x012654e8/0x012654ec.
#include <math.h>

struct CThing;
struct C3DVector { float x, y, z; };
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

#include <stddef.h>
#include <stdio.h>

struct CThing { int id; };
namespace NTCScriptedControl {
struct CSubtitledMessage::CCameraTag {
    const CThing* cameraThing;
    const CThing* targetThing;
    C3DVector cameraPosition;
    C3DVector targetPosition;
    const CThing* seenCameraThing;
    const CThing* seenTargetThing;
    int calls;
};
}

const float CameraSetupVerticalTolerance = 0.25f;
const float CameraSetupHorizontalTolerance = 0.8f;

void NTCScriptedControl::CActionTalkBase::GetCameraSetupPositions(
    const CSubtitledMessage::CCameraTag& tag, const CThing*& cameraThing, C3DVector& cameraPosition,
    const CThing*& targetThing, C3DVector& targetPosition) const
{
    NTCScriptedControl::CSubtitledMessage::CCameraTag& writable =
        const_cast<NTCScriptedControl::CSubtitledMessage::CCameraTag&>(tag);
    ++writable.calls;
    cameraThing = writable.cameraThing;
    cameraPosition = writable.cameraPosition;
    targetThing = writable.targetThing;
    targetPosition = writable.targetPosition;
    writable.seenCameraThing = cameraThing;
    writable.seenTargetThing = targetThing;
}

static int check(bool ok, const char* name)
{
    if (!ok) { printf("FAIL %s\n", name); return 1; }
    return 0;
}

static void set_vector(C3DVector& v, float x, float y, float z)
{
    v.x = x; v.y = y; v.z = z;
}

int main()
{
    typedef char from_offset[(offsetof(NTCScriptedControl::CActionTalkBase, ThingLookingFromPos) == 0x74) ? 1 : -1];
    typedef char at_offset[(offsetof(NTCScriptedControl::CActionTalkBase, ThingLookingAtPos) == 0x80) ? 1 : -1];
    typedef char set_offset[(offsetof(NTCScriptedControl::CActionTalkBase, ThingLookingPosSet) == 0x8c) ? 1 : -1];
    typedef char demon_door_offset[(offsetof(NTCScriptedControl::CActionTalkBase, ParentIsDemonDoor) == 0x8d) ? 1 : -1];
    typedef char getting_up_offset[(offsetof(NTCScriptedControl::CActionTalkBase, GettingUp) == 0x8e) ? 1 : -1];
    (void)sizeof(from_offset); (void)sizeof(at_offset); (void)sizeof(set_offset);
    (void)sizeof(demon_door_offset); (void)sizeof(getting_up_offset);

    NTCScriptedControl::CActionTalkBase action = {};
    NTCScriptedControl::CSubtitledMessage::CCameraTag tag = {};
    tag.calls = 0;
    tag.seenCameraThing = 0;
    tag.seenTargetThing = 0;
    CThing camera = { 1 }, target = { 2 };
    tag.cameraThing = &camera;
    tag.targetThing = &target;
    action.ThingLookingPosSet = true;
    action.GettingUp = true;
    set_vector(action.ThingLookingFromPos, 1.0f, 0.0f, 0.0f);
    set_vector(action.ThingLookingAtPos, 2.0f, 0.0f, 0.0f);
    set_vector(tag.cameraPosition, 1.0f, 0.0f, 0.0f);
    set_vector(tag.targetPosition, 2.0f, 0.0f, 0.0f);

    int failures = 0;
    action.ParentIsDemonDoor = true;
    failures += check(!action.IsCameraToBeSetupDueToMovedSetupPositions(tag) && tag.calls == 0,
                      "demon door gate precedes helper");
    action.ParentIsDemonDoor = false;
    failures += check(!action.IsCameraToBeSetupDueToMovedSetupPositions(tag) && tag.calls == 1,
                      "matching nonzero from/at outputs");
    failures += check(tag.seenCameraThing == &camera && tag.seenTargetThing == &target,
                      "helper output pointer order");

    action.ThingLookingAtPos.x = 2.8f;
    failures += check(!action.IsCameraToBeSetupDueToMovedSetupPositions(tag),
                      "strict horizontal equality does not move camera");
    action.ThingLookingAtPos.x = 2.81f;
    failures += check(action.IsCameraToBeSetupDueToMovedSetupPositions(tag),
                      "at position beyond horizontal tolerance");
    action.ThingLookingAtPos.x = 2.0f;

    action.ThingLookingFromPos.z = 0.25f;
    failures += check(!action.IsCameraToBeSetupDueToMovedSetupPositions(tag),
                      "strict vertical equality does not move camera");
    action.ThingLookingFromPos.z = 0.251f;
    failures += check(action.IsCameraToBeSetupDueToMovedSetupPositions(tag),
                      "from position beyond vertical tolerance");
    action.ThingLookingFromPos.z = 0.0f;

    action.ThingLookingPosSet = false;
    int calls = tag.calls;
    failures += check(!action.IsCameraToBeSetupDueToMovedSetupPositions(tag) && tag.calls == calls,
                      "unset position gate precedes helper");
    action.ThingLookingPosSet = true;
    action.GettingUp = false;
    calls = tag.calls;
    failures += check(!action.IsCameraToBeSetupDueToMovedSetupPositions(tag) && tag.calls == calls,
                      "getting-up gate precedes helper");
    action.GettingUp = true;

    volatile float zero = 0.0f;
    action.ThingLookingAtPos.x = zero / zero;
    failures += check(!action.IsCameraToBeSetupDueToMovedSetupPositions(tag),
                      "unordered horizontal comparison remains false");

    action.ThingLookingAtPos.x=2.0f;
    action.ThingLookingFromPos.x=1.81f;
    failures += check(action.IsCameraToBeSetupDueToMovedSetupPositions(tag), "from horizontal beyond tolerance");
    action.ThingLookingFromPos.x=1.0f;
    action.ThingLookingAtPos.z=-0.25f;
    failures += check(!action.IsCameraToBeSetupDueToMovedSetupPositions(tag), "negative target vertical equality");
    action.ThingLookingAtPos.z=-0.251f;
    failures += check(action.IsCameraToBeSetupDueToMovedSetupPositions(tag), "negative target vertical beyond tolerance");
    action.ThingLookingAtPos.z=zero/zero;
    failures += check(!action.IsCameraToBeSetupDueToMovedSetupPositions(tag), "unordered vertical comparison false");
    action.ThingLookingFromPos.x=1.81f;
    failures += check(action.IsCameraToBeSetupDueToMovedSetupPositions(tag), "unordered target does not suppress camera movement");
    if (failures) { printf("00713010_TEST FAIL\n"); return 1; }
    printf("00713010_TEST PASS\n");
    return 0;
}