// CAudioListener_00c11150::GetValue() -- take the audio library's global profile lock, read the float at +0x7c0 and return it once the lock is released
// Leaf: GetValue
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C11150, length 48 bytes (vtable slot @ 0x012B2768 of a no-RTTI audio-library interface).
// Callees: CProfileLock::CProfileLock(void*) @ 0x00C0FFC0 / ~CProfileLock @ 0x00C0FFE0 (the 8-byte scoped lock: section + entry tick).
// Shape: a 12-byte frame -- the lock plus a float slot. The member is copied into the slot as a dword
// (`mov eax,[esi+7C0h] ; mov [esp+4],eax`) before the destructor and `fld`ed back after it: a float result
// has no callee-saved register to ride in across the call. The manifest label is propagated; the class is
// named by address (its neighbour 0x00C10D40 talks to the edit control at +0x7bc).
struct CProfileLock {                              // 8 bytes: section + entry tick (GOTCHAS wave 23f)
    void* m_pSection;
    long  m_entryTick;
    CProfileLock(void* section);                   // 0x00C0FFC0
    ~CProfileLock();                               // 0x00C0FFE0
};
extern unsigned char g_AudioProfileSection_0143bb14[1];   // 0x0143BB14

struct CAudioListener_00c11150 {
    unsigned char _pad[0x7c0];
    float m_value;                                 // +0x7c0
    float GetValue();
};

float CAudioListener_00c11150::GetValue()
{
    CProfileLock lock(g_AudioProfileSection_0143bb14);
    return m_value;
}
