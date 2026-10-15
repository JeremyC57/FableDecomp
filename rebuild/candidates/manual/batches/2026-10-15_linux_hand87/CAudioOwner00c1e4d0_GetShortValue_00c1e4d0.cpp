// CAudioOwner_00c1e4d0::GetShortValue() -- take the audio library's global profile lock and return the unsigned 16-bit word at +0x14 of the object at +4, widened, once the lock is released
// Leaf: GetShortValue
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C1E4D0, length 43 bytes (vtable slot @ 0x012B33F8 of a no-RTTI audio-library interface).
// Callees: CProfileLock::CProfileLock(void*) @ 0x00C0FFC0 / ~CProfileLock @ 0x00C0FFE0 (the 8-byte scoped lock: section + entry tick).
// Shape: the `movzx esi,word ptr [eax+14h]` keeps the widened value in the callee-saved esi across the lock's
// destructor (`mov eax,esi` after it): an `unsigned short` member returned as a long. The vtable neighbour
// 0x00C1E570 (landed) returns the long at +0x18 of the same record. The manifest label is propagated; the
// class is named by address.
struct CProfileLock {                              // 8 bytes: section + entry tick (GOTCHAS wave 23f)
    void* m_pSection;
    long  m_entryTick;
    CProfileLock(void* section);                   // 0x00C0FFC0
    ~CProfileLock();                               // 0x00C0FFE0
};
extern unsigned char g_AudioProfileSection_0143bb14[1];   // 0x0143BB14

struct SAudioOwnerRecord_00c1e4d0 {
    unsigned char  _pad[0x14];
    unsigned short m_shortValue;                   // +0x14
};

struct CAudioOwner_00c1e4d0 {
    unsigned char               _pad[4];
    SAudioOwnerRecord_00c1e4d0* m_pOwner;          // +0x04
    long GetShortValue();
};

long CAudioOwner_00c1e4d0::GetShortValue()
{
    CProfileLock lock(g_AudioProfileSection_0143bb14);
    return m_pOwner->m_shortValue;
}
