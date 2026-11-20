// CGameCameraManager::GetNoControlMovementTime @ 0x00697710 (VC7.1 / MSVC 2003)
// This-only member accessor modeled as __fastcall (object ptr arrives in ecx,
// byte-identical to __fastcall for a this-only method). Compile: cl /c /O2 /Oy /W3


// Engine callee: returns a monotonically increasing millisecond tick (int in eax).
// Layout: retail STLport 4.6 header offsets; member names re-bound by the bytes this body touches: `InsideHeroCount` -> `ControlNoControlMovementStartFrame` (+0xc0).
#include "engine/CGameCameraManager.h"  // retyped onto the PDB layout; byte parity re-verified
class CWorld { public: static long __fastcall GetFrame(); };   // 0x0049D870 ?GetFrame@CWorld@@SIJXZ

// Relocation-masked globals read directly by the accessor.
extern float g_defaultNoControlMovementTime; // ds:0x122dedc, read via fld
// NF166 (round 10, q11679 / q11691 / q11725): 0x01375550 is ?ConstantFPS@CMainGameComponent@@1JA (protected static long,
//   engine/CMainGameComponent.h), read through the inline public accessor GetConstantFPS() (Ego_r + FableWin
//   ?GetConstantFPS@CMainGameComponent@@SIJXZ) as retail code outside the class does; was the row-local stand-in g_noControlTimeDivisor.
#include "engine/CMainGameComponent.h"

float __fastcall CGameCameraManager_GetNoControlMovementTime(CGameCameraManager *manager)
{
    if (manager->ControlNoControlMovementStartFrame == 0)
        return g_defaultNoControlMovementTime;

    long frames = CWorld::GetFrame() - manager->ControlNoControlMovementStartFrame;
    return (float)frames / CMainGameComponent::GetConstantFPS();
}