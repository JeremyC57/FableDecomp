// CPersistContext_TransferableComponent::TransferIn(CPersistContext*, CStringParser*) -- parses a long through 0x009E2590 and stores it, converted to double, through the target pointer
// Leaf: TransferIn
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004AA3F0, 32 bytes.
// Callee: 0x009E2590 TransferInLong (__fastcall: context in ecx, parser in edx, long* result on the stack).
//
// Persist READ hook (vtable slot 1) of a double component saved as a whole number: the target
// pointer at +0x4 is read before the call (kept in esi), the long reader parses into a local that VC
// homes in the parser's dead argument slot (`lea eax,[esp+0Ch]`), and the value is widened with
// `fild` and stored with `fstp qword ptr [esi]`.
struct CPersistContext;
struct CStringParser;
void __fastcall TransferInLong(CPersistContext* context, CStringParser* parser, long* result);   // 0x009E2590

struct CPersistContext_TransferableComponent {
    virtual void TransferOut(CPersistContext* context, void* out);
    virtual void TransferIn(CPersistContext* context, CStringParser* parser);
    double* m_pValue;                      // +0x4
};

void CPersistContext_TransferableComponent::TransferIn(CPersistContext* context, CStringParser* parser)
{
    double* target = m_pValue;
    long value;
    TransferInLong(context, parser, &value);
    *target = value;
}