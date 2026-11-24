// Behaviour test for CUserProfileManager::SetAssignedInputMouse (0x00407BA0): the first control for the action in the chosen set
// (passive / aggressive) gets the new key only when it is a mouse control (type 3); ApplyValues runs on every path.
#include <cstdio>
#include <vector>
struct Control { long GameAction, ControllerType, KeyboardKey, XboxButton, MouseButton; float DirX, DirY; };
struct Manager {
    std::vector<Control> AssignedInputsPassive, AssignedInputsAggressive; int Applied;
    void ApplyValues() { ++Applied; }
    void SetAssignedInputMouse(long action, long button, bool passive)
    {
        if (passive) {
            for (unsigned long i = 0; i < AssignedInputsPassive.size(); ++i) {
                if (AssignedInputsPassive[i].GameAction == action) {
                    if (AssignedInputsPassive[i].ControllerType == 3) {
                        AssignedInputsPassive[i].MouseButton = button;
                        AssignedInputsPassive[i].ControllerType = 3;
                    }
                    break;
                }
            }
        } else {
            for (unsigned long i = 0; i < AssignedInputsAggressive.size(); ++i) {
                if (AssignedInputsAggressive[i].GameAction == action) {
                    if (AssignedInputsAggressive[i].ControllerType == 3) {
                        AssignedInputsAggressive[i].MouseButton = button;
                        AssignedInputsAggressive[i].ControllerType = 3;
                    }
                    break;
                }
            }
        }
        ApplyValues();
    }
};
static Control C(long a, long t, long k) { Control c = { a, t, 0, 0, k, 0.0f, 0.0f }; return c; }
int main()
{
    int fails = 0;
    Manager m; m.Applied = 0;
    m.AssignedInputsPassive.push_back(C(1, 3, 10)); m.AssignedInputsPassive.push_back(C(2, 2, 11)); m.AssignedInputsPassive.push_back(C(1, 3, 12));
    m.AssignedInputsAggressive.push_back(C(1, 3, 20)); m.AssignedInputsAggressive.push_back(C(4, 3, 21));
    m.SetAssignedInputMouse(1, 99, true);           // first match only
    if (m.AssignedInputsPassive[0].MouseButton != 99 || m.AssignedInputsPassive[2].MouseButton != 12 || m.AssignedInputsAggressive[0].MouseButton != 20) ++fails;
    m.SetAssignedInputMouse(2, 77, true);           // keyboard control: untouched
    if (m.AssignedInputsPassive[1].MouseButton != 11 || m.AssignedInputsPassive[1].ControllerType != 2) ++fails;
    m.SetAssignedInputMouse(4, 55, false);          // aggressive set
    if (m.AssignedInputsAggressive[1].MouseButton != 55) ++fails;
    m.SetAssignedInputMouse(9, 1, false);           // not found
    if (m.Applied != 4) ++fails;
    if (fails) { std::printf("00407ba0_TEST FAIL\n"); return 1; }
    std::printf("00407ba0_TEST PASS\n");
    return 0;
}