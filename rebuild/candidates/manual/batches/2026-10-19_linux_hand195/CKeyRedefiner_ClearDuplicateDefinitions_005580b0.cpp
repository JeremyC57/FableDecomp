// CKeyRedefiner::ClearDuplicateDefinitions(const CActionInputControl& other) -- when another redefiner's new binding collides with ours, re-bind ours as an unassigned keyboard control
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005580B0, length 189 bytes (`ret 4`).
// Callees: AreAllowedToCoexist(EGameAction, EGameAction) @ 0x005578A0 (`__fastcall`), CKeyRedefiner::Redefine(CActionInputControl*, bool)
// @ 0x00557D20, CKeyRedefiner::UpdateKeyText() @ 0x00557A10.
// Layout: the landed ctor 0x00558540 (retail CClickable is 0x194 bytes): Actions (list<EGameAction>, one sentinel pointer) +0x19C,
// Definition (CActionInputControl, 28 bytes) +0x1A4. No engine header exists for NUISystem::CKeyRedefiner.
// Shape: the front action (head->next->value) is loaded first (right-to-left argument order of the fastcall); a three-case switch on the
// controller type compares the type-specific field; the replacement definition is a copy of ours with ControllerType = 2 and
// KeyboardKey = 0 (the copied +4/+8 words are dead and dropped).
struct C2DVector {
    float X, Y;
};
struct CActionInputControl {                    // 28 bytes
    long GameAction;                            // +0x00
    long ControllerType;                        // +0x04
    long KeyboardKey;                           // +0x08
    long XboxButton;                            // +0x0c
    long MouseButton;                           // +0x10
    C2DVector ControlDirection;                 // +0x14
    CActionInputControl(const CActionInputControl& o)
        : GameAction(o.GameAction), ControllerType(o.ControllerType), KeyboardKey(o.KeyboardKey),
          XboxButton(o.XboxButton), MouseButton(o.MouseButton), ControlDirection(o.ControlDirection) { }
};
bool __fastcall AreAllowedToCoexist(long a, long b);   // 0x005578A0
namespace NUISystem {
struct CActionList {                            // std::list<EGameAction>: one sentinel pointer
    struct node { node* next; node* prev; long value; };
    node* head;
    long front() const { return head->next->value; }
};
struct CKeyRedefiner {
    unsigned char _clickable[0x194];            // CClickable (three vptrs) +0x000..+0x193
    void* PActionText;                          // +0x194
    void* PKeyText;                             // +0x198
    CActionList Actions;                        // +0x19c
    long SubType;                               // +0x1a0
    CActionInputControl Definition;             // +0x1a4
    void Redefine(CActionInputControl* control, bool notify);   // 0x00557D20
    void UpdateKeyText();                                       // 0x00557A10
    void ClearDuplicateDefinitions(const CActionInputControl& other);
};

void CKeyRedefiner::ClearDuplicateDefinitions(const CActionInputControl& other)
{
    if (AreAllowedToCoexist(other.GameAction, Actions.front()))
        return;
    if (Definition.ControllerType != other.ControllerType)
        return;
    switch (Definition.ControllerType) {
    case 1:
        if (Definition.XboxButton != other.XboxButton)
            return;
        break;
    case 2:
        if (Definition.KeyboardKey != other.KeyboardKey)
            return;
        break;
    case 3:
        if (Definition.MouseButton != other.MouseButton)
            return;
        break;
    }
    CActionInputControl cleared = Definition;
    cleared.ControllerType = 2;
    cleared.KeyboardKey = 0;
    Redefine(&cleared, false);
    UpdateKeyText();
}
}
