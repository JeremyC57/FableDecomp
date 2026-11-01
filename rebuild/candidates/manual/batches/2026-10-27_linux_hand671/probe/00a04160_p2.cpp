// CInputEvent::GetChecksum() const -- input-event checksum: the sum of the integer fields, then each float view of the event-data union folded in through unsigned float conversions
// Leaf: ?GetChecksum@CInputEvent@@QBEKXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A04160, 145 bytes, thiscall, tail jump to __ftol2 0x00BFEA70.
// Identity (proven): the sole caller 0x009F51E6 is CInputManager::CalculateInternalChecksum() const 0x009F5170 (Ego_r.pdb
//   ?CalculateInternalChecksum@CInputManager@@QBEKXZ), which walks EventStore (CInputEvent, 0x34-byte stride, count
//   NoStoredEvents) and adds this function's result per element; Ego_r.pdb / FableWin.pdb ?GetChecksum@CInputEvent@@QBEKXZ is
//   the only CInputEvent member returning unsigned long with no argument. The catalog label
//   "_Dest_val<std::_Dest_val<std::allocator<CMacro>,CMacro>" is wrong (propagated).
// Callees: __ftol2 0x00BFEA70 (x4, the last as a tail jump); float pool 0x0122DCB4 = 4294967296.0f (unsigned -> float fix-up).
// Layout: Ego_r.pdb CInputEvent (0x34): Data (UInputEventData, 0x20: an anonymous union at +0x00 and one at +0x0C), DeviceType
//   +0x20, DeviceNumber +0x24, Type +0x28, CurrentTime +0x2C, StartTime +0x30. Union arm names: Ego_r UInputEventData_u_0_s_3
//   (JoystickPos C2DVector +0x00, JoystickButton +0x08), UInputEventData_u_12_s_0 (MouseMovement C3DVector +0x0C, MousePos
//   C2DVector +0x18); HeldMouseMovement (C3DVector +0x00) is the PDB getter ?GetHeldMouseMovement@CInputEvent@@QBE?AVC3DVector@@XZ
//   whose value SetAsLeftMouseButtonHeld 0x00A03D10 stores at +0x00..+0x08 (its C3DVector argument); Key (+0x00) as the landed
//   GetKey 0x00A03B70. No engine/CInputEvent.h; local model.
// Shape: one unsigned sum of the integer words, then `checksum += a + b (+ c);` per float view; VC7.1 adds the floats onto the
//   converted checksum last-first (fild ; fadd Z ; fadd Y ; fadd X), each `+=` closing with __ftol2.
class C2DVector { public: float X, Y; };
class C3DVector { public: float X, Y, Z; };
enum EInputDeviceType { };
enum EInputEventType { };
enum EInputKey { };
class UInputEventData {                                     // 32 bytes
public:
    union {                                                 // +0x00
        EInputKey      Key;
        unsigned short Char;
        C3DVector      HeldMouseMovement;
        struct {
            C2DVector     JoystickPos;                      // +0x00
            unsigned char JoystickButton;                   // +0x08
        };
    };
    union {                                                 // +0x0C
        struct {
            C3DVector MouseMovement;                        // +0x0C
            C2DVector MousePos;                             // +0x18
        };
    };
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
    unsigned long checksum = Data.Key + DeviceType + DeviceNumber + Type;
    checksum += Data.HeldMouseMovement.X + Data.HeldMouseMovement.Y + Data.HeldMouseMovement.Z;
    checksum += Data.MouseMovement.X + Data.MouseMovement.Y + Data.MouseMovement.Z;
    checksum += Data.MousePos.X + Data.MousePos.Y;
    checksum += Data.JoystickPos.X + Data.JoystickPos.Y;
    return checksum;
}
