// CJoystickDX::SetHasInputFocus(bool focus) -- when the device (+0xd38) exists: on focus acquire it (COM slot 7) if not yet acquired (+0xd3c) and set the flag; on losing focus unacquire it (slot 8) if acquired and clear the flag
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AB6D10, length 80 bytes.
// Vtable 0x0129EA7C slot 16. Shape: the argument byte and the flag byte both in cl before the branch, the lose-focus arm first (`if (!focus)`), two epilogues, `ret 4`.
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
    virtual void v012();
    virtual void v013();
    virtual void v014();
    virtual void v015();
    unsigned char              _pad[0xd38 - 4];
    IDirectInputDevice8_Retail* DJoystick;         // +0xd38
    bool                        Acquired;          // +0xd3c
    unsigned char               _pad0d3d[3];
    CInputManagerDX*            InputManagerDX;    // +0xd40
    virtual void SetHasInputFocus(bool focus);     // slot 16
};

void CJoystickDX::SetHasInputFocus(bool focus)
{
    if (DJoystick) {
        if (!focus) {
            if (Acquired == true) {
                DJoystick->Unacquire();
                Acquired = false;
            }
        } else {
            if (!Acquired) {
                DJoystick->Acquire();
                Acquired = true;
            }
        }
    }
}