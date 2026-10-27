// CUnprovenPKInterface_012B2748::SetMember884(unsigned char) -- store the argument into the word at +0x884 under the audio library's profile lock
// Leaf: ?SetMember884@CUnprovenPKInterface_012B2748@@UAEXE@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C11200, 46 bytes, thiscall, `ret 4`.
// Identity (descriptive): slot 6 of vtable 0x012B2748 (at 0x012B2760); a byte store (the landed slot-2 neighbour 0x00C10A90 stores the byte at +0x885). libpk has no RTTI and no PDB: the class is named by its vtable and the member by
//   its offset.
// Callees: PKCProtectionInstance::PKCProtectionInstance(PKCCriticalSection*) 0x00C0FFC0 / ~PKCProtectionInstance 0x00C0FFE0
//   over the module section 0x0143BB14.
// Shape (family K059, representative 0x00C10B00): the landed CAtmosOwner_00c14820 lock-guarded setter with the member on
//   `this`: the 8-byte guard on the frame, the argument loaded after the guard, one store, the guard destroyed.
class PKCCriticalSection {                               // a Win32 CRITICAL_SECTION (0x18 bytes)
public:
    unsigned char m_section[0x18];
};
class PKCProtectionInstance {                            // scoped guard: section + entry tick
public:
    PKCProtectionInstance(PKCCriticalSection* section);  // 0x00C0FFC0
    ~PKCProtectionInstance();                            // 0x00C0FFE0
    PKCCriticalSection* m_pSection;                      // +0
    unsigned long m_tick;                                // +4
};
extern PKCCriticalSection g_lock_0143bb14;               // 0x0143BB14
class CUnprovenPKInterface_012B2748 {
public:
    virtual void SetMember884(unsigned char value);           // slot 6
    unsigned char _body[0x880];                      // +0x04 .. (not touched here)
    unsigned char Member884;                                  // +0x884
};
void CUnprovenPKInterface_012B2748::SetMember884(unsigned char value)
{
    PKCProtectionInstance lock(&g_lock_0143bb14);
    Member884 = value;
}