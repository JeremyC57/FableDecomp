// CInputManager::SetExclusiveMode(long exclusive) -- forward the exclusive-mode change to the mouse (+0x24, slot 6), the keyboard (+0x28, slot 2) and every non-null joystick of PJoysticks (+0x2c, slot 12); always returns true
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009F4E70, length 79 bytes (+ int3 padding). No direct callees (three virtual slots).
// Layout from Ego_r.pdb CInputManager with 12-byte vectors: PMouse +0x24, PKeyboard +0x28, PJoysticks {first +0x2c, last +0x30}.
// The device pointers are scoped_ptr members read through the inline `operator->` (the pointer lands straight in ecx).
// Shape: as SetHasInputFocus (0x009F4E20) but each joystick object is null-tested before its virtual call and the body ends
// with `mov al,1`.
struct CMouse {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void SetExclusiveMode(long exclusive);  // slot 6
};
struct CKeyboard {
    virtual void v00(); virtual void v01();
    virtual void SetExclusiveMode(long exclusive);  // slot 2
};
struct CJoystick {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06();
    virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void SetExclusiveMode(long exclusive);  // slot 12
};
template<class T> struct scoped_ptr {                 // boost-style: the PDB types PMouse / PKeyboard as scoped_ptr
    T* ptr;
    T* get() const { return ptr; }
    T* operator->() const { return ptr; }
};
struct CCountedPointer_CJoystick {                  // 8 bytes: object, count block
    CJoystick* m_pObject;
    void*      m_pCount;
};

struct CInputManager {
    virtual void v00();
    unsigned char _pad04[0x20];                     // +0x04 the event vectors and counters
    scoped_ptr<CMouse>    PMouse;                   // +0x24
    scoped_ptr<CKeyboard> PKeyboard;                // +0x28
    CCountedPointer_CJoystick* m_pJoysticksFirst;   // +0x2c PJoysticks.first
    CCountedPointer_CJoystick* m_pJoysticksLast;    // +0x30
    CCountedPointer_CJoystick* m_pJoysticksEnd;     // +0x34
    bool SetExclusiveMode(long exclusive);
};

bool CInputManager::SetExclusiveMode(long exclusive)
{
    if (PMouse.get())
        PMouse->SetExclusiveMode(exclusive);
    if (PKeyboard.get())
        PKeyboard->SetExclusiveMode(exclusive);
    long count = m_pJoysticksLast - m_pJoysticksFirst;
    for (long i = 0; i < count; ++i) {
        CJoystick* joystick = m_pJoysticksFirst[i].m_pObject;
        if (joystick)
            joystick->SetExclusiveMode(exclusive);
    }
    return true;
}
