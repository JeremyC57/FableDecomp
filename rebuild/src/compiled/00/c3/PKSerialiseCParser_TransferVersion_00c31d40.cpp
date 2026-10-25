// PKSerialiseCParser::TransferVersion(unsigned long version) -- libpk serialiser version check: when reading (a reader is attached) it reads the stored dword and reports whether it equals the expected version; when writing it sends the version
// Leaf: TransferVersion
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C31D40, 53 bytes, __fastcall `ret 4`. Name from the landed caller
//   CPKSerialisedShaderGroup_00c2ee80::Transfer 0x00C2EE80 (`parser.TransferVersion(1)`); the catalog's
//   `CMainGameComponent_BeginInputLoading_Wrapper` is propagated.
// Callee: TransferSize @ 0x00C31B50 (__fastcall, transfers 4 bytes at the pointer; landed as
//   "CMainGameComponent::BeginInputLoading", the name used by the landed CShaderResource::LoadPixelShaderData 0x00C31B90).
// Shape: the stored word lives in the `push ecx` frame slot; the read failure returns the callee's al; the write path
//   (reader null) is placed last and passes the argument's own slot; an `unsigned char` result (a `bool` one adds
//   `xor eax,eax` before the `sete`; the landed caller tests al either way).
struct PKSerialiseCParser {
    void* m_pReader;                                // +0x00 (null when writing)
    void* m_pStream;                                // +0x04
    bool TransferSize(unsigned long* size);         // 0x00C31B50
    unsigned char TransferVersion(unsigned long version);
};

unsigned char PKSerialiseCParser::TransferVersion(unsigned long version)
{
    if (m_pReader) {
        unsigned long stored;
        if (!TransferSize(&stored))
            return false;
        return stored == version;
    }
    return TransferSize(&version);
}