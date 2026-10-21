// Behaviour test for CUserProfileManager::GetAssignedInputForAction (0x00408c90): the matching control is copied from the
// selected table; an empty table is reset first; a missing action returns false.
#include <cstdio>

struct CActionInputControl { long GameAction; long ControllerType; long KeyboardKey; long XboxButton; long MouseButton; float X, Y; };
struct CTable {
    CActionInputControl Items[8]; unsigned long Count;
    bool empty() const { return Count == 0; }
    unsigned long size() const { return Count; }
    CActionInputControl& operator[](unsigned long i) { return Items[i]; }
};
static int g_resets;
struct CUserProfileManager {
    CTable AssignedInputsPassive, AssignedInputsAggressive;
    void ResetAssignedInputs()
    {
        ++g_resets;
        CActionInputControl c = { 9, 1, 2, 3, 4, 0, 0 };
        AssignedInputsPassive.Items[0] = c; AssignedInputsPassive.Count = 1;
    }
    bool Find(CTable& t, long action, long index, CActionInputControl* out)
    {
        if (t.empty())
            ResetAssignedInputs();
        (void)index;
        for (unsigned long i = 0; i < t.size(); ++i) {
            if (t[i].GameAction == action) { *out = t[i]; return true; }
        }
        return false;
    }
    bool Get(long action, bool passive, long index, CActionInputControl* out)
    {
        return passive ? Find(AssignedInputsPassive, action, index, out) : Find(AssignedInputsAggressive, action, index, out);
    }
};

int main()
{
    CUserProfileManager m; m.AssignedInputsPassive.Count = 0; m.AssignedInputsAggressive.Count = 3;
    for (int i = 0; i < 3; ++i) { CActionInputControl c = { 5, i, 0, 0, 0, 0, 0 }; m.AssignedInputsAggressive.Items[i] = c; }
    CActionInputControl out = { 0, 0, 0, 0, 0, 0, 0 };
    bool ok = m.Get(9, true, 0, &out) && g_resets == 1 && out.KeyboardKey == 2;
    ok = ok && m.Get(5, false, 0, &out) && out.ControllerType == 0;
    ok = ok && !m.Get(7, false, 0, &out) && !m.Get(8, true, 0, &out) && g_resets == 1;
    if (!ok) { std::printf("00408c90_TEST FAIL\n"); return 1; }
    std::printf("00408c90_TEST PASS\n");
    return 0;
}
