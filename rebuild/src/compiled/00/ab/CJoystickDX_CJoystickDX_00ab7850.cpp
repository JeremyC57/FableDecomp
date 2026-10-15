// CJoystickDX::CJoystickDX(CInputManagerDX* inputManager, bool exclusiveMode) -- DirectInput joystick constructor: the CJoystick base, zeroed axes, the default axis ranges (0x5556..0xaaaa), no device yet, the owning DX input manager, then SetExclusiveMode
// Leaf: ??0CJoystickDX
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AB7850, length 114 bytes.
// Callees: CJoystick::CJoystick(CInputManager*, bool) @ 0x009E43B0, CJoystickDX::SetExclusiveMode(bool) @ 0x00AB6DE0 (a virtual
// called from the constructor, so bound statically).
// Model: the PDB member order of CJoystickDX (XAxis, YAxis, XAxis2, YAxis2, XAxisMin, XAxisMax, YAxisMin, YAxisMax, DJoystick,
// Acquired, InputManagerDX) at retail offsets: the retail CJoystick base is 0xd18 bytes (its STLport list is 4 bytes), so
// every member sits 8 below engine/CJoystickDX.h (XAxis +0xd18 ... InputManagerDX +0xd40); the header's own +0xd18/+0xd1c
// "retail-only longs" are XAxis/YAxis. Vtable 0x0129EA7C.
// Shape: body assignments in retail's store order (manager, device, ranges, axes; initialisers would store in declaration
// order); VC keeps 0, 0x5556 and 0xaaaa in registers and schedules the 0xaaaa stores ahead of the vtable store.
struct CInputManager;
struct CInputManagerDX;
struct IDirectInputDevice8W;
class CJoystick {
public:
    virtual void v00();
    unsigned char _base04[0xd18 - 4];              // EventStore .. NoEventScansRunning
    CJoystick(CInputManager* inputManager, bool exclusiveMode);   // 0x009E43B0
};
class CJoystickDX : public CJoystick {
public:
    float                 XAxis;                   // +0xd18
    float                 YAxis;                   // +0xd1c
    float                 XAxis2;                  // +0xd20
    float                 YAxis2;                  // +0xd24
    long                  XAxisMin;                // +0xd28
    long                  XAxisMax;                // +0xd2c
    long                  YAxisMin;                // +0xd30
    long                  YAxisMax;                // +0xd34
    IDirectInputDevice8W* DJoystick;               // +0xd38
    bool                  Acquired;                // +0xd3c
    CInputManagerDX*      InputManagerDX;          // +0xd40
    CJoystickDX(CInputManagerDX* inputManager, bool exclusiveMode);
    virtual bool SetExclusiveMode(bool exclusive); // 0x00AB6DE0
};

CJoystickDX::CJoystickDX(CInputManagerDX* inputManager, bool exclusiveMode)
    : CJoystick((CInputManager*)inputManager, exclusiveMode)
{
    InputManagerDX = inputManager;
    DJoystick = 0;
    XAxisMin = 0x5556;
    XAxisMax = 0xaaaa;
    YAxisMin = 0x5556;
    YAxisMax = 0xaaaa;
    XAxis = 0.0f;
    YAxis = 0.0f;
    XAxis2 = 0.0f;
    YAxis2 = 0.0f;
    SetExclusiveMode(exclusiveMode);
}