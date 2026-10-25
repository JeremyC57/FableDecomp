// PKSerialiseCParser::TransferSize(unsigned long* size) -- transfer a 4-byte size/count through the parser
// Leaf: ?TransferSize@PKSerialiseCParser@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C31B50, 15 bytes, thiscall `ret 4`: `push 4 ; push [esp+8] ; call 0x00C31B30`
//   with ecx passed straight through.
// Identity (sweep2, with the 1g PKSerialiseCParser::TransferString 0x00C31B90; descriptive names, libpk has no
//   PDB/RTTI): libpk's PKSerialiseCParser holds the reader at +0 and the writer at +4 (landed ValidateWriter 0x00C31A70,
//   "Null writer" at .\PKSerialiseCParser.cpp line 17; TransferVersion 0x00C31D40; TransferString 0x00C31B90).
//   The landed PKSerialiseCParser::TransferVersion 0x00C31D40 and TransferString 0x00C31B90 already bind this address as
//   ?TransferSize@PKSerialiseCParser@@QAE_NPAK@Z (20 callers in all, the shader-group Serialise bodies among them).
//   The landed name "CMainGameComponent::BeginInputLoading" (a __stdcall free-function model) is propagated.
class PKSerialiseCParser {
public:
    void* m_pReader;                                // +0 (null when writing)
    void* m_pWriter;                                // +4
    bool Transfer(void* data, unsigned long size);  // 0x00C31B30
    bool TransferSize(unsigned long* size);
};

bool PKSerialiseCParser::TransferSize(unsigned long* size)
{
    return Transfer(size, 4);
}
