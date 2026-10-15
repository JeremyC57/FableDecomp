// CAudioObject_00c0aa10::GetElementCount() -- take the audio library's global profile lock and return the constant 16 once the lock is released
// Leaf: GetElementCount
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C0AA10, length 34 bytes (vtable slot @ 0x012B1E90 of a no-RTTI audio-library interface).
// Callees: CProfileLock::CProfileLock(void*) @ 0x00C0FFC0 / ~CProfileLock @ 0x00C0FFE0 (the 8-byte scoped lock: section + entry tick).
// Shape: `sub esp,8` for the lock, the constructor on the global section @ 0x0143BB14, the destructor, then the
// constant in eax. No call can throw between the two, so no EH frame is needed even in the /EHsc module. The
// manifest label "CTCElectrocutedEffect::OnInitialActivate" is propagated across byte-identical lock bodies;
// the slots that follow in the vtable are SetName 0x00C0AA40, SetElement 0x00C0AB60 and GetElement 0x00C0AAD0,
// and a later slot (0x00C0AC00) returns 8 the same way: constant capacity/count queries. The class is named by address.
struct CProfileLock {                              // 8 bytes: section + entry tick (GOTCHAS wave 23f)
    void* m_pSection;
    long  m_entryTick;
    CProfileLock(void* section);                   // 0x00C0FFC0
    ~CProfileLock();                               // 0x00C0FFE0
};
extern unsigned char g_AudioProfileSection_0143bb14[1];   // 0x0143BB14

struct CAudioObject_00c0aa10 {
    long GetElementCount();
};

long CAudioObject_00c0aa10::GetElementCount()
{
    CProfileLock lock(g_AudioProfileSection_0143bb14);
    return 16;
}
