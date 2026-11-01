// CInputEvent::GetChecksum() const -- input-event checksum: the sum of the integer fields, then each float view of the event-data union folded in through unsigned float conversions
// Leaf: ?GetChecksum@CInputEvent@@QBEKXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A04160, 145 bytes, thiscall, tail jump to __ftol2 0x00BFEA70.
// Identity (proven): the sole caller 0x009F51E6 is CInputManager::CalculateInternalChecksum() const 0x009F5170 (Ego_r.pdb
//   ?CalculateInternalChecksum@CInputManager@@QBEKXZ), which walks EventStore (CInputEvent, 0x34-byte stride, count
//   NoStoredEvents) and adds this function's result per element; Ego_r.pdb / FableWin.pdb ?GetChecksum@CInputEvent@@QBEKXZ is
//   the only CInputEvent member returning unsigned long with no argument. The catalog label
//   "_Dest_val<std::_Dest_val<std::allocator<CMacro>,CMacro>" is wrong (propagated).
// Callees: __ftol2 0x00BFEA70 (x4, the last as a tail jump); float pool 0x0122DCB4 = 4294967296.0f (unsigned -> float fix-up).
// Layout: Ego_r.pdb CInputEvent (0x34): Data (UInputEventData, 0x20: union of anonymous arms at +0x00 and +0x0C -- Key / Char,
//   JoystickPos (C2DVector +0x00), JoystickButton +0x08; MouseMovement (C3DVector +0x0C), MousePos (C2DVector +0x18)),
//   DeviceType +0x20, DeviceNumber +0x24, Type +0x28, CurrentTime +0x2C, StartTime +0x30. The float triple at +0x00 is the
//   arm read here as a C3DVector (descriptive: Ego_r gives the arm no name).
class C2DVector { public: float X, Y; };
class C3DVector { public: float X, Y, Z; };
enum EInputDeviceType { };
enum EInputEventType { };
enum EInputKey { };
union UInputEventData {                                     // 32 bytes
    struct { EInputKey Key; } KeyArm;                       // +0x00
    struct { C3DVector Vector; } VectorArm;                 // +0x00 (descriptive)
    struct { C2DVector JoystickPos; unsigned char JoystickButton; } JoystickArm;   // +0x00
    struct { unsigned char Head[0x0C]; C3DVector MouseMovement; C2DVector MousePos; } MouseArm;   // +0x0C / +0x18
};
class CInputEvent {
public:
    UInputEventData  Data;                                  // +0x00
    EInputDeviceType DeviceType;                            // +0x20
    long             DeviceNumber;                          // +0x24
    EInputEventType  Type;                                  // +0x28
    float            CurrentTime;                           // +0x2C
    float            StartTime;                             // +0x30
    unsigned long GetChecksum() const;
};

unsigned long CInputEvent::GetChecksum() const
{
    unsigned long checksum = Data.KeyArm.Key + DeviceType + DeviceNumber + Type;
    checksum += Data.VectorArm.Vector.X + Data.VectorArm.Vector.Y + Data.VectorArm.Vector.Z;
    checksum += Data.MouseArm.MouseMovement.X + Data.MouseArm.MouseMovement.Y + Data.MouseArm.MouseMovement.Z;
    checksum += Data.MouseArm.MousePos.X + Data.MouseArm.MousePos.Y;
    checksum += Data.JoystickArm.JoystickPos.X + Data.JoystickArm.JoystickPos.Y;
    return checksum;
}
