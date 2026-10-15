// CAudioObject_00c15f10::SetUnused(long) -- take the audio library's global profile lock and release it again; the argument is ignored (a stubbed setter)
// Leaf: SetUnused
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C15F10, length 31 bytes (vtable slot @ 0x012B2A88 of a no-RTTI audio-library interface).
// Callees: CProfileLock::CProfileLock(void*) @ 0x00C0FFC0 / ~CProfileLock @ 0x00C0FFE0 (the 8-byte scoped lock: section + entry tick).
// Shape: the lock's constructor and destructor back to back and `ret 4` for the one unused stack argument
// (a thiscall member, `this` unused too). The manifest label is propagated; the class is named by address.
struct CProfileLock {                              // 8 bytes: section + entry tick (GOTCHAS wave 23f)
    void* m_pSection;
    long  m_entryTick;
    CProfileLock(void* section);                   // 0x00C0FFC0
    ~CProfileLock();                               // 0x00C0FFE0
};
extern unsigned char g_AudioProfileSection_0143bb14[1];   // 0x0143BB14

struct CAudioObject_00c15f10 {
    void SetUnused(long value);
};

void CAudioObject_00c15f10::SetUnused(long)
{
    CProfileLock lock(g_AudioProfileSection_0143bb14);
}