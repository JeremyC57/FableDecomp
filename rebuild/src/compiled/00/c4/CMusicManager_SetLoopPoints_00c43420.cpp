// CMusicManager::SetLoopPoints(SLoopPoints**, SLoopPoints*, const SLoopPoints*) -- set or clear an optional loop-point record and mark it dirty
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C43420, 116 bytes (+ int3 padding; `ret 0Ch`).
// A null value clears the current pointer (dirty only if it was set); with no current record the value is copied into the
//   caller's storage, which becomes current; otherwise the storage is overwritten only when the value differs. Each change
//   sets dirty bit 0x80 of the flag word at +0x14.
// Shape: the record is a long plus a float compared field by field (`cmp` then `fucompp ; test ah,44h ; jnp`), copied word
//   by word; the inequality is the negated inline operator==.
struct SLoopPoints {
    long  m_start;
    float m_length;
    bool operator==(const SLoopPoints& other) const { return m_start == other.m_start && m_length == other.m_length; }
};

struct CMusicManager {
    unsigned char _pad00[0x14];
    unsigned long m_dirtyFlags;                    // +0x14
    void SetLoopPoints(SLoopPoints** current, SLoopPoints* storage, const SLoopPoints* value);
};

void CMusicManager::SetLoopPoints(SLoopPoints** current, SLoopPoints* storage, const SLoopPoints* value)
{
    if (!value) {
        if (*current) {
            *current = 0;
            m_dirtyFlags |= 0x80;
        }
        return;
    }
    if (!*current) {
        *current = storage;
        *storage = *value;
        m_dirtyFlags |= 0x80;
        return;
    }
    if (!(*storage == *value)) {
        *storage = *value;
        m_dirtyFlags |= 0x80;
    }
}