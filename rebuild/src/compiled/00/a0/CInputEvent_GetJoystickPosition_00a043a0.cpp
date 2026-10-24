// CInputEvent::GetJoystickPosition() const -- the event's stick position mapped from the radially limited input to the rectangularly limited range, returned by value
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A043A0, length 35 bytes (`ret 4`: the hidden result pointer, + int3 padding).
// Identity: Ego_r.pdb name set `?GetJoystickPosition@CInputEvent@@QBE?AVC2DVector@@XZ`; 31 callers (landed ones call it
// GetJoystickPosition / GetAnalogueVector / GetAnaloguePosition). The catalog's "GetDefaultVal<CEngineGraphic>" is propagated.
// Callees: TransformControllerInputFromRadiallyToRectangularlyLimited @ 0x00A04200 (Ego_r.pdb
// `?TransformControllerInputFromRadiallyToRectangularlyLimited@?A0x9ab0773c@@YI?AVC2DVector@@ABV2@@Z`: an anonymous-namespace
// `__fastcall` helper of this TU, hidden result in ecx, the input in edx; declared at global scope here).
// Layout: CInputEvent (Ego_r.pdb, 0x34 bytes) -- Data (UInputEventData, 0x20 bytes) whose JoystickPos is at +0.
// Shape: the helper's result lands in a NAMED local and is returned through C2DVector's memberwise user copy constructor
// (X through `fld ; fstp`, Y through `mov`); returning the call straight passes the hidden pointer through (20 v 35).
// Leaf: ?GetJoystickPosition@CInputEvent
class C2DVector {
public:
    float X, Y;
    C2DVector() { }
    C2DVector(const C2DVector& v) : X(v.X), Y(v.Y) { }
};

C2DVector __fastcall TransformControllerInputFromRadiallyToRectangularlyLimited(const C2DVector& input);   // 0x00A04200

class CInputEvent {                       // 0x34 bytes
public:
    C2DVector     JoystickPos;            // +0x00 Data.JoystickPos (the UInputEventData union)
    unsigned char _data[0x18];            // +0x08 rest of the union
    long          DeviceType;             // +0x20 EInputDeviceType
    long          DeviceNumber;           // +0x24
    long          Type;                   // +0x28 EInputEventType
    float         CurrentTime;            // +0x2c
    float         StartTime;              // +0x30
    C2DVector GetJoystickPosition() const;
};

C2DVector CInputEvent::GetJoystickPosition() const
{
    C2DVector position = TransformControllerInputFromRadiallyToRectangularlyLimited(JoystickPos);
    return position;
}