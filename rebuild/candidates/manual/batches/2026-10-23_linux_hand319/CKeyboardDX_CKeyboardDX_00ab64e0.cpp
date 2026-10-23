// CKeyboardDX::CKeyboardDX(CInputManagerDX* manager, bool exclusive) -- build the DirectInput keyboard: CKeyboard base, remember the input manager, clear the key states, create the system-keyboard device, set its cooperative level, data format and a 256-entry buffer and acquire it, then fill the engine-key -> DIK scan-code table
// Leaf: ??0CKeyboardDX
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AB64E0, length 1293 bytes, `ret 8`.
// Callees: CKeyboard::CKeyboard(CInputManager*, bool) @ 0x00A66930 (manifest label "NHeroInformationScreens_Keyboard_Constructor"
//   is propagated), CKeyboardDX::SetExclusiveMode(bool) @ 0x00AB6450 (landed; bound statically inside the ctor), the
//   IDirectInput8 COM slot 3 (+0x0c CreateDevice) on the manager's DirectInput (+0x80), and the IDirectInputDevice8 COM
//   slots 6 (+0x18 SetProperty), 7 (+0x1c Acquire) and 11 (+0x2c SetDataFormat). Vtable 0x0129EA60.
//   Data: GUID_SysKeyboard @ 0x012ACA94, c_dfDIKeyboard @ 0x012AE004; DIPROP_BUFFERSIZE = (GUID*)1.
// Layout: retail CKeyboardDX (as the landed CKeyboardDX::SetExclusiveMode 0x00AB6450 / GetKeyName 0x00AB6180): InputManagerDX
//   +0x3414, DKeyboard +0x3418, KeyStates[256] +0x341c, InputKeyToDXKey[122] +0x351c, Acquired +0x3610. The PDB view
//   (engine/CKeyboard.h, sizeof 0x341c) is 8 bytes larger: retail's STLport maintained-event list is 4 bytes and the
//   base ends at +0x3414, so the base is modelled locally as an opaque 0x3414-byte block.
// Shape: the key-state clear is an inlined memset (`rep stosd`, 0x40 dwords); HRESULTs are compared with DI_OK (`test ; jne`);
//   the zero and 0x10 key-table entries reuse the DIPROPDWORD constants VC7.1 keeps in ebp/ebx.
#include <string.h>
struct DIPROPHEADER_Retail {
    unsigned long dwSize;
    unsigned long dwHeaderSize;
    unsigned long dwObj;
    unsigned long dwHow;
};
struct DIPROPDWORD_Retail {
    DIPROPHEADER_Retail diph;
    unsigned long       dwData;
};
struct GUID_Retail { unsigned long Data1; unsigned short Data2, Data3; unsigned char Data4[8]; };
extern const GUID_Retail GUID_SysKeyboard;         // 0x012ACA94
extern const unsigned char c_dfDIKeyboard[0x18];   // 0x012AE004 (DIDATAFORMAT)
struct IDirectInputDevice8_Retail {                // COM: __stdcall methods, `this` pushed
    virtual long __stdcall QueryInterface(void* iid, void** out);
    virtual unsigned long __stdcall AddRef();
    virtual unsigned long __stdcall Release();
    virtual long __stdcall GetCapabilities(void* caps);
    virtual long __stdcall EnumObjects(void* callback, void* ref, unsigned long flags);
    virtual long __stdcall GetProperty(const GUID_Retail* guid, DIPROPHEADER_Retail* header);
    virtual long __stdcall SetProperty(const GUID_Retail* guid, const DIPROPHEADER_Retail* header);   // slot 6 (+0x18)
    virtual long __stdcall Acquire();              // slot 7 (+0x1c)
    virtual long __stdcall Unacquire();            // slot 8 (+0x20)
    virtual long __stdcall GetDeviceState(unsigned long size, void* data);
    virtual long __stdcall GetDeviceData(unsigned long size, void* data, unsigned long* count, unsigned long flags);
    virtual long __stdcall SetDataFormat(const void* format);   // slot 11 (+0x2c)
};
struct IDirectInput8_Retail {
    virtual long __stdcall QueryInterface(void* iid, void** out);
    virtual unsigned long __stdcall AddRef();
    virtual unsigned long __stdcall Release();
    virtual long __stdcall CreateDevice(const GUID_Retail& guid, IDirectInputDevice8_Retail** device, void* outer);   // slot 3 (+0x0c)
};
struct CInputManager {};
struct CInputManagerDX : CInputManager {
    unsigned char         _pad00[0x80];
    IDirectInput8_Retail* DInput;                  // +0x80
};
struct CKeyboard {                                 // retail layout 0x3414 bytes (vtable 0x0129DBA8)
    CKeyboard(CInputManager* manager, bool exclusive);   // 0x00A66930
    virtual ~CKeyboard();
    unsigned char _body[0x3414 - 4];
};
class CKeyboardDX : public CKeyboard {
public:
    CKeyboardDX(CInputManagerDX* manager, bool exclusive);
    virtual ~CKeyboardDX();
    bool SetExclusiveMode(bool exclusive);         // 0x00AB6450
    CInputManagerDX*            InputManagerDX;    // +0x3414
    IDirectInputDevice8_Retail* DKeyboard;         // +0x3418
    unsigned char               KeyStates[0x100];  // +0x341c
    unsigned short              InputKeyToDXKey[0x7a];   // +0x351c
    bool                        Acquired;          // +0x3610
};

