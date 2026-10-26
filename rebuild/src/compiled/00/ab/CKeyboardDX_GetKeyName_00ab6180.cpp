// CKeyboardDX::GetKeyName(long key, CWideString& name) -- DirectInput's display name of a game key: DIPROP_KEYNAME of the key's mapped scan code (by offset) copied into `name`; false when the property read fails
// Grade: RELOCATION_MATCH (retry 10, RTM 3077 + /GS) -- one nested `if` with a single `return false` after it: the failure tail sits after the success path as retail
// Leaf: GetKeyName
// Flags: /GS
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AB6180, length 146 bytes.
// Callees: the IDirectInputDevice8 COM slot 5 (+0x14, GetProperty(DIPROP_KEYNAME = 20, &prop.diph)), CWideString::operator=
// (const wchar_t*) @ 0x0099B800, __security_check_cookie @ 0x00BFE9F9.
// Model: retail CKeyboardDX layout (device +0x3418, the 16-bit key map +0x351c, as the landed CKeyboardDX::IsKeyPressed
// 0x00AB6220); DIPROPSTRING is the 16-byte DIPROPHEADER (dwSize 0x218, dwHeaderSize 0x10, dwObj, dwHow = DIPH_BYOFFSET)
// plus 260 wide characters, whose array gives the /GS cookie frame.
struct IDirectInputDevice8_Retail {                // COM: __stdcall methods, `this` pushed
    virtual long __stdcall QueryInterface(void* iid, void** out);
    virtual unsigned long __stdcall AddRef();
    virtual unsigned long __stdcall Release();
    virtual long __stdcall GetCapabilities(void* caps);
    virtual long __stdcall EnumObjects(void* callback, void* ref, unsigned long flags);
    virtual long __stdcall GetProperty(const void* guid, void* header);
    virtual long __stdcall SetProperty(const void* guid, const void* header);
    virtual long __stdcall Acquire();              // slot 7 (+0x1c)
    virtual long __stdcall Unacquire();            // slot 8 (+0x20)
    virtual long __stdcall GetDeviceState(unsigned long size, void* data);   // slot 9 (+0x24)
    virtual long __stdcall GetDeviceData(unsigned long size, void* data, unsigned long* count, unsigned long flags);   // slot 10 (+0x28)
    virtual long __stdcall SetDataFormat(const void* format);   // slot 11 (+0x2c)
    virtual long __stdcall SetEventNotification(void* event);   // slot 12 (+0x30)
    virtual long __stdcall SetCooperativeLevel(void* window, unsigned long flags);   // slot 13 (+0x34)
    virtual long __stdcall GetObjectInfo(void* info, unsigned long object, unsigned long how);   // slot 14 (+0x38)
    virtual long __stdcall GetDeviceInfo(void* info);   // slot 15 (+0x3c)
};
class CWideString { public:
    void* PStringData;
    CWideString& operator=(const unsigned short* text);   // 0x0099B800
};
struct SDIPropHeader {                             // DIPROPHEADER
    unsigned long dwSize;
    unsigned long dwHeaderSize;
    unsigned long dwObj;
    unsigned long dwHow;
};
struct SDIPropString {                             // DIPROPSTRING
    SDIPropHeader  diph;
    unsigned short wsz[260];
};
class CKeyboardDX {
public:
    unsigned char               _pad[0x3418];
    IDirectInputDevice8_Retail* DKeyboard;         // +0x3418
    unsigned char               KeyStates[0x100];  // +0x341c
    unsigned short              KeyMap[0x7a];      // +0x351c
    bool GetKeyName(long key, CWideString& name);
};

bool CKeyboardDX::GetKeyName(long key, CWideString& name)
{
    SDIPropString prop;
    prop.diph.dwSize = sizeof(SDIPropString);
    prop.diph.dwHeaderSize = sizeof(SDIPropHeader);
    prop.diph.dwObj = KeyMap[key];
    prop.diph.dwHow = 1;
    if (DKeyboard->GetProperty((const void*)20, &prop.diph) >= 0) {
        name = prop.wsz;
        return true;
    }
    return false;
}