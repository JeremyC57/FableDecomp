// CUserProfileManager::RedefineControlPassive(const CActionInputControl& newControl, const CActionInputControl& oldControl, long index) -- replace the passive-table entry that matches the old control (for the two context-sensitive actions only its index-th occurrence), then apply the values
// Grade: DIFFER(309v310) -- register choice only: the entry address is built as mov edi,begin / add edi,offset where retail loads begin into ecx and forms edi with lea [edx+ecx] after the action load
// Leaf: RedefineControlPassive
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00407CD0, length 311 bytes (+ int3 padding), `ret 0Ch`.
// Callees: CActionInputControl::GetDirection() const @ 0x0040ED10, CUserProfileManager::ApplyValues @ 0x00407370.
// Layout: as GetAssignedInputForAction 0x00408C90 (AssignedInputsPassive +0x54, 28-byte CActionInputControl:
// GameAction, ControllerType, KeyboardKey +8, XboxButton +0xc, MouseButton +0x10, ControlDirection).
// Shape: per entry an inline "same control" test (action, direction, controller type, then the key/button of that
// controller: 1 Xbox, 2 keyboard, 3 mouse, anything else matches); the copy is gated by a switch on the old control's
// action (0x37 / 0x38 also need their running occurrence count to equal `index`), then the occurrence counters are bumped
// by a switch on the entry's action; size() is recomputed every pass.
enum EGameAction { GAME_ACTION_CONTEXT_37 = 0x37, GAME_ACTION_CONTEXT_38 = 0x38 };
enum EActionDirection { ACTION_DIRECTION_NONE = 0 };
struct C2DVector { float X, Y; };
struct CActionInputControl {
    EGameAction GameAction;                                    // +0x00
    long        ControllerType;                                // +0x04 (1 Xbox, 2 keyboard, 3 mouse)
    long        KeyboardKey;                                   // +0x08
    long        XboxButton;                                    // +0x0c
    long        MouseButton;                                   // +0x10
    C2DVector   ControlDirection;                              // +0x14
    EActionDirection GetDirection() const;                     // 0x0040ED10
};

__forceinline bool IsSameControl(const CActionInputControl& a, const CActionInputControl& b)
{
    if (a.GameAction != b.GameAction || a.GetDirection() != b.GetDirection() || a.ControllerType != b.ControllerType)
        return false;
    switch (a.ControllerType) {
    case 1:
        if (a.XboxButton != b.XboxButton)
            return false;
        break;
    case 2:
        if (a.KeyboardKey != b.KeyboardKey)
            return false;
        break;
    case 3:
        if (a.MouseButton != b.MouseButton)
            return false;
        break;
    }
    return true;
}
template<class T> struct CRetailVector {                       // STLport vector: {start, finish, end_of_storage}
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T* begin() const { return _M_start; }
    unsigned long size() const { return (unsigned long)(_M_finish - _M_start); }
    T& operator[](unsigned long n) const { return *(begin() + n); }
};
class CUserProfileManager {
public:
    unsigned char                      _pad00[0x54];
    CRetailVector<CActionInputControl> AssignedInputsPassive;  // +0x54
    void ApplyValues();                                        // 0x00407370
    void RedefineControlPassive(const CActionInputControl& newControl, const CActionInputControl& oldControl, long index);
};

void CUserProfileManager::RedefineControlPassive(const CActionInputControl& newControl, const CActionInputControl& oldControl, long index)
{
    long count38 = -1;
    long count37 = 0;
    for (unsigned long i = 0; i < AssignedInputsPassive.size(); ++i) {
        CActionInputControl& control = AssignedInputsPassive[i];
        EGameAction oldAction = oldControl.GameAction;
        bool same = IsSameControl(control, oldControl);
        switch (oldAction) {
        case GAME_ACTION_CONTEXT_37:
            if (count37 == index && same)
                control = newControl;
            break;
        case GAME_ACTION_CONTEXT_38:
            if (count38 == index && same)
                control = newControl;
            break;
        default:
            if (same)
                control = newControl;
            break;
        }
        switch (AssignedInputsPassive[i].GameAction) {
        case GAME_ACTION_CONTEXT_37:
            ++count37;
            break;
        case GAME_ACTION_CONTEXT_38:
            ++count38;
            break;
        }
    }
    ApplyValues();
}