CKeyboardDX::CKeyboardDX(CInputManagerDX* manager, bool exclusive)
    : CKeyboard(manager, exclusive), InputManagerDX(manager)
{
    memset(KeyStates, 0, sizeof(KeyStates));
    if (manager->DInput->CreateDevice(GUID_SysKeyboard, &DKeyboard, 0) == 0
        && SetExclusiveMode(exclusive)
        && DKeyboard->SetDataFormat(c_dfDIKeyboard) == 0) {
        DIPROPDWORD_Retail prop;
        prop.diph.dwSize = sizeof(DIPROPDWORD_Retail);
        prop.diph.dwHeaderSize = sizeof(DIPROPHEADER_Retail);
        prop.diph.dwObj = 0;
        prop.diph.dwHow = 0;
        prop.dwData = 0x100;
        if (DKeyboard->SetProperty((const GUID_Retail*)1, &prop.diph) == 0 && DKeyboard->Acquire() == 0)
            Acquired = true;
    }
    InputKeyToDXKey[0] = 0x00;  InputKeyToDXKey[1] = 0x01;  InputKeyToDXKey[2] = 0x02;  InputKeyToDXKey[3] = 0x03;
    InputKeyToDXKey[4] = 0x04;  InputKeyToDXKey[5] = 0x05;  InputKeyToDXKey[6] = 0x06;  InputKeyToDXKey[7] = 0x07;
    InputKeyToDXKey[8] = 0x08;  InputKeyToDXKey[9] = 0x09;  InputKeyToDXKey[10] = 0x0a; InputKeyToDXKey[11] = 0x0b;
    InputKeyToDXKey[12] = 0x0c; InputKeyToDXKey[13] = 0x0d; InputKeyToDXKey[14] = 0x0e; InputKeyToDXKey[15] = 0x0f;
    InputKeyToDXKey[16] = 0x10; InputKeyToDXKey[17] = 0x11; InputKeyToDXKey[18] = 0x12; InputKeyToDXKey[19] = 0x13;
    InputKeyToDXKey[20] = 0x14; InputKeyToDXKey[21] = 0x15; InputKeyToDXKey[22] = 0x16; InputKeyToDXKey[23] = 0x17;
    InputKeyToDXKey[24] = 0x18; InputKeyToDXKey[25] = 0x19; InputKeyToDXKey[26] = 0x1a; InputKeyToDXKey[27] = 0x1b;
    InputKeyToDXKey[28] = 0x1c; InputKeyToDXKey[29] = 0x1d; InputKeyToDXKey[30] = 0x1e; InputKeyToDXKey[31] = 0x1f;
    InputKeyToDXKey[32] = 0x20; InputKeyToDXKey[33] = 0x21; InputKeyToDXKey[34] = 0x22; InputKeyToDXKey[35] = 0x23;
    InputKeyToDXKey[36] = 0x24; InputKeyToDXKey[37] = 0x25; InputKeyToDXKey[38] = 0x26; InputKeyToDXKey[39] = 0x27;
    InputKeyToDXKey[40] = 0x28; InputKeyToDXKey[41] = 0x29; InputKeyToDXKey[42] = 0x2a; InputKeyToDXKey[43] = 0x2b;
    InputKeyToDXKey[44] = 0x2c; InputKeyToDXKey[45] = 0x2d; InputKeyToDXKey[46] = 0x2e; InputKeyToDXKey[47] = 0x2f;
    InputKeyToDXKey[48] = 0x30; InputKeyToDXKey[49] = 0x31; InputKeyToDXKey[50] = 0x32; InputKeyToDXKey[51] = 0x33;
    InputKeyToDXKey[52] = 0x34; InputKeyToDXKey[53] = 0x35; InputKeyToDXKey[54] = 0x36; InputKeyToDXKey[55] = 0x37;
    InputKeyToDXKey[56] = 0x38; InputKeyToDXKey[57] = 0x39; InputKeyToDXKey[58] = 0x3a; InputKeyToDXKey[59] = 0x3b;
    InputKeyToDXKey[60] = 0x3c; InputKeyToDXKey[61] = 0x3d; InputKeyToDXKey[62] = 0x3e; InputKeyToDXKey[63] = 0x3f;
    InputKeyToDXKey[64] = 0x40; InputKeyToDXKey[65] = 0x41; InputKeyToDXKey[66] = 0x42; InputKeyToDXKey[67] = 0x43;
    InputKeyToDXKey[68] = 0x44; InputKeyToDXKey[69] = 0x45; InputKeyToDXKey[70] = 0x46; InputKeyToDXKey[71] = 0x47;
    InputKeyToDXKey[72] = 0x48; InputKeyToDXKey[73] = 0x49; InputKeyToDXKey[74] = 0x4a; InputKeyToDXKey[75] = 0x4b;
    InputKeyToDXKey[76] = 0x4c; InputKeyToDXKey[77] = 0x4d; InputKeyToDXKey[78] = 0x4e; InputKeyToDXKey[79] = 0x4f;
    InputKeyToDXKey[80] = 0x50; InputKeyToDXKey[81] = 0x51; InputKeyToDXKey[82] = 0x52; InputKeyToDXKey[83] = 0x53;
    InputKeyToDXKey[84] = 0x57; InputKeyToDXKey[85] = 0x58; InputKeyToDXKey[86] = 0x64; InputKeyToDXKey[87] = 0x65;
    InputKeyToDXKey[88] = 0x66; InputKeyToDXKey[89] = 0x70; InputKeyToDXKey[90] = 0x79; InputKeyToDXKey[91] = 0x7b;
    InputKeyToDXKey[92] = 0x7d; InputKeyToDXKey[93] = 0x8d; InputKeyToDXKey[94] = 0x90; InputKeyToDXKey[95] = 0x91;
    InputKeyToDXKey[96] = 0x92; InputKeyToDXKey[97] = 0x93; InputKeyToDXKey[98] = 0x94; InputKeyToDXKey[99] = 0x95;
    InputKeyToDXKey[100] = 0x96; InputKeyToDXKey[101] = 0x97; InputKeyToDXKey[102] = 0x9c; InputKeyToDXKey[103] = 0x9d;
    InputKeyToDXKey[104] = 0xb3; InputKeyToDXKey[105] = 0xb5; InputKeyToDXKey[106] = 0xb7; InputKeyToDXKey[107] = 0xb8;
    InputKeyToDXKey[108] = 0xc7; InputKeyToDXKey[109] = 0xc8; InputKeyToDXKey[110] = 0xc9; InputKeyToDXKey[111] = 0xcb;
    InputKeyToDXKey[112] = 0xcd; InputKeyToDXKey[113] = 0xcf; InputKeyToDXKey[114] = 0xd0; InputKeyToDXKey[115] = 0xd1;
    InputKeyToDXKey[116] = 0xd2; InputKeyToDXKey[117] = 0xd3; InputKeyToDXKey[118] = 0xdb; InputKeyToDXKey[119] = 0xdc;
    InputKeyToDXKey[120] = 0xdd; InputKeyToDXKey[121] = 0xc5;
}
