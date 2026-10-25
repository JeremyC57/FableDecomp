// CAudioDevice::GetDefaultRate() -- under the audio lock, fetches the library's default settings block and returns its rate word (+0x60)
// Leaf: GetDefaultRate
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C1C030, 54 bytes (`ret`). Descriptive name (no PDB covers this library): the
//   manifest's "GetDefaultVal<CAnimationSet>" is propagated. Caller: the landed 0x00C10BA0 (`m_pTarget->Get()` on the
//   object at +0x7BC), so this is a member that ignores `this`.
// Callees: CScopedLock::CScopedLock / ~CScopedLock @ 0x00C0FFC0 / 0x00C0FFE0 over the lock @ 0x0143BAFC, the library's
//   `__stdcall` GetDefaultSettings @ 0x00C33400 (copies the 0xA0-byte default block; the landed StartTimer 0x00C1C1A0
//   reads the same +0x60 rate word).
// Shape: no EH frame (this TU is built without /EHsc); the result is held in esi across the lock's destructor; the
//   frame holds the 8-byte lock below the 0xA0-byte block.
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
    unsigned long More[0x0F];               // +0x64 .. +0xA0
};

void __stdcall GetDefaultSettings(CDefaultSettings* out);   // 0x00C33400

struct CAudioDevice {
    unsigned long GetDefaultRate();
};

unsigned long CAudioDevice::GetDefaultRate()
{
    CScopedLock lock(&g_audioLock);
    CDefaultSettings settings;
    GetDefaultSettings(&settings);
    return settings.Rate;
}