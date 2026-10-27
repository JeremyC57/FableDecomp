// CUnprovenPKInterface_012B2748::SetMember7C0(float) -- store the argument into the word at +0x7C0 under the audio library's profile lock
// Leaf: ?SetMember7C0@CUnprovenPKInterface_012B2748@@UAEXM@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C11120, 46 bytes, thiscall, `ret 4`.
// Identity (descriptive): slot 7 of vtable 0x012B2748 (at 0x012B2764); slot 8 is the landed getter 0x00C11150 that returns the float at +0x7C0 under the same lock, so the value is a float. libpk has no RTTI and no PDB: the class is named by its vtable and the member by
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
    virtual void SetMember7C0(float value);           // slot 7
    unsigned char _body[0x7BC];                      // +0x04 .. (not touched here)
    float Member7C0;                                  // +0x7C0
};
void CUnprovenPKInterface_012B2748::SetMember7C0(float value)
{
    PKCProtectionInstance lock(&g_lock_0143bb14);
    Member7C0 = value;
}