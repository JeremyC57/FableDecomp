// PKSerialiseCParser::Transfer(void* data, unsigned long size) -- move size bytes through the attached reader (reading) or the asserted writer (writing): both are slot-0 tail calls
// Leaf: ?Transfer@PKSerialiseCParser@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C31B30, 23 bytes, thiscall `ret 8` (through the tail-jumped virtuals): reader
//   non-null -> `jmp [vtbl+0]` with ecx = reader, else ValidateWriter 0x00C31A70 then `jmp [vtbl+0]` on the writer.
// Identity (sweep2, with the 1g PKSerialiseCParser::TransferString 0x00C31B90; descriptive names, libpk has no
//   PDB/RTTI): libpk's PKSerialiseCParser holds the reader at +0 and the writer at +4 (landed ValidateWriter 0x00C31A70,
//   "Null writer" at .\PKSerialiseCParser.cpp line 17; TransferVersion 0x00C31D40; TransferString 0x00C31B90).
//   Callers: the four typed transfer helpers 0x00C31B50 (4 bytes, TransferSize), 0x00C31B60 (4), 0x00C31B70 (2) and
//   0x00C31B80 (4). The landed name "CDisplayViewManager::Render" (fallback view = 0x00C31A70) is propagated.
class PKSerialiseCReader {
public:
    virtual bool Transfer(void* data, unsigned long size);   // slot 0
};
class PKSerialiseCWriter {
public:
    virtual bool Transfer(void* data, unsigned long size);   // slot 0
};
class PKSerialiseCParser {
public:
    PKSerialiseCReader* m_pReader;                  // +0 (null when writing)
    PKSerialiseCWriter* m_pWriter;                  // +4
    PKSerialiseCWriter* ValidateWriter();           // 0x00C31A70
    bool Transfer(void* data, unsigned long size);
};

bool PKSerialiseCParser::Transfer(void* data, unsigned long size)
{
    if (m_pReader)
        return m_pReader->Transfer(data, size);
    return ValidateWriter()->Transfer(data, size);
}
