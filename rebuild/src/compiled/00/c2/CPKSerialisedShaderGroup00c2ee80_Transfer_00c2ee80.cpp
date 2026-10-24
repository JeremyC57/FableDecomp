// CPKSerialisedShaderGroup_00c2ee80::Transfer(PKSerialiseCParser& parser) -- libpk serialiser: check/write the record version (1), then transfer the three embedded sub-records in order, failing on the first that fails
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C2EE80, length 76 bytes (80 with int3 padding).
// Leaf: Transfer
// Callees: PKSerialiseCParser version transfer @ 0x00C31D40 (thiscall on the parser, `push 1`: reading compares
// the stored dword with 1 via the 4-byte TransferSize 0x00C31B50, writing sends it), sub-record transfers
// @ 0x00C2F7D0 (+0x00), 0x00C2FA20 (+0x10), 0x00C2FC70 (+0x20) (each thiscall, parser by reference, `ret 4`, bool results tested with `test al,al`).
// Label correction: the manifest's "CFilter_Bind4<CMessageEventFilter_IsOfType,...>::operator()<CMessageEvent>"
// is propagated. A real Bind4 is 16 bytes of filter POINTERS (PDB F1..F4) and would load them; this body passes
// `this`, `this+0x10` and `this+0x20` as objects, and its callees are PK-parser transfers (0x00C31D40 reads 4
// bytes through 0x00C31B50, the same TransferSize the landed CShaderResource::LoadPixelShaderData 0x00C31B90
// uses; 0x00C2F7D0 starts with the parser's IsValid 0x00C31960 and a 4-byte read). The record and parser
// names are descriptive; the sole caller 0x00C17820 builds the parser on its stack ({0, stream} via
// 0x00C31990) and calls this on its own `this`.
// Shape: a chain of nested ifs with one `return 1` and a shared `return 0` (an int result: `mov eax,1` /
// `xor eax,eax`, not the 8-bit bool form); the parser reference stays in edi.
struct PKSerialiseCParser {
    void*         m_pReader;                        // +0x00 (null when writing)
    void*         m_pStream;                        // +0x04
    bool TransferVersion(unsigned long version);    // 0x00C31D40
};

struct CPKSubRecordA { unsigned char _body[0x10]; bool Transfer(PKSerialiseCParser& parser); };   // 0x00C2F7D0
struct CPKSubRecordB { unsigned char _body[0x10]; bool Transfer(PKSerialiseCParser& parser); };   // 0x00C2FA20
struct CPKSubRecordC { unsigned char _body[0x10]; bool Transfer(PKSerialiseCParser& parser); };   // 0x00C2FC70

struct CPKSerialisedShaderGroup_00c2ee80 {
    CPKSubRecordA m_first;     // +0x00
    CPKSubRecordB m_second;    // +0x10
    CPKSubRecordC m_third;     // +0x20
    int Transfer(PKSerialiseCParser& parser);
};

int CPKSerialisedShaderGroup_00c2ee80::Transfer(PKSerialiseCParser& parser)
{
    if (parser.TransferVersion(1))
        if (m_first.Transfer(parser))
            if (m_second.Transfer(parser))
                if (m_third.Transfer(parser))
                    return 1;
    return 0;
}