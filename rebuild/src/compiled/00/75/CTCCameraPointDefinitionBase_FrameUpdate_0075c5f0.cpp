// CTCCameraPointDefinitionBase::FrameUpdate() -- keeps the camera point registered with the game camera manager while its leaf criteria are met (and the approach angle is valid), updating the active camera position meanwhile
// Leaf: ?FrameUpdate@CTCCameraPointDefinitionBase@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0075C5F0, thiscall, `ret`.
// Identity (agentFAM03 proof, re-verified by agentFAM05): slot 9 of CTCCameraPointScriptedSpline's vtable 0x0126A0BC (the slot holds this body in 10 vtable(s), owner CTCCameraPointDefinitionBase); the Xbox 360 Test PDB virtual order (third source) aligned on 18 TLC-named slots of that vtable puts FrameUpdate(void) at this slot [gap kind GS]; Ego_r/FableWin name set: ?FrameUpdate@CTCCameraPointDefinitionBase@@UAEXXZ; `ret` matches the signature.
// Was: CTCCameraPointShapeActivationBase::vfunc_9 (rebuild/src/compiled/00/75/CTCCameraPointShapeActivationBase_vfunc9_0075c5f0.cpp).
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CAIGAMECAMERABASE
#define FABLE_RETAIL_CGAMECAMERAMANAGER_ADDCAMERAPOINT
#define FABLE_RETAIL_CTCCAMERAPOINTDEFINITIONBASE_VTABLE
#include "fable_string.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CTCBase.h"
#include "engine/CWorld.h"
#include "engine/CAIGameCameraBase.h"
#include "engine/CGameCameraManager.h"
#include "engine/CTCCameraPointGeneralCase.h"
void CTCCameraPointDefinitionBase::FrameUpdate()
{
    CGameCameraManager* camera_manager;
    CWorld* world = &GetWorld();
    if (world->PCurrentGameCamera->GetCameraManager(&camera_manager)) {
        bool criteria_met = IsLeafCriteriaMet();
        if (criteria_met)
            UpdateActiveCameraPosition();
        if (Activated && !criteria_met) {
            camera_manager->RemoveCameraPoint(*ParentThing);
            Activated = false;
        } else if (!Activated && criteria_met && IsAngleValid()) {
            camera_manager->AddCameraPoint(*ParentThing);
            Activated = true;
        }
    }
}
