// CPersistContext_TransferableComponent::TransferIn(CPersistContext*, CStringParser*) -- reads a 64-bit integer with CStringParser::ReadInt64 and stores it through the target pointer
// Leaf: TransferIn
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0044B7E0, 22 bytes (+10 int3 padding).
// Callee: 0x009BA5B0 CStringParser::ReadInt64 (__fastcall, result in edx:eax).
//
// Persist READ hook (vtable slot 1) of a 64-bit component: the target pointer at +0x4 is read before
// the call (named local, kept in esi), the parser (second argument) is `this` of the reader, and the
// edx:eax result is stored as two dwords.
struct CPersistContext;

struct CStringParser {
    __int64 ReadInt64();                   // 0x009BA5B0
};

struct CPersistContext_TransferableComponent {
    virtual void TransferOut(CPersistContext* context, void* out);
    virtual void TransferIn(CPersistContext* context, CStringParser* parser);
    __int64* m_pValue;                     // +0x4
};

void CPersistContext_TransferableComponent::TransferIn(CPersistContext* context, CStringParser* parser)
{
    __int64* target = m_pValue;
    *target = parser->ReadInt64();
}