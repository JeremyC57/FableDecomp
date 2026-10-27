// CAudioObject::SetVectorParam(const SVectorParam_00c431a0* value) -- set or clear one of the audio object's optional parameter records: a null value clears the current pointer (+0x34), a first value is copied into the inline storage (+0xb4) which becomes current, a later value overwrites the storage only when it differs; each change sets dirty bit 0x20 of +0x14
// Leaf: ?SetVectorParam@CAudioObject@@QAEXPBUSVectorParam_00c431a0@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C431A0, 138 bytes, thiscall `ret 4`.
// Identity (descriptive method and record names; libpk has no PDB or RTTI): the set-or-clear member family of the landed
//   CAudioObject::SetFilterParams 0x00C43000 (pointer +0x2C, storage +0x7C, bit 8) on the next pointer slot; its only
//   caller is the one-line `__stdcall` libpk API wrapper at 0x00C35B70 (object, record). Catalog gapscan name is a placeholder.
// Shape (as 0x00C43000): the record's operator== is `__forceinline`, each `fld [value] ; fld [storage] ; fucompp ;
//   test ah,44h ; jp` loads the right operand first; the copies are the implicit record assignment; the clear arm keeps
//   the flag OR in a register with the zero store between.
struct SVectorParam_00c431a0 {               // 12 bytes: three floats
    float m_x, m_y, m_z;
    __forceinline bool operator==(const SVectorParam_00c431a0& other) const
    {
        return m_x == other.m_x && m_y == other.m_y && m_z == other.m_z;
    }
};

class CAudioObject {
public:
    unsigned char  m_pad00[0x14];
    unsigned long  m_dirtyFlags;                // +0x14
    unsigned char  m_pad18[0x34 - 0x18];
    SVectorParam_00c431a0*  m_pRecord;                  // +0x34 current record (0 = none)
    unsigned char  m_padA[0xb4 - 0x34 - 4];
    SVectorParam_00c431a0   m_recordStorage;            // +0xb4
    void SetVectorParam(const SVectorParam_00c431a0* value);
};

void CAudioObject::SetVectorParam(const SVectorParam_00c431a0* value)
{
    if (!value) {
        if (m_pRecord) {
            m_pRecord = 0;
            m_dirtyFlags |= 0x20;
        }
        return;
    }
    if (!m_pRecord) {
        m_pRecord = &m_recordStorage;
        m_recordStorage = *value;
        m_dirtyFlags |= 0x20;
        return;
    }
    if (!(*m_pRecord == *value)) {
        *m_pRecord = *value;
        m_dirtyFlags |= 0x20;
    }
}