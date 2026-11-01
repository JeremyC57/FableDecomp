// CTCActivationReceptorDoor::Deactivate() -- close the parent thing's door (SetDoorState(false)), then the base receptor Deactivate
// Leaf: ?Deactivate@CTCActivationReceptorDoor@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007F8140, 18 bytes, thiscall: `push 0 ; call SetDoorState`, then
//   `jmp CTCActivationReceptorBase::Deactivate` 0x007F7700 (non-virtual base call as a tail jump).
// Identity: the only reference is slot 29 of the CTCActivationReceptorDoor vtable 0x0124146C (rtti-vtables.tsv); slot 29 is
//   Deactivate in the receptor family (landed CTCActivationReceptorChest::Deactivate 0x007FFCD0, Ego_r
//   ?Deactivate@CTCActivationReceptorChest@@UAEXXZ; base slot 29 = CTCActivationReceptorBase::Deactivate 0x007F7700, bound).
//   Slot 28 (0x007F8120) is the same body with `push 1` and a tail jump to CTCActivationReceptorBase::Activate 0x007F76E0.
//   Ego_r.pdb lists, in this order, ?SetDoorState@CTCActivationReceptorDoor@@AAEX_N@Z (0x007EFD10),
//   ?Activate@CTCActivationReceptorDoor@@UAEXXZ (0x007EFD70) and ?Deactivate@CTCActivationReceptorDoor@@UAEXXZ (0x007EFDF0),
//   exactly as retail places 0x007F80C0 / 0x007F8120 / 0x007F8140. The manifest "vfunc_29" and the twin 0x00C2D2D0 are
//   other names.
// Callees: CTCActivationReceptorDoor::SetDoorState(bool) 0x007F80C0 (private; landed under the descriptive "SetDoorOpen",
//   NAME_FIXES), CTCActivationReceptorBase::Deactivate 0x007F7700.
// Model: the landed receptor-family local model (CTCActivationReceptorChest 0x007FFCD0): engine/CTCActivationReceptorBase.h
//   flattens the vptr into a data member, so it cannot carry the virtuals (header debt, NOTES).
class CThing;

class CTCBase {
public:                                                 // engine/CTCBase.h layout, modelled polymorphic
    virtual void v00();
    CThing* ParentThing;                                // +0x04
    bool    ToBeRemoved;                                // +0x08
    bool    IsInGlobalUpdateTCs;                        // +0x09
};

class CTCActivationReceptorBase : public CTCBase {     // retail 0x2c bytes
public:
    unsigned char _body[0x20];                          // +0x0c Activated .. +0x28 FrameAtLastActivation
    virtual void Activate();                            // slot 28: 0x007F76E0
    virtual void Deactivate();                          // slot 29: 0x007F7700
};

class CTCActivationReceptorDoor : public CTCActivationReceptorBase {   // vtable 0x0124146C
public:
    virtual void Activate();                            // slot 28: 0x007F8120
    virtual void Deactivate();                          // slot 29: this row
private:
    void SetDoorState(bool open);                       // 0x007F80C0 (Ego_r ?SetDoorState@CTCActivationReceptorDoor@@AAEX_N@Z)
};

void CTCActivationReceptorDoor::Deactivate()
{
    SetDoorState(false);
    CTCActivationReceptorBase::Deactivate();
}