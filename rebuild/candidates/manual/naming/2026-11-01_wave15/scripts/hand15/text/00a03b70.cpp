// CInputEvent::GetKey() const -- the key of a key event (the first dword of the event data, +0x00)
// Leaf: ?GetKey@CInputEvent@@QBE?AW4EInputKey@@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A03B70 (`mov eax,[ecx]` / `ret`); Ego_r.pdb
// ?GetKey@CInputEvent@@QBE?AW4EInputKey@@XZ. Layout: Ego_r.pdb CInputEvent (0x34 bytes; UInputEventData at +0x00).
// Re-modelled as the member in naming wave 15 (was the packed stand-in `struct T { int field; int Get(); }`).
enum EInputKey { };                             // values unused here
enum EInputDeviceType { };
enum EInputEventType { };
class CInputEvent { public:
    EInputKey        Key;                       // +0x00 UInputEventData (key events)
    unsigned char    _Data04[0x1c];             // +0x04 rest of UInputEventData
    EInputDeviceType DeviceType;                // +0x20
    long             DeviceNumber;              // +0x24
    EInputEventType  Type;                      // +0x28
    float            CurrentTime;               // +0x2c
    float            StartTime;                 // +0x30
    EInputKey GetKey() const;
};

EInputKey CInputEvent::GetKey() const
{
    return Key;
}
