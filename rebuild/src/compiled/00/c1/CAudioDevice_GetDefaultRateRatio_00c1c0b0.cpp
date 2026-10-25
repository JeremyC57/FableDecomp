// CAudioDevice::GetDefaultRateRatio() -- under the audio lock, fetches the library's default settings block and returns the float ratio of its rate word (+0x60) to the word at +0x6C
// Leaf: GetDefaultRateRatio
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C1C0B0, 90 bytes (`ret`). Descriptive name (no PDB covers this library): the
//   manifest's "C3DMeshFileBoneChunk_GetDeterminantRatio" is propagated. Caller 0x00C11E90. Sibling of
//   GetDefaultRate 0x00C1C030 (same lock, same settings call).
// Callees: CScopedLock::CScopedLock / ~CScopedLock @ 0x00C0FFC0 / 0x00C0FFE0 over the lock @ 0x0143BAFC, the library's
//   `__stdcall` GetDefaultSettings @ 0x00C33400; the unsigned-to-float fix-up constant 2^32 @ 0x0122DCB4.
// Shape: both words are unsigned (`fild ; test ; jge ; fadd 2^32`); the quotient is parked in the float frame slot
//   across the lock's destructor and reloaded (`fstp [esp] ... fld [esp]`).
struct CGlobalLockTarget;
extern CGlobalLockTarget g_audioLock;       // 0x0143BAFC

struct CScopedLock {                        // 8-byte scoped lock
    CGlobalLockTarget* m_pTarget;
    long m_state;
    CScopedLock(CGlobalLockTarget* target); // 0x00C0FFC0
    ~CScopedLock();                         // 0x00C0FFE0
};

struct CDefaultSettings {
    unsigned long Words[0x18];              // +0x00
    unsigned long Rate;                     // +0x60
    unsigned long Words64[2];               // +0x64
    unsigned long Base;                     // +0x6C
    unsigned long More[0x0C];               // +0x70 .. +0xA0
};

void __stdcall GetDefaultSettings(CDefaultSettings* out);   // 0x00C33400

struct CAudioDevice {
    float GetDefaultRateRatio();
};

float CAudioDevice::GetDefaultRateRatio()
{
    CScopedLock lock(&g_audioLock);
    CDefaultSettings settings;
    GetDefaultSettings(&settings);
    return (float)settings.Rate / (float)settings.Base;
}