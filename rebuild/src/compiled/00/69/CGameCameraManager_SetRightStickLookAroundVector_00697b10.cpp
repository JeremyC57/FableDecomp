// CGameCameraManager::SetRightStickLookAroundVector(const C2DVector&) -- scale the right-stick input into the target look-around angles (X by 0.25, Y by 0.125) and pass the raw vector on to the current camera mode
// Leaf: ?SetRightStickLookAroundVector@CGameCameraManager@@QAEXABVC2DVector@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00697B10, 68 bytes (int3 padding to 0x00697B60), thiscall, tail `jmp` with the argument re-stored.
// Identity: callers CInputProcessCameraLookAround::ProcessInput 0x0068D790 (call at 0x0068D81E; landed, calling it by this name) and an
//   uncatalogued __fastcall helper at 0x0068D590 (call at 0x0068D5AE). Ego_r.pdb + FableWin.pdb
//   ?SetRightStickLookAroundVector@CGameCameraManager@@QAEXABVC2DVector@@@Z; donor order ResetRightStickLookAroundVector /
//   SetRightStickLookAroundVector (Ego_r 0x006BDFB0 / 0x006BE7D0) = retail 0x006976B0 (landed) / 0x00697B10.
// Callees: NCameraModes::CBase::SetRightStickLookAround(const C2DVector&) 0x0084CF40 (both PDBs ?SetRightStickLookAround@CBase@NCameraModes@@
//   QAEXABVC2DVector@@@Z; its body copies the vector into the mode's +0x90 / +0x94; catalog label "CEditControlCentre::OnEngineSurveyLMBHeld"
//   is wrong) on CurrentMode, a CCountedPointer<NCameraModes::CBase> at +0x98 (engine/CGameCameraManager.h CurrentMode_Data +0x98).
// Constants: 0.25f (0x0125C1E4) and 0.125f (0x0125C1E8) as literals (this TU's pool entries: 4 and 1 references).
// Shape: `Target = C2DVector(x * 0.25f, y * 0.125f)` builds the 8-byte frame temporary (fstp [esp] / [esp+4]) and copies it with two dword
//   moves interleaved with the second product, X first; the separate float member assignments store straight from the FPU (48v68).
// Layout: engine/CGameCameraManager.h has the retail offsets but flattens the vptr and the C2DVector / CCountedPointer members into raw
//   fields and has no member functions, so the class is restated here with its RightStick members (as the landed sibling
//   ResetRightStickLookAroundVector 0x006976B0); CCountedPointer from engine/CCountedPointer.h.
#include "engine/CCountedPointer.h"

class C2DVector {
public:
    float X, Y;
    C2DVector() { }
    C2DVector(float x, float y) : X(x), Y(y) { }        // FableWin ??0C2DVector@@QAE@MM@Z (inline)
};

namespace NCameraModes {
class CBase {
public:
    void SetRightStickLookAround(const C2DVector& vector);   // 0x0084CF40
};
}

class CGameCameraManager {
public:
    unsigned char _pad00[0x98];
    CCountedPointer<NCameraModes::CBase> CurrentMode;   // +0x98
    unsigned char _padA0[0xc4 - 0xa0];
    C2DVector     RightStickLookAroundAngles;           // +0xc4
    C2DVector     RightStickTargetLookAroundAngles;     // +0xcc
    void SetRightStickLookAroundVector(const C2DVector& vector);
};

void CGameCameraManager::SetRightStickLookAroundVector(const C2DVector& vector)
{
    RightStickTargetLookAroundAngles = C2DVector(vector.X * 0.25f, vector.Y * 0.125f);
    CurrentMode->SetRightStickLookAround(vector);
}