// CActionInputControl::IsSameButton(const CActionInputControl& other) const -- same controller type and, for the keyboard / Xbox / mouse types, the same button of that device
// Leaf: ?IsSameButton@CActionInputControl@@QBE_NABV1@@Z
// Retail: Fable.exe @ 0x00405DB0, 65 bytes, __fastcall `ret 4`, bool in al.
// Identity: Ego_r.pdb / FableWin.pdb ?IsSameButton@CActionInputControl@@QBE_NABV1@@Z. Its only caller is the sibling
// ?IsSameActionAndButton@CActionInputControl@@QBE_NABV1@@Z @ 0x0040EDA0 (same GameAction, same GetDirection() @
// 0x0040ED10, then `call 0x00405DB0` on the same this/other).
// Layout (PDB names, as the landed GetDirection 0x0040ED10): GameAction +0x00, ControllerType +0x04, KeyboardKey +0x08,
// XboxButton +0x0C, MouseButton +0x10, ControlDirection +0x14.
// Shape: a switch on the controller type (dec / je chain): type 1 compares +0x0C, type 2 +0x08, type 3 +0x10; any other
// type counts as the same button. Each case is `if (a != b) return false; break;` (`je true ; xor al,al ; ret 4`);
// a `return a == b;` case materialises the compare with sub/neg/sbb. The cases are written 3, 2, 1: in 1, 2, 3 order the
// type-mismatch exit jumps to the first merged `xor al,al` tail instead of the last one.
class C2DVector { public:
    float X, Y;
};

class CActionInputControl { public:
    long GameAction;                            // +0x00 EGameAction
    long ControllerType;                        // +0x04 EControllerType
    long KeyboardKey;                           // +0x08 EInputKey
    long XboxButton;                            // +0x0c EXboxControllerButton
    long MouseButton;                           // +0x10 EMouseButtonControl
    C2DVector ControlDirection;                 // +0x14
    bool IsSameButton(const CActionInputControl& other) const;
};

bool CActionInputControl::IsSameButton(const CActionInputControl& other) const
{
    if (ControllerType != other.ControllerType)
        return false;
    switch (ControllerType) {
    case 3:
        if (MouseButton != other.MouseButton)
            return false;
        break;
    case 2:
        if (KeyboardKey != other.KeyboardKey)
            return false;
        break;
    case 1:
        if (XboxButton != other.XboxButton)
            return false;
        break;
    }
    return true;
}