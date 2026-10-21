// CUserProfileManager::GetAssignedInputForContextSensitiveItemAction(EGameAction action, bool passive, long index, CActionInputControl* out) -- copy the index-th control bound to the action from the passive or aggressive table (resetting the tables when empty)
// Leaf: GetAssignedInputForContextSensitiveItemAction
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00408F50, length 309 bytes (+ int3 padding), `ret 10h`.
// Callees: CUserProfileManager::ResetAssignedInputs @ 0x004085F0.
// Layout: Ego_r.pdb CUserProfileManager with retail 12-byte STLport vectors: AssignedInputsPassive +0x54 (= PDB),
// AssignedInputsAggressive +0x60 (PDB +0x64); CActionInputControl (28 bytes, = PDB): GameAction +0, ControllerType,
// KeyboardKey, XboxButton, MouseButton, ControlDirection (C2DVector).
// Shape: the sibling GetAssignedInputForAction 0x00408C90 with a match counter (incremented after the index test); each
// table arm is its own loop (size() recomputed per pass as the signed /28 difference, compared unsigned;
// the element walk strength-reduced, the hit copied from begin() + i with a 7-dword rep movsd).
enum EGameAction { GAME_ACTION_NONE = 0 };
struct C2DVector { float X, Y; };
struct CActionInputControl {
    EGameAction GameAction;                                    // +0x00
    long        ControllerType;                                // +0x04
    long        KeyboardKey;                                   // +0x08
    long        XboxButton;                                    // +0x0c
    long        MouseButton;                                   // +0x10
    C2DVector   ControlDirection;                              // +0x14
};
template<class T> struct CRetailVector {                       // STLport vector: {start, finish, end_of_storage}
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T* begin() const { return _M_start; }
    T* end() const { return _M_finish; }
    bool empty() const { return _M_start == _M_finish; }
    unsigned long size() const { return (unsigned long)(_M_finish - _M_start); }
    T& operator[](unsigned long n) const { return *(begin() + n); }
};
class CUserProfileManager {
public:
    unsigned char                       _pad00[0x54];
    CRetailVector<CActionInputControl> AssignedInputsPassive;     // +0x54
    CRetailVector<CActionInputControl> AssignedInputsAggressive;  // +0x60
    void ResetAssignedInputs();                                // 0x004085F0
    bool GetAssignedInputForContextSensitiveItemAction(EGameAction action, bool passive, long index, CActionInputControl* out);
};

bool CUserProfileManager::GetAssignedInputForContextSensitiveItemAction(EGameAction action, bool passive, long index, CActionInputControl* out)
{
    if (passive) {
        if (AssignedInputsPassive.empty())
            ResetAssignedInputs();
        long found = 0;
        for (unsigned long i = 0; i < AssignedInputsPassive.size(); ++i) {
            if (AssignedInputsPassive[i].GameAction == action) {
                if (found == index) {
                    *out = AssignedInputsPassive[i];
                    return true;
                }
                ++found;
            }
        }
    } else {
        if (AssignedInputsAggressive.empty())
            ResetAssignedInputs();
        long found = 0;
        for (unsigned long i = 0; i < AssignedInputsAggressive.size(); ++i) {
            if (AssignedInputsAggressive[i].GameAction == action) {
                if (found == index) {
                    *out = AssignedInputsAggressive[i];
                    return true;
                }
                ++found;
            }
        }
    }
    return false;
}
