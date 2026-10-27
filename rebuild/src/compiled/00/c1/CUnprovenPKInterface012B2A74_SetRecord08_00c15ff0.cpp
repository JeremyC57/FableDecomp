// CUnprovenPKInterface_012B2A74::SetRecord08(long) -- store the argument into the word at +0x8 of the record at +8, under the audio library's profile lock
// Leaf: ?SetRecord08@CUnprovenPKInterface_012B2A74@@UAEXJ@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C15FF0, 46 bytes, thiscall, `ret 4`.
// Identity (descriptive): slot 11 (at 0x012B2AA0) of the no-RTTI vtable 0x012B2A74, which the constructor 0x00C16050 and
//   destructor 0x00C16130 store; slot 10 is the landed 0x00C15FC0 that stores the record's word +0 the same way, and
//   slots 7..12 set the record's words +0x0C, +0x10, +0x14, +0, +0x08, +0x04. libpk has no RTTI and no PDB: the class
//   is named by its vtable, the record by the same address and the member by the field offset.
// Callees: PKCProtectionInstance::PKCProtectionInstance(PKCCriticalSection*) 0x00C0FFC0 / ~PKCProtectionInstance 0x00C0FFE0
//   over the module section 0x0143BB14.
// Shape (family K060, representative 0x00C15F30): the landed CAtmosOwner_00c14820 setter -- the record pointer read after
//   the guard is built, then the argument, one store, the guard destroyed.
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
struct CUnprovenPKRecord_012B2A74 {
    long Word00;                                         // +0x00
    long Word04;                                         // +0x04
    long Word08;                                         // +0x08
    long Word0C;                                         // +0x0C
    long Word10;                                         // +0x10
    long Word14;                                         // +0x14
};
class CUnprovenPKInterface_012B2A74 {
public:
    virtual void SetRecord08(long value);           // slot 11
    long Field04;                                        // +0x04 (not touched here)
    CUnprovenPKRecord_012B2A74* Record;                    // +0x08
};
void CUnprovenPKInterface_012B2A74::SetRecord08(long value)
{
    PKCProtectionInstance lock(&g_lock_0143bb14);
    Record->Word08 = value;
}