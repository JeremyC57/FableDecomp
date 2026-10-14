// CInputManager::SetHasInputFocus(long hasFocus) -- forward the focus change to the mouse (+0x24, slot 14), the keyboard (+0x28, slot 3) and every joystick of the PJoysticks vector (+0x2c, counted pointers, slot 16)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009F4E20, length 75 bytes (+ int3 padding). No direct callees (three virtual slots).
// Layout from Ego_r.pdb CInputManager with 12-byte vectors: PMouse +0x24, PKeyboard +0x28, PJoysticks {first +0x2c, last +0x30}.
// The device pointers are scoped_ptr members read through the inline `operator->` (the pointer lands straight in ecx).
// Shape: the argument rides in ebp for the three pushes; the two device pointers are null-tested; the joystick count is a
// signed pointer difference (`sar edi,3`, 8-byte counted pointers) taken once before the loop, while the vector's first
// pointer is re-read every iteration after the virtual call.
struct CMouse {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06();
    virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13();
    virtual void SetHasInputFocus(long hasFocus);   // slot 14
};
struct CKeyboard {
    virtual void v00(); virtual void v01(); virtual void v02();
    virtual void SetHasInputFocus(long hasFocus);   // slot 3
};
struct CJoystick {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06();
    virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13();
    virtual void v14(); virtual void v15();
    virtual void SetHasInputFocus(long hasFocus);   // slot 16
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
    void SetHasInputFocus(long hasFocus);
};

void CInputManager::SetHasInputFocus(long hasFocus)
{
    if (PMouse.get())
        PMouse->SetHasInputFocus(hasFocus);
    if (PKeyboard.get())
        PKeyboard->SetHasInputFocus(hasFocus);
    long count = m_pJoysticksLast - m_pJoysticksFirst;
    for (long i = 0; i < count; ++i)
        m_pJoysticksFirst[i].m_pObject->SetHasInputFocus(hasFocus);
}
