// CInputEvent::GetType() const -- the event's type (+0x28)
// Leaf: ?GetType@CInputEvent@@QBE?AW4EInputEventType@@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A03B40 (`mov eax,[ecx+28h]` / `ret`); Ego_r.pdb
// ?GetType@CInputEvent@@QBE?AW4EInputEventType@@XZ. Layout: Ego_r.pdb CInputEvent (0x34 bytes).
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
    EInputEventType GetType() const;
};

EInputEventType CInputEvent::GetType() const
{
    return Type;
}
