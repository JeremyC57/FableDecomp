// CMusicManager::SetVolumeParams(SVolumeParams**, SVolumeParams*, const SVolumeParams*) -- set or clear an optional volume-parameter record and mark it dirty
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C434A0, 143 bytes (+ int3 padding; `ret 0Ch`).
// A null value clears the current pointer (dirty only if it was set); with no current record the value is copied into the
//   caller's storage, which becomes current; otherwise the storage is overwritten only when the value differs. Each change
//   sets dirty bit 0x100 of the flag word at +0x14.
// Shape: the record is a long plus two floats compared field by field (`cmp` then `fucompp ; test ah,44h ; jnp`), copied word
//   by word; the inequality is the negated inline operator==.
struct SVolumeParams {
    long  m_mode;
    float m_volume;
    float m_fadeTime;
    bool operator==(const SVolumeParams& other) const
    {
        return m_mode == other.m_mode && m_volume == other.m_volume && m_fadeTime == other.m_fadeTime;
    }
};

class CMusicManager { public:
    unsigned char _pad00[0x14];
    unsigned long m_dirtyFlags;                    // +0x14
    void SetVolumeParams(SVolumeParams** current, SVolumeParams* storage, const SVolumeParams* value);
};

void CMusicManager::SetVolumeParams(SVolumeParams** current, SVolumeParams* storage, const SVolumeParams* value)
{
    if (!value) {
        if (*current) {
            *current = 0;
            m_dirtyFlags |= 0x100;
        }
        return;
    }
    if (!*current) {
        *current = storage;
        *storage = *value;
        m_dirtyFlags |= 0x100;
        return;
    }
    if (!(*storage == *value)) {
        *storage = *value;
        m_dirtyFlags |= 0x100;
    }
}