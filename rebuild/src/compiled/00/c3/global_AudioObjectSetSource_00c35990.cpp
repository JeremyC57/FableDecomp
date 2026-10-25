// _global::AudioObject_SetSource(CAudioObject* object, CAudioSource* source) -- libpk audio API: refuses (error -6, returns 0) a source whose +0x2C flags have bits 2 and 3 set while its +0xC word is non-zero; otherwise returns the object's bool setter result for the source as a BOOL
// Leaf: ?AudioObject_SetSource@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C35990, 79 bytes, __stdcall `ret 8`. Descriptive name (no PDB covers libpk):
//   the catalog's `Inventory_OnItemEquipped_ValidateWeapon` is propagated. Caller: 0x00C274BE.
// Callees: the global audio system (0x0143BB58)'s SetError @ 0x00C3D090 and the object's setter @ 0x00C43E20
//   (__fastcall, one pointer, returns a bool).
// Shape: a null source skips the checks (the setter gets `xor esi,esi`); the flag bits come through inline
//   `(m_flags >> n) & 1` accessors (`mov edx,eax ; shr edx,2 ; test dl,1` -- 1-bit fields give `test al,4`);
//   `set ? 1 : 0` gives the `neg al ; sbb ; neg` tail. Retry pass 3: the checked local assigned in both arms of an
//   if/else (`if (source == 0) checked = 0; else { checked = source; ...checks... }`) gives retail's fall-through
//   null arm with the checks moved to the end (a `checked = 0` initialiser hoists `xor esi,esi` above the test).
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
    bool SetSource(CAudioSource* source);    // 0x00C43E20
};

int __stdcall AudioObject_SetSource(CAudioObject* object, CAudioSource* source)
{
    CAudioSource* checked;
    if (source == 0) {
        checked = 0;
    } else {
        checked = source;
        if (checked->IsStreamed() && checked->IsLocked() && checked->m_pending) {
            g_audioSystem.SetError(-6);
            return 0;
        }
    }
    return object->SetSource(checked) ? 1 : 0;
}