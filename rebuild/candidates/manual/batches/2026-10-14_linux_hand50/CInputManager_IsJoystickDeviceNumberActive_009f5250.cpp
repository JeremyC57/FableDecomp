// CInputManager::IsJoystickDeviceNumberActive(long deviceNumber) -- true when one of the PJoysticks counted pointers (+0x2c, 8-byte elements) owns a joystick reporting that device number (slot 8)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009F5250, length 68 bytes (+ int3 padding). No direct callees (one virtual slot).
// Layout from Ego_r.pdb CInputManager with 12-byte vectors: PJoysticks {first +0x2c, last +0x30}.
// Shape: the count is a signed pointer difference (`sar esi,3`) taken before the loop, the argument is loaded into ebp only
// inside the non-empty branch, the first pointer is re-read after every virtual call, `cmp ebp,eax` keeps the source
// operand order (argument first), `xor al,al` / `mov al,1` tails.
struct CJoystick {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual long GetDeviceNumber();                 // slot 8
};
struct CCountedPointer_CJoystick {                  // 8 bytes: object, count block
    CJoystick* m_pObject;
    void*      m_pCount;
};

struct CInputManager {
    virtual void v00();
    unsigned char _pad04[0x28];                     // +0x04 the event vectors, counters and the mouse/keyboard pointers
    CCountedPointer_CJoystick* m_pJoysticksFirst;   // +0x2c PJoysticks.first
    CCountedPointer_CJoystick* m_pJoysticksLast;    // +0x30
    CCountedPointer_CJoystick* m_pJoysticksEnd;     // +0x34
    bool IsJoystickDeviceNumberActive(long deviceNumber);
};

bool CInputManager::IsJoystickDeviceNumberActive(long deviceNumber)
{
    long count = m_pJoysticksLast - m_pJoysticksFirst;
    for (long i = 0; i < count; ++i) {
        if (deviceNumber == m_pJoysticksFirst[i].m_pObject->GetDeviceNumber())
            return true;
    }
    return false;
}
