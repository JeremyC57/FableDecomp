// CDisplayEngine::PostChangeResolution -- pass the new resolution to the 3D engine: forwards to the engine interface Engine3D (+0x2C), CIEngine vtable slot 16, result dropped
// Leaf: ?PostChangeResolution@CDisplayEngine@@QAEXKKK@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00434A20, 8 bytes: `mov ecx,[ecx+2Ch] ; mov eax,[ecx] ; jmp dword ptr [eax+40h]` (a sibling call:
//   the arguments stay in place; the engine's bool result is discarded by the void member).
// Identity: FableWin.pdb ?PostChangeResolution@CDisplayEngine@@QAEXKKK@Z. FableWin orders the CDisplayEngine TU
//   PreChangeResolution, PostChangeResolution, ChangeTextureColourDepth, ChangeMaxTextureSize -- retail 0x00434A10, 0x00434A20.
//   Callers: CMainGameComponent::PostChangeResolution 0x00416377 (ecx = PDisplayEngine +0x28; the same three arguments then go to CWorld::ChangeResolution 0x0049D820 through PWorld +0x24).
//   The CIEngine slot: CEngine (vtable 0x012A0F3C) slot 15 / 16 = 0x00B2AA20 / 0x00B2AA30 (catalog CEngine::PreChangeResolution /
//   PostChangeResolution; FableWin ?PreChangeResolution@CEngine@@UAE_NXZ, ?PostChangeResolution@CEngine@@UAE_NKKK@Z); CIEngine's
//   own vtable 0x012A0D24 has _purecall there. The catalog label "CScriptThing::operator==" is propagated and wrong.
// Header: engine/CDisplayEngine.h FABLE_RETAIL_CDISPLAYENGINE and engine/CIEngine.h FABLE_RETAIL_CIENGINE (overlay hdr/).
#define FABLE_RETAIL_CIENGINE
#define FABLE_RETAIL_CDISPLAYENGINE
#include "engine/CIEngine.h"
#include "engine/CDisplayEngine.h"

void CDisplayEngine::PostChangeResolution(unsigned long width, unsigned long height, unsigned long depth)
{
    Engine3D->PostChangeResolution(width, height, depth);
}
