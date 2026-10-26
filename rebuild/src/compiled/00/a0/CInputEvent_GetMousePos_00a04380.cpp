// CInputEvent::GetMousePos() const -- the mouse event's 2D position, returned by value
// Leaf: ?GetMousePos@CInputEvent@@QBE?AVC2DVector@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A04380, 18 bytes, thiscall `ret 4` (the hidden result pointer): the two dwords at
//   this+0x18 / this+0x1C copied into the result.
// Identity (sweep3; catalog CMessageEventFilter_IsOriginatedByThing::CMessageEventFilter_IsOriginatedByThing, a `struct T`
//   stand-in; FINDINGS chunk 483 "0xA04380/0xA043D0 likely GetMousePos/GetMouseMovement"): Ego_r.pdb defines
//   GetMousePos, GetJoystickPosition, GetMouseMovement, GetHeldMouseMovement, CInputEvent() in that order (0xB6EF60 ..
//   0xB6EFE0); retail 0x00A04380 / 0x00A043A0 (landed GetJoystickPosition) / 0x00A043D0 / 0x00A043F0 / 0x00A04410 is that
//   run with matching shapes: an 8-byte by-value result here (C2DVector), 12-byte results at 0x00A043D0 / 0x00A043F0
//   (C3DVector), the constructor zeroing the fields at 0x00A04410.
// Layout: CInputEvent (Ego_r.pdb, 0x34 bytes): Data (UInputEventData, 0x20 bytes) +0x00, DeviceType +0x20, DeviceNumber
//   +0x24, Type +0x28, CurrentTime +0x2C, StartTime +0x30. The mouse position sits at Data+0x18; the union member's name
//   is not in the layout dump (`MousePos` is descriptive).
// Model: C2DVector as in the landed same-TU GetJoystickPosition 0x00A043A0 (memberwise user copy constructor).
class C2DVector {
public:
    float X, Y;
    C2DVector() { }
    C2DVector(const C2DVector& v) : X(v.X), Y(v.Y) { }
};

class CInputEvent {                       // 0x34 bytes
public:
    unsigned char _data[0x18];            // +0x00 the UInputEventData union up to the mouse position
    C2DVector     MousePos;               // +0x18 (Data+0x18; name descriptive)
    long          DeviceType;             // +0x20 EInputDeviceType
    long          DeviceNumber;           // +0x24
    long          Type;                   // +0x28 EInputEventType
    float         CurrentTime;            // +0x2c
    float         StartTime;              // +0x30
    C2DVector GetMousePos() const;
};

C2DVector CInputEvent::GetMousePos() const
{
    return MousePos;
}
