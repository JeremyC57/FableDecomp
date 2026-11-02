// CTCActivationReceptorDoor::Activate() -- open the parent thing's door (SetDoorState(true)), then the base receptor Activate
// Leaf: ?Activate@CTCActivationReceptorDoor@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007F8120, 18 bytes, thiscall: `push 1 ; call SetDoorState`, then
//   `jmp CTCActivationReceptorBase::Activate` 0x007F76E0 (non-virtual base call as a tail jump).
// Identity (proven): the only reference is slot 28 of the CTCActivationReceptorDoor vtable 0x0124146C (0x012414DC); slot 29
//   0x007F8140 is the landed CTCActivationReceptorDoor::Deactivate (same body, `push 0`, jmp base Deactivate 0x007F7700).
//   Ego_r.pdb lists, in this order, ?SetDoorState@CTCActivationReceptorDoor@@AAEX_N@Z (0x007EFD10),
//   ?Activate@CTCActivationReceptorDoor@@UAEXXZ (0x007EFD70) and ?Deactivate@CTCActivationReceptorDoor@@UAEXXZ (0x007EFDF0),
//   exactly as retail places 0x007F80C0 / 0x007F8120 / 0x007F8140; the tail target 0x007F76E0 is bound as
//   ?Activate@CTCActivationReceptorBase@@UAEXXZ (Ego_r 0x007EF540). The catalog label "vfunc_28" is a placeholder.
// Callees: CTCActivationReceptorDoor::SetDoorState(bool) 0x007F80C0 (private), CTCActivationReceptorBase::Activate 0x007F76E0.
// Model: the landed sibling's receptor-family local model (CTCActivationReceptorDoor::Deactivate 0x007F8140, same retail TU):
//   engine/CTCActivationReceptorBase.h flattens the vptr into a data member, so it cannot carry the virtuals (header debt).
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
    virtual void Activate();                            // slot 28: this row
    virtual void Deactivate();                          // slot 29: 0x007F8140
private:
    void SetDoorState(bool open);                       // 0x007F80C0 (Ego_r ?SetDoorState@CTCActivationReceptorDoor@@AAEX_N@Z)
};

void CTCActivationReceptorDoor::Activate()
{
    SetDoorState(true);
    CTCActivationReceptorBase::Activate();
}