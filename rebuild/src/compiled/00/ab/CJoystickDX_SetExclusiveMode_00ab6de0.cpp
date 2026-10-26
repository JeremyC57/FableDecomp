// CJoystickDX::SetExclusiveMode(bool exclusive) -- drop the current device (+0xd38: unacquire, release, clear), flag exclusive mode (+0xd08) and enumerate attached game controllers through the DX manager's DirectInput (+0xd40 -> +0x80, EnumDevices slot 4 with the callback 0x00AB6B10 and `this`); true when a device was created
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AB6DE0, length 96 bytes.
// Callees: COM slots 8 / 2 of the device, slot 4 of IDirectInput8 (4 = DI8DEVCLASS_GAMECTRL, 1 = DIEDFL_ATTACHEDONLY). Vtable 0x0129EA7C slot 12. The argument is not read (the flag is always set).
// Shape: the flag byte stored between the pushes, `test eax,eax ; setne al`, `ret 4`.
struct IDirectInputDevice8_Retail {              // COM: __stdcall methods, `this` pushed
    virtual long __stdcall QueryInterface(void* iid, void** out);   // slot 0
    virtual unsigned long __stdcall AddRef();                       // slot 1
    virtual unsigned long __stdcall Release();                      // slot 2 (+0x08)
    virtual long __stdcall GetCapabilities(void* caps);             // slot 3
    virtual long __stdcall EnumObjects(void* callback, void* ref, unsigned long flags);   // slot 4
    virtual long __stdcall GetProperty(void* guid, void* header);   // slot 5
    virtual long __stdcall SetProperty(void* guid, void* header);   // slot 6
    virtual long __stdcall Acquire();                               // slot 7 (+0x1c)
    virtual long __stdcall Unacquire();                             // slot 8 (+0x20)
};
struct IDirectInput8_Retail {                     // COM
    virtual long __stdcall QueryInterface(void* iid, void** out);
    virtual unsigned long __stdcall AddRef();
    virtual unsigned long __stdcall Release();
    virtual long __stdcall CreateDevice(void* guid, IDirectInputDevice8_Retail** device, void* outer);
    virtual long __stdcall EnumDevices(unsigned long devClass, void* callback, void* ref, unsigned long flags);   // slot 4 (+0x10)
};
class CInputManagerDX { public:
    unsigned char        _pad[0x80];
    IDirectInput8_Retail* m_pDirectInput;          // +0x80
};
long __stdcall EnumJoysticksCallback_00ab6b10(const void* instance, void* ref);   // 0x00AB6B10
class CInputManagerDX;
struct CJoystickDX {                               // retail layout: the device at +0xd38, the acquired flag at +0xd3c, the DX manager at +0xd40 (the engine header, from Ego_r, is 8 bytes longer)
    virtual void v000();
    virtual void v001();
    virtual void v002();
    virtual void v003();
    virtual void v004();
    virtual void v005();
    virtual void v006();
    virtual void v007();
    virtual void v008();
    virtual void v009();
    virtual void v010();
    virtual void v011();
    unsigned char              _pad[0xd08 - 4];
    bool                        ExclusiveMode;     // +0xd08
    unsigned char               _pad0d09[0xd38 - 0xd09];
    IDirectInputDevice8_Retail* DJoystick;         // +0xd38
    bool                        Acquired;          // +0xd3c
    unsigned char               _pad0d3d[3];
    CInputManagerDX*            InputManagerDX;    // +0xd40
    virtual bool SetExclusiveMode(bool exclusive); // slot 12
};

bool CJoystickDX::SetExclusiveMode(bool exclusive)
{
    if (DJoystick) {
        DJoystick->Unacquire();
        DJoystick->Release();
        DJoystick = 0;
    }
    ExclusiveMode = true;
    InputManagerDX->m_pDirectInput->EnumDevices(4, EnumJoysticksCallback_00ab6b10, this, 1);
    return DJoystick != 0 ? true : false;
}