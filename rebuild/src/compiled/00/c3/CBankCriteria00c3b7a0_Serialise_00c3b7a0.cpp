// CBankCriteria_00c3b7a0::Serialise(CPKSerialiseParser_00c3b7a0& parser) -- transfer the name string, then the value count (read and resize the PKCAutoDeleteArray at +4 when the parser is reading, else write the current count), then every element; false at the first failed transfer
// Leaf: ?Serialise@CBankCriteria_00c3b7a0@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C3B7A0, 149 bytes (+ int3 padding), __fastcall `ret 4`, bool in al.
// Library: libpk audio (no PDB/RTTI); descriptive names. The parser is PKSerialiseCParser (reader at +0;
//   IsReading 0x00C31960 = `m_pReader != 0`; 4-byte transfer 0x00C31B50; CString transfer 0x00C31B90). The array is
//   PKCAutoDeleteArray (`PKCAutoDeleteArray.h`: data +0, size +4; Resize 0x00C31810, checked operator[] 0x00C2EBC0).
//   Reached from the serialise visitor 0x00C2EDF0 and 0x00C2FD1A. The manifest's "CGraphicsSystem_LoadShaderData" is
//   propagated.
// Shape: the count local lives in the dead parser argument slot (both compilers); the read arm falls through.
struct CString_00c3b7a0 {
    char* m_pData;
};

struct CPKSerialiseParser_00c3b7a0 {
    void* m_pReader;                                    // +0
    void* m_pWriter;                                    // +4
    bool IsReading();                                   // 0x00C31960
    bool Serialise(unsigned long& value);               // 0x00C31B50
    bool Serialise(CString_00c3b7a0& text);             // 0x00C31B90
};

struct CAutoDeleteArray_00c3b7a0 {
    unsigned long* m_pData;                             // +0
    unsigned long m_size;                               // +4
    void Resize(unsigned long size);                    // 0x00C31810
    unsigned long& operator[](unsigned long index);     // 0x00C2EBC0
};

struct CBankCriteria_00c3b7a0 {
    CString_00c3b7a0 m_name;                            // +0
    CAutoDeleteArray_00c3b7a0 m_values;                 // +4
    bool Serialise(CPKSerialiseParser_00c3b7a0& parser);
};

bool CBankCriteria_00c3b7a0::Serialise(CPKSerialiseParser_00c3b7a0& parser)
{
    if (!parser.Serialise(m_name))
        return false;
    unsigned long count;
    if (parser.IsReading()) {
        if (!parser.Serialise(count))
            return false;
        m_values.Resize(count);
    } else {
        count = m_values.m_size;
        if (!parser.Serialise(count))
            return false;
    }
    for (unsigned long i = 0; i < m_values.m_size; ++i) {
        if (!parser.Serialise(m_values[i]))
            return false;
    }
    return true;
}