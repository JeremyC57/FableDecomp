// CFlammableDef::TransferHelper(CPersistContext* context) -- name the helper "" (0x00404500), hold a 20-byte transfer scope on the stack, and serialise the def's map selected by the kind at +0x18: kind 2 -> the map at +0x24 (0x004E5A76), kind 3 -> the map at +0x28 (0x004E5A39), both over the context's stream at +8
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004E59E1, length 88 bytes.
// Callees: the name setter @ 0x00404500 (on `this`, the empty string @ 0x0122D70E; labelled "CPersistContext::CheckCRC", propagated), the scope constructor
// @ 0x004E0A05 (labelled "CAIStateGroupBase", propagated), the two `__fastcall` serialisers @ 0x004E5A76 / 0x004E5A39 (map in ecx, context+8 in edx), the scope's
// inlined destructor: its 12-byte member at +8 destroyed @ 0x004DD470 ("CActiveFile::OnReadFinished", propagated) then the CInitBaseClass base @ 0x0099A410.
// Shape: the frame-pointer module (`leave`), the kind decoded with `dec ; dec ; je ; dec ; jne`, `ret 4`.
#pragma optimize("y", off)
#pragma optimize("s", on)
extern const char g_emptyString_0122d70e[];       // 0x0122D70E ("")

struct CInitBaseClass {                            // 8 bytes
    unsigned char _body[8];
    ~CInitBaseClass();                             // 0x0099A410
};
struct CTransferScopeMember_004dd470 {             // 12 bytes
    unsigned char _body[12];
    ~CTransferScopeMember_004dd470();              // 0x004DD470
};
struct CTransferScope_004e0a05 : CInitBaseClass {  // 20 bytes on the stack
    CTransferScopeMember_004dd470 m_member;        // +0x08
    CTransferScope_004e0a05();                     // 0x004E0A05
    ~CTransferScope_004e0a05() { }
};

struct CPersistContext_004e59e1 {
    unsigned char _pad[8];
    unsigned char m_stream[4];                     // +0x08
};
struct CDefMap_004e59e1;
void __fastcall SerialiseMapBinaryIn_004e5a76(CDefMap_004e59e1* map, void* stream);   // 0x004E5A76 (GFSerialiseMapBinaryIn<long, CDefString, ...>)
void __fastcall SerialiseAllocatedAreas_004e5a39(CDefMap_004e59e1* map, void* stream);   // 0x004E5A39 ("WriteAllocatedAreasData")

struct CFlammableDef {
    unsigned char      _pad[0x18];
    long               m_kind;                     // +0x18
    unsigned char      _pad1c[8];
    CDefMap_004e59e1*  m_pMapA;                    // +0x24
    CDefMap_004e59e1*  m_pMapB;                    // +0x28
    void SetHelperName(const char* name);          // 0x00404500
    void TransferHelper(CPersistContext_004e59e1* context);
};

void CFlammableDef::TransferHelper(CPersistContext_004e59e1* context)
{
    SetHelperName(g_emptyString_0122d70e);
    CTransferScope_004e0a05 scope;
    switch (m_kind) {
    case 2:
        SerialiseMapBinaryIn_004e5a76(m_pMapA, context->m_stream);
        break;
    case 3:
        SerialiseAllocatedAreas_004e5a39(m_pMapB, context->m_stream);
        break;
    }
}
