// CAbilityButtonGuiDef::Transfer(CPersistContext& context) -- persist every member without member names
// Leaf: ?Transfer@CAbilityButtonGuiDef@@UAEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0046F203, 79 bytes, thiscall, `ret 4` (size-optimised module).
// Identity (proven): slot 18 of vtable 0x0123610C (RTTI .?AVCAbilityButtonGuiDef@@; Ego_r.pdb ?Transfer@CAbilityButtonGuiDef@@UAEXAAVCPersistContext@@@Z);
//   the 6 Transfer calls take, in call order, the members at +0x3c, +0x40, +0x44, +0x48, +0x4c, +0x54 with the per-type callees 0x0046F252, 0x0046F2F6, 0x0046F39A, 0x00431102, 0x004568BC, 0x004568BC, which are
//   Ego_r.pdb CAbilityButtonGuiDef's members ControllerType, ControllerButton, MouseButton, ButtonGraphic, Position, Offset.
// Shape (family M050, representative 0x0046651B): `context.Transfer(Member);` per member; `optimize("s", on)` (each member
//   address formed with `lea eax,[esi+N]`, the last as `add esi,N`).
// Header debt: the def base is modelled by its retail size (pack(1), vptr + members) with the virtual Transfer only.
#pragma optimize("s", on)
enum EControllerType { ECONTROLLERTYPE_NONE = 0 };
enum EXboxControllerButton { EXBOXCONTROLLERBUTTON_NONE = 0 };
enum EMouseButtonControl { EMOUSEBUTTONCONTROL_NONE = 0 };
class C2DVector { public: float X; float Y; };
class CPersistContext {
public:
    template <class T> void Transfer(T& value);
};
#pragma pack(push, 1)
class CParentDefClassBase {
public:
    virtual void Transfer(CPersistContext& context);
    unsigned char _base[0x39 - 4];
};
#pragma pack(pop)
class CAbilityButtonGuiDef : public CParentDefClassBase {
public:
    virtual void Transfer(CPersistContext& context);
    EControllerType          ControllerType;                // +0x3c
    EXboxControllerButton    ControllerButton;              // +0x40
    EMouseButtonControl      MouseButton;                   // +0x44
    long                     ButtonGraphic;                 // +0x48
    C2DVector                Position;                      // +0x4c
    C2DVector                Offset;                        // +0x54
};
void CAbilityButtonGuiDef::Transfer(CPersistContext& context)
{
    context.Transfer(ControllerType);
    context.Transfer(ControllerButton);
    context.Transfer(MouseButton);
    context.Transfer(ButtonGraphic);
    context.Transfer(Position);
    context.Transfer(Offset);
}