// _global::AudioObject_PlaySource(CAudioObject* object, CAudioSource* source, long a, long b) -- libpk audio API: refuses (error -6, returns 0) a source whose +0x2C flags have bits 2 and 3 set while its +0xC word is non-zero; otherwise returns the object's bool member result for (source, a, b) as a BOOL
// Leaf: AudioObject_PlaySource
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C35A10, 77 bytes, __stdcall `ret 0x10`. Descriptive name (no PDB covers
//   libpk): the catalog's `Inventory_OnItemEquipped_ValidateWeaponExtended` is propagated. Caller: 0x00C495A1.
//   The sibling 0x00C35990 runs the same source check (with a null-source escape this row does not have).
// Shape: the flag bits come through inline `(m_flags >> n) & 1` accessors (`mov edx,eax ; shr edx,2 ; test dl,1`;
//   1-bit fields give `test al,4`); `r ? 1 : 0` gives the `neg al ; sbb ; neg` tail.
// Callees: the global audio system (0x0143BB58)'s SetError @ 0x00C3D090 and the object's member @ 0x00C43F80
//   (__fastcall, three words, returns a bool).
struct CAudioSystem {
    void SetError(long code);                // 0x00C3D090
};
extern CAudioSystem g_audioSystem;           // 0x0143BB58

struct CAudioSource {
    unsigned char _pad00[0x0c];
    long          m_pending;                 // +0x0C
    unsigned char _pad10[0x2c - 0x10];
    unsigned long m_flags;                   // +0x2C
    bool IsStreamed() const { return (m_flags >> 2) & 1; }
    bool IsLocked() const { return (m_flags >> 3) & 1; }
};
struct CAudioObject {
    bool PlaySource(CAudioSource* source, long a, long b);   // 0x00C43F80
};

int __stdcall AudioObject_PlaySource(CAudioObject* object, CAudioSource* source, long a, long b)
{
    if (source->IsStreamed() && source->IsLocked() && source->m_pending) {
        g_audioSystem.SetError(-6);
        return 0;
    }
    return object->PlaySource(source, a, b) ? 1 : 0;
}