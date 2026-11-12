// Behaviour test for CUserProfileManager::RedefineControlPassive (0x00407CD0): the passive input equal to the old control (same
// action, direction, controller type and that controller's key / button) is replaced; for actions 55 / 56 only the entry whose
// running count of that action equals `index` (the 56 counter starts at -1, the 55 counter at 0); then the values are applied.
#include <cstdio>
#include <vector>
struct Control { long GameAction, ControllerType, KeyboardKey, XboxButton, MouseButton; long Direction; long Tag; };
static bool IsSameControl(const Control& control, const Control& other)
{
    if (control.GameAction != other.GameAction || control.Direction != other.Direction || control.ControllerType != other.ControllerType)
        return false;
    switch (control.ControllerType) {
    case 1: if (control.XboxButton != other.XboxButton) return false; break;
    case 2: if (control.KeyboardKey != other.KeyboardKey) return false; break;
    case 3: if (control.MouseButton != other.MouseButton) return false; break;
    }
    return true;
}
struct Manager {
    std::vector<Control> AssignedInputsPassive; int Applied;
    void RedefineControlPassive(const Control& new_control, const Control& old_control, long index)
    {
        long context_sensitive_count = -1, quick_access_count = 0;
        for (unsigned long i = 0; i < AssignedInputsPassive.size(); ++i) {
            bool same = IsSameControl(AssignedInputsPassive[i], old_control);
            switch (old_control.GameAction) {
            case 55: if (quick_access_count == index && same) AssignedInputsPassive[i] = new_control; break;
            case 56: if (context_sensitive_count == index && same) AssignedInputsPassive[i] = new_control; break;
            default: if (same) AssignedInputsPassive[i] = new_control; break;
            }
            switch (AssignedInputsPassive[i].GameAction) {
            case 55: ++quick_access_count; break;
            case 56: ++context_sensitive_count; break;
            }
        }
        ++Applied;
    }
};
static Control Make(long action, long type, long key, long xbox, long mouse, long tag) { Control c = { action, type, key, xbox, mouse, 0, tag }; return c; }
int main()
{
    Manager m; m.Applied = 0; bool ok = true;
    m.AssignedInputsPassive.push_back(Make(10, 2, 30, 0, 0, 1));     // plain action on keyboard key 30
    m.AssignedInputsPassive.push_back(Make(55, 2, 31, 0, 0, 2));     // quick access #0
    m.AssignedInputsPassive.push_back(Make(55, 2, 31, 0, 0, 3));     // quick access #1 (same key)
    m.AssignedInputsPassive.push_back(Make(56, 3, 0, 0, 1, 4));      // context sensitive, first: count -1 -> 0 after it
    m.AssignedInputsPassive.push_back(Make(56, 3, 0, 0, 1, 5));      // context sensitive, second: seen with count 0
    m.AssignedInputsPassive.push_back(Make(10, 1, 0, 7, 0, 6));      // same action on an Xbox button
    m.RedefineControlPassive(Make(10, 2, 99, 0, 0, 100), Make(10, 2, 30, 0, 0, 0), 5);       // index ignored for plain actions
    ok = ok && m.AssignedInputsPassive[0].Tag == 100 && m.AssignedInputsPassive[5].Tag == 6 && m.Applied == 1;
    m.RedefineControlPassive(Make(55, 2, 98, 0, 0, 101), Make(55, 2, 31, 0, 0, 0), 1);       // only the second quick-access entry
    ok = ok && m.AssignedInputsPassive[1].Tag == 2 && m.AssignedInputsPassive[2].Tag == 101;
    m.RedefineControlPassive(Make(56, 3, 0, 0, 2, 102), Make(56, 3, 0, 0, 1, 0), 0);         // count is -1 at the first, 0 at the second
    ok = ok && m.AssignedInputsPassive[3].Tag == 4 && m.AssignedInputsPassive[4].Tag == 102;
    m.RedefineControlPassive(Make(10, 1, 0, 8, 0, 103), Make(10, 1, 0, 9, 0, 0), 0);         // button differs: nothing replaced
    ok = ok && m.AssignedInputsPassive[5].Tag == 6 && m.Applied == 4;
    if (!ok) { std::printf("00407cd0_TEST FAIL\n"); return 1; }
    std::printf("00407cd0_TEST PASS\n");
    return 0;
}