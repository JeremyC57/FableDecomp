// CAudioObject::SetFilterParams(const SFilterParams* value) -- set or clear the audio object's optional six-float parameter record: a null value clears the current pointer (+0x2C), a first value is copied into the inline storage (+0x7C) which becomes current, a later value overwrites the storage only when it differs; each change sets dirty bit 8 of +0x14
// Leaf: ?SetFilterParams@CAudioObject@@QAEXPBUSFilterParams@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C43000, 218 bytes (+ int3 padding), __fastcall `ret 4`.
// Library: libpk audio (no PDB/RTTI): descriptive names. Only caller: the `__stdcall` API wrapper 0x00C35B30 (object,
//   record), reached from the game's sound code at 0x00C27325 (a stack record of six floats). The same set-or-clear
//   pattern as the landed out-of-line helpers 0x00C43420 (loop points, bit 0x80) / 0x00C434A0 (volume, bit 0x100),
//   written in place here. The manifest's "Audio_SetFilterParams" is kept as the description.
// Shape: the record's operator== must be `__forceinline` (plain in-class inline, VC7.1 calls it out of line and moves
//   `this` to edi); it compares words 0, 1, 2, 0, 4, 5 -- the original repeats the first field where
//   the fourth belongs (retail never compares +0x0C), so the model does too; each `fld [value] ; fld [storage] ;
//   fucompp ; test ah,44h ; jp` loads the right operand first. The copies are the implicit 24-byte assignment
//   (six dword moves through one register). The clear arm is `or eax,8` in a register with the zero store between.
struct SFilterParams {
    float m_a;                                  // +0x00
    float m_b;                                  // +0x04
    float m_c;                                  // +0x08
    float m_d;                                  // +0x0c
    float m_e;                                  // +0x10
    float m_f;                                  // +0x14
    __forceinline bool operator==(const SFilterParams& other) const
    {
        return m_a == other.m_a && m_b == other.m_b && m_c == other.m_c
            && m_a == other.m_a && m_e == other.m_e && m_f == other.m_f;
    }
};

class CAudioObject {
public:
    unsigned char  m_pad00[0x14];
    unsigned long  m_dirtyFlags;                // +0x14
    unsigned char  m_pad18[0x2c - 0x18];
    SFilterParams* m_pFilterParams;             // +0x2c current record (0 = none)
    unsigned char  m_pad30[0x7c - 0x30];
    SFilterParams  m_filterParamsStorage;       // +0x7c
    void SetFilterParams(const SFilterParams* value);
};

void CAudioObject::SetFilterParams(const SFilterParams* value)
{
    if (!value) {
        if (m_pFilterParams) {
            m_pFilterParams = 0;
            m_dirtyFlags |= 8;
        }
        return;
    }
    if (!m_pFilterParams) {
        m_pFilterParams = &m_filterParamsStorage;
        m_filterParamsStorage = *value;
        m_dirtyFlags |= 8;
        return;
    }
    if (!(*m_pFilterParams == *value)) {
        *m_pFilterParams = *value;
        m_dirtyFlags |= 8;
    }
}