// CUnprovenPKInterface_012B29F8::SetMemberA0(long) -- store the argument into the word at +0xA0 under the audio library's profile lock
// Leaf: ?SetMemberA0@CUnprovenPKInterface_012B29F8@@UAEXJ@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C14890, 46 bytes, thiscall, `ret 4`.
// Identity (descriptive): slot 9 of vtable 0x012B29F8 (at 0x012B2A1C), the twin of slot 8 one word on. libpk has no RTTI and no PDB: the class is named by its vtable and the member by
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
class CUnprovenPKInterface_012B29F8 {
public:
    virtual void SetMemberA0(long value);           // slot 9
    unsigned char _body[0x9C];                      // +0x04 .. (not touched here)
    long MemberA0;                                  // +0xA0
};
void CUnprovenPKInterface_012B29F8::SetMemberA0(long value)
{
    PKCProtectionInstance lock(&g_lock_0143bb14);
    MemberA0 = value;
}