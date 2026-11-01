// CInputEvent::GetDeviceType() const -- the device type of the event (+0x20)
// Leaf: ?GetDeviceType@CInputEvent@@QBE?AW4EInputDeviceType@@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A03B50 (`mov eax,[ecx+20h]` / `ret`); Ego_r.pdb
// ?GetDeviceType@CInputEvent@@QBE?AW4EInputDeviceType@@XZ. Layout: Ego_r.pdb CInputEvent (0x34 bytes).
// Re-modelled as the member in naming wave 15 (was the free __fastcall stand-in DrawGetEnvironment(const unsigned char*)).
enum EInputKey { };                             // values unused here
enum EInputDeviceType { };
enum EInputEventType { };
class CInputEvent { public:
    unsigned char    Data[0x20];                // +0x00 UInputEventData
    EInputDeviceType DeviceType;                // +0x20
    long             DeviceNumber;              // +0x24
    EInputEventType  Type;                      // +0x28
    float            CurrentTime;               // +0x2c
    float            StartTime;                 // +0x30
    EInputDeviceType GetDeviceType() const;
};

EInputDeviceType CInputEvent::GetDeviceType() const
{
    return DeviceType;
}
