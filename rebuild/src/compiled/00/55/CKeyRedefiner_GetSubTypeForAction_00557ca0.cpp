// CKeyRedefiner::GetSubTypeForAction(const CActionInputControl& control) -- the sub-type of an action input: a fresh counter value for kinds 0x37 / 0x38, sub-types 10..13 for the four directions of a kind-0x3c (directional) input, else 0
// Leaf: GetSubTypeForAction
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00557CA0, length 120 bytes (including the four-entry jump table).
// Callees: CActionInputControl::GetDirection() @ 0x0040ED10 (EActionDirection 1..4). Globals 0x013B8ACC / 0x013B8AD0: the
// running sub-type counters handed out for kinds 0x38 / 0x37 (pre-incremented).
// Model: `this` is never read (the control reference replaces ecx; thiscall, `ret 4`); the control's first word is its
// input kind. Shape: the outer switch compiles to the `sub ; je ; dec ; je ; sub ; jne` chain, the inner dense
// switch to the `dec ; cmp 3 ; ja ; jmp [table]` form.
enum EActionDirection { DIRECTION_NONE, DIRECTION_UP, DIRECTION_DOWN, DIRECTION_LEFT, DIRECTION_RIGHT };
class CActionInputControl {
public:
    long GameAction;                               // +0x00 EGameAction (Ego_r.pdb)
    EActionDirection GetDirection() const;         // 0x0040ED10
};
extern long g_nextSubType38;                       // 0x013B8ACC
extern long g_nextSubType37;                       // 0x013B8AD0
class CKeyRedefiner {
public:
    long GetSubTypeForAction(const CActionInputControl& control);
};

long CKeyRedefiner::GetSubTypeForAction(const CActionInputControl& control)
{
    switch (control.GameAction) {
    case 0x37:
        return ++g_nextSubType37;
    case 0x38:
        return ++g_nextSubType38;
    case 0x3c:
        switch (control.GetDirection()) {
        case DIRECTION_UP:    return 10;
        case DIRECTION_DOWN:  return 11;
        case DIRECTION_LEFT:  return 12;
        case DIRECTION_RIGHT: return 13;
        }
        break;
    }
    return 0;
}