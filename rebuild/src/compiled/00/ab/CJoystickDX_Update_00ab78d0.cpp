// Leaf: ?Update@CJoystickDX@@UAEXXZ
struct CJoystickDX; extern void __fastcall h1(CJoystickDX* joystickDX); extern void __fastcall h2(CJoystickDX* joystickDX);
struct CJoystickDX {
    virtual void Update();
};
void CJoystickDX::Update()
{
    CJoystickDX* joystickDX = this; h1(joystickDX); h2(joystickDX); }
// Flatten route (flatten_pilot v4, NF324): CJoystickDX_Update -> CJoystickDX::Update, the RTTI-placed PDB member; class model: the row's local class model.
