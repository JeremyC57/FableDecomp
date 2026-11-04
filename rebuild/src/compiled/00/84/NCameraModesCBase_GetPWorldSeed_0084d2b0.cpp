// NCameraModes::CBase::GetPWorldSeed() const -- the world's random seed, reached through the camera manager this mode belongs to
// Leaf: ?GetPWorldSeed@CBase@NCameraModes@@QBEPAKXZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x0084D2B0, 11 bytes: `mov eax,[ecx+40h] ; mov ecx,[eax+74h] ; jmp 0x0049D810`
//   (CameraManager->World->GetWorldSeed(), tail call).
// Identity (two sources): Ego_r.pdb ?GetPWorldSeed@CBase@NCameraModes@@QBEPAKXZ (the only GetPWorldSeed whose class holds a
//   CGameCameraManager*: Ego_r "CBase" record CameraManager, retail +0x40 after the 0x30-byte CPhysicsState LookAt); the chain matches
//   CGameCameraManager::World +0x74 (engine/CGameCameraManager.h) and the target is ?GetWorldSeed@CWorld@@QBEPAKXZ 0x0049D810. Its three
//   callers are camera script modules (0x00851E08, 0x008537B4, 0x00853B27) that seed random camera choices. The catalog's
//   "_Cons_val<..CParticleMorphs..>" was a propagated label.
// Callees: ?GetWorldSeed@CWorld@@QBEPAKXZ 0x0049D810.
// Header: engine/NCameraModes_CBase.h (FABLE_RETAIL_NCAMERAMODES_CBASE_MEMBERS, new), engine/CGameCameraManager.h, engine/CWorld.h.
#define FABLE_RETAIL_NCAMERAMODES_CBASE_MEMBERS
#include "engine/NCameraModes_CBase.h"
#include "engine/CGameCameraManager.h"
#include "engine/CWorld.h"
namespace NCameraModes {
unsigned long* CBase::GetPWorldSeed() const
{
    return CameraManager->World->GetWorldSeed();
}
}