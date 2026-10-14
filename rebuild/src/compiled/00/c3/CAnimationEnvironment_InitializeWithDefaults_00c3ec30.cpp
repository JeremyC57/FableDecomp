// CAnimationEnvironment::InitializeWithDefaults(long, long) -- bind the global animation device, initialise it, reset it; false when the device refuses
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C3EC30, length 79 bytes (+ int3 padding).
// Callees: virtual slot 1 (+0x04) of the global device object @ 0x0143BDB0 (Initialise(a, b), a signed result),
// virtual slot 3 (+0x0c) of the bound device (Reset).
// Shape: a zero store at +0x1d0 and the device address at +0x1d4 before the call (arguments loaded first,
// pushed right to left around the stores); the call goes through m_pDevice, whose just-stored value VC
// forwards, so the vtable is read through the global's absolute address (a direct call on the global
// object would be devirtualised). A
// negative result clears the pointer and returns false (`xor al,al`); otherwise the pointer is re-read for
// the second virtual call and the function returns true (`mov al,1`).
struct CAnimationDevice {
    virtual void v0();
    virtual long Initialise(long a, long b);    // slot 1 (+0x04)
    virtual void v2();
    virtual void Reset();                       // slot 3 (+0x0c)
};

extern CAnimationDevice g_animationDevice;      // 0x0143BDB0

struct CAnimationEnvironment {
    unsigned char _pad[0x1d0];
    long m_value1d0;                            // +0x1d0 zeroed here
    CAnimationDevice* m_pDevice;                // +0x1d4
    bool InitializeWithDefaults(long a, long b);
};

bool CAnimationEnvironment::InitializeWithDefaults(long a, long b)
{
    m_value1d0 = 0;
    m_pDevice = &g_animationDevice;
    if (m_pDevice->Initialise(a, b) < 0) {
        m_pDevice = 0;
        return false;
    }
    m_pDevice->Reset();
    return true;
}