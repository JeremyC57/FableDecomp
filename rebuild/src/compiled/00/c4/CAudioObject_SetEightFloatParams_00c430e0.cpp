// CAudioObject::SetEightFloatParams(const SEightFloatParams_00c430e0* value) -- set or clear one of the audio object's optional parameter records: a null value clears the current pointer (+0x30), a first value is copied into the inline storage (+0x94) which becomes current, a later value overwrites the storage only when it differs; each change sets dirty bit 0x10 of +0x14
// Leaf: ?SetEightFloatParams@CAudioObject@@QAEXPBUSEightFloatParams_00c430e0@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C430E0, 189 bytes, thiscall `ret 4`.
// Identity (descriptive method and record names; libpk has no PDB or RTTI): the set-or-clear member family of the landed
//   CAudioObject::SetFilterParams 0x00C43000 (pointer +0x2C, storage +0x7C, bit 8) on the next pointer slot; its only
//   caller is the one-line `__stdcall` libpk API wrapper at 0x00C35B50 (object, record). Catalog gapscan name is a placeholder.
// Shape (as 0x00C43000): the record's operator== is `__forceinline`, each `fld [value] ; fld [storage] ; fucompp ;
//   test ah,44h ; jp` loads the right operand first; the copies are the implicit record assignment; the clear arm keeps
//   the flag OR in a register with the zero store between.
struct SEightFloatParams_00c430e0 {           // 0x20 bytes: eight floats
    float m_a, m_b, m_c, m_d, m_e, m_f, m_g, m_h;
    __forceinline bool operator==(const SEightFloatParams_00c430e0& other) const
    {   // retail compares words 0, 1, 2, 0, 4, 5, 6, 7: the fourth test repeats the first field (as SFilterParams 0x00C43000)
        return m_a == other.m_a && m_b == other.m_b && m_c == other.m_c && m_a == other.m_a
            && m_e == other.m_e && m_f == other.m_f && m_g == other.m_g && m_h == other.m_h;
    }
};

class CAudioObject {
public:
    unsigned char  m_pad00[0x14];
    unsigned long  m_dirtyFlags;                // +0x14
    unsigned char  m_pad18[0x30 - 0x18];
    SEightFloatParams_00c430e0*  m_pRecord;                  // +0x30 current record (0 = none)
    unsigned char  m_padA[0x94 - 0x30 - 4];
    SEightFloatParams_00c430e0   m_recordStorage;            // +0x94
    void SetEightFloatParams(const SEightFloatParams_00c430e0* value);
};

void CAudioObject::SetEightFloatParams(const SEightFloatParams_00c430e0* value)
{
    if (!value) {
        if (m_pRecord) {
            m_pRecord = 0;
            m_dirtyFlags |= 0x10;
        }
        return;
    }
    if (!m_pRecord) {
        m_pRecord = &m_recordStorage;
        m_recordStorage = *value;
        m_dirtyFlags |= 0x10;
        return;
    }
    if (!(*m_pRecord == *value)) {
        *m_pRecord = *value;
        m_dirtyFlags |= 0x10;
    }
}