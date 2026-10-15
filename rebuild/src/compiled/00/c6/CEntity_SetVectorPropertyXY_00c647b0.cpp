// CEntity::SetVectorProperty_XY(const SLevelRatioProperty* value) -- validate a (level in millibels -10000..0, ratio 0..1) property pair; reject it with the owner's last error -2 and result -3, else store it (current and deferred copies) and mark the property dirty when it changed; returns 0
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C647B0, 129 bytes (+ int3 padding).
// Callees: none. Float pool: 0x0122DEDC = 0.0f, 0x0122DED8 = 1.0f.
// Layout: local model (no PDB class carries these offsets; the module looks like an EAX/DirectSound-style property block):
//   flags +0x04 (bit 0 = this property dirty), owner +0x14 (last error at +0x3d4), the pair at +0x28/+0x2c and its deferred
//   copy at +0x218/+0x21c. The sibling 0x00C64940 stores the pair at +0x40/+0x44 with dirty bit 0x400000.
// Shape: one combined range test whose failure block sits at the end (`jl`/`jg` on the level, `test ah,5 ; jnp` = `< 0`,
//   `test ah,41h ; je` = `> 1`); the level is read once into a local (kept in edx: no reload after the stores) while the
//   ratio is re-read through the pointer; the change test is `level != m || ratio != m` (fucompp, `test ah,44h ; jnp`); the
//   stores are chained assignments `current = deferred = x` (deferred stored first, the float through `fst` + `fstp`).
struct SLevelRatioProperty {
    long  Level;                                 // millibels
    float Ratio;
};

struct CEntityOwner {
    unsigned char _pad[0x3d4];
    long          LastError;                     // +0x3d4
};

struct CEntity {
    unsigned char       _pad0[0x4];
    unsigned long       m_dirtyFlags;            // +0x04
    unsigned char       _pad8[0x14 - 0x8];
    CEntityOwner*       m_pOwner;                // +0x14
    unsigned char       _pad18[0x28 - 0x18];
    SLevelRatioProperty m_current;               // +0x28
    unsigned char       _pad30[0x218 - 0x30];
    SLevelRatioProperty m_deferred;              // +0x218
    long SetVectorProperty_XY(const SLevelRatioProperty* value);
};

long CEntity::SetVectorProperty_XY(const SLevelRatioProperty* value)
{
    long level = value->Level;
    if (level < -10000 || level > 0 || value->Ratio < 0.0f || value->Ratio > 1.0f) {
        m_pOwner->LastError = -2;
        return -3;
    }
    if (level != m_current.Level || value->Ratio != m_current.Ratio) {
        m_current.Level = m_deferred.Level = level;
        m_current.Ratio = m_deferred.Ratio = value->Ratio;
        m_dirtyFlags |= 1;
    }
    return 0;
}