// CPersistContext_TransferableComponent<unsigned __int64>::TransferIn(CPersistContext*, CStringParser*) -- reads a 64-bit integer with CStringParser::ReadAsUHUGE and stores it through the target pointer
// Leaf: TransferIn
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0044B7E0, 22 bytes (+10 int3 padding).
// Callee: 0x009BA5B0 CStringParser::ReadAsUHUGE (__fastcall, result in edx:eax).
//
// Persist READ hook (vtable slot 1) of a 64-bit component: the target pointer at +0x4 is read before
// the call (named local, kept in esi), the parser (second argument) is `this` of the reader, and the
// edx:eax result is stored as two dwords.
class CPersistContext;

class CStringParser {
public:
    unsigned __int64 ReadAsUHUGE();   // 0x009BA5B0 (Ego_r.pdb ?ReadAsUHUGE@CStringParser@@QAE_KXZ)
private:                    // 0x009BA5B0
};

struct CPersistContext_TransferableComponent {
    virtual void TransferOut(CPersistContext* context, void* out);
    virtual void TransferIn(CPersistContext* context, CStringParser* parser);
    __int64* m_pValue;                     // +0x4
};

void CPersistContext_TransferableComponent::TransferIn(CPersistContext* context, CStringParser* parser)
{
    __int64* target = m_pValue;
    *target = ((CStringParser*)parser)->ReadAsUHUGE();
}