// CMouseDX::SetHasInputFocus(bool focus) -- when the device (+0x341c) exists: on losing focus unacquire it (COM slot 8) if acquired (+0x3424) and clear the flag when the call returned 0 or 1; on gaining focus acquire it (slot 7) if not yet acquired and set the flag when the call returned 0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AB47D0, length 96 bytes.
// Leaf: ?SetHasInputFocus@CMouseDX@@UAEX_N@Z
// Vtable slot of the agent52 CJoystickDX twin (0x00AB6D10). Shape: the argument byte and the flag byte both in cl before the branch, the lose-focus arm first, two epilogues, `ret 4`.
struct IDirectInputDevice8_Retail {              // COM: __stdcall methods, `this` pushed
    virtual long __stdcall QueryInterface(void* iid, void** out);
    virtual unsigned long __stdcall AddRef();
    virtual unsigned long __stdcall Release();
    virtual long __stdcall GetCapabilities(void* caps);
    virtual long __stdcall EnumObjects(void* callback, void* ref, unsigned long flags);
    virtual long __stdcall GetProperty(void* guid, void* header);
    virtual long __stdcall SetProperty(void* guid, void* header);
    virtual long __stdcall Acquire();              // slot 7 (+0x1c)
    virtual long __stdcall Unacquire();            // slot 8 (+0x20)
};
struct CMouseDX {                                  // retail layout: the device at +0x341c, the acquired flag at +0x3424
    unsigned char               _pad[0x341c - 4];
    IDirectInputDevice8_Retail* DMouse;            // +0x341c
    long                        m_3420;
    bool                        Acquired;          // +0x3424
    virtual void SetHasInputFocus(bool focus);
};

void CMouseDX::SetHasInputFocus(bool focus)
{
    if (DMouse) {
        if (!focus) {
            if (Acquired == true) {
                long result = DMouse->Unacquire();
                if (result == 0 || result == 1)
                    Acquired = false;
            }
        } else {
            if (!Acquired) {
                if (DMouse->Acquire() == 0)
                    Acquired = true;
            }
        }
    }
}
