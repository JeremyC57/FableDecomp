// CBankSampleInfo_00c3b850::Serialise(CPKSerialiseParser_00c3b850& parser) -- transfer the bank sample record through the parser field by field (id, name string, two words, two shorts, a word, two floats); true only when every transfer succeeded
// Leaf: ?Serialise@CBankSampleInfo_00c3b850@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C3B850, 161 bytes (+ int3 padding), __fastcall `ret 4`.
// Library: libpk audio (no PDB/RTTI); descriptive names. The parser is PKSerialiseCParser (`.\PKSerialiseCParser.cpp`,
//   "Null reader"/"Null writer"): reader at +0, its typed transfers are thin `Transfer(&v, size)` wrappers --
//   0x00C31B50 / 0x00C31B60 (4 bytes), 0x00C31B70 (2 bytes) -- and 0x00C31B90 transfers a CString. Reached from the
//   serialise visitor 0x00C2ED90 (`if (m_errors == 0 && !item->Serialise(*m_pParser)) ++m_errors;`) and 0x00C2F87A.
//   The manifest's "CShaderGroup_LoadAllShaders" is propagated.
// Shape: one `return a && b && ...;` chain -- the int-width `mov eax,1` / `xor eax,eax` bool tail.
struct CString_00c3b850 {
    char* m_pData;
};

struct CPKSerialiseParser_00c3b850 {
    void* m_pReader;                                    // +0
    void* m_pWriter;                                    // +4
    bool Serialise(unsigned long& value);               // 0x00C31B50
    bool Serialise(float& value);                       // 0x00C31B60
    bool Serialise(unsigned short& value);              // 0x00C31B70
    bool Serialise(CString_00c3b850& text);             // 0x00C31B90
};

struct CBankSampleInfo_00c3b850 {
    unsigned long m_id;                                 // +0
    CString_00c3b850 m_name;                            // +4
    unsigned long m_offset;                             // +8
    unsigned long m_length;                             // +0xc
    unsigned short m_channels;                          // +0x10
    unsigned short m_bits;                              // +0x12
    unsigned long m_rate;                               // +0x14
    float m_volume;                                     // +0x18
    float m_pitch;                                      // +0x1c
    bool Serialise(CPKSerialiseParser_00c3b850& parser);
};

bool CBankSampleInfo_00c3b850::Serialise(CPKSerialiseParser_00c3b850& parser)
{
    return parser.Serialise(m_id) && parser.Serialise(m_name) && parser.Serialise(m_offset)
        && parser.Serialise(m_length) && parser.Serialise(m_channels) && parser.Serialise(m_bits)
        && parser.Serialise(m_rate) && parser.Serialise(m_volume) && parser.Serialise(m_pitch);
}