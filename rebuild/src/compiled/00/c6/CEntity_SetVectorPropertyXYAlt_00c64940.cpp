// CEntity::SetVectorProperty_XY_Alt(const SLevelRatioProperty* value) -- validate a (level in millibels -10000..0, ratio 0..1) property pair; reject it with the owner's last error -2 and result -3, else store it and mark the property dirty (bit 22) when it changed; returns 0
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C64940, 115 bytes (+ int3 padding).
// Callees: none. Float pool: 0x0122DEDC = 0.0f, 0x0122DED8 = 1.0f.
// Layout: local model (no PDB class carries these offsets; the module looks like an EAX/DirectSound-style property block):
//   flags +0x04 (bit 22 = this property dirty), owner +0x14 (last error at +0x3d4), the pair at +0x40/+0x44 (no deferred
//   copy). The sibling 0x00C647B0 keeps its pair at +0x28/+0x2c with a deferred copy and dirty bit 0.
// Shape: one combined range test whose failure block sits at the end (`jl`/`jg` on the level, `test ah,5 ; jnp` = `< 0`,
//   `test ah,41h ; je` = `> 1`); the level is read once into a local (kept in edx: no reload after the stores) while the
//   ratio is re-read through the pointer; the change test is `level != m || ratio != m` (fucompp, `test ah,44h ; jnp`); a
//   single float assignment is an integer move, and the dirty bit is `or dword ptr [ecx+4],400000h`.
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
    unsigned char       _pad28[0x40 - 0x28];
    SLevelRatioProperty m_current;               // +0x40
    long SetVectorProperty_XY_Alt(const SLevelRatioProperty* value);
};

long CEntity::SetVectorProperty_XY_Alt(const SLevelRatioProperty* value)
{
    long level = value->Level;
    if (level < -10000 || level > 0 || value->Ratio < 0.0f || value->Ratio > 1.0f) {
        m_pOwner->LastError = -2;
        return -3;
    }
    if (level != m_current.Level || value->Ratio != m_current.Ratio) {
        m_current.Level = level;
        m_current.Ratio = value->Ratio;
        m_dirtyFlags |= 0x400000;
    }
    return 0;
}