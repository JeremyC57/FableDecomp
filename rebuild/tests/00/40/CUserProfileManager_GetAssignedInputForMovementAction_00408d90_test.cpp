#include <cstdio>
#include <vector>
// behaviour test: CUserProfileManager::GetAssignedInputForMovementAction searches the passive or aggressive binding
// list (resetting the bindings first when it is empty) for the action whose direction matches after truncation to whole
// units, copies the first hit out and reports whether one was found
struct C2DVector { float X, Y; };
struct C2DCoordI {
    long X;
    long Y;
    C2DCoordI(const C2DVector& v) : X((long)v.X), Y((long)v.Y) { }
    bool operator==(const C2DCoordI& other) const { return X == other.X && Y == other.Y; }
};
enum EGameAction { GAME_ACTION_NONE = 0, GAME_ACTION_MOVE = 5, GAME_ACTION_STRAFE = 6 };
struct CActionInputControl {
    EGameAction GameAction;
    long ControllerType;
    long KeyboardKey;
    long XboxButton;
    long MouseButton;
    C2DVector ControlDirection;
};
static CActionInputControl Make(EGameAction a, long key, float x, float y)
{
    CActionInputControl c;
    c.GameAction = a; c.ControllerType = 0; c.KeyboardKey = key; c.XboxButton = 0; c.MouseButton = 0;
    c.ControlDirection.X = x; c.ControlDirection.Y = y;
    return c;
}
class CUserProfileManager {
public:
    std::vector<CActionInputControl> AssignedInputsPassive;
    std::vector<CActionInputControl> AssignedInputsAggressive;
    int Resets;
    void ResetAssignedInputs()
    {
        ++Resets;
        AssignedInputsPassive.push_back(Make(GAME_ACTION_MOVE, 87, 0.0f, 1.0f));
        AssignedInputsAggressive.push_back(Make(GAME_ACTION_MOVE, 38, 0.0f, 1.0f));
    }
    bool GetAssignedInputForMovementAction(EGameAction action, bool passive, const C2DVector& direction, CActionInputControl& control);
};
bool CUserProfileManager::GetAssignedInputForMovementAction(EGameAction action, bool passive, const C2DVector& direction, CActionInputControl& control)
{
    if (passive) {
        if (AssignedInputsPassive.empty())
            ResetAssignedInputs();
        for (unsigned long i = 0; i < AssignedInputsPassive.size(); ++i) {
            if (AssignedInputsPassive[i].GameAction == action
                && C2DCoordI(AssignedInputsPassive[i].ControlDirection) == C2DCoordI(direction)) {
                control = AssignedInputsPassive[i];
                return true;
            }
        }
    } else {
        if (AssignedInputsAggressive.empty())
            ResetAssignedInputs();
        for (unsigned long i = 0; i < AssignedInputsAggressive.size(); ++i) {
            if (AssignedInputsAggressive[i].GameAction == action
                && C2DCoordI(AssignedInputsAggressive[i].ControlDirection) == C2DCoordI(direction)) {
                control = AssignedInputsAggressive[i];
                return true;
            }
        }
    }
    return false;
}
int main()
{
    CUserProfileManager m;
    m.Resets = 0;
    CActionInputControl out = Make(GAME_ACTION_NONE, 0, 0.0f, 0.0f);
    C2DVector up = { 0.3f, 1.7f };
    bool ok = m.GetAssignedInputForMovementAction(GAME_ACTION_MOVE, true, up, out) && out.KeyboardKey == 87 && m.Resets == 1;
    m.AssignedInputsAggressive.push_back(Make(GAME_ACTION_STRAFE, 65, -1.0f, 0.0f));
    C2DVector left = { -1.9f, 0.2f };
    ok = ok && m.GetAssignedInputForMovementAction(GAME_ACTION_STRAFE, false, left, out) && out.KeyboardKey == 65 && m.Resets == 1;
    C2DVector right = { 1.0f, 0.0f };
    out.KeyboardKey = 1;
    ok = ok && !m.GetAssignedInputForMovementAction(GAME_ACTION_STRAFE, false, right, out) && out.KeyboardKey == 1;
    ok = ok && !m.GetAssignedInputForMovementAction(GAME_ACTION_STRAFE, true, left, out);
    if (!ok) { std::printf("00408d90_TEST FAIL\n"); return 1; }
    std::printf("00408d90_TEST PASS\n");
    return 0;
}