// CUserProfileManager::SetAssignedInputKeyboard(EGameAction action, EInputKey key, bool passive) -- rebind the keyboard key of one action:
// in the passive (true) or aggressive input set, find the first control for `action`; if it is a keyboard control (ControllerType 2)
// store the new key (and the keyboard type again); then apply the profile values whether or not anything changed.
// Grade: RELOCATION_MATCH 293v293 (VC7.1 RTM, stock flags; shared engine/CUserProfileManager.h + overlay opt-in
//        FABLE_RETAIL_CUSERPROFILEMANAGER_SETASSIGNEDINPUT, declarations only).
// Leaf: ?SetAssignedInputKeyboard@CUserProfileManager@@QAEXW4EGameAction@@W4EInputKey@@_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00407A70, 293 bytes (`ret 0Ch`), no callers (unreferenced; the linker kept it).
// Name: FableWin.pdb (0x01886A20, 749 debug bytes) and Ego_r.pdb list ?SetAssignedInputKeyboard / ?SetAssignedInputMouse (same debug size)
//   right before RedefineControlPassive, which retail places at 0x00407CD0 straight after this pair (0x00407A70, 0x00407BA0); the
//   keyboard twin is the one that tests ControllerType 2 and writes KeyboardKey (+0x08), the mouse twin type 3 and MouseButton (+0x10),
//   matching the landed IsSameControl of 0x00407CD0 (2 = keyboard key, 3 = mouse button).
// Members (Ego_r.pdb): AssignedInputsPassive +0x54, AssignedInputsAggressive +0x60 (STLport vector<CActionInputControl>, 28-byte
//   elements). Every element access goes through operator[] (retail reloads the vector start after each store). Callee: the
//   protected ApplyValues 0x00407370 (landed).
#define FABLE_RETAIL_CUSERPROFILEMANAGER_PASSIVEINPUTS
#define FABLE_RETAIL_CUSERPROFILEMANAGER_RESETASSIGNEDINPUTS
#define FABLE_RETAIL_CUSERPROFILEMANAGER_SETASSIGNEDINPUT
#include "engine/CUserProfileManager.h"
void CUserProfileManager::SetAssignedInputKeyboard(EGameAction action, EInputKey key, bool passive)
{
    if (passive) {
        for (unsigned long i = 0; i < AssignedInputsPassive.size(); ++i) {
            if (AssignedInputsPassive[i].GameAction == action) {
                if (AssignedInputsPassive[i].ControllerType == (EControllerType)2) {
                    AssignedInputsPassive[i].KeyboardKey = key;
                    AssignedInputsPassive[i].ControllerType = (EControllerType)2;
                }
                break;
            }
        }
    } else {
        for (unsigned long i = 0; i < AssignedInputsAggressive.size(); ++i) {
            if (AssignedInputsAggressive[i].GameAction == action) {
                if (AssignedInputsAggressive[i].ControllerType == (EControllerType)2) {
                    AssignedInputsAggressive[i].KeyboardKey = key;
                    AssignedInputsAggressive[i].ControllerType = (EControllerType)2;
                }
                break;
            }
        }
    }
    ApplyValues();
}
