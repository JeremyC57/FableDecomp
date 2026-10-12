// The Xbox camera on the native pad.
//
// Every CAMERA_MANAGER_SET def names two camera managers: CameraManagerMain (the Xbox
// chase camera: CAMERA_MANAGER_MAIN, mode class 3, string length, auto-go-behind) and
// CameraManagerPCMain (CAMERA_MANAGER_PC_MAIN, mode class 17: the mouse orbit camera that
// returns behind the hero when the stick is released). The PC build always takes the PC
// pair (CGameCameraManager::SetUpMainCameraManagerSetDef 0x69ADC0 reads +0x48/+0x4C). With
// XBOX_CAMERA (controller settings) the set def is patched to name the Xbox pair instead,
// so the camera behaves as on the console: the right stick looks around and the camera
// follows the hero's movement.
#include "controller.hpp"

using namespace host;

extern "C" void F_0069AC90_orig(Ctx* c);

// 0x69AC90 void __thiscall CGameCameraManager::SetCameraManagerSetDef(CCameraManagerSetDef** def)
// (CWorld::UpdateRegionLoad 0x4A539B passes the region's set def; the function reads the
// manager def id at +0x44, CameraManagerPCMain: retail def fields sit 4 bytes before the PDB's).
extern "C" void host_camera_set(Ctx* c) {
    const uint32_t pdef = rd32(c->esp + 4);
    const uint32_t set = pdef ? rd32(pdef) : 0;
    // retail CCameraManagerSetDef: +0x3C CameraManagerMain, +0x40 Combat, +0x44 PCMain, +0x48 PCCombat
    if (set && host::pad::xboxCameraEnabled() && rd32(set + 0x3C) > 0 && rd32(set + 0x44) != rd32(set + 0x3C)) {
        HLOG(1, "camera: set def 0x%08X uses the Xbox managers %u/%u instead of %u/%u", set, rd32(set + 0x3C), rd32(set + 0x40),
             rd32(set + 0x44), rd32(set + 0x48));
        wr32(set + 0x44, rd32(set + 0x3C));
        wr32(set + 0x48, rd32(set + 0x40));
    }
    F_0069AC90_orig(c);
}
