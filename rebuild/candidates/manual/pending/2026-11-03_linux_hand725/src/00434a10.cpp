// CDisplayEngine::PreChangeResolution -- tell the 3D engine a resolution change is about to happen: forwards to the engine interface Engine3D (+0x2C), CIEngine vtable slot 15, result dropped
// Leaf: ?PreChangeResolution@CDisplayEngine@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00434A10, 8 bytes: `mov ecx,[ecx+2Ch] ; mov eax,[ecx] ; jmp dword ptr [eax+3Ch]` (a sibling call:
//   the arguments stay in place; the engine's bool result is discarded by the void member).
// Identity: FableWin.pdb ?PreChangeResolution@CDisplayEngine@@QAEXXZ. FableWin orders the CDisplayEngine TU
//   PreChangeResolution, PostChangeResolution, ChangeTextureColourDepth, ChangeMaxTextureSize -- retail 0x00434A10, 0x00434A20.
//   Callers: the inlined CMainGameComponent::PreChangeResolution in SetExclusiveMode 0x004163C4, SetDisplayMode 0x0041642C, AppReinitFunc 0x004167E8 and ConsoleSetResolution 0x004175A3 (ecx = PDisplayEngine +0x28, no arguments). Landed spellings: ?PreChangeResolution@CDisplayEngine@@QAEXXZ (right) and CDisplayHolder::OnExclusiveModeChange (wrong, NAME_FIXES).
//   The CIEngine slot: CEngine (vtable 0x012A0F3C) slot 15 / 16 = 0x00B2AA20 / 0x00B2AA30 (catalog CEngine::PreChangeResolution /
//   PostChangeResolution; FableWin ?PreChangeResolution@CEngine@@UAE_NXZ, ?PostChangeResolution@CEngine@@UAE_NKKK@Z); CIEngine's
//   own vtable 0x012A0D24 has _purecall there. The catalog label "CScriptThing::operator==" is propagated and wrong.
// Header: engine/CDisplayEngine.h FABLE_RETAIL_CDISPLAYENGINE and engine/CIEngine.h FABLE_RETAIL_CIENGINE (overlay hdr/).
#define FABLE_RETAIL_CIENGINE
#define FABLE_RETAIL_CDISPLAYENGINE
#include "engine/CIEngine.h"
#include "engine/CDisplayEngine.h"

void CDisplayEngine::PreChangeResolution()
{
    Engine3D->PreChangeResolution();
}
