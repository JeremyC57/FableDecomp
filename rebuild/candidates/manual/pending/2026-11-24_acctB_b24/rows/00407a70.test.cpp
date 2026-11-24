// Behaviour test for CUserProfileManager::SetAssignedInputKeyboard (0x00407A70): the first control for the action in the chosen set
// (passive / aggressive) gets the new key only when it is a keyboard control (type 2); ApplyValues runs on every path.
#include <cstdio>
#include <vector>
struct Control { long GameAction, ControllerType, KeyboardKey, XboxButton, MouseButton; float DirX, DirY; };
struct Manager {
    std::vector<Control> AssignedInputsPassive, AssignedInputsAggressive; int Applied;
    void ApplyValues() { ++Applied; }
    void SetAssignedInputKeyboard(long action, long key, bool passive)
    {
        if (passive) {
            for (unsigned long i = 0; i < AssignedInputsPassive.size(); ++i) {
                if (AssignedInputsPassive[i].GameAction == action) {
                    if (AssignedInputsPassive[i].ControllerType == 2) {
                        AssignedInputsPassive[i].KeyboardKey = key;
                        AssignedInputsPassive[i].ControllerType = 2;
                    }
                    break;
                }
            }
        } else {
            for (unsigned long i = 0; i < AssignedInputsAggressive.size(); ++i) {
                if (AssignedInputsAggressive[i].GameAction == action) {
                    if (AssignedInputsAggressive[i].ControllerType == 2) {
                        AssignedInputsAggressive[i].KeyboardKey = key;
                        AssignedInputsAggressive[i].ControllerType = 2;
                    }
                    break;
                }
            }
        }
        ApplyValues();
    }
};
static Control C(long a, long t, long k) { Control c = { a, t, k, 0, 0, 0.0f, 0.0f }; return c; }
int main()
{
    int fails = 0;
    Manager m; m.Applied = 0;
    m.AssignedInputsPassive.push_back(C(1, 2, 10)); m.AssignedInputsPassive.push_back(C(2, 3, 11)); m.AssignedInputsPassive.push_back(C(1, 2, 12));
    m.AssignedInputsAggressive.push_back(C(1, 2, 20)); m.AssignedInputsAggressive.push_back(C(4, 2, 21));
    m.SetAssignedInputKeyboard(1, 99, true);           // first match only
    if (m.AssignedInputsPassive[0].KeyboardKey != 99 || m.AssignedInputsPassive[2].KeyboardKey != 12 || m.AssignedInputsAggressive[0].KeyboardKey != 20) ++fails;
    m.SetAssignedInputKeyboard(2, 77, true);           // mouse control: untouched
    if (m.AssignedInputsPassive[1].KeyboardKey != 11 || m.AssignedInputsPassive[1].ControllerType != 3) ++fails;
    m.SetAssignedInputKeyboard(4, 55, false);          // aggressive set
    if (m.AssignedInputsAggressive[1].KeyboardKey != 55) ++fails;
    m.SetAssignedInputKeyboard(9, 1, false);           // not found
    if (m.Applied != 4) ++fails;
    if (fails) { std::printf("00407a70_TEST FAIL\n"); return 1; }
    std::printf("00407a70_TEST PASS\n");
    return 0;
}
