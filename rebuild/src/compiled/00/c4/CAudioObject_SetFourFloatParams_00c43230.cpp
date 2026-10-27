// CAudioObject::SetFourFloatParams(const SFourFloatParams_00c43230* value) -- set or clear one of the audio object's optional parameter records: a null value clears the current pointer (+0x38), a first value is copied into the inline storage (+0xc0) which becomes current, a later value overwrites the storage only when it differs; each change sets dirty bit 0x40 of +0x14
// Leaf: ?SetFourFloatParams@CAudioObject@@QAEXPBUSFourFloatParams_00c43230@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C43230, 189 bytes, thiscall `ret 4`.
// Identity (descriptive method and record names; libpk has no PDB or RTTI): the set-or-clear member family of the landed
//   CAudioObject::SetFilterParams 0x00C43000 (pointer +0x2C, storage +0x7C, bit 8) on the next pointer slot; its only
//   caller is the one-line `__stdcall` libpk API wrapper at 0x00C35B90 (object, record). Catalog gapscan name is a placeholder.
// Shape (as 0x00C43000): the record's operator== is `__forceinline`, each `fld [value] ; fld [storage] ; fucompp ;
//   test ah,44h ; jp` loads the right operand first; the copies are the implicit record assignment; the clear arm keeps
//   the flag OR in a register with the zero store between.
struct SFourFloatParams_00c43230 {           // 0x14 bytes: four floats and a word (compared as an integer)
    float m_a, m_b, m_c, m_d;
    long  m_e;
    __forceinline bool operator==(const SFourFloatParams_00c43230& other) const
    {
        return m_a == other.m_a && m_b == other.m_b && m_c == other.m_c && m_d == other.m_d && m_e == other.m_e;
    }
};

class CAudioObject {
public:
    unsigned char  m_pad00[0x14];
    unsigned long  m_dirtyFlags;                // +0x14
    unsigned char  m_pad18[0x38 - 0x18];
    SFourFloatParams_00c43230*  m_pRecord;                  // +0x38 current record (0 = none)
    unsigned char  m_padA[0xc0 - 0x38 - 4];
    SFourFloatParams_00c43230   m_recordStorage;            // +0xc0
    void SetFourFloatParams(const SFourFloatParams_00c43230* value);
};

void CAudioObject::SetFourFloatParams(const SFourFloatParams_00c43230* value)
{
    if (!value) {
        if (m_pRecord) {
            m_pRecord = 0;
            m_dirtyFlags |= 0x40;
        }
        return;
    }
    if (!m_pRecord) {
        m_pRecord = &m_recordStorage;
        m_recordStorage = *value;
        m_dirtyFlags |= 0x40;
        return;
    }
    if (!(*m_pRecord == *value)) {
        *m_pRecord = *value;
        m_dirtyFlags |= 0x40;
    }
}