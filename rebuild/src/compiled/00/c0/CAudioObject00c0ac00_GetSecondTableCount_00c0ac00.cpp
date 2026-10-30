// CAudioObject_00c0ac00::GetSecondTableCount() -- take the audio library's global profile lock and return the constant 8 once the lock is released
// Leaf: GetSecondTableCount
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C0AC00, length 34 bytes (vtable slot @ 0x012B1EA0 of a no-RTTI audio-library interface).
// Callees: PKCProtectionInstance::PKCProtectionInstance(void*) @ 0x00C0FFC0 / ~PKCProtectionInstance @ 0x00C0FFE0 (the 8-byte scoped lock: section + entry tick).
// Shape: `sub esp,8` for the lock, the constructor on the global section @ 0x0143BB14, the destructor, then the
// constant in eax. No call can throw between the two, so no EH frame is needed even in the /EHsc module. The
// manifest label "CTCElectrocutedEffect::OnInitialActivate" is propagated across byte-identical lock bodies;
// it is the slot after GetElement (0x00C0AAD0) in the SAME vtable whose slot @ 0x012B1E90 (0x00C0AA10)
// returns 16, and it precedes a second SetName (0x00C0AC30): a second constant capacity/count query. The class is named by address.
class PKCCriticalSection;
class PKCProtectionInstance { public:                              // 8 bytes: section + entry tick (GOTCHAS wave 23f)
    void* m_pSection;
    long  m_entryTick;
    PKCProtectionInstance(PKCCriticalSection* section);                   // 0x00C0FFC0
    ~PKCProtectionInstance();                               // 0x00C0FFE0
};
extern unsigned char g_AudioProfileSection_0143bb14[1];   // 0x0143BB14

struct CAudioObject_00c0ac00 {
    long GetSecondTableCount();
};

long CAudioObject_00c0ac00::GetSecondTableCount()
{
    PKCProtectionInstance lock((PKCCriticalSection*)g_AudioProfileSection_0143bb14);
    return 8;
}