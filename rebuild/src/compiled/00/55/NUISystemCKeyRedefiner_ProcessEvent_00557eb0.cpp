// NUISystem::CKeyRedefiner::ProcessEvent(EEvent event) -- while this redefiner is the selected one, turn a key press (event 0x21: Escape-code 1 cancels, a redefinable key becomes a keyboard control) or a mouse event (0x1a/0x1b/0x23/0x26..0x2a -> mouse buttons 1/2/3/8..12) into a control for the first action, redefine it and drop the selection; anything else goes to CClickable::ProcessEvent
// Leaf: ProcessEvent
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00557EB0, length 440 bytes + a 9-entry jump table and 17-byte index table
//   (`ret 4`). Vtable slot 1 of the event-receiver base at +4: `this` is the full object + 4 (`lea ecx,[esi-4]` for the
//   primary-base calls), as the CMenuEntry::ProcessEvent row of this chunk (0x0053D200).
// Callees: CFrontEndManager::GetInstance 0x0041E5F2 (slot 13 +0x34: the last key code, unsigned short; slot 4 +0x10: focus
//   control), CKeyRedefiner::CancelSelection 0x00557BD0 / CanRedefineAction(long) 0x00557960 / Redefine(const
//   CActionInputControl&, bool) 0x00557D20 / UpdateKeyText 0x00557A10 (all landed), CClickable::ProcessEvent 0x0055AD60;
//   the control base's slots 3 (mode) and 4 (state) as the landed CancelSelection.
// Layout: CActionInputControl (28 bytes) and the retail CKeyRedefiner offsets (Actions list sentinel +0x19c, Definition
//   +0x1a4) as the landed CKeyRedefiner::Redefine 0x00557D20; the selected-redefiner global 0x013B8AC8 as the landed
//   CancelSelection. Flag 0x013B864F (CanRedefineAction's key-58 gate).
// Shape: the key code stays in edx across CanRedefineAction because its body (landed 0x00557960) is defined above under
//   `#pragma auto_inline(off)`, so VC7.1 knows it leaves edx alone; CancelSelection is called out of line for key 1 and
//   its body is inlined after Redefine (the forceinline EndSelection twin).
namespace NUISystem {
enum EEvent { EVENT_KEY = 0x21 };
struct C2DVector {
    float X, Y;
    C2DVector() : X(0), Y(0) {}
};
struct CActionInputControl {
    long      GameAction;                          // +0x00
    long      ControllerType;                      // +0x04
    long      KeyboardKey;                         // +0x08
    long      XboxButton;                          // +0x0c
    long      MouseButton;                         // +0x10
    C2DVector ControlDirection;                    // +0x14
    CActionInputControl() : GameAction(0), ControllerType(0), KeyboardKey(0), XboxButton(0), MouseButton(0) {}
};
struct ActionNode {
    ActionNode* _M_next;
    ActionNode* _M_prev;
    long        _M_data;
};
struct CComponentVirtuals {
    virtual void v00();
};
struct CEventReceiver {                            // the second base (vptr at +4)
    virtual void v00();
    virtual void ProcessEvent(EEvent event);       // slot 1
    virtual void v02();
    virtual void SetMode(long mode);               // slot 3 (+0x0c)
    virtual void SetState(long state);             // slot 4 (+0x10)
};
struct CFrontEndManager {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void SetFocusControl(CEventReceiver* control);   // slot 4 (+0x10)
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12();
    virtual unsigned short GetLastKey();           // slot 13 (+0x34) (descriptive)
    static CFrontEndManager* __fastcall GetInstance();   // 0x0041E5F2
};
class CClickable : public CComponentVirtuals, public CEventReceiver {
public:
    virtual void ProcessEvent(EEvent event);       // 0x0055AD60
};
class CKeyRedefiner : public CClickable {
public:
    unsigned char       _pad008[0x19c - 0x08];
    ActionNode*         Actions;                   // +0x19c
    long                SubType;                   // +0x1a0
    CActionInputControl Definition;                // +0x1a4
    static CKeyRedefiner* s_pSelected;             // 0x013B8AC8
    void CancelSelection();                        // 0x00557BD0
    bool CanRedefineAction(long key);              // 0x00557960
    void Redefine(const CActionInputControl& control, bool notify);   // 0x00557D20
    void UpdateKeyText();                          // 0x00557A10
    virtual void ProcessEvent(EEvent event);
    __forceinline void EndSelection()              // the body of CancelSelection, inlined at the end of a redefinition
    {
        if (s_pSelected == this) {
            s_pSelected = 0;
            UpdateKeyText();
            SetState(0x21);
            SetMode(0x19);
            CFrontEndManager::GetInstance()->SetFocusControl(this);
        }
    }
};

extern unsigned char g_KeyRedefinerFlag;           // 0x013B864F
#pragma auto_inline(off)
bool CKeyRedefiner::CanRedefineAction(long category)   // 0x00557960 (the landed body)
{
    switch (category) {
    case 1: case 89: case 90: case 91: case 98: case 118: case 119:
        return false;
    case 58:
        if (g_KeyRedefinerFlag == 0)
            break;
        return false;
    default:
        break;
    }
    return true;
}
#pragma auto_inline(on)

void CKeyRedefiner::ProcessEvent(EEvent event)
{
    CActionInputControl control;
    control.GameAction = Actions->_M_next->_M_data;
    control.ControllerType = 0;
    control.ControlDirection = Definition.ControlDirection;
    if (s_pSelected == this) {
        switch (event) {
        case EVENT_KEY:
            {
                long key = CFrontEndManager::GetInstance()->GetLastKey();
                if (key == 1) {
                    CancelSelection();
                    return;
                }
                if (CanRedefineAction(key)) {
                    control.ControllerType = 2;
                    control.KeyboardKey = key;
                }
            }
            break;
        case 0x1a: control.ControllerType = 3; control.MouseButton = 1; break;
        case 0x1b: control.ControllerType = 3; control.MouseButton = 2; break;
        case 0x23: control.ControllerType = 3; control.MouseButton = 3; break;
        case 0x26: control.ControllerType = 3; control.MouseButton = 8; break;
        case 0x27: control.ControllerType = 3; control.MouseButton = 9; break;
        case 0x28: control.ControllerType = 3; control.MouseButton = 0xa; break;
        case 0x29: control.ControllerType = 3; control.MouseButton = 0xb; break;
        case 0x2a: control.ControllerType = 3; control.MouseButton = 0xc; break;
        }
        if (control.ControllerType != 0) {
            Redefine(control, true);
            EndSelection();
            return;
        }
    }
    CClickable::ProcessEvent(event);
}
}