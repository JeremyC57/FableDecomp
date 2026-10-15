// CKeyboardDX::SetHasInputFocus(bool focus) -- losing focus unacquires the keyboard (clearing Acquired when the call returned 0 or 1); gaining focus acquires it and, if it was not acquired before, drains the buffered DirectInput key data
// Leaf: ?SetHasInputFocus@CKeyboardDX@@UAEX_N@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AB6060, length 160 bytes. Vtable slot 3 (as the landed CKeyboardDX::UpdateAcquired
// 0x00AB6100 calls it).
// Callees: _chkstk @ 0x00BFEA30 (the 0x1404-byte frame), the IDirectInputDevice8 COM slots 8 (+0x20, Unacquire), 7 (+0x1c,
// Acquire) and 10 (+0x28, GetDeviceData(sizeof(DIDEVICEOBJECTDATA) = 0x14, buffer, &count, 0)).
// Model: retail CKeyboardDX layout (device +0x3418, Acquired +0x3610) and COM model as the landed CMouseDX::SetHasInputFocus
// 0x00AB47D0. The drain buffer is 256 DIDEVICEOBJECTDATA records (20 bytes each) with the in/out count below it.
// Shape: the old flag is read after Acquire and the new flag (`hr >= 0`, `setge`) stored before the test; the drain loop
// is `do { if (count == 0) break; read = ...; } while (read >= 0);` (a `while (count)` loop is rotated to a bottom test).
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
struct SDeviceObjectData {                         // DIDEVICEOBJECTDATA (DirectInput 8): 20 bytes
    unsigned long dwOfs;
    unsigned long dwData;
    unsigned long dwTimeStamp;
    unsigned long dwSequence;
    unsigned long uAppData;
};
class CKeyboardDX {
public:
    virtual void v000(); virtual void v001(); virtual void v002();
    virtual void SetHasInputFocus(bool focus);     // slot 3 (+0xc)
    unsigned char               _pad04[0x3418 - 4];
    IDirectInputDevice8_Retail* DKeyboard;         // +0x3418
    unsigned char               KeyStates[0x100];  // +0x341c
    unsigned short              KeyMap[0x7a];      // +0x351c
    bool                        Acquired;          // +0x3610
};

void CKeyboardDX::SetHasInputFocus(bool focus)
{
    unsigned long count;
    SDeviceObjectData data[256];
    if (!focus) {
        if (Acquired == true) {
            long result = DKeyboard->Unacquire();
            if (result == 0 || result == 1)
                Acquired = false;
        }
    } else {
        long result = DKeyboard->Acquire();
        bool wasAcquired = Acquired;
        Acquired = result >= 0;
        if (!wasAcquired) {
            count = 0x100;
            long read;
            do {
                if (count == 0)
                    break;
                read = DKeyboard->GetDeviceData(sizeof(SDeviceObjectData), data, &count, 0);
            } while (read >= 0);
        }
    }
}