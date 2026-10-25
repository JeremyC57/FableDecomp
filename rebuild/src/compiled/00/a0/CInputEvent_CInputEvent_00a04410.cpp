// CInputEvent::CInputEvent() -- an empty input event: Type and DeviceNumber zeroed, the 32-byte event-data union cleared;
// DeviceType (+0x20) and the two times are left unset
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A04410, length 36 bytes.
// Callees: none (the 32-byte memset is the intrinsic). Name: Ego_r.pdb ??0CInputEvent@@QAE@XZ (0x00B6EFE0); the PDB
// layout is the 0x34-byte CInputEvent { UInputEventData Data +0 (32 bytes); EInputDeviceType DeviceType +0x20;
// long DeviceNumber +0x24; EInputEventType Type +0x28; float CurrentTime +0x2C; float StartTime +0x30 }, matching the
// stores here, and the CKeyboard / CMouse constructors build their CInputEvent EventStore[256] with it at a 0x34 stride
// (agent420 NOTES). The manifest label "CDefaultEngineMemoryInit::CDefaultEngineMemoryInit" is propagated.
// Shape: the two scalar assignments are stored first (+0x28, +0x24), then the intrinsic memset of the union runs through
// a second copy of `this` (`mov edx,eax`) with the same zero register (an inline member constructor would zero the block
// through eax with no edx copy).
// Model: local class (no engine/CInputEvent.h); the union's anonymous members are kept opaque.
// Leaf: ??0CInputEvent@@QAE@XZ
#include <string.h>

enum EInputDeviceType { };
enum EInputEventType { };

union UInputEventData {                            // 32 bytes (Ego_r.pdb: two anonymous structs at +0 / +0xC)
    unsigned char Raw[0x20];
};

class CInputEvent {
public:
    UInputEventData  Data;                         // +0x00
    EInputDeviceType DeviceType;                   // +0x20 (not initialised)
    long             DeviceNumber;                 // +0x24
    EInputEventType  Type;                         // +0x28
    float            CurrentTime;                  // +0x2C (not initialised)
    float            StartTime;                    // +0x30 (not initialised)
    CInputEvent();
};

CInputEvent::CInputEvent()
{
    Type = (EInputEventType)0;
    DeviceNumber = 0;
    memset(&Data, 0, sizeof(Data));
}
