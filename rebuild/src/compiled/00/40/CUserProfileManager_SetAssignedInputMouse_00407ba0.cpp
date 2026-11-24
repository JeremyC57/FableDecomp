// CUserProfileManager::SetAssignedInputMouse(EGameAction action, EMouseButtonControl button, bool passive) -- rebind the mouse button of
// one action: in the passive (true) or aggressive input set, find the first control for `action`; if it is a mouse control
// (ControllerType 3) store the new button (and the mouse type again); then apply the profile values whether or not anything changed.
// Grade: RELOCATION_MATCH 293v293 (VC7.1 RTM, stock flags; shared engine/CUserProfileManager.h + overlay opt-in
//        FABLE_RETAIL_CUSERPROFILEMANAGER_SETASSIGNEDINPUT, declarations only).
// Leaf: ?SetAssignedInputMouse@CUserProfileManager@@QAEXW4EGameAction@@W4EMouseButtonControl@@_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00407BA0, 293 bytes (`ret 0Ch`), no callers (unreferenced; the linker kept it). Byte twin of
//   SetAssignedInputKeyboard 0x00407A70 except the type constant (3 v 2) and the stored member (+0x10 MouseButton v +0x08 KeyboardKey).
// Name: FableWin.pdb (0x01886DD0, 749 debug bytes, the same size as the keyboard twin) and Ego_r.pdb list ?SetAssignedInputMouse
//   right after ?SetAssignedInputKeyboard and before RedefineControlPassive (retail 0x00407CD0, straight after this function); type 3 =
//   mouse button as in the landed IsSameControl of 0x00407CD0.
// Members (Ego_r.pdb): AssignedInputsPassive +0x54, AssignedInputsAggressive +0x60 (STLport vector<CActionInputControl>, 28-byte
//   elements). Every element access goes through operator[] (retail reloads the vector start after each store). Callee: the
//   protected ApplyValues 0x00407370 (landed).
#define FABLE_RETAIL_CUSERPROFILEMANAGER_PASSIVEINPUTS
#define FABLE_RETAIL_CUSERPROFILEMANAGER_RESETASSIGNEDINPUTS
#define FABLE_RETAIL_CUSERPROFILEMANAGER_SETASSIGNEDINPUT
#include "engine/CUserProfileManager.h"
void CUserProfileManager::SetAssignedInputMouse(EGameAction action, EMouseButtonControl button, bool passive)
{
    if (passive) {
        for (unsigned long i = 0; i < AssignedInputsPassive.size(); ++i) {
            if (AssignedInputsPassive[i].GameAction == action) {
                if (AssignedInputsPassive[i].ControllerType == (EControllerType)3) {
                    AssignedInputsPassive[i].MouseButton = button;
                    AssignedInputsPassive[i].ControllerType = (EControllerType)3;
                }
                break;
            }
        }
    } else {
        for (unsigned long i = 0; i < AssignedInputsAggressive.size(); ++i) {
            if (AssignedInputsAggressive[i].GameAction == action) {
                if (AssignedInputsAggressive[i].ControllerType == (EControllerType)3) {
                    AssignedInputsAggressive[i].MouseButton = button;
                    AssignedInputsAggressive[i].ControllerType = (EControllerType)3;
                }
                break;
            }
        }
    }
    ApplyValues();
}