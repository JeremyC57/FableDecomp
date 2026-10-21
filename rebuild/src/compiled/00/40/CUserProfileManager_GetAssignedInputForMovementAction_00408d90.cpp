// CUserProfileManager::GetAssignedInputForMovementAction(EGameAction, bool, const C2DVector&, CActionInputControl&) -- find the passive (or aggressive) input binding for the action whose control direction matches the given one (compared as integer coordinates), resetting the bindings first if the list is empty
// Leaf: GetAssignedInputForMovementAction
// Retail: Fable.exe (TLC, VC7.1) @ 0x00408D90, 439 bytes (448 with padding), `ret 10h`.
// Callees: CUserProfileManager::ResetAssignedInputs 0x004085F0, __ftol2 0x00BFEA70.
// Layout (Ego_r.pdb, retail STLport vectors 12 bytes): AssignedInputsPassive +0x54, AssignedInputsAggressive +0x60
//   (vector<CActionInputControl>, 0x1C-byte elements: GameAction +0, ControlDirection +0x14).
// Shape: the direction test compares two integer coordinates built member-wise from the floats (the argument's first);
//   elements are read as *(begin() + i) with size() re-read every iteration; if/else arms with ONE shared trailing
//   `return false` (a return false in each arm lets VC7.1 merge the two found-copy blocks).
struct C2DVector {
    float X;
    float Y;
};
struct C2DCoordI {
    long X;
    long Y;
    C2DCoordI(const C2DVector& v) : X((long)v.X), Y((long)v.Y) { }
    bool operator==(const C2DCoordI& other) const { return X == other.X && Y == other.Y; }
};
enum EGameAction { GAME_ACTION_NONE = 0 };
struct CActionInputControl {                            // 0x1C bytes
    EGameAction GameAction;                             // +0x00
    long        ControllerType;                         // +0x04 EControllerType
    long        KeyboardKey;                            // +0x08 EInputKey
    long        XboxButton;                             // +0x0c EXboxControllerButton
    long        MouseButton;                            // +0x10 EMouseButtonControl
    C2DVector   ControlDirection;                       // +0x14
};
struct CActionInputControlVector {                      // std::vector<CActionInputControl> (STLport, 12 bytes)
    CActionInputControl* _M_start;
    CActionInputControl* _M_finish;
    CActionInputControl* _M_end_of_storage;
    CActionInputControl* begin() const { return _M_start; }
    bool empty() const { return _M_start == _M_finish; }
    unsigned long size() const { return (unsigned long)(_M_finish - _M_start); }
    CActionInputControl& operator[](unsigned long n) const { return *(begin() + n); }
};
class CUserProfileManager {
public:
    unsigned char             _pad00[0x54];
    CActionInputControlVector AssignedInputsPassive;    // +0x54
    CActionInputControlVector AssignedInputsAggressive; // +0x60
    void ResetAssignedInputs();                         // 0x004085F0
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