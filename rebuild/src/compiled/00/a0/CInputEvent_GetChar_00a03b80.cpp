// CInputEvent::GetChar() const -- returns the character code held in the event data
// Retail: Fable.exe 0x00A03B80, 4 bytes: `mov ax,word ptr [ecx] ; ret`.
// Identity: catalog name; Ego_r.pdb ?GetChar@CInputEvent@@QBE_WXZ (VC7.1 retail builds wchar_t as unsigned short,
// so the TLC symbol mangles the return as G). PDB layout (0x34 bytes): Data (UInputEventData union) +0x00,
// DeviceType +0x20, DeviceNumber +0x24, Type +0x28, CurrentTime +0x2C, StartTime +0x30; the char is the union's
// first word.
// Leaf: ?GetChar@CInputEvent@@QBEGXZ
#include <stddef.h>

union UInputEventData {
    wchar_t       Char;
    unsigned char Raw[0x20];
};

class CInputEvent {
public:
    wchar_t GetChar() const;
    UInputEventData Data;          // +0x00
    int           DeviceType;      // +0x20 EInputDeviceType
    long          DeviceNumber;    // +0x24
    int           Type;            // +0x28 EInputEventType
    float         CurrentTime;     // +0x2C
    float         StartTime;       // +0x30
};

wchar_t CInputEvent::GetChar() const
{
    return Data.Char;
}