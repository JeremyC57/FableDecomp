// CKeyboardDX::UpdateKeyStates() -- clear the 256-byte key-state buffer and, while the keyboard is acquired, refill it from DirectInput (re-acquiring once when the input was lost); true when the read succeeded
// Leaf: UpdateKeyStates
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AB5FF0, length 101 bytes.
// Callees: the IDirectInputDevice8 COM slots 9 (+0x24, GetDeviceState(256, buffer)) and 7 (+0x1c, Acquire).
// Model: retail CKeyboardDX layout as the landed CKeyboardDX::IsKeyPressed 0x00AB6220 (KeyStates +0x341c) and the
// CMouseDX::SetHasInputFocus 0x00AB47D0 COM model; the device pointer at +0x3418 and the acquired flag at +0x3610.
// Shape: the intrinsic memset is `rep stosd` x 0x40; `Acquired == true` gives `cmp byte ptr [..],1`; DIERR_INPUTLOST is
// 0x8007001E; the device is re-read from the member after Acquire; `return result == 0 ? true : false` gives `sete al` (the plain `== 0` materialises through `xor ecx ; sete cl ; mov al,cl`).
extern "C" void* __cdecl memset(void* dest, int value, unsigned int bytes);
#pragma intrinsic(memset)
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
struct CKeyboardDX {
    unsigned char               _pad[0x3418];
    IDirectInputDevice8_Retail* DKeyboard;         // +0x3418
    unsigned char               KeyStates[0x100];  // +0x341c
    unsigned short              KeyMap[0x7a];      // +0x351c
    bool                        Acquired;          // +0x3610
    bool UpdateKeyStates();
};

bool CKeyboardDX::UpdateKeyStates()
{
    memset(KeyStates, 0, sizeof(KeyStates));
    if (Acquired == true) {
        long result = DKeyboard->GetDeviceState(0x100, KeyStates);
        if (result == (long)0x8007001E) {
            DKeyboard->Acquire();
            result = DKeyboard->GetDeviceState(0x100, KeyStates);
        }
        return result == 0 ? true : false;
    }
    return false;
}
